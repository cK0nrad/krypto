//Test file
let krypto = require("./index");
let string =  "krypto";
let test = [
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

  [krypto.haval3_128(string), "HAVAL3 128"],
  [krypto.haval3_160(string), "HAVAL3 160"],
  [krypto.haval3_192(string), "HAVAL3 192"],
  [krypto.haval3_224(string), "HAVAL3 224"],
  [krypto.haval3_256(string), "HAVAL3 256"],
  [krypto.haval4_128(string), "HAVAL4 128"],
  [krypto.haval4_160(string), "HAVAL4 160"],
  [krypto.haval4_192(string), "HAVAL4 192"],
  [krypto.haval4_224(string), "HAVAL4 224"],
  [krypto.haval4_256(string), "HAVAL4 256"],
  [krypto.haval5_128(string), "HAVAL5 128"],
  [krypto.haval5_160(string), "HAVAL5 160"],
  [krypto.haval5_192(string), "HAVAL5 192"],
  [krypto.haval5_224(string), "HAVAL5 224"],
  [krypto.haval5_256(string), "HAVAL5 256"],

  [krypto.tiger3_128(string), "TIGER3 128"],
  [krypto.tiger3_160(string), "TIGER3 160"],
  [krypto.tiger3_192(string), "TIGER3 192"],
  [krypto.tiger4_128(string), "TIGER4 128"],
  [krypto.tiger4_160(string), "TIGER4 160"],
  [krypto.tiger4_192(string), "TIGER4 192"],
];

test.map(([a, x]) => console.log(x + ": " + a));
