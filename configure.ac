# package name, version, bug report email, tarball name, project home page:
AC_INIT([argv_printer], [0.0.1], [ole@northern.tech],
        [argv_printer], [https://northern.tech/])

# Required autoconf version:
AC_PREREQ([2.68])

# Defines variables like target_os:
AC_CANONICAL_TARGET

# Initialize libtool:
AM_PROG_AR # libtool needs archiver program
LT_INIT

# Check that sources are available:
AC_CONFIG_SRCDIR([libntech/libutils/sequence.c])
# If sequence.c is not there, it probably means git submodule is missing

# If you want to have a unique name or location for config.h, edit here:
AC_CONFIG_HEADERS([config.h])

# Initialize automake with a string of options (explained below):
AM_INIT_AUTOMAKE([1.11 -Wall -Werror foreign subdir-objects])
# 1.11: the required automake version
# -Wall -Werror: enables all warnings in automake(not gcc) and makes them errors
# foreign: tell automake that this project does not follow GNU Standards
# subdir-objects: .o files are placed in subdirectories (along sources)

# AC_OUTPUT (configure) will generate Makefile from Makefile.in
# performing substitutions according to AC_SUBST macros:
AC_CONFIG_FILES([Makefile tests/unit/Makefile])

# Declare that we need a C compiler:
AC_PROG_CC

# Use the m4 directory for m4 macros (convention):
AC_CONFIG_MACRO_DIR([m4])

# cmocka is a cmake project, but we don't have to run cmake, we can just
# make sure our config.h file has the neccessary HAVE_MALLOC_H etc.:
AC_CHECK_HEADERS([malloc.h inttypes.h signal.h strings.h])

# Platform specific linker flags for stripping dead code:
STRIP_LDFLAGS=""

case "$target_os" in
   linux*|*bsd*|*gnu*)
        STRIP_LDFLAGS="-Wl,--gc-sections"
        ;;
   freebsd*|dragonfly*)
        ;;
   openbsd*|obsd*)
        ;;
   cygwin*)
        ;;
   mingw*)
        ;;
   darwin*)
        STRIP_LDFLAGS="-Wl,-dead_strip"
        ;;
   *)
        AC_MSG_ERROR(Unknown system type $target_os)
        ;;
esac

AC_SUBST([STRIP_LDFLAGS]) # Substitutes the variable in Makefile.am

# Run autoconf/configure in libntech, generating necessary makefiles:
AC_CONFIG_SUBDIRS([libntech])

AC_OUTPUT
