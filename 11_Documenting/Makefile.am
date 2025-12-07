CFLAGS= -Wall -Wextra -O0 -g
SUBDIRS = src po

@DX_RULES@

all-local:	doxygen-doc

doxygen-doc/man/man1/guess.1: doxygen-doc guess
	help2man $(top_builddir)/guess > doxygen-doc/man/man1/guess.1

man1_MANS = doxygen-doc/man/man1/guess.1

http:	doxygen-doc
	python3 -m http.server --directory $</html

gitclean:
	git clean -xdf