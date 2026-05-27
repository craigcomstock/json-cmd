# This Makefile provides 2 example targets, they both print argv, but
# one uses a libutils data structure to do it. Both binaries are
# built using size-optimizing flags and options for stripping dead code.
# On my system the binaries are 9KB (without libutils) and 11KB (with libutils).

# In my testing, trying to include individual files from libutils
# is actually worse than linking to libutils.la and letting the compiler/linker
# optimize for size and strip dead code.

# Run make inside libutils as well:
SUBDIRS = libntech tests/unit

# Look for macros inside m4 directory:
ACLOCAL_AMFLAGS = -I m4 --install

# "Trick" to ensure m4 directory exists in source tarballs:
EXTRA_DIST = m4/NOTES

bin_PROGRAMS = \
	argv_printer_zero \
	argv_printer_libs # Equal length file name for fair comparison

# An example target with no dependencies, for comparison:
argv_printer_zero_SOURCES = \
	argv_printer/argv_lib.c \
	argv_printer/argv_printer_zero.c

# Optimize for binary size and strip dead code:
argv_printer_zero_CFLAGS = -Os -fdata-sections -ffunction-sections
argv_printer_zero_LDFLAGS = $(STRIP_LDFLAGS) # Platform specific

# An example target using sequnce data structure from libutils:
argv_printer_libs_SOURCES = \
	argv_printer/argv_lib.c \
	argv_printer/argv_printer_libs.c

# Statically link to libutils:
argv_printer_libs_LDADD = libntech/libutils/.libs/libutils.la
# Tell the c preprocessor (CPP) where to find headers:
argv_printer_libs_CPPFLAGS = -I libntech/libutils/ -I libntech/libcompat/
# Optimize for binary size and strip dead code:
argv_printer_libs_CFLAGS = -Os -fdata-sections -ffunction-sections
argv_printer_libs_LDFLAGS = $(STRIP_LDFLAGS) # Platform specific
