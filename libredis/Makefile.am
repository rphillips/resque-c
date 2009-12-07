lib_LTLIBRARIES = libredis-c.la
libredis_c_la_SOURCES = redis-client.c
libredis_c_la_LDFLAGS = \
	-lev \
	$(top_srcdir)/deps/evcom/libevcom.so
libredis_c_la_CFLAGS = \
	-I$(top_srcdir)/deps/evcom \
	-I$(top_srcdir)/deps/libev
