# What's is it ?

Krypto is a crypto library written in C++ as an Node.JS module.

# Supported algorithm

    [❌] ADLER32
    [❌] CRC32
    [❌] CRC32B
    [❌] DJB2
    [❌] FARMHASH
    [❌] FNV132
    [❌] FNV164
    [❌] FNV1A1024
    [❌] FNV1A128
    [❌] FNV1A32
    [❌] FNV1A512
    [❌] FNV1A52
    [❌] FNV1A64
    [❌] GOST
    [❌] HAVAL-3 128-160-192-224-256
    [❌] HAVAL-4 128-160-192-224-256
    [❌] HAVAL-5 128-160-192-224-256
    [❌] JOAAT
    [❌] LOSELOSE
    [✔️] MD2
    [✔️] MD4
    [✔️] MD5
    [❌] MD5X    2-3-4-5
    [❌] MURMUR3
    [❌] PEARSON
    [❌] RIPEMD128
    [❌] RIPEMD160
    [❌] RIPEMD256
    [❌] RIPEMD320
    [❌] SDBM
    [✔️] SHA-1
    [❌] SHA-224
    [✔️] SHA-256
    [❌] SHA-384
    [❌] SHA-512
    [❌] SNEFRU
    [❌] SNEFRU256
    [❌] TIGER-3 128-160-192
    [❌] TIGER-4 128-160-192
    [❌] WHIRLPOOL

# Usage

```js
const krypro = require("krypro");
let MD5 = krypro.md5("Test"); //0cbc6611f5540bd0809a388dc95a615b
let SDBM = krypro.sdbm("Test"); //
```
