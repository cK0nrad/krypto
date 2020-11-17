//Test file 
let krypto = require('./index');

let test =  [
    krypto.md2('krypto'),
    krypto.md4('krypto'),
    krypto.md5('krypto'),
]

test.map(x => console.log(x))