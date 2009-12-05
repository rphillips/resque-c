# Define EVDIR=/foo/bar if your libev header and library files are in
# /foo/bar/include and /foo/bar/lib directories.
EVDIR=../libev

# Define GNUTLSDIR=/foo/bar if your gnutls header and library files are in
# /foo/bar/include and /foo/bar/lib directories.
#
# Comment out the following line to disable TLS
#GNUTLSDIR=/usr

# CFLAGS and LDFLAGS are for the users to override from the command line.
CFLAGS	= -fPIC -g -I. -Wall -Werror -Wextra #-DNDEBUG=1
LDFLAGS	=

CC ?= gcc
AR ?= ar
RANLIB ?= ranlib

ifdef EVDIR
	CFLAGS += -I$(EVDIR)/
	LDFLAGS += -L$(EVDIR)/
endif

LDFLAGS += -lev

ifdef GNUTLSDIR
	CFLAGS += -I$(GNUTLSDIR)/include -DEVCOM_HAVE_GNUTLS=1
	LDFLAGS += -L$(GNUTLSDIR)/lib
	LDFLAGS += -lgnutls
endif

DEP = evcom.h
SRC = evcom.c
OBJ = ${SRC:.c=.o}

NAME=libevcom
OUTPUT_A=$(NAME).a
OUTPUT_S=$(NAME).so.1

all: $(OUTPUT_S)

$(OUTPUT_A): $(OBJ)
	$(AR) cru $(OUTPUT_A) $(OBJ)
	$(RANLIB) $(OUTPUT_A)

$(OUTPUT_S): $(OBJ)
	$(CC) -shared -Wl,-soname,libevcom.so.1 -o libevcom.so.1.0 $(OBJ)
	ln -sf libevcom.so.1.0 libevcom.so.1
	ln -sf libevcom.so.1 libevcom.so

.c.o:
	$(CC) -c ${CFLAGS} $<

${OBJ}: ${DEP}

FAIL=ruby -e 'puts "\033[1;31m FAIL\033[m"'
PASS=ruby -e 'puts "\033[1;32m PASS\033[m"'

test: test/test test/echo test/timeout.rb
	@echo test.c
	@test/test > /dev/null && $(PASS) || $(FAIL)
	@echo timeout.rb
	@test/timeout.rb

test/test: test/test.c $(OUTPUT_A)
	$(CC) $(LDFLAGS) $(CFLAGS) -o $@ test/test.c $(OUTPUT_A)

test/echo: test/echo.c $(OUTPUT_A)
	$(CC) $(LDFLAGS) $(CFLAGS) -o $@ test/echo.c $(OUTPUT_A)

send_states.png: send_states.dot
	dot -Tpng -o send_states.png send_states.dot

recv_states.png: recv_states.dot
	dot -Tpng -o recv_states.png recv_states.dot

clean:
	rm -rf test/test test/echo
	rm -f $(OUTPUT_A) $(OUTPUT_S) *.o

.PHONY: all clean test
