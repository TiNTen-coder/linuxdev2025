#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.71])
AC_INIT([Programm to guess numbers], [0.0.1], [vyacheslav.kret@gmail.com], [guess])
AM_INIT_AUTOMAKE([foreign subdir-objects])
DX_INIT_DOXYGEN([guess])
AM_GNU_GETTEXT(external)
AM_GNU_GETTEXT_VERSION(0.21)
AC_SUBST(GETTEXT_PACKAGE, "$PACKAGE")
AC_CONFIG_HEADERS([config.h])
ALL_LINGUAS="ru"

AC_PROG_CC
IT_PROG_INTLTOOL

AC_CHECK_HEADERS([libintl.h unistd.h])

AC_CHECK_FUNCS([setlocale])

AC_CONFIG_FILES([Makefile po/Makefile.in src/Makefile Doxyfile])
AC_OUTPUT
