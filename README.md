# Ethernet Manager

[ethmngr](https://dev.iopsys.eu/iopsys/ethmngr)

## Introduction

`ethmngr` implements fetching statistics, publishing the ethernet object over ubus.
It helps in getting port and interface stats. In order to remain agnostic to the vendor
and underlying modules present in the system, *ethmngr* uses the easy-soc-libs libethernet
APIs to expose supported methods within its objects.

## Project Components

Project consists of following components:

- Application itself written in C programming language
- Documentation in a Markdown format

## Build Instructions

`ethmngr` is written using C programming language and depends on a number of components found in `OpenWrt` for building and running.

## uBus API

`ethmngr` needs to be started on startup after `ubusd`, as it exposes the ethernet
objects over `ubus`. By default `ethmngr` registers below namespace with `ubus`.

```
root@iopsys:~# ubus list |grep ethernet
ethernet
```
Default namespace with functionalities:

```
root@iopsys:~# ubus -v list ethernet
'ethernet' @2954fe5b
	"ifstats":{"ifname":"String"}
	"rmonstats":{"ifname":"String","vid":"Integer","qid":"Integer"}
root@iopsys:~# 
```

Registered method can be called with appropriate parameters, like:

```
root@iopsys:~# ubus call ethernet rmonstats '{"ifname":"eth0"}'
{
	"ifname": "eth0",
	"tx_priority_q": -1,
	"tx_bytes": 3519036,
	"tx_packets": 13332,
	"tx_broadcast_packets": 8927,
	"tx_multicast_packets": 4405,
	"tx_crc_error_packets": 0,
	"tx_undersize_packets": 0,
	"tx_oversize_packets": 0,
	"tx_packets_64bytes": 0,
	"tx_packets_65to127bytes": 4399,
	"tx_packets_128to255bytes": 6,
	"tx_packets_256to511bytes": 8927,
	"tx_packets_512to1023bytes": 0,
	"tx_packets_1024to1518bytes": 0,
	"rx_bytes": 592,
	"rx_packets": 8,
	"rx_broadcast_packets": 0,
	"rx_multicast_packets": 8,
	"rx_crc_error_packets": 0,
	"rx_undersize_packets": 0,
	"rx_oversize_packets": 0,
	"rx_packets_64bytes": 0,
	"rx_packets_65to127bytes": 8,
	"rx_packets_128to255bytes": 0,
	"rx_packets_256to511bytes": 0,
	"rx_packets_512to1023bytes": 0,
	"rx_packets_1024to1518bytes": 0
}

root@iopsys:~# ubus call ethernet ifstats '{"ifname":"eth0"}'
{
	"ifname": "eth0",
	"tx_bytes": 3524572,
	"tx_packets": 13348,
	"tx_errors": 0,
	"tx_unicast_packets": 0,
	"tx_multicast_packets": 4405,
	"tx_broadcast_packets": 8943,
	"tx_discard_packets": 0,
	"rx_bytes": 592,
	"rx_packets": 8,
	"rx_errors": 0,
	"rx_unicast_packets": 0,
	"rx_multicast_packets": 8,
	"rx_broadcast_packets": 0,
	"rx_discard_packets": 0,
	"rx_unknown_packets": 0
}
```

## Overview

`ethmngr` provides below commands:

 1. ifstats
 2. rmonstats

### 1. ifstats
API to query interface stats.

```
root@iopsys:~# ubus call ethernet ifstats '{"ifname":"eth0"}'
{
	"ifname": "eth0",
	"tx_bytes": 3524572,
	"tx_packets": 13348,
	"tx_errors": 0,
	"tx_unicast_packets": 0,
	"tx_multicast_packets": 4405,
	"tx_broadcast_packets": 8943,
	"tx_discard_packets": 0,
	"rx_bytes": 592,
	"rx_packets": 8,
	"rx_errors": 0,
	"rx_unicast_packets": 0,
	"rx_multicast_packets": 8,
	"rx_broadcast_packets": 0,
	"rx_discard_packets": 0,
	"rx_unknown_packets": 0
}

```
- For more info on the `ethernet` ubus API see [link](./docs/api/ethernet.md#ifstats)

### 2. rmonstats
API to query port stats.

```
root@iopsys:~# ubus call ethernet rmonstats '{"ifname":"eth0", "qid":0, "vid":0}'
{
	"ifname": "eth0",
	"tx_priority_q": 0,
	"tx_bytes": 3570124,
	"tx_packets": 13522,
	"tx_broadcast_packets": 9058,
	"tx_multicast_packets": 4464,
	"tx_crc_error_packets": 0,
	"tx_undersize_packets": 0,
	"tx_oversize_packets": 0,
	"tx_packets_64bytes": 0,
	"tx_packets_65to127bytes": 4458,
	"tx_packets_128to255bytes": 6,
	"tx_packets_256to511bytes": 9058,
	"tx_packets_512to1023bytes": 0,
	"tx_packets_1024to1518bytes": 0,
	"rx_bytes": 592,
	"rx_packets": 8,
	"rx_broadcast_packets": 0,
	"rx_multicast_packets": 8,
	"rx_crc_error_packets": 0,
	"rx_undersize_packets": 0,
	"rx_oversize_packets": 0,
	"rx_packets_64bytes": 0,
	"rx_packets_65to127bytes": 8,
	"rx_packets_128to255bytes": 0,
	"rx_packets_256to511bytes": 0,
	"rx_packets_512to1023bytes": 0,
	"rx_packets_1024to1518bytes": 0
}

```
- For more info on the `ethernet` ubus API see [link](./docs/api/ethernet.md#rmonstats)

## Tests

This section gives a brief overview of the tests for *ethmngr*, for a detailed report see
[test specification](./docs/testspec.md)

To test *ethmngr*, the scope of the tests has to be clearly defined, as *ethmngr*
is heavily dependent on libethernet:

1. Verify linkage between *ethmngr* and libethernet APIs
2. Verify that the structures are correctly prepared and returned data is
used correctly by *ethmngr*
3. Verify that API calls successfully reach libethernet and if passed input is of
correct format

As the test environment runs in a Ubuntu Docker environment, with little
possibility to prepare ethernet drivers and kernel version, the easy-soc-libs has to
be extended to support APIs to run on a test platform, returning dummy data for
getters.

To ensure full coverage, the getters are tested by both unit-test and functional-api test.

## Dependencies ##

To successfully build ethmngr, the following libraries are needed:

| Dependency		| Link								| License	|
| -----------------	| ------------------------------------------------------------- | --------------|
| libuci		| https://git.openwrt.org/project/uci.git			| LGPL 2.1	|
| libubox		| https://git.openwrt.org/project/libubox.git			| BSD		|
| libubus		| https://git.openwrt.org/project/ubus.git			| LGPL 2.1	|
| libjson-c		| https://s3.amazonaws.com/json-c_releases			| MIT		|
| libnl3		|								|		|
| libblobmsg_json	|								|		|                                     
| libnl-genl		|								|		|
| libethernet		|								|		|

### Run-Time Dependencies

In order to run the `ethmngr`, following dependencies are needed to be running/available before `ethmngr`.

| Dependency |                   Link                   | License  |
| ---------- | ---------------------------------------- | -------- |
| ubusd      | https://git.openwrt.org/project/ubus.git | LGPL 2.1 |

System daemon `ethmngr` is used to expose the ethernet functionality over `ubus`.
