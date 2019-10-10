#include "ecdsa.hpp"

#include <mbedtls/ctr_drbg.h>
#include <mbedtls/ecdsa.h>
#include <mbedtls/pk.h>

#include "common/code_utils.hpp"
#include "common/debug.hpp"
#include "common/random.hpp"

namespace vc {
namespace Crypto {

#if OPENVC_CONFIG_ECDSA_ENABLE

vcError Ecdsa::Sign(uint8_t *      aOutput,
                    uint16_t *     aOutputLength,
                    const uint8_t *aInputHash,
                    uint16_t       aInputHashLength,
                    const uint8_t *aPrivateKey,
                    uint16_t       aPrivateKeyLength)
{
    vcError               error = VC_ERROR_NONE;
    mbedtls_ecdsa_context ctx;
    mbedtls_pk_context    pkCtx;
    mbedtls_ecp_keypair * keypair;
    mbedtls_mpi           rMpi;
    mbedtls_mpi           sMpi;

    mbedtls_pk_init(&pkCtx);
    mbedtls_ecdsa_init(&ctx);
    mbedtls_mpi_init(&rMpi);
    mbedtls_mpi_init(&sMpi);

    // Parse a private key in PEM format.
    VerifyOrExit(mbedtls_pk_parse_key(&pkCtx, aPrivateKey, aPrivateKeyLength, NULL, 0) == 0,
                 error = VC_ERROR_INVALID_ARGS);
    VerifyOrExit(mbedtls_pk_get_type(&pkCtx) == MBEDTLS_PK_ECKEY, error = VC_ERROR_INVALID_ARGS);

    keypair = mbedtls_pk_ec(pkCtx);
    assert(keypair != NULL);

    VerifyOrExit(mbedtls_ecdsa_from_keypair(&ctx, keypair) == 0, error = VC_ERROR_FAILED);

    // Sign using ECDSA.
    VerifyOrExit(mbedtls_ecdsa_sign(&ctx.grp, &rMpi, &sMpi, &ctx.d, aInputHash, aInputHashLength,
                                    mbedtls_ctr_drbg_random, Random::Crypto::MbedTlsContextGet()) == 0,
                 error = VC_ERROR_FAILED);
    VerifyOrExit(mbedtls_mpi_size(&rMpi) + mbedtls_mpi_size(&sMpi) <= *aOutputLength, error = VC_ERROR_NO_BUFS);

    // Concatenate the two octet sequences in the order R and then S.
    VerifyOrExit(mbedtls_mpi_write_binary(&rMpi, aOutput, mbedtls_mpi_size(&rMpi)) == 0, error = VC_ERROR_FAILED);
    *aOutputLength = static_cast<uint16_t>(mbedtls_mpi_size(&rMpi));

    VerifyOrExit(mbedtls_mpi_write_binary(&sMpi, aOutput + *aOutputLength, mbedtls_mpi_size(&sMpi)) == 0,
                 error = VC_ERROR_FAILED);
    *aOutputLength += mbedtls_mpi_size(&sMpi);

exit:
    mbedtls_mpi_free(&rMpi);
    mbedtls_mpi_free(&sMpi);
    mbedtls_ecdsa_free(&ctx);
    mbedtls_pk_free(&pkCtx);

    return error;
}

#endif // OPENVC_CONFIG_ECDSA_ENABLE

} // namespace Crypto
} // namespace vc
