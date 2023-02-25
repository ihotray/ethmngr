#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include <libubus.h>
#include <libubox/blobmsg_json.h>
#include <libubox/blobmsg.h>

#include <json-validator.h>
#include <json-c/json.h>
#include <json-editor.h>
#include "utils.h"

#define JSON_GET_OBJ(parent_obj, child_obj, child_name) do { \
                        (child_obj) = json_object_get_by_string((parent_obj), (child_name)); \
                        assert_non_null(child_obj); \
                } while(0)

static const char *if_stats[] = { "tx_bytes", "tx_packets", "tx_errors", "tx_unicast_packets", "tx_multicast_packets", 
			"tx_broadcast_packets", "tx_discard_packets", "rx_bytes", "rx_packets", "rx_errors",
			"rx_unicast_packets", "rx_multicast_packets", "rx_broadcast_packets",
			"rx_discard_packets", "rx_unknown_packets", NULL };

static const char *rmon_stats[] = { "tx_bytes", "tx_packets", "tx_broadcast_packets", "tx_multicast_packets", "tx_crc_error_packets", 
			"tx_undersize_packets", "tx_oversize_packets", "tx_packets_64bytes", "tx_packets_65to127bytes",
			"tx_packets_128to255bytes", "tx_packets_256to511bytes", "tx_packets_512to1023bytes",
			"tx_packets_1024to1518bytes", "rx_bytes", "rx_packets", "rx_broadcast_packets",
			"rx_multicast_packets", "rx_crc_error_packets", "rx_undersize_packets", "rx_oversize_packets",
			"rx_packets_64bytes", "rx_packets_65to127bytes", "rx_packets_128to255bytes", 
			"rx_packets_256to511bytes", "rx_packets_512to1023bytes", "rx_packets_1024to1518bytes", NULL };

void validate_stats(struct json_object *json_obj, int ifstats)
{
        struct json_object *tmp;
        const char **counter;

        assert_non_null(json_obj);

	if (ifstats == 1)
		counter = if_stats;
	else
		counter = rmon_stats;

	for (counter ; *counter != NULL; counter++) {
		JSON_GET_OBJ(json_obj, tmp, (char *)*counter);
		assert_true(json_object_get_int(tmp) > 0);
	}
}
