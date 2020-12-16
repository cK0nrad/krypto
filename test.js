//Test file 
let krypto = require('./index');
let string = "a"
let test =  [
    [krypto.md2(string), "MD2"],
    [krypto.md4(string), "MD4"],
    [krypto.md5(string), "MD5"],
    [krypto.md5x2(string), "MD5x2"],
    [krypto.md5x3(string), "MD5x3"],
    [krypto.md5x4(string), "MD5x4"],
    [krypto.md5x5(string), "MD5x5"],

    [krypto.sha1(string), "SHA1"],
    [krypto.sha224(string), "SHA224"],
    [krypto.sha256(string), "SHA256"],
    [krypto.sha384(string), "SHA384"],
    [krypto.sha512(string), "SHA512"],

    [krypto.ripemd128(string), "RIPEMD128"],
    [krypto.ripemd160(string), "RIPEMD160"],
    [krypto.ripemd256(string), "RIPEMD256"],
    [krypto.ripemd320(string), "RIPEMD320"],
]

test.map(([a,x]) => console.log(x+ ": " +a))