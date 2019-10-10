#include "platform-posix.h"

#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <poll.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

#include <openvc/platform/debug_uart.h>
#include <openvc/platform/uart.h>

#include "utils/code_utils.h"

#ifdef OPENVC_TARGET_LINUX
#include <sys/prctl.h>
int   posix_openpt(int oflag);
int   grantpt(int fildes);
int   unlockpt(int fd);
char *ptsname(int fd);
#endif // OPENVC_TARGET_LINUX

static uint8_t        s_receive_buffer[128];
static const uint8_t *s_write_buffer;
static uint16_t       s_write_length;
static int            s_in_fd;
static int            s_out_fd;

static struct termios original_stdin_termios;
static struct termios original_stdout_termios;

static void restore_stdin_termios(void)
{
    tcsetattr(s_in_fd, TCSAFLUSH, &original_stdin_termios);
}

static void restore_stdout_termios(void)
{
    tcsetattr(s_out_fd, TCSAFLUSH, &original_stdout_termios);
}

void platformUartRestore(void)
{
    restore_stdin_termios();
    restore_stdout_termios();
    dup2(s_out_fd, STDOUT_FILENO);
}

vcError vcPlatUartEnable(void)
{
    vcError        error = VC_ERROR_NONE;
    struct termios termios;

#ifdef OPENTHREAD_TARGET_LINUX
    // Ensure we terminate this process if the
    // parent process dies.
    prctl(PR_SET_PDEATHSIG, SIGHUP);
#endif

    s_in_fd  = dup(STDIN_FILENO);
    s_out_fd = dup(STDOUT_FILENO);
    dup2(STDERR_FILENO, STDOUT_FILENO);

    // We need this signal to make sure that this
    // process terminates properly.
    signal(SIGPIPE, SIG_DFL);

    if (isatty(s_in_fd))
    {
        tcgetattr(s_in_fd, &original_stdin_termios);
        atexit(&restore_stdin_termios);
    }

    if (isatty(s_out_fd))
    {
        tcgetattr(s_out_fd, &original_stdout_termios);
        atexit(&restore_stdout_termios);
    }

    if (isatty(s_in_fd))
    {
        // get current configuration
        vcEXPECT_ACTION(tcgetattr(s_in_fd, &termios) == 0, perror("tcgetattr"); error = VC_ERROR_GENERIC);

        // Set up the termios settings for raw mode. This turns
        // off input/output processing, line processing, and character processing.
        cfmakeraw(&termios);

        // Set up our cflags for local use. Turn on hangup-on-close.
        termios.c_cflag |= HUPCL | CREAD | CLOCAL;

        // "Minimum number of characters for noncanonical read"
        termios.c_cc[VMIN] = 1;

        // "Timeout in deciseconds for noncanonical read"
        termios.c_cc[VTIME] = 0;

        // configure baud rate
        vcEXPECT_ACTION(cfsetispeed(&termios, B115200) == 0, perror("cfsetispeed"); error = VC_ERROR_GENERIC);

        // set configuration
        vcEXPECT_ACTION(tcsetattr(s_in_fd, TCSANOW, &termios) == 0, perror("tcsetattr"); error = VC_ERROR_GENERIC);
    }

    if (isatty(s_out_fd))
    {
        // get current configuration
        vcEXPECT_ACTION(tcgetattr(s_out_fd, &termios) == 0, perror("tcgetattr"); error = VC_ERROR_GENERIC);

        // Set up the termios settings for raw mode. This turns
        // off input/output processing, line processing, and character processing.
        cfmakeraw(&termios);

        // Absolutely obliterate all output processing.
        termios.c_oflag = 0;

        // Set up our cflags for local use. Turn on hangup-on-close.
        termios.c_cflag |= HUPCL | CREAD | CLOCAL;

        // configure baud rate
        vcEXPECT_ACTION(cfsetospeed(&termios, B115200) == 0, perror("cfsetospeed"); error = VC_ERROR_GENERIC);

        // set configuration
        vcEXPECT_ACTION(tcsetattr(s_out_fd, TCSANOW, &termios) == 0, perror("tcsetattr"); error = VC_ERROR_GENERIC);
    }

    return error;

exit:
    close(s_in_fd);
    close(s_out_fd);
    return error;
}

vcError vcPlatUartDisable(void)
{
    vcError error = VC_ERROR_NONE;

    close(s_in_fd);
    close(s_out_fd);

    return error;
}

vcError vcPlatUartSend(const uint8_t *aBuf, uint16_t aBufLength)
{
    vcError error = VC_ERROR_NONE;

    vcEXPECT_ACTION(s_write_length == 0, error = VC_ERROR_BUSY);

    s_write_buffer = aBuf;
    s_write_length = aBufLength;

exit:
    return error;
}

