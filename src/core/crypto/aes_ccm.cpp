#include "aes_ccm.hpp"

#include "common/code_utils.hpp"
#include "common/debug.hpp"

namespace vc {
namespace Crypto {

vcError AesCcm::SetKey(const uint8_t *aKey, uint16_t aKeyLength)
{
    mEcb.SetKey(aKey, 8 * aKeyLength);
    return VC_ERROR_NONE;
}

vcError AesCcm::Init(uint32_t    aHeaderLength,
                     uint32_t    aPlainTextLength,
                     uint8_t     aTagLength,
                     const void *aNonce,
                     uint8_t     aNonceLength)
{
    const uint8_t *nonceBytes  = reinterpret_cast<const uint8_t *>(aNonce);
    vcError        error       = VC_ERROR_NONE;
    uint8_t        blockLength = 0;
    uint32_t       len;
    uint8_t        L;
    uint8_t        i;

    // aTagLength must be even
    aTagLength &= ~1;

    if (aTagLength > sizeof(mBlock))
    {
        aTagLength = sizeof(mBlock);
    }
    else if (aTagLength < kTagLengthMin)
    {
        ExitNow(error = VC_ERROR_INVALID_ARGS);
    }

    L = 0;

    for (len = aPlainTextLength; len; len >>= 8)
    {
        L++;
    }

    if (L <= 1)
    {
        L = 2;
    }

    if (aNonceLength > 13)
    {
        aNonceLength = 13;
    }

    // increase L to match nonce len
    if (L < (15 - aNonceLength))
    {
        L = 15 - aNonceLength;
    }

    // decrease nonceLength to match L
    if (aNonceLength > (15 - L))
    {
        aNonceLength = 15 - L;
    }

    // setup initial block

    // write flags
    mBlock[0] = (static_cast<uint8_t>((aHeaderLength != 0) << 6) | static_cast<uint8_t>(((aTagLength - 2) >> 1) << 3) |
                 static_cast<uint8_t>(L - 1));

    // write nonce
    for (i = 0; i < aNonceLength; i++)
    {
        mBlock[1 + i] = nonceBytes[i];
    }

    // write len
    len = aPlainTextLength;

    for (i = sizeof(mBlock) - 1; i > aNonceLength; i--)
    {
        mBlock[i] = len & 0xff;
        len >>= 8;
    }

    // encrypt initial block
    mEcb.Encrypt(mBlock, mBlock);

    // process header
    if (aHeaderLength > 0)
    {
        // process length
        if (aHeaderLength < (65536U - 256U))
        {
            mBlock[blockLength++] ^= aHeaderLength >> 8;
            mBlock[blockLength++] ^= aHeaderLength >> 0;
        }
        else
        {
            mBlock[blockLength++] ^= 0xff;
            mBlock[blockLength++] ^= 0xfe;
            mBlock[blockLength++] ^= aHeaderLength >> 24;
            mBlock[blockLength++] ^= aHeaderLength >> 16;
            mBlock[blockLength++] ^= aHeaderLength >> 8;
            mBlock[blockLength++] ^= aHeaderLength >> 0;
        }
    }

    // init counter
    mCtr[0] = L - 1;

    for (i = 0; i < aNonceLength; i++)
    {
        mCtr[1 + i] = nonceBytes[i];
    }

    for (i = i + 1; i < sizeof(mCtr); i++)
    {
        mCtr[i] = 0;
    }

    mNonceLength     = aNonceLength;
    mHeaderLength    = aHeaderLength;
    mHeaderCur       = 0;
    mPlainTextLength = aPlainTextLength;
    mPlainTextCur    = 0;
    mBlockLength     = blockLength;
    mCtrLength       = sizeof(mCtrPad);
    mTagLength       = aTagLength;

exit:
    return error;
}

void AesCcm::Header(const void *aHeader, uint32_t aHeaderLength)
{
    const uint8_t *headerBytes = reinterpret_cast<const uint8_t *>(aHeader);

    assert(mHeaderCur + aHeaderLength <= mHeaderLength);

    // process header
    for (unsigned i = 0; i < aHeaderLength; i++)
    {
        if (mBlockLength == sizeof(mBlock))
        {
            mEcb.Encrypt(mBlock, mBlock);
            mBlockLength = 0;
        }

        mBlock[mBlockLength++] ^= headerBytes[i];
    }

    mHeaderCur += aHeaderLength;

    if (mHeaderCur == mHeaderLength)
    {
        // process remainder
        if (mBlockLength != 0)
        {
            mEcb.Encrypt(mBlock, mBlock);
        }

        mBlockLength = 0;
    }
}

void AesCcm::Payload(void *plaintext, void *ciphertext, uint32_t len, bool aEncrypt)
{
    uint8_t *plaintextBytes  = reinterpret_cast<uint8_t *>(plaintext);
    uint8_t *ciphertextBytes = reinterpret_cast<uint8_t *>(ciphertext);
    uint8_t  byte;

    assert(mPlainTextCur + len <= mPlainTextLength);

    for (unsigned i = 0; i < len; i++)
    {
        if (mCtrLength == 16)
        {
            for (int j = sizeof(mCtr) - 1; j > mNonceLength; j--)
            {
                if (++mCtr[j])
                {
                    break;
                }
            }

            mEcb.Encrypt(mCtr, mCtrPad);
            mCtrLength = 0;
        }

        if (aEncrypt)
        {
            byte               = plaintextBytes[i];
            ciphertextBytes[i] = byte ^ mCtrPad[mCtrLength++];
        }
        else
        {
            byte              = ciphertextBytes[i] ^ mCtrPad[mCtrLength++];
            plaintextBytes[i] = byte;
        }

        if (mBlockLength == sizeof(mBlock))
        {
            mEcb.Encrypt(mBlock, mBlock);
            mBlockLength = 0;
        }

        mBlock[mBlockLength++] ^= byte;
    }

    mPlainTextCur += len;

    if (mPlainTextCur >= mPlainTextLength)
    {
        if (mBlockLength != 0)
        {
            mEcb.Encrypt(mBlock, mBlock);
        }

        // reset counter
        for (uint8_t i = mNonceLength + 1; i < sizeof(mCtr); i++)
        {
            mCtr[i] = 0;
        }
    }
}

void AesCcm::Finalize(void *tag, uint8_t *aTagLength)
{
    uint8_t *tagBytes = reinterpret_cast<uint8_t *>(tag);

    assert(mPlainTextCur == mPlainTextLength);

    if (mTagLength > 0)
    {
        mEcb.Encrypt(mCtr, mCtrPad);

        for (int i = 0; i < mTagLength; i++)
        {
            tagBytes[i] = mBlock[i] ^ mCtrPad[i];
        }
    }

    if (aTagLength)
    {
        *aTagLength = mTagLength;
    }
}

} // namespace Crypto
} // namespace vc
