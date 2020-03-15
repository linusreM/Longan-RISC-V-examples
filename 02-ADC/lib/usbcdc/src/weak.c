#include <write.c>

/* This is a workaround to be able to redirect printf() from the _write call,
   which in the gd32v sdk is not defined as weak 
   (if you don't need to use DMA for transfers then you can usually just implement putc() instead) */
#ifdef USE_USB_PRINTF
extern ssize_t _write(int fd, const void* ptr, size_t len) __attribute__((weak));
#endif