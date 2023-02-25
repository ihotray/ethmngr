#include <libubus.h>
#include <libubox/blobmsg.h>

struct json_object *json_output;
void validate_stats(struct json_object *json_obj, int ifstats);
