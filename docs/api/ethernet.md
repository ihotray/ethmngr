# ethernet Schema

```
http://example.com/root.json
```

| Custom Properties | Additional Properties |
| ----------------- | --------------------- |
| Forbidden         | Permitted             |

# ethernet

| List of Methods         |
| ----------------------- |
| [ifstats](#ifstats)     | Method | ethernet (this schema) |
| [rmonstats](#rmonstats) | Method | ethernet (this schema) |
| `*`                     | any | Additional | Yes | this schema _allows_ additional properties |

## ifstats

`ifstats`

- type: `Method`

### ifstats Type

`object` with following properties:

| Property | Type   | Required |
| -------- | ------ | -------- |
| `input`  | object | Optional |
| `output` | object | Optional |

#### input

`input`

- is optional
- type: `object`

##### input Type

`object` with following properties:

| Property | Type   | Required     |
| -------- | ------ | ------------ |
| `ifname` | string | **Required** |

#### ifname

##### Interface

`ifname`

- is **required**
- type: reference

##### ifname Type

`string`

- minimum length: 1 characters
- maximum length: 16 characters

### Ubus CLI Example

```
ubus call ethernet ifstats {"ifname":"anim est id lab"}
```

### JSONRPC Example

```json
{
  "jsonrpc": "2.0",
  "id": 0,
  "method": "call",
  "params": ["<SID>", "ethernet", "ifstats", { "ifname": "anim est id lab" }]
}
```

#### output

`output`

- is optional
- type: `object`

##### output Type

`object` with following properties:

| Property               | Type    | Required     |
| ---------------------- | ------- | ------------ |
| `ifname`               | string  | **Required** |
| `rx_broadcast_packets` | integer | **Required** |
| `rx_bytes`             | integer | **Required** |
| `rx_discard_packets`   | integer | **Required** |
| `rx_errors`            | integer | **Required** |
| `rx_multicast_packets` | integer | **Required** |
| `rx_packets`           | integer | **Required** |
| `rx_unicast_packets`   | integer | **Required** |
| `rx_unknown_packets`   | integer | **Required** |
| `tx_broadcast_packets` | integer | **Required** |
| `tx_bytes`             | integer | **Required** |
| `tx_discard_packets`   | integer | **Required** |
| `tx_errors`            | integer | **Required** |
| `tx_multicast_packets` | integer | **Required** |
| `tx_packets`           | integer | **Required** |
| `tx_unicast_packets`   | integer | **Required** |

#### ifname

##### Interface

`ifname`

- is **required**
- type: reference

##### ifname Type

`string`

- minimum length: 1 characters
- maximum length: 16 characters

#### rx_broadcast_packets

`rx_broadcast_packets`

- is **required**
- type: `integer`

##### rx_broadcast_packets Type

`integer`

- minimum value: `0`

#### rx_bytes

`rx_bytes`

- is **required**
- type: `integer`

##### rx_bytes Type

`integer`

- minimum value: `0`

#### rx_discard_packets

`rx_discard_packets`

- is **required**
- type: `integer`

##### rx_discard_packets Type

`integer`

- minimum value: `0`

#### rx_errors

`rx_errors`

- is **required**
- type: `integer`

##### rx_errors Type

`integer`

- minimum value: `0`

#### rx_multicast_packets

`rx_multicast_packets`

- is **required**
- type: `integer`

##### rx_multicast_packets Type

`integer`

- minimum value: `0`

#### rx_packets

`rx_packets`

- is **required**
- type: `integer`

##### rx_packets Type

`integer`

- minimum value: `0`

#### rx_unicast_packets

`rx_unicast_packets`

- is **required**
- type: `integer`

##### rx_unicast_packets Type

`integer`

- minimum value: `0`

#### rx_unknown_packets

`rx_unknown_packets`

- is **required**
- type: `integer`

##### rx_unknown_packets Type

`integer`

- minimum value: `0`

#### tx_broadcast_packets

`tx_broadcast_packets`

- is **required**
- type: `integer`

##### tx_broadcast_packets Type

`integer`

- minimum value: `0`

#### tx_bytes

`tx_bytes`

- is **required**
- type: `integer`

##### tx_bytes Type

`integer`

- minimum value: `0`

#### tx_discard_packets

`tx_discard_packets`

- is **required**
- type: `integer`

##### tx_discard_packets Type

`integer`

- minimum value: `0`

#### tx_errors

`tx_errors`

- is **required**
- type: `integer`

##### tx_errors Type

`integer`

- minimum value: `0`

#### tx_multicast_packets

`tx_multicast_packets`

- is **required**
- type: `integer`

##### tx_multicast_packets Type

`integer`

- minimum value: `0`

#### tx_packets

`tx_packets`

- is **required**
- type: `integer`

##### tx_packets Type

`integer`

- minimum value: `0`

#### tx_unicast_packets

`tx_unicast_packets`

- is **required**
- type: `integer`

##### tx_unicast_packets Type

`integer`

- minimum value: `0`

### Output Example

```json
{
  "ifname": "velit non",
  "tx_bytes": 21230077,
  "tx_packets": 22267359,
  "tx_errors": 52152815,
  "tx_unicast_packets": 64762317,
  "tx_multicast_packets": 98919966,
  "tx_broadcast_packets": 59967920,
  "tx_discard_packets": 6486449,
  "rx_bytes": 41464309,
  "rx_packets": 74784972,
  "rx_errors": 14195922,
  "rx_unicast_packets": 74892827,
  "rx_multicast_packets": 42714464,
  "rx_broadcast_packets": 8451409,
  "rx_discard_packets": 12497452,
  "rx_unknown_packets": 82902261
}
```

## rmonstats

`rmonstats`

- type: `Method`

### rmonstats Type

`object` with following properties:

| Property | Type   | Required |
| -------- | ------ | -------- |
| `input`  | object | Optional |
| `output` | object | Optional |

#### input

`input`

- is optional
- type: `object`

##### input Type

`object` with following properties:

| Property | Type    | Required     |
| -------- | ------- | ------------ |
| `ifname` | string  | **Required** |
| `qid`    | integer | Optional     |
| `vid`    | integer | Optional     |

#### ifname

##### Interface

`ifname`

- is **required**
- type: reference

##### ifname Type

`string`

- minimum length: 1 characters
- maximum length: 16 characters

#### qid

`qid`

- is optional
- type: `integer`

##### qid Type

`integer`

- minimum value: `0`

#### vid

`vid`

- is optional
- type: `integer`

##### vid Type

`integer`

- minimum value: `0`

### Ubus CLI Example

```
ubus call ethernet rmonstats {"ifname":"et e","vid":15783591,"qid":71102987}
```

### JSONRPC Example

```json
{
  "jsonrpc": "2.0",
  "id": 0,
  "method": "call",
  "params": ["<SID>", "ethernet", "rmonstats", { "ifname": "et e", "vid": 15783591, "qid": 71102987 }]
}
```

#### output

`output`

- is optional
- type: `object`

##### output Type

`object` with following properties:

| Property                     | Type    | Required     |
| ---------------------------- | ------- | ------------ |
| `ifname`                     | string  | **Required** |
| `rx_broadcast_packets`       | integer | **Required** |
| `rx_bytes`                   | integer | **Required** |
| `rx_crc_error_packets`       | integer | **Required** |
| `rx_multicast_packets`       | integer | **Required** |
| `rx_oversize_packets`        | integer | **Required** |
| `rx_packets`                 | integer | **Required** |
| `rx_packets_1024to1518bytes` | integer | **Required** |
| `rx_packets_128to255bytes`   | integer | **Required** |
| `rx_packets_256to511bytes`   | integer | **Required** |
| `rx_packets_512to1023bytes`  | integer | **Required** |
| `rx_packets_64bytes`         | integer | **Required** |
| `rx_packets_65to127bytes`    | integer | **Required** |
| `rx_undersize_packets`       | integer | **Required** |
| `tx_broadcast_packets`       | integer | **Required** |
| `tx_bytes`                   | integer | **Required** |
| `tx_crc_error_packets`       | integer | **Required** |
| `tx_multicast_packets`       | integer | **Required** |
| `tx_oversize_packets`        | integer | **Required** |
| `tx_packets`                 | integer | **Required** |
| `tx_packets_1024to1518bytes` | integer | **Required** |
| `tx_packets_128to255bytes`   | integer | **Required** |
| `tx_packets_256to511bytes`   | integer | **Required** |
| `tx_packets_512to1023bytes`  | integer | **Required** |
| `tx_packets_64bytes`         | integer | **Required** |
| `tx_packets_65to127bytes`    | integer | **Required** |
| `tx_priority_q`              | integer | **Required** |
| `tx_undersize_packets`       | integer | **Required** |

#### ifname

##### Interface

`ifname`

- is **required**
- type: reference

##### ifname Type

`string`

- minimum length: 1 characters
- maximum length: 16 characters

#### rx_broadcast_packets

`rx_broadcast_packets`

- is **required**
- type: `integer`

##### rx_broadcast_packets Type

`integer`

- minimum value: `0`

#### rx_bytes

`rx_bytes`

- is **required**
- type: `integer`

##### rx_bytes Type

`integer`

- minimum value: `0`

#### rx_crc_error_packets

`rx_crc_error_packets`

- is **required**
- type: `integer`

##### rx_crc_error_packets Type

`integer`

- minimum value: `0`

#### rx_multicast_packets

`rx_multicast_packets`

- is **required**
- type: `integer`

##### rx_multicast_packets Type

`integer`

- minimum value: `0`

#### rx_oversize_packets

`rx_oversize_packets`

- is **required**
- type: `integer`

##### rx_oversize_packets Type

`integer`

- minimum value: `0`

#### rx_packets

`rx_packets`

- is **required**
- type: `integer`

##### rx_packets Type

`integer`

- minimum value: `0`

#### rx_packets_1024to1518bytes

`rx_packets_1024to1518bytes`

- is **required**
- type: `integer`

##### rx_packets_1024to1518bytes Type

`integer`

- minimum value: `0`

#### rx_packets_128to255bytes

`rx_packets_128to255bytes`

- is **required**
- type: `integer`

##### rx_packets_128to255bytes Type

`integer`

- minimum value: `0`

#### rx_packets_256to511bytes

`rx_packets_256to511bytes`

- is **required**
- type: `integer`

##### rx_packets_256to511bytes Type

`integer`

- minimum value: `0`

#### rx_packets_512to1023bytes

`rx_packets_512to1023bytes`

- is **required**
- type: `integer`

##### rx_packets_512to1023bytes Type

`integer`

- minimum value: `0`

#### rx_packets_64bytes

`rx_packets_64bytes`

- is **required**
- type: `integer`

##### rx_packets_64bytes Type

`integer`

- minimum value: `0`

#### rx_packets_65to127bytes

`rx_packets_65to127bytes`

- is **required**
- type: `integer`

##### rx_packets_65to127bytes Type

`integer`

- minimum value: `0`

#### rx_undersize_packets

`rx_undersize_packets`

- is **required**
- type: `integer`

##### rx_undersize_packets Type

`integer`

- minimum value: `0`

#### tx_broadcast_packets

`tx_broadcast_packets`

- is **required**
- type: `integer`

##### tx_broadcast_packets Type

`integer`

- minimum value: `0`

#### tx_bytes

`tx_bytes`

- is **required**
- type: `integer`

##### tx_bytes Type

`integer`

- minimum value: `0`

#### tx_crc_error_packets

`tx_crc_error_packets`

- is **required**
- type: `integer`

##### tx_crc_error_packets Type

`integer`

- minimum value: `0`

#### tx_multicast_packets

`tx_multicast_packets`

- is **required**
- type: `integer`

##### tx_multicast_packets Type

`integer`

- minimum value: `0`

#### tx_oversize_packets

`tx_oversize_packets`

- is **required**
- type: `integer`

##### tx_oversize_packets Type

`integer`

- minimum value: `0`

#### tx_packets

`tx_packets`

- is **required**
- type: `integer`

##### tx_packets Type

`integer`

- minimum value: `0`

#### tx_packets_1024to1518bytes

`tx_packets_1024to1518bytes`

- is **required**
- type: `integer`

##### tx_packets_1024to1518bytes Type

`integer`

- minimum value: `0`

#### tx_packets_128to255bytes

`tx_packets_128to255bytes`

- is **required**
- type: `integer`

##### tx_packets_128to255bytes Type

`integer`

- minimum value: `0`

#### tx_packets_256to511bytes

`tx_packets_256to511bytes`

- is **required**
- type: `integer`

##### tx_packets_256to511bytes Type

`integer`

- minimum value: `0`

#### tx_packets_512to1023bytes

`tx_packets_512to1023bytes`

- is **required**
- type: `integer`

##### tx_packets_512to1023bytes Type

`integer`

- minimum value: `0`

#### tx_packets_64bytes

`tx_packets_64bytes`

- is **required**
- type: `integer`

##### tx_packets_64bytes Type

`integer`

- minimum value: `0`

#### tx_packets_65to127bytes

`tx_packets_65to127bytes`

- is **required**
- type: `integer`

##### tx_packets_65to127bytes Type

`integer`

- minimum value: `0`

#### tx_priority_q

`tx_priority_q`

- is **required**
- type: `integer`

##### tx_priority_q Type

`integer`

- minimum value: `-1`

#### tx_undersize_packets

`tx_undersize_packets`

- is **required**
- type: `integer`

##### tx_undersize_packets Type

`integer`

- minimum value: `0`

### Output Example

```json
{
  "ifname": "dolore mi",
  "tx_priority_q": 46124367,
  "tx_bytes": 58672357,
  "tx_packets": 58397584,
  "tx_broadcast_packets": 72801538,
  "tx_multicast_packets": 76412635,
  "tx_crc_error_packets": 11323722,
  "tx_undersize_packets": 60955689,
  "tx_oversize_packets": 51213141,
  "tx_packets_64bytes": 33528761,
  "tx_packets_65to127bytes": 20377489,
  "tx_packets_128to255bytes": 68371123,
  "tx_packets_256to511bytes": 61837228,
  "tx_packets_512to1023bytes": 76166544,
  "tx_packets_1024to1518bytes": 3394615,
  "rx_bytes": 82149056,
  "rx_packets": 40761918,
  "rx_broadcast_packets": 49015013,
  "rx_multicast_packets": 47251307,
  "rx_crc_error_packets": 15892442,
  "rx_undersize_packets": 4978918,
  "rx_oversize_packets": 43260175,
  "rx_packets_64bytes": 99214026,
  "rx_packets_65to127bytes": 18963977,
  "rx_packets_128to255bytes": 63277701,
  "rx_packets_256to511bytes": 417117,
  "rx_packets_512to1023bytes": 94078491,
  "rx_packets_1024to1518bytes": 71282285
}
```
