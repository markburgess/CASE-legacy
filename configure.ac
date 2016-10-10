dnl
dnl Process this file with autoconf to produce a configure script.
dnl $Id: configure.in,v 1.6 1996/11/25 16:52:17 mark Exp $
dnl ##################################################################

AC_INIT([CASE],[1.7])
AC_CONFIG_SRCDIR([models/CASE-model.C])
AC_CANONICAL_TARGET

AC_CONFIG_HEADERS([conf.h])
AM_INIT_AUTOMAKE

AC_PROG_RANLIB

dnl ##################################################################
dnl Checks for programs.
dnl ##################################################################

AC_PROG_CC
AC_PROG_CXX
AC_PROG_CXXCPP
AC_PATH_XTRA

AC_CHECK_FUNCS(bzero)
AC_CHECK_FUNCS(floor)
AC_CHECK_FUNCS(pow)
AC_CHECK_FUNCS(sqrt)
AC_CHECK_FUNCS(strstr)
AC_CHECK_HEADERS(stdlib.h)
AC_CHECK_HEADERS(string.h)
AC_FUNC_LSTAT
AC_FUNC_MALLOC

dnl ##################################################################
dnl Checks for libraries.
dnl ##################################################################

AC_PATH_X

dnl ##################################################################
dnl Checks for header files.
dnl ##################################################################

AC_HEADER_DIRENT
AC_HEADER_STDC
AC_CHECK_HEADERS(unistd.h sys/cdefs.h features.h)

dnl ##################################################################
dnl Checks for typedefs, structures, and compiler characteristics.
dnl ##################################################################

AC_C_CONST

dnl ##################################################################
dnl Checks for library functions.
dnl ##################################################################

AC_FUNC_VPRINTF
AC_CHECK_FUNCS(getcwd)
AC_CHECK_HEADERS(math.h)

dnl ##################################################################
dnl Now make the Makefiles
dnl ##################################################################

dnl CFLAGS="$CFLAGS -ansi"
CXXFLAGS="$CXXFLAGS -fpermissive"
dnl DEFS="$DEFS -ansi"

AC_PROG_MAKE_SET

AC_OUTPUT(models/Makefile doc/Makefile x2dview/Makefile Makefile case/Makefile)

