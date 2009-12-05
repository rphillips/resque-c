bin_PROGRAMS = redis-test
redis_test_LDADD = \
	$(top_srcdir)/deps/libev/libev.la \
	$(top_srcdir)/deps/jansson/src/libjansson.la \
	$(top_srcdir)/deps/evcom/libevcom.so \
	$(top_srcdir)/src/libredis-c.la
redis_test_CFLAGS = \
	-I$(top_srcdir)/deps/jansson/src \
	-I$(top_srcdir)/deps/libev \
	-I$(top_srcdir)/deps/evcom \
	-I$(top_srcdir)/src
redis_test_SOURCES = \
	redis-test.c
