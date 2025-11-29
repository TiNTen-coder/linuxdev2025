#include <unistd.h>
#include <dlfcn.h>
#include <string.h>
#include <errno.h>

typedef void (*abort_type)(void);
extern void __gcov_dump();

void abort(void){
    abort_type real_abort = (abort_type)(dlsym(RTLD_NEXT, "abort"));
    __gcov_dump();
    real_abort();
}