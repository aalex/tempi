man_MANS = \
	tempi-draw.1 \
	tempi-osc-forward.1 \
	tempi-midi-looper.1 \
	tempi-launch.1 \
	tempi-osc-looper.1 \
	tempi-inspect.1

EXTRA_DIST = $(man_MANS) \
   man-tempi-midi-looper.txt \
   man-tempi-osc-forward.txt \
   man-tempi-draw.txt \
   man-tempi-launch.txt \
   man-tempi-osc-looper.txt \
   man-tempi-inspect.txt

MAINTAINERCLEANFILES = $(man_MANS)

# The man pages depend on the --help strings and the version number.
common_mandeps = $(top_srcdir)/configure.ac

# Depend on the source, not the binary; we don't need to regenerate the
# binary when any source file changes, only the main one.
# Use -o so that the `missing' program can infer the output file.

tempi-midi-looper.1: $(top_srcdir)/src/tempi-midi-looper/main.cpp $(common_mandeps) $(top_srcdir)/man/man-tempi-midi-looper.txt
	$(HELP2MAN) --include=man-tempi-midi-looper.txt --name="tempi-midi-looper" --no-info $(top_builddir)/src/tempi-midi-looper/tempi-midi-looper -o $@

tempi-osc-forward.1: $(top_srcdir)/src/tempi-osc-forward/main.cpp $(common_mandeps) $(top_srcdir)/man/man-tempi-osc-forward.txt
	$(HELP2MAN) --include=man-tempi-osc-forward.txt --name="tempi-osc-forward" --no-info $(top_builddir)/src/tempi-osc-forward/tempi-osc-forward -o $@

tempi-draw.1: $(top_srcdir)/src/tempi-draw/main.cpp $(common_mandeps) $(top_srcdir)/man/man-tempi-draw.txt
	$(HELP2MAN) --include=man-tempi-draw.txt --name="tempi-draw" --no-info $(top_builddir)/src/tempi-draw/tempi-draw -o $@

tempi-launch.1: $(top_srcdir)/src/tempi-launch/main.cpp $(common_mandeps) $(top_srcdir)/man/man-tempi-launch.txt
	$(HELP2MAN) --include=man-tempi-launch.txt --name="tempi-launch" --no-info $(top_builddir)/src/tempi-launch/tempi-launch -o $@

tempi-inspect.1: $(top_srcdir)/src/tempi-inspect/main.cpp $(common_mandeps) $(top_srcdir)/man/man-tempi-inspect.txt
	$(HELP2MAN) --include=man-tempi-inspect.txt --name="tempi-inspect" --no-info $(top_builddir)/src/tempi-inspect/tempi-inspect -o $@

tempi-osc-looper.1: $(top_srcdir)/src/tempi-osc-looper/main.cpp $(common_mandeps) $(top_srcdir)/man/man-tempi-osc-looper.txt
	$(HELP2MAN) --include=man-tempi-osc-looper.txt --name="tempi-osc-looper" --no-info $(top_builddir)/src/tempi-osc-looper/tempi-osc-looper -o $@

