CC		= gcc
ETHMNGR_LIB_DIR	?= $(shell dirname $(PWD))
ETHMNGR_LIB	= -lethmngr -L$(ETHMNGR_LIB_DIR)
CMOCKA_LIB	= -l cmocka
LIBS		= $(ETHMNGR_LIB) $(CMOCKA_LIB) -pthread -luci -lubus -lubox -ljson-c -lblobmsg_json -lnl-genl-3 -lnl-3 -ljson-validator -ljson-schema-validator -ljson-editor -lethernet
CFLAGS		= -g -Wall -I../..
LDFLAGS		= $(LIBS) -Wl,-rpath=$(ETHMNGR_LIB_DIR) -I$(ETHMNGR_LIB_DIR)
UNIT_TESTS	= unit_test_ethmngr
FUNCTIONAL_TESTS	= functional_test_ethmngr
UTILS           = utils.o


VALGRIND	= valgrind --leak-check=full --show-reachable=no \
	--show-leak-kinds=all --errors-for-leak-kinds=all \
	--error-exitcode=1 --track-origins=yes

unit_test_ethmngr: $(UTILS) unit_test_ethmngr.o
	$(CC) -o $@ $^ $(LDFLAGS)

functional_test_ethmngr: $(UTILS) functional_test_ethmngr.o
	$(CC) -o $@ $^ $(LDFLAGS)

unit-test: $(UNIT_TESTS)
	$(foreach testprog, $(UNIT_TESTS), sudo $(VALGRIND) ./$(testprog);)

functional-test: $(FUNCTIONAL_TESTS)
	$(foreach testprog, $(FUNCTIONAL_TESTS), sudo $(VALGRIND) ./$(testprog);)

.PHONY: clean
clean:
	rm $(UNIT_TESTS) $(FUNCTIONAL_TESTS) *.o -fv
