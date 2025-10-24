#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

int unlink(const char *pathname) {
    static int (*original_unlink)(const char *) = NULL;
    
    if (!original_unlink) {
        original_unlink = dlsym(RTLD_NEXT, "unlink");
        if (!original_unlink) {
            fprintf(stderr, "dlsym failed: %s\n", dlerror());
            abort();
        }
    }
    
    if (strstr(pathname, "PROTECT") != NULL) {
        fprintf(stderr, "Cannot unlink '%s': filename contains PROTECT\n", pathname);
        errno = EPERM;
        return -1;
    }
    
    return original_unlink(pathname);
}
