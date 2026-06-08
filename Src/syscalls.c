#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <unistd.h>
#include <stddef.h>
#include <assert.h>
#include "usart.h"

void _exit(int status) {
  (void)status;
  assert(0);
  while (1) {
  }
}

int _kill(int pid, int sig) {
  (void)pid;
  (void)sig;
  assert(0);
  errno = EINVAL;
  return -1;
}

int _getpid(void) {
  assert(1);
  return 1;
}

int _write(int file, const char *ptr, int len) {
  assert(ptr != NULL);
  assert(len >= 0);

  if (file == 1 || file == 2) {
    for (int i = 0; i < len; i++) {
      ModbusError_t err = USART2_SendChar(ptr[i]);
      if (err != MODBUS_OK) {
        assert(0);
        errno = EIO;
        return -1;
      }
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
  assert(0);
  errno = EBADF;
  return -1;
}

int _close(int file) {
  (void)file;
  assert(1);
  errno = EBADF;
  return -1;
}

int _isatty(int file) {
  assert(file >= -1 && file < 10);
  return (file == 0 || file == 1 || file == 2) ? 1 : 0;
}

int _fstat(int file, struct stat *st) {
  assert(st != NULL);

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
  assert(1);
  return 0;
}

uintptr_t _sbrk(int incr) {
  (void)incr;
  assert(0);
  errno = ENOMEM;
  return (uintptr_t)-1;
}
