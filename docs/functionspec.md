# Function Specification

This package can be used provide statistics about the ethernet interfaces and 
ports through UBUS. It uses APIs from the libethernet.

```
root@iopsys:~# ubus -v list ethernet
'ethernet' @2954fe5b
	"ifstats":{"ifname":"String"}
	"rmonstats":{"ifname":"String","vid":"Integer","qid":"Integer"}
```
# Contents
* [ethernet](#ethernet)

## APIs

Ethmngr publishes one object, `ethernet`.

### ethernet

An object that publishes interface and ports stats information.

```
	"ifstats":{"ifname":"String"}
	"rmonstats":{"ifname":"String","vid":"Integer","qid":"Integer"}
```

| Method					|Function ID			|
| :---						| :---				|
| [ifstats](#ifstats)				| 1				|
| [rmonstats](#rmonstats)			| 2				|

#### Methods

Method descriptions of the `ethernet` object.

##### ifstats

Exposes data model schema registered.

* [ifstats documentation](./api/ethernet.md#ifstats)

##### rmonstats

Exposes data model schema registered.

* [rmonstats documentation](./api/ethernet.md#rmonstats)
