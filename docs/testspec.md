# Test Specification

Most of the functionality in *ethmngr* can be tested via its UBUS API. Each
API can be broken down into an individual test case to show full coverage is
achieved.

## Prerequisites

The only prerequisite for the *ethmngr* test suites is that *libethernet* has to be
built for the TEST platform, returning dummy data for getter APIs.

## Test Suites

The *ethmngr* build pipe has three test suites, a functional-api suite, a unit
test suite and a functional test suite.

### Functional API Tests

The functional API test suite consists of two test cases verifying output of
ifstats and rmonstats returned by UBUS calls. *ethmngr* and ubusd are started
before the whole test cases' invoke. Each test case issues a UBUS call and saves
the output to a temporary file. Then the contents of the file are analyzed by
using JSON-C library APIs.

### ethernet

| Execution ID	| Method	| Description			| Function ID Coverage			|
| :---		| :---		| :---				| :---					|
| 1		| ifstats	| ifname argument		| [1](./api/ethernet.md#ifstats)	|
| 2		| rmonstats	| ifname,qid,vid argument	| [2](./api/ethernet.md#rmonstats)	|

### Unit Tests

The *ethmngr* unit tests are written in CMocka, invoking the UBUS callbacks directly from the source code,
which is compiled into a shared library. This means mocking the arguments of a CLI or 
libubus invoke in a `struct blob_attr *`.

The results of the call will be logged to the logfile. Then the output is analyzed and verified by each test case.

| Execution ID	| Method		| Test Case Name				| Function ID Coverage			|
| :---		| :---			| :---						| :---					|
| 1		| ifstats		| test_api_ethernet_ifstats			| [1](./api/ethernet.md#ifstats)	|
| 2		| wrong_ifstats		| test_api_ethernet_wrong_ifstats		| [2](./api/ethernet.md#ifstats)	|
| 3		| rmonifstats		| test_api_ethernet_rmonstats			| [3](./api/ethernet.md#rmonstats)	|
| 4		| wrong_rmonstats	| test_api_ethernet_wrong_rmonstats		| [4](./api/ethernet.md#rmonstats)	|
| 5		| rmonstats_options	| test_api_ethernet_rmonstats_with_options	| [5](./api/ethernet.md#rmonstats)	|

#### test_api_ethernet_ifstats

##### Description

Test the *ethmngr* UBUS API `ifstats`, publishing the method [ifstats](./api/ethernet.md#ifstats).

##### Test Steps

Prepare the arguments as:

```
{"ifname":"eth0"}
```
- Analyze and verify the output

##### Expected Results

The expected result is for the log file to have recorded a call to the
`libethernet`, through ubus and with the argument `ifname` as `eth0.`

Parameters' values included in the returned JSON object of the output
shall be equal to those returned by the corresponding stub API in 
*libethernet* building for TEST platform.

```
{
        "ifname": "eth0",
        "tx_bytes": 100,
        "tx_packets": 900,
        "tx_errors": 10,
        "tx_unicast_packets": 1,
        "tx_multicast_packets": 3,
        "tx_broadcast_packets": 5,
        "tx_discard_packets": 7,
        "rx_bytes": 1200,
        "rx_packets": 100,
        "rx_errors": 20,
        "rx_unicast_packets": 2,
        "rx_multicast_packets": 4,
        "rx_broadcast_packets": 6,
        "rx_discard_packets": 8,
        "rx_unknown_packets": 9
}
```
#### test_api_ethernet_wrong_ifstats

##### Description

Test the *ethmngr* UBUS API `ifstats`, publishing the method [ifstats](./api/ethernet.md#ifstats).

##### Test Steps

- There will be no arguement.
- Analyze and verify the output

##### Expected Results

The expected result is that without arguement, JSON object of the output
will be NULL.

#### test_api_ethernet_rmonstats

##### Description

Test the *ethmngr* UBUS API `rmonstats`, publishing the method [rmonstats](./api/ethernet.md#rmonstats).

##### Test Steps

Prepare the arguments as:

```
{"ifname":"eth0"}
```
- Analyze and verify the output

##### Expected Results

The expected result is for the log file to have recorded a call to the
`libethernet`, through ubus and with the argument `ifname` as `eth0.`

Parameters' values included in the returned JSON object of the output
shall be equal to those returned by the corresponding stub API in 
*libethernet* building for TEST platform.

```

        "ifname": "eth0",
        "tx_priority_q": -1,
        "tx_bytes": 230000,
        "tx_packets": 355000,
        "tx_broadcast_packets": 2300,
        "tx_multicast_packets": 3,
        "tx_crc_error_packets": 43,
        "tx_undersize_packets": 2,
        "tx_oversize_packets": 300,
        "tx_packets_64bytes": 900000,
        "tx_packets_65to127bytes": 8200,
        "tx_packets_128to255bytes": 120000,
        "tx_packets_256to511bytes": 2400,
        "tx_packets_512to1023bytes": 100000,
        "tx_packets_1024to1518bytes": 27000,
        "rx_bytes": 12000,
        "rx_packets": 800000,
        "rx_broadcast_packets": 3,
        "rx_multicast_packets": 4,
        "rx_crc_error_packets": 6000,
        "rx_undersize_packets": 24,
        "rx_oversize_packets": 4500,
        "rx_packets_64bytes": 6000,
        "rx_packets_65to127bytes": 41000,
        "rx_packets_128to255bytes": 3000000,
        "rx_packets_256to511bytes": 4500,
        "rx_packets_512to1023bytes": 560000,
        "rx_packets_1024to1518bytes": 34000

```
#### test_api_ethernet_wrong_rmonstats

##### Description

Test the *ethmngr* UBUS API `rmonstats`, publishing the method [rmonstats](./api/ethernet.md#rmonstats).

##### Test Steps

- There will be no arguement.
- Analyze and verify the output

##### Expected Results

The expected result is that without arguement, JSON object of the output
will be NULL.

#### test_api_ethernet_rmonstats_with_options

##### Description

Test the *ethmngr* UBUS API `rmonstats`, publishing the method [rmonstats](./api/ethernet.md#rmonstats).

##### Test Steps

Prepare the arguments as:

```
{"ifname":"eth0", "qid": 0, "vid":0}
```
- Analyze and verify the output

##### Expected Results

The expected result is for the log file to have recorded a call to the
`libethernet`, through ubus and with the argument `ifname` as `eth0.`,
`qid` as `0`, `vid` as `0`.

Parameters' values included in the returned JSON object of the output
shall be equal to those returned by the corresponding stub API in 
*libethernet* building for TEST platform.

```

        "ifname": "eth0",
        "tx_priority_q": 0,
        "tx_bytes": 230000,
        "tx_packets": 355000,
        "tx_broadcast_packets": 2300,
        "tx_multicast_packets": 3,
        "tx_crc_error_packets": 43,
        "tx_undersize_packets": 2,
        "tx_oversize_packets": 300,
        "tx_packets_64bytes": 900000,
        "tx_packets_65to127bytes": 8200,
        "tx_packets_128to255bytes": 120000,
        "tx_packets_256to511bytes": 2400,
        "tx_packets_512to1023bytes": 100000,
        "tx_packets_1024to1518bytes": 27000,
        "rx_bytes": 12000,
        "rx_packets": 800000,
        "rx_broadcast_packets": 3,
        "rx_multicast_packets": 4,
        "rx_crc_error_packets": 6000,
        "rx_undersize_packets": 24,
        "rx_oversize_packets": 4500,
        "rx_packets_64bytes": 6000,
        "rx_packets_65to127bytes": 41000,
        "rx_packets_128to255bytes": 3000000,
        "rx_packets_256to511bytes": 4500,
        "rx_packets_512to1023bytes": 560000,
        "rx_packets_1024to1518bytes": 34000
```
