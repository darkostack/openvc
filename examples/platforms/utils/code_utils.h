#ifndef CODE_UTILS_H
#define CODE_UTILS_H

#define vcEXPECT(aCondition) \
    do                       \
    {                        \
        if (!(aCondition))   \
        {                    \
            goto exit;       \
        }                    \
    } while (0)

#define vcEXPECT_ACTION(aCondition, aAction) \
    do                                       \
    {                                        \
        if (!(aCondition))                   \
        {                                    \
            aAction;                         \
            goto exit;                       \
        }                                    \
    } while (0)

#endif // CODE_UTILS_H
