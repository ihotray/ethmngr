/*
 * ethmngr.c - provides "ethernet" ubus objects
 *
 * Copyright (C) 2020 iopsys Software Solutions AB. All rights reserved.
 *
 * Author: Anjan Chanda <anjan.chanda@iopsys.eu>
 *
 * See LICENSE file for license related information.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <net/if.h>

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <json-c/json.h>
#include <libubox/blobmsg.h>
#include <libubox/blobmsg_json.h>
#include <libubox/uloop.h>
#include <libubox/ustream.h>
#include <libubox/utils.h>
#include <libubus.h>

#include <easy/easy.h>
#include <ethernet.h>


/* ifstats policy */
enum {
	IFSTATS_POLICY_IFNAME,
	IFSTATS_POLICY_MAX,
	NUM_IFSTATS_POLICY = IFSTATS_POLICY_MAX,
};

static const struct blobmsg_policy ifstats_policy[NUM_IFSTATS_POLICY] = {
	[IFSTATS_POLICY_IFNAME] = { .name = "ifname", .type = BLOBMSG_TYPE_STRING },
};


/* rmonstats policy */
enum {
	RMONSTATS_POLICY_IFNAME,
	RMONSTATS_POLICY_VLANID,
	RMONSTATS_POLICY_QID,
	RMONSTATS_POLICY_MAX,
	NUM_RMONSTATS_POLICY = RMONSTATS_POLICY_MAX,
};

static const struct blobmsg_policy rmonstats_policy[NUM_RMONSTATS_POLICY] = {
	[RMONSTATS_POLICY_IFNAME] = { .name = "ifname", .type = BLOBMSG_TYPE_STRING },
	[RMONSTATS_POLICY_VLANID] = { .name = "vid", .type = BLOBMSG_TYPE_INT32 },
	[RMONSTATS_POLICY_QID] = { .name = "qid", .type = BLOBMSG_TYPE_INT32 },

};


int ethmngr_ifstats(struct ubus_context *ctx, struct ubus_object *obj,
			struct ubus_request_data *req, const char *method,
			struct blob_attr *msg)
{
	struct blob_attr *tb[NUM_IFSTATS_POLICY];
	struct eth_stats s;
	char ifname[16] = {0};
	struct blob_buf bb;
	int ret;


	blobmsg_parse(ifstats_policy, IFSTATS_POLICY_MAX, tb, blob_data(msg),
							blob_len(msg));

	if (!tb[IFSTATS_POLICY_IFNAME]) {
		fprintf(stderr, "\"ifname\" not specified!\n");
		return UBUS_STATUS_INVALID_ARGUMENT;
	}

	strncpy(ifname, blobmsg_data(tb[IFSTATS_POLICY_IFNAME]), sizeof(ifname)-1);

	memset(&s, 0, sizeof(s));
	ret = eth_get_stats(ifname, &s);
	if (ret) {
		fprintf(stderr, "ethmngr: failed to get interface stats!\n");
		return ret;
	}

	memset(&bb, 0, sizeof(bb));
	blob_buf_init(&bb, 0);
	blobmsg_add_string(&bb, "ifname", ifname);
	blobmsg_add_u64(&bb, "tx_bytes", s.tx_bytes);
	blobmsg_add_u64(&bb, "tx_packets", s.tx_packets);
	blobmsg_add_u64(&bb, "tx_errors", s.tx_errors);
	blobmsg_add_u64(&bb, "tx_unicast_packets", s.tx_ucast_packets);
	blobmsg_add_u64(&bb, "tx_multicast_packets", s.tx_mcast_packets);
	blobmsg_add_u64(&bb, "tx_broadcast_packets", s.tx_bcast_packets);
	blobmsg_add_u64(&bb, "tx_discard_packets", s.tx_discard_packets);

	blobmsg_add_u64(&bb, "rx_bytes", s.rx_bytes);
	blobmsg_add_u64(&bb, "rx_packets", s.rx_packets);
	blobmsg_add_u64(&bb, "rx_errors", s.rx_errors);
	blobmsg_add_u64(&bb, "rx_unicast_packets", s.rx_ucast_packets);
	blobmsg_add_u64(&bb, "rx_multicast_packets", s.rx_mcast_packets);
	blobmsg_add_u64(&bb, "rx_broadcast_packets", s.rx_bcast_packets);
	blobmsg_add_u64(&bb, "rx_discard_packets", s.rx_discard_packets);
	blobmsg_add_u64(&bb, "rx_unknown_packets", s.rx_unknown_packets);

	ubus_send_reply(ctx, req, bb.head);
	blob_buf_free(&bb);

	return 0;
}

int ethmngr_rmonstats(struct ubus_context *ctx, struct ubus_object *obj,
			struct ubus_request_data *req, const char *method,
			struct blob_attr *msg)
{
	struct blob_attr *tb[NUM_RMONSTATS_POLICY];
	struct eth_rmon_stats rmon;
	char ifname[16] = {0};
	struct blob_buf bb;
	int queueid = ETH_TXQ_ALL;
	int vlanid = 0;
	int ret;

	blobmsg_parse(rmonstats_policy, RMONSTATS_POLICY_MAX, tb,
					blob_data(msg), blob_len(msg));

	if (!tb[RMONSTATS_POLICY_IFNAME]) {
		fprintf(stderr, "\"ifname\" not specified!\n");
		return UBUS_STATUS_INVALID_ARGUMENT;
	}

	strncpy(ifname, blobmsg_data(tb[RMONSTATS_POLICY_IFNAME]), sizeof(ifname)-1);

	if (tb[RMONSTATS_POLICY_QID])
		queueid = (int)blobmsg_get_u32(tb[RMONSTATS_POLICY_QID]);


	if (tb[RMONSTATS_POLICY_VLANID])
		vlanid = (int)blobmsg_get_u32(tb[RMONSTATS_POLICY_VLANID]);

	if (vlanid < 0 || vlanid > 0xFFFF) {
		fprintf(stderr, "ethmongr: VLAN ID is out of range\n");
		return -1;
	}

	memset(&rmon, 0, sizeof(rmon));
	rmon.txq = queueid;
	rmon.vlanid = (uint16_t)vlanid;

	ret = eth_get_rmon_stats(ifname, &rmon);
	if (ret != 0) {
		fprintf(stderr, "ethmngr: failed to get rmonstats!\n");
		return ret;
	}

	memset(&bb, 0, sizeof(bb));
	blob_buf_init(&bb, 0);
	blobmsg_add_string(&bb, "ifname", ifname);

	blobmsg_add_u32(&bb, "tx_priority_q", (uint32_t)rmon.txq);
	blobmsg_add_u64(&bb, "tx_bytes", rmon.tx.bytes);
	blobmsg_add_u64(&bb, "tx_packets", rmon.tx.packets);
	blobmsg_add_u64(&bb, "tx_broadcast_packets", rmon.tx.bcast_packets);
	blobmsg_add_u64(&bb, "tx_multicast_packets", rmon.tx.mcast_packets);
	blobmsg_add_u64(&bb, "tx_crc_error_packets", rmon.tx.crc_err_packets);
	blobmsg_add_u64(&bb, "tx_undersize_packets", rmon.tx.under_sz_packets);
	blobmsg_add_u64(&bb, "tx_oversize_packets", rmon.tx.over_sz_packets);
	blobmsg_add_u64(&bb, "tx_packets_64bytes", rmon.tx.packets_64bytes);
	blobmsg_add_u64(&bb, "tx_packets_65to127bytes", rmon.tx.packets_65to127bytes);
	blobmsg_add_u64(&bb, "tx_packets_128to255bytes", rmon.tx.packets_128to255bytes);
	blobmsg_add_u64(&bb, "tx_packets_256to511bytes", rmon.tx.packets_256to511bytes);
	blobmsg_add_u64(&bb, "tx_packets_512to1023bytes", rmon.tx.packets_512to1023bytes);
	blobmsg_add_u64(&bb, "tx_packets_1024to1518bytes", rmon.tx.packets_1024to1518bytes);

	blobmsg_add_u64(&bb, "rx_bytes", rmon.rx.bytes);
	blobmsg_add_u64(&bb, "rx_packets", rmon.rx.packets);
	blobmsg_add_u64(&bb, "rx_broadcast_packets", rmon.rx.bcast_packets);
	blobmsg_add_u64(&bb, "rx_multicast_packets", rmon.rx.mcast_packets);
	blobmsg_add_u64(&bb, "rx_crc_error_packets", rmon.rx.crc_err_packets);
	blobmsg_add_u64(&bb, "rx_undersize_packets", rmon.rx.under_sz_packets);
	blobmsg_add_u64(&bb, "rx_oversize_packets", rmon.rx.over_sz_packets);
	blobmsg_add_u64(&bb, "rx_packets_64bytes", rmon.rx.packets_64bytes);
	blobmsg_add_u64(&bb, "rx_packets_65to127bytes", rmon.rx.packets_65to127bytes);
	blobmsg_add_u64(&bb, "rx_packets_128to255bytes", rmon.rx.packets_128to255bytes);
	blobmsg_add_u64(&bb, "rx_packets_256to511bytes", rmon.rx.packets_256to511bytes);
	blobmsg_add_u64(&bb, "rx_packets_512to1023bytes", rmon.rx.packets_512to1023bytes);
	blobmsg_add_u64(&bb, "rx_packets_1024to1518bytes", rmon.rx.packets_1024to1518bytes);

	ubus_send_reply(ctx, req, bb.head);
	blob_buf_free(&bb);

	return 0;
}

struct ubus_method ethmngr_methods[] = {
	UBUS_METHOD("ifstats", ethmngr_ifstats, ifstats_policy),
	UBUS_METHOD("rmonstats", ethmngr_rmonstats, rmonstats_policy),
};

struct ubus_object_type ethmngr_type =
	UBUS_OBJECT_TYPE("ethernet", ethmngr_methods);

struct ubus_object ethmngr_object = {
	.name = "ethernet",
	.type = &ethmngr_type,
	.methods = ethmngr_methods,
	.n_methods = ARRAY_SIZE(ethmngr_methods),
};


static int ethmngr_publish_object(struct ubus_context *ctx)
{
	int ret;

	ret = ubus_add_object(ctx, &ethmngr_object);
	if (ret)
		fprintf(stderr, "Failed to add 'ethernet' ubus object: %s\n",
				ubus_strerror(ret));

	return ret;
}

static void ethmngr_sighandler(int sig)
{
	switch (sig) {
	case SIGTERM:
	case SIGINT:
		exit(0);
		break;
	default:
		break;
	}
}

#define MAX_EVENT_RESPONSE_LEN		512

struct ethmngr_event {
	struct uloop_fd uloop_fd;
	struct event_struct event;
	void *handle;
};

static int ethmngr_ubus_send_event(struct ubus_context *ctx, char *message)
{
	char event[32] = {0};
	char data[512] = {0};
	struct blob_buf b;

	sscanf(message, "%31s '%511[^\n]s'", event, data);

	/* ignore non ethernet events */
	if (!strstr(event, "eth"))
		return 0;

	memset(&b, 0, sizeof(b));
	blob_buf_init(&b, 0);

	if (!blobmsg_add_json_from_string(&b, data)) {
		fprintf(stderr, "Failed to parse message data: %s\n", data);
		return -1;
	}

	ubus_send_event(ctx, event, b.head);
	blob_buf_free(&b);

	return 0;
}

int ethmngr_event_handler(struct event_struct *e)
{
	struct ubus_context *ctx = (struct ubus_context *)e->priv;
	struct event_response *resp = &e->resp;
	char evtbuf[512] = {0};

	strncpy(evtbuf, (char *)resp->data, resp->len);
	ethmngr_ubus_send_event(ctx, evtbuf);

	return 0;
}

static void ethmngr_event_recv(struct uloop_fd *fd, unsigned int events)
{
	struct ethmngr_event *ev =
			container_of(fd, struct ethmngr_event, uloop_fd);

	for (;;) {
		int err;
		err = easy_recv_event(ev->handle);
		if (err < 0)
			break;
	}
}

int ethmngr_event_setup(struct ubus_context *ctx, const char *family,
			const char *group)
{
	struct ethmngr_event *ev;

	ev = calloc(1, sizeof(*ev));
	if (!ev) {
		fprintf(stderr, "calloc(ev) failed\n");
		return -1;
	}

	strncpy(ev->event.family, family, 32);
	strncpy(ev->event.group, group, 32);
	ev->event.priv = ctx;
	ev->event.cb = ethmngr_event_handler;
	ev->event.resp.data = calloc(MAX_EVENT_RESPONSE_LEN, sizeof(uint8_t));
	if (ev->event.resp.data == NULL) {
		free(ev);
		fprintf(stderr, "%s %s %s calloc(resp) failed\n",
			ev->event.ifname, ev->event.family, ev->event.group);
		return -1;
	}

	ev->event.resp.len = ev->event.resp.data ? MAX_EVENT_RESPONSE_LEN : 0;

	if (easy_register_event(&ev->event, &ev->handle)) {
		fprintf(stderr, "easy_register_revent() failed! (%s, %s)\n",
			ev->event.family, ev->event.group);
		free(ev->event.resp.data);
		free(ev);
		return -1;
	}

	ev->uloop_fd.fd = easy_get_event_fd(ev->handle);
	ev->uloop_fd.cb = ethmngr_event_recv;
	if (uloop_fd_add(&ev->uloop_fd, ULOOP_READ | ULOOP_EDGE_TRIGGER)) {
		fprintf(stderr, "uloop_fd_add(%d) failed\n", ev->uloop_fd.fd);
		easy_unregister_event(ev->handle);
		free(ev->event.resp.data);
		free(ev);
		return -1;
	}

	return 0;
}

int main(int argc, char **argv)
{
	struct ubus_context *ctx = NULL;
	const char *ubus_socket = NULL;
	int ret;
	int ch;

	while ((ch = getopt(argc, argv, "s:")) != -1) {
		switch (ch) {
		case 's':
			ubus_socket = optarg;
			break;
		default:
			break;
		}
	}

	argc -= optind;
	argv += optind;

	uloop_init();
	ctx = ubus_connect(ubus_socket);
	if (!ctx) {
		fprintf(stderr, "Failed to connect to ubus\n");
		return -1;
	}

	ubus_add_uloop(ctx);

	ethmngr_event_setup(ctx, "easysoc", "notify");

	ret = ethmngr_publish_object(ctx);
	if (ret)
		goto out;

	set_sighandler(SIGPIPE, SIG_DFL);
	set_sighandler(SIGINT, ethmngr_sighandler);
	set_sighandler(SIGTERM, ethmngr_sighandler);

	uloop_run();
out:
	ubus_free(ctx);
	uloop_done();

	return 0;
}
