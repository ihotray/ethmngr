#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <libubus.h>
#include <libubox/blobmsg_json.h>
#include <libubox/blobmsg.h>
#include <libubox/list.h>
#include <syslog.h>

#include <json-c/json.h>
#include <json-validator.h>
#include <json-c/json.h>
#include <json-editor.h>
#include <json-c/json_tokener.h>

#include <easy/easy.h>
#include "utils.h"

#define LIBETH_LOG_INPUT_PARAMS "/tmp/libeth-input-params.log"

struct test_ctx {
	struct blob_buf bb;
	struct ubus_object ethernet;
};

int ethmngr_ifstats(struct ubus_context *ctx, struct ubus_object *obj,
                        struct ubus_request_data *req, const char *method,
                        struct blob_attr *msg);

int ethmngr_rmonstats(struct ubus_context *ctx, struct ubus_object *obj,
		     struct ubus_request_data *req, const char *method,
		     struct blob_attr *msg);

/* Overload ubus_send_reply to prevent segment fault*/
int ubus_send_reply(struct ubus_context *ctx, struct ubus_request_data *req, struct blob_attr *msg)
{
        if (msg) {
                char *json_text;

                json_text = blobmsg_format_json_indent(msg, true, 0);
                //printf("%s\n", json_text);
                json_output = json_tokener_parse(json_text);
                free(json_text);
        }

        if (json_output)
                return UBUS_STATUS_OK;
        return UBUS_STATUS_NO_DATA;
}

static int group_setup(void **state)
{
	struct test_ctx *ctx = calloc(1, sizeof(struct test_ctx));
	if (!ctx)
		return -1;

	ctx->ethernet.name = "ethernet";
	memset(&ctx->bb, 0, sizeof(struct blob_buf));
	*state = ctx;
	return 0;
}

static int setup(void **state)
{
	struct test_ctx *ctx = (struct test_ctx *) *state;
	blob_buf_init(&ctx->bb, 0);
	unlink(LIBETH_LOG_INPUT_PARAMS);
	return 0;
}

static int teardown(void **state)
{
        if (json_output) {
                json_object_put(json_output); // Free the output JSON object
                json_output = NULL;
        }

        return 0;
}

static int group_teardown(void **state)
{
	struct test_ctx *ctx = (struct test_ctx *) *state;

	blob_buf_free(&ctx->bb);
	free(ctx);
	return 0;
}

static void test_api_ethernet_ifstats(void **state)
{
        struct test_ctx *ctx = (struct test_ctx *) *state;
        struct blob_buf *bb = &ctx->bb;
        struct ubus_object *obj = &ctx->ethernet;

        blobmsg_add_string(bb, "ifname", "eth0");
        int res = ethmngr_ifstats(NULL, obj, NULL, "ifstats", bb->head);
	assert_true(res == UBUS_STATUS_OK);

	validate_stats(json_output, 1);

        return;
}

static void test_api_ethernet_wrong_ifstats(void **state)
{
        struct test_ctx *ctx = (struct test_ctx *) *state;
        struct blob_buf *bb = &ctx->bb;
        struct ubus_object *obj = &ctx->ethernet;

        ethmngr_ifstats(NULL, obj, NULL, "ifstats", bb->head);
        assert_null(json_output);

        return;
}

static void test_api_ethernet_rmonstats(void **state)
{
        struct test_ctx *ctx = (struct test_ctx *) *state;
        struct blob_buf *bb = &ctx->bb;
        struct ubus_object *obj = &ctx->ethernet;

        blobmsg_add_string(bb, "ifname", "eth0");
        int res = ethmngr_rmonstats(NULL, obj, NULL, "rmonstats", bb->head);
	assert_true(res == UBUS_STATUS_OK);

	validate_stats(json_output, 0);
        return;
}

static void test_api_ethernet_wrong_rmonstats(void **state)
{
        struct test_ctx *ctx = (struct test_ctx *) *state;
        struct blob_buf *bb = &ctx->bb;
        struct ubus_object *obj = &ctx->ethernet;

        ethmngr_rmonstats(NULL, obj, NULL, "rmonstats", bb->head);
        assert_null(json_output);

        return;
}


static void test_api_ethernet_rmonstats_with_options(void **state)
{
        struct test_ctx *ctx = (struct test_ctx *) *state;
        struct blob_buf *bb = &ctx->bb;
        struct ubus_object *obj = &ctx->ethernet;

        blobmsg_add_string(bb, "ifname", "eth0");
	blobmsg_add_u32(bb, "vid", 0);
	blobmsg_add_u32(bb, "qid", 0);
        int res = ethmngr_rmonstats(NULL, obj, NULL, "rmonstats", bb->head);
	assert_true(res == UBUS_STATUS_OK);

	validate_stats(json_output, 0);
        return;
}

int main(void)
{
	const struct CMUnitTest tests[] = {
		// ethernet object test cases
		cmocka_unit_test_setup_teardown(test_api_ethernet_ifstats, setup, teardown),
		cmocka_unit_test_setup_teardown(test_api_ethernet_wrong_ifstats, setup, teardown),
		cmocka_unit_test_setup_teardown(test_api_ethernet_rmonstats, setup, teardown),
		cmocka_unit_test_setup_teardown(test_api_ethernet_wrong_rmonstats, setup, teardown),
		cmocka_unit_test_setup_teardown(test_api_ethernet_rmonstats_with_options, setup, teardown),
	};

	return cmocka_run_group_tests(tests, group_setup, group_teardown);
}

