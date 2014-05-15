AM_CFLAGS = -I$(top_srcdir)/lib @AM_CFLAGS@
bin_PROGRAMS = readdir
readdir_SOURCES = readdir.c
readdir_LDADD = $(top_builddir)/lib/libbenchmark.a $(AM_LDFLAGS)

GRAPHS = readdir.csv malloc.csv
PROG   = readdir
TMP    = tmp.dat

include ../lib/lib.mk
