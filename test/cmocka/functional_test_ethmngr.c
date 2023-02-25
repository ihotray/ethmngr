#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <errno.h>
#include <cmocka.h>

#include <libubus.h>
#include <libubox/blobmsg_json.h>
#include <libubox/blobmsg.h>

#include <json-validator.h>
#include <json-c/json.h>
#include <json-editor.h>
#include <json-c/json_tokener.h>

#include "utils.h"

#define UBUS_OUTPUT_FILE "/tmp/ethmngr_ubus_output.txt"

struct json_object *json_output = NULL;

static int group_setup(void **state)
{
	// Start ubusd if it is not running
	system("pidof ubusd || ubusd &");

	// Restart ethmngr
	system("kill -9 $(pidof ethmngr) 2>/dev/null; ../../ethmngr &");

	return 0;
}

static int group_teardown(void **state)
{
	// Stop ethmngr
	system("kill -9 $(pidof ethmngr)");

	// Delete test logs
	unlink(UBUS_OUTPUT_FILE);

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

static void get_ubus_call_output(const char *ubus_cmd)
{
	char cmd[256];
	int fd = -1;
	char *str = NULL;

	// Put the output of ubus call into a string
	snprintf(cmd, sizeof(cmd), "%s > %s", ubus_cmd, UBUS_OUTPUT_FILE);
	int rc = system(cmd);
	assert_return_code(rc, errno);

	fd = open(UBUS_OUTPUT_FILE, O_RDONLY);
	assert_return_code(fd, errno);

	struct stat st;
	rc = fstat(fd, &st);
	assert_true(rc == 0 && st.st_size > 0);

	str = calloc(1, (size_t)st.st_size + 1);
	assert_non_null(str);

	ssize_t read_len = read(fd, str, (size_t)st.st_size);
	assert_int_equal((int)read_len, (int)st.st_size);

	// Parse the string to a json_object
	//puts(str);
	json_output = json_tokener_parse(str);

	if (fd >= 0)
		close(fd);
	if (str)
		free(str);
}

static void test_ubus_ifstats(void **state)
{
	get_ubus_call_output("ubus call ethernet ifstats '{\"ifname\":\"eth0\"}'");
	validate_stats(json_output, 1);
}

static void test_ubus_rmonstats(void **state)
{
	get_ubus_call_output("ubus call ethernet rmonstats '{\"ifname\":\"eth0\"}'");
	validate_stats(json_output, 0);
}

int main(void)
{
	const struct CMUnitTest tests[] = {
		cmocka_unit_test_teardown(test_ubus_ifstats, teardown),
		cmocka_unit_test_teardown(test_ubus_rmonstats, teardown),
	};

	return cmocka_run_group_tests(tests, group_setup, group_teardown);
}
