lib_LTLIBRARIES = libredis-c.la
libredis_c_la_SOURCES = redis-client.c
libredis_c_la_CFLAGS = \
	-I$(top_srcdir)/deps/evcom \
	-I$(top_srcdir)/deps/libev
libredis_c_la_LIBADD = \
	$(top_srcdir)/deps/evcom/libevcom.so \
	$(top_srcdir)/deps/libev/libev.la

bin_PROGRAMS = resque
resque_LDADD = \
	$(top_srcdir)/deps/libev/libev.la \
	$(top_srcdir)/deps/jansson/src/libjansson.la \
	$(top_srcdir)/deps/evcom/libevcom.so \
	$(top_srcdir)/src/libredis-c.la
resque_CFLAGS = \
	-I$(top_srcdir)/deps/jansson/src \
	-I$(top_srcdir)/deps/libev \
	-I$(top_srcdir)/deps/evcom
resque_SOURCES = \
	resque.c \
	resque-error.c

