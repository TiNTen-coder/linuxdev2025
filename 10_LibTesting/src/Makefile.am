CFLAGS = -ansi -pedantic -Wall -Wextra -O0 -g -std=c99
if COND_GCOV
CFLAGS  += -O0 -g -fprofile-arcs -ftest-coverage -D_GNU_SOURCE
noinst_LTLIBRARIES = libgcov_abort.la
libgcov_abort_la_SOURCES = libgcov_abort.c

lib_LTLIBRARIES = libgrow.la 
libgrow_la_LIBADD = libgcov_abort.la
else
CFLAGS  += -O3 -g3
lib_LTLIBRARIES = libgrow.la 
endif

libgrow_la_SOURCES = libgrow.c

libgrow.c: grow.h

gcov:
	gcov -o .libs libgrow -t -k -b | egrep -v '[[:digit:]]:[ /][*]'