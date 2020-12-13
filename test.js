//Test file 
let krypto = require('./index');
let string = "471992"
let test =  [
    krypto.md2(string),
    krypto.md4(string),
    krypto.md5(string),
    krypto.sha1(string),
    krypto.sha224(string),
    krypto.sha256(string),
    krypto.sha384(string),
    krypto.sha512(string),
]

test.map(x => console.log(x))