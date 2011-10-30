man_MANS = tempi-draw.1 osc-forward.1 midilooper.1 
EXTRA_DIST = $(man_MANS) \
   man-midilooper.txt \
   man-osc-forward.txt \
   man-tempi-draw.txt

MAINTAINERCLEANFILES = $(man_MANS)

# The man pages depend on the --help strings and the version number.
common_mandeps = $(top_srcdir)/configure.ac

# Depend on the source, not the binary; we don't need to regenerate the
# binary when any source file changes, only the main one.
# Use -o so that the `missing' program can infer the output file.

midilooper.1: $(top_srcdir)/src/midilooper/main.cpp $(common_mandeps) $(top_srcdir)/man/man-midilooper.txt
	$(HELP2MAN) --include=man-midilooper.txt --name="midilooper" --no-info $(top_builddir)/src/midilooper/midilooper -o $@

osc-forward.1: $(top_srcdir)/src/osc-forward/main.cpp $(common_mandeps) $(top_srcdir)/man/man-osc-forward.txt
	$(HELP2MAN) --include=man-osc-forward.txt --name="osc-forward" --no-info $(top_builddir)/src/osc-forward/osc-forward -o $@

tempi-draw.1: $(top_srcdir)/src/tempi-draw/main.cpp $(common_mandeps) $(top_srcdir)/man/man-tempi-draw.txt
	$(HELP2MAN) --include=man-tempi-draw.txt --name="tempi-draw" --no-info $(top_builddir)/src/tempi-draw/tempi-draw -o $@
