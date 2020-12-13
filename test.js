//Test file 
let krypto = require('./index');
let string = " "
let test =  [
    krypto.md2(string),
    krypto.md4(string),
    krypto.md5(string),
    krypto.sha1(string),
    krypto.sha256(string),
]

test.map(x => console.log(x))