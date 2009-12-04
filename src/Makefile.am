bin_PROGRAMS = resque
resque_LDADD = \
	$(top_srcdir)/deps/libev/libev.la \
	$(top_srcdir)/deps/jansson/src/libjansson.la \
	$(top_srcdir)/deps/evcom/libevcom.a
resque_CFLAGS = \
	-I$(top_srcdir)/deps/jansson/src \
	-I$(top_srcdir)/deps/libev \
	-I$(top_srcdir)/deps/evcom
resque_SOURCES = \
	resque.c \
	resque-error.c \
	redis-client.c
