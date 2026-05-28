#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <unistd.h>
#include "usart.h"

void _exit(int status) {
    (void)status;
    while (1) {
    }
}

int _kill(int pid, int sig) {
    (void)pid;
    (void)sig;
    errno = EINVAL;
    return -1;
}

int _getpid(void) {
    return 1;
}

int _write(int file, const char *ptr, int len) {
    if (file == 1 || file == 2) {
        for (int i = 0; i < len; i++) {
            USART2_SendChar(ptr[i]);
        }
        return len;
    }

    errno = EBADF;
    return -1;
}

int _read(int file, char *ptr, int len) {
    (void)file;
    (void)ptr;
    (void)len;
    errno = EBADF;
    return -1;
}

int _close(int file) {
    (void)file;
    errno = EBADF;
    return -1;
}

int _isatty(int file) {
    return (file == 0 || file == 1 || file == 2) ? 1 : 0;
}

int _fstat(int file, struct stat *st) {
    if (st == NULL) {
        errno = EINVAL;
        return -1;
    }

    st->st_mode = S_IFCHR;
    return 0;
}

int _lseek(int file, int ptr, int dir) {
    (void)file;
    (void)ptr;
    (void)dir;
    return 0;
}

uintptr_t _sbrk(int incr) {
    (void)incr;
    errno = ENOMEM;
    return (uintptr_t)-1;
}
