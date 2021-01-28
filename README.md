# What's is it ?

Krypto is a crypto library written in C++ as an Node.JS module.

# Supported algorithm

    [❌] ADLER32
    [❌] CRC32
    [❌] CRC32B
    [❌] FARMHASH
    [✔️] FNV1    32-64
    [✔️] FNV1A   32-64-128-256-512-1024
    [❌] GOST
    [✔️] HAVAL-3 128-160-192-224-256
    [✔️] HAVAL-4 128-160-192-224-256
    [✔️] HAVAL-5 128-160-192-224-256
    [❌] JOAAT
    [✔️] MD2
    [✔️] MD4
    [✔️] MD5
    [✔️] MD5X    2-3-4-5
    [❌] MURMUR3
    [❌] PEARSON
    [✔️] RIPEMD  128-160-256-320
    [✔️] SHA-1
    [✔️] SHA     224-256-384-512
    [❌] SNEFRU  128-256
    [✔️] TIGER-3 128-160-192
    [✔️] TIGER-4 128-160-192
    [❌] WHIRLPOOL

# Usage

```js
const krypro = require("krypro");
let MD5 = krypro.md5("Test"); //0cbc6611f5540bd0809a388dc95a615b
let SDBM = krypro.sdbm("Test"); //
```
