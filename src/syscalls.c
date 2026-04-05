/*
 * syscalls.c
 *
 *  Created on: Apr 05, 2025
 *  Author    : AbdullahDarwish
 *
 *  Description: Minimal newlib syscall stubs required for printf and
 *               other C library functions on bare-metal ARM.
 *               These stubs satisfy the linker; _write can be redirected
 *               to UART or semihosting as needed.
 */

#include <sys/stat.h>
#include <errno.h>

/* _write: Send characters to an output device (stub — no-op for now).
 * Redirect this to UART for real output on hardware. */
int _write(int file, char *ptr, int len)
{
    (void)file;
    (void)ptr;
    /* Return len to indicate all bytes were "written" successfully */
    return len;
}

/* _read: Read characters from an input device (stub). */
int _read(int file, char *ptr, int len)
{
    (void)file;
    (void)ptr;
    (void)len;
    return 0;
}

/* _close: Close a file (stub). */
int _close(int file)
{
    (void)file;
    return -1;
}

/* _lseek: Seek within a file (stub). */
int _lseek(int file, int offset, int whence)
{
    (void)file;
    (void)offset;
    (void)whence;
    return 0;
}

/* _fstat: Get file status (stub — report as character device). */
int _fstat(int file, struct stat *st)
{
    (void)file;
    st->st_mode = S_IFCHR;
    return 0;
}

/* _isatty: Check if file is a terminal (stub — always yes). */
int _isatty(int file)
{
    (void)file;
    return 1;
}

/* _sbrk: Increase program data space (heap allocator for malloc/printf).
 * Uses the linker-defined symbols _end, _estack. */
extern char _end;          /* Defined by the linker script (end of BSS) */
extern char __end_stack;   /* Defined by the linker script (top of RAM) */

char *_sbrk(int incr)
{
    static char *heap_end = 0;
    char        *prev_heap_end;

    if (heap_end == 0)
    {
        heap_end = &_end;
    }

    prev_heap_end = heap_end;

    /* Simple check: don't let heap collide with stack */
    if (heap_end + incr > &__end_stack)
    {
        errno = ENOMEM;
        return (char *)-1;
    }

    heap_end += incr;
    return prev_heap_end;
}
