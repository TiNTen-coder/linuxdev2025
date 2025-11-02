AC_INIT([rhasher], [1.0.0], [your@email.com])
AC_CONFIG_SRCDIR([src/rhasher.c])
AC_CONFIG_HEADERS([config.h])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])

AC_PROG_CC

AC_CANONICAL_HOST
case "$host_os" in
    darwin*)
        is_darwin=yes
        ;;
    *)
        is_darwin=no
        ;;
esac
AM_CONDITIONAL([DARWIN], [test "x$is_darwin" = "xyes"])

AC_CHECK_HEADER([rhash.h], [],
    [AC_MSG_ERROR([rhash.h not found. Please install librhash development files.])])

AC_CHECK_LIB([rhash], [rhash_library_init], [],
    [AC_MSG_ERROR([librhash not found. Please install librhash library.])])

AC_ARG_ENABLE([readline],
    [AS_HELP_STRING([--disable-readline], [Disable readline support])],
    [enable_readline=$enableval],
    [enable_readline=yes])

AS_IF([test "x$enable_readline" != "xno"], [
    AC_CHECK_HEADER([readline/readline.h],
        [AC_CHECK_LIB([readline], [readline],
            [AC_DEFINE([HAVE_READLINE], [1], [Define if you have readline])
             READLINE_LIBS="-lreadline"
             have_readline=yes],
            [have_readline=no])],
        [have_readline=no])

    AS_IF([test "x$have_readline" = "xno" && test "x$enable_readline" = "xyes"], [
        AC_MSG_WARN([readline library not found, building without readline support])
    ])
])

AC_SUBST([READLINE_LIBS])

AC_CHECK_FUNCS([strcasecmp strncpy])

AC_CONFIG_FILES([
    Makefile
    src/Makefile
])

AC_OUTPUT

echo "
  $PACKAGE_NAME $PACKAGE_VERSION configuration:

  Installation prefix:    $prefix
  Operating system:      $host_os
  Compiler:              $CC
  Compiler flags:        $CFLAGS
  Readline support:      ${have_readline:-no}
"
