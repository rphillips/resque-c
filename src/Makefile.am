bin_PROGRAMS = resque
resque_LDADD = \
	$(top_srcdir)/deps/jansson/src/libjansson.la \
	$(top_srcdir)/deps/evcom/libevcom.la \
	$(top_srcdir)/libredis/libredis-c.la
resque_CFLAGS = \
	-I$(top_srcdir)/deps/jansson/src \
	-I$(top_srcdir)/deps/evcom
resque_LDFLAGS = -lev
resque_SOURCES = \
	resque.c \
	resque-error.c