void platformUartUpdateFdSet(fd_set *aReadFdSet, fd_set *aWriteFdSet, fd_set *aErrorFdSet, int *aMaxFd)
{
    if (aReadFdSet != NULL)
    {
        FD_SET(s_in_fd, aReadFdSet);

        if (aErrorFdSet != NULL)
        {
            FD_SET(s_in_fd, aErrorFdSet);
        }

        if (aMaxFd != NULL && *aMaxFd < s_in_fd)
        {
            *aMaxFd = s_in_fd;
        }
    }

    if ((aWriteFdSet != NULL) && (s_write_length > 0))
    {
        FD_SET(s_out_fd, aWriteFdSet);

        if (aErrorFdSet != NULL)
        {
            FD_SET(s_out_fd, aErrorFdSet);
        }

        if (aMaxFd != NULL && *aMaxFd < s_out_fd)
        {
            *aMaxFd = s_out_fd;
        }
    }
}

vcError vcPlatUartFlush(void)
{
    vcError error = VC_ERROR_NONE;
    ssize_t count;

    vcEXPECT_ACTION(s_write_buffer != NULL && s_write_length > 0, error = VC_ERROR_INVALID_STATE);

    while ((count = write(s_out_fd, s_write_buffer, s_write_length)) > 0 && (s_write_length -= count) > 0)
    {
        s_write_buffer += count;
    }

    if (count != -1)
    {
        assert(s_write_length == 0);
        s_write_buffer = NULL;
    }
    else
    {
        perror("write(UART)");
        exit(EXIT_FAILURE);
    }

exit:
    return error;
}

void platformUartProcess(void)
{
    ssize_t       rval;
    const int     error_flags = POLLERR | POLLNVAL | POLLHUP;
    struct pollfd pollfd[]    = {
        {s_in_fd, POLLIN | error_flags, 0},
        {s_out_fd, POLLOUT | error_flags, 0},
    };

    errno = 0;

    rval = poll(pollfd, sizeof(pollfd) / sizeof(*pollfd), 0);

    if (rval < 0)
    {
        perror("poll");
        exit(EXIT_FAILURE);
    }

    if (rval > 0)
    {
        if ((pollfd[0].revents & error_flags) != 0)
        {
            perror("s_in_fd");
            exit(EXIT_FAILURE);
        }

        if ((pollfd[1].revents & error_flags) != 0)
        {
            perror("s_out_fd");
            exit(EXIT_FAILURE);
        }

        if (pollfd[0].revents & POLLIN)
        {
            rval = read(s_in_fd, s_receive_buffer, sizeof(s_receive_buffer));

            if (rval <= 0)
            {
                perror("read");
                exit(EXIT_FAILURE);
            }

            vcPlatUartReceived(s_receive_buffer, (uint16_t)rval);
        }

        if ((s_write_length > 0) && (pollfd[1].revents & POLLOUT))
        {
            rval = write(s_out_fd, s_write_buffer, s_write_length);

            if (rval >= 0)
            {
                s_write_buffer += (uint16_t)rval;
                s_write_length -= (uint16_t)rval;

                if (s_write_length == 0)
                {
                    vcPlatUartSendDone();
                }
            }
            else if (errno != EINTR)
            {
                perror("write");
                exit(EXIT_FAILURE);
            }
        }
    }
}

#if OPENVC_CONFIG_DEBUG_UART_ENABLE && (OPENVC_CONFIG_LOG_OUTPUT == OPENVC_CONFIG_LOG_OUTPUT_DEBUG_UART)
static FILE *posix_logfile;

vcError vcPlatDebugUart_logfile(const char *aFilename)
{
    posix_logfile = fopen(aFilename, "wt");

    return posix_logfile ? VC_ERROR_NONE : VC_ERROR_FAILED;
}

void vcPlatDebugUart_putchar_raw(int aChar)
{
    FILE *fp;

    /* note: log file will have a mix of cr/lf and
     * in some/many cases duplicate cr because in
     * some cases the log function {ie: Mbed} already
     * includes the CR or LF... but other log functions
     * do not include cr/lf and expect it appened
     */
    fp = posix_logfile;

    if (fp != NULL)
    {
        /* log is lost ... until a file is setup */
        fputc(aChar, fp);
        /* we could "fflush" but will not */
    }
}

int vcPlatDebugUart_kbhit(void)
{
    /* not supported */
    return 0;
}

int vcPlatDebugUart_getc(void)
{
    /* not supported */
    return -1;
}
#endif
