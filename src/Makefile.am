bin_PROGRAMS = resque
resque_LDADD = \
	$(top_srcdir)/deps/libev/libev.la \
	$(top_srcdir)/deps/jansson/src/libjansson.la
resque_CFLAGS = \
	-I$(top_srcdir)/deps/jansson/src \
	-I$(top_srcdir)/deps/libev
resque_SOURCES = \
	resque.c \
	resque-error.c \
	redis-client.c
