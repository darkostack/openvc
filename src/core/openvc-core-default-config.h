#ifndef OPENVC_CORE_DEFAULT_CONFIG_H_
#define OPENVC_CORE_DEFAULT_CONFIG_H_

// CLI config
#ifndef OPENVC_CONFIG_CLI_MAX_LINE_LENGTH
#define OPENVC_CONFIG_CLI_MAX_LINE_LENGTH 128
#endif

#ifndef OPENVC_CONFIG_CLI_UART_RX_BUFFER_SIZE
#define OPENVC_CONFIG_CLI_UART_RX_BUFFER_SIZE 512
#endif

#ifndef OPENVC_CONFIG_CLI_UART_TX_BUFFER_SIZE
#define OPENVC_CONFIG_CLI_UART_TX_BUFFER_SIZE 1024
#endif

// Heap config

#ifndef OPENVC_CONFIG_HEAP_SIZE
#define OPENVC_CONFIG_HEAP_SIZE (1536 * sizeof(void *))
#endif

// Stack information config

#ifndef OPENVC_CONFIG_STACK_VENDOR_OUI
#define OPENVC_CONFIG_STACK_VENDOR_OUI 0x18b430
#endif

#ifndef OPENVC_CONFIG_STACK_VERSION_REV
#define OPENVC_CONFIG_STACK_VERSION_REV 0
#endif

#ifndef OPENVC_CONFIG_STACK_VERSION_MAJOR
#define OPENVC_CONFIG_STACK_VERSION_MAJOR 0
#endif

#ifndef OPENVC_CONFIG_STACK_VERSION_MINOR
#define OPENVC_CONFIG_STACK_VERSION_MINOR 1
#endif

// Platform information config

#ifndef OPENVC_CONFIG_PLATFORM_INFO
#define OPENVC_CONFIG_PLATFORM_INFO "NONE"
#endif

// Logging config

#ifndef OPENVC_CONFIG_LOG_OUTPUT
#define OPENVC_CONFIG_LOG_OUTPUT OPENVC_CONFIG_LOG_OUTPUT_PLATFORM_DEFINED
#endif

#define OPENVC_CONFIG_LOG_OUTPUT_NONE 0
#define OPENVC_CONFIG_LOG_OUTPUT_DEBUG_UART 1
#define OPENVC_CONFIG_LOG_OUTPUT_APP 2
#define OPENVC_CONFIG_LOG_OUTPUT_PLATFORM_DEFINED 3

#ifndef OPENVC_CONFIG_LOG_LEVEL
#define OPENVC_CONFIG_LOG_LEVEL VC_LOG_LEVEL_CRIT
#endif

#ifndef OPENVC_CONFIG_ENABLE_DYNAMIC_LOG_LEVEL
#define OPENVC_CONFIG_ENABLE_DYNAMIC_LOG_LEVEL 0
#endif

#ifndef OPENVC_CONFIG_LOG_API
#define OPENVC_CONFIG_LOG_API 1
#endif

#ifndef OPENVC_CONFIG_LOG_MEM
#define OPENVC_CONFIG_LOG_MEM 1
#endif

#ifndef OPENVC_CONFIG_LOG_PLATFORM
#define OPENVC_CONFIG_LOG_PLATFORM 0
#endif

#ifndef OPENVC_CONFIG_LOG_CLI
#define OPENVC_CONFIG_LOG_CLI 1
#endif

#ifndef OPENVC_CONFIG_LOG_CORE
#define OPENVC_CONFIG_LOG_CORE 1
#endif

#ifndef OPENVC_CONFIG_LOG_UTIL
#define OPENVC_CONFIG_LOG_UTIL 1
#endif

#ifndef OPENVC_CONFIG_LOG_PREPEND_LEVEL
#define OPENVC_CONFIG_LOG_PREPEND_LEVEL 1
#endif

#ifndef OPENVC_CONFIG_LOG_PREPEND_REGION
#define OPENVC_CONFIG_LOG_PREPEND_REGION 1
#endif

#ifndef OPENVC_CONFIG_LOG_SUFFIX
#define OPENVC_CONFIG_LOG_SUFFIX ""
#endif

#ifndef OPENVC_CONFIG_PLAT_LOG_FUNCTION
#define OPENVC_CONFIG_PLAT_LOG_FUNCTION vcPlatLog
#endif

// Debug UART config (requires second uart on the platform)

#ifndef OPENVC_CONFIG_ENABLE_DEBUG_UART
#define OPENVC_CONFIG_ENABLE_DEBUG_UART 0
#endif

// Timer config

#ifndef OPENVC_CONFIG_ENABLE_PLATFORM_USEC_TIMER
#define OPENVC_CONFIG_ENABLE_PLATFORM_USEC_TIMER 0
#endif

#endif // OPENVC_CORE_DEFAULT_CONFIG_H_
