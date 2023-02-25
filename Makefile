CC=gcc
PROG = ethmngr
SRC_DIR = src
OBJS = $(addprefix $(SRC_DIR)/, ethmngr.o)

-include Makefile.diag.inc

PROG_CFLAGS = $(CFLAGS) $(DIAG_CFLAGS) -Werror -fstrict-aliasing -fPIC

# MUSL has the following issue in snprintf, so it is ignored:
PROG_CFLAGS += -Wno-format-nonliteral

PROG_LDFLAGS = $(LDFLAGS)
PROG_LIBS += -luci -lubus -lubox -ljson-c -lblobmsg_json \
	     -lnl-genl-3 -lnl-3 -lgcov -leasy \
	     -lethernet
GCOV = gcov
CODECOVERAGE_SRC = $(addprefix $(SRC_DIR)/, ethmngr.c)

%.o: %.c
	$(CC) $(PROG_CFLAGS) $(FPIC) -c -o $@ $<

.PHONY: all clean

all: $(PROG)

$(PROG): $(OBJS)
	$(CC) $(PROG_LDFLAGS) -o $@ $^ $(PROG_LIBS)

test: CFLAGS += -fPIC
test: ${OBJS}
	${CC} ${LDFLAGS} -shared -o libethmngr.so ${OBJS} $(LIBS)

unit-test: coverage
	make -C test/cmocka unit-test ETHMNGR_LIB_DIR=$(PWD)

functional-test: coverage
	make -C test/cmocka functional-test ETHMNGR_LIB_DIR=$(PWD)

coverage: CFLAGS  += -g -O0 -fprofile-arcs -ftest-coverage -fPIC
coverage: LDFLAGS += --coverage
coverage: test ethmngr
	$(foreach testprog, $(CODECOVERAGE_SRC), $(GCOV) $(testprog);)

clean:
	rm -f *.o *.so $(PROG)
	rm -f *.xml *.html
	find -name '*.gcda' -exec rm {} -fv \;
	find -name '*.gcno' -exec rm {} -fv \;
	find -name '*.gcov' -exec rm {} -fv \;
	make -C test/cmocka clean

.PHONY: clean
