#Warning

Full rewrite in progress due to lack of class usage.

# What's is it ?

krypto is a NodeJS crypto module written in C++.

# Supported algorithm

    [✔️] ADLER32
    [✔️] CRC32
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
    [❌] SMDB
    [❌] DJB2
    [❌] LOSE LOSE

# Usage

```js
const krypto = require("@ck0nrad/krypto");
const crypto = require('crypto')

console.time("krypto");
let MD5 = krypto.md5("Test"); //0cbc6611f5540bd0809a388dc95a615b
console.timeEnd("krypto");

console.time("crypto")
let hash = crypto.createHash('md5').update('Test').digest("hex")
console.timeEnd("crypto")

console.log(MD5)
console.log(hash)
```
```sh
> node test.js
krypto: 0.167ms
crypto: 0.715ms
0cbc6611f5540bd0809a388dc95a615b
0cbc6611f5540bd0809a388dc95a615b
```
# Install

```sh
npm i @ck0nrad/krypto
```
