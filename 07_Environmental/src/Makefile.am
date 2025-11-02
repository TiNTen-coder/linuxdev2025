bin_PROGRAMS = rhasher

rhasher_SOURCES = rhasher.c

rhasher_CFLAGS = $(AM_CFLAGS) -Wall -Wextra -std=c99
rhasher_LDADD = -lrhash $(READLINE_LIBS)

if DARWIN
rhasher_LDFLAGS = -Wl,-rpath,$(prefix)/lib
endif
