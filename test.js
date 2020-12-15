//Test file 
let krypto = require('./index');
let string = "azeazezaeza'('()à)à)à)à)à)à)à)à)àà)'(àà'à-à'(-àà'è(-çè(-à)_ç)-à(çè_)(-$$^*ù^$ù*$^*ù*^$$*45gd45f64g4df564gdfw516g1 45f01gw451wdf46g1 0dfw7g4w674g7))))"
let test =  [
    krypto.md2(string),
    krypto.md4(string),
    krypto.md5(string),

    krypto.sha1(string),
    krypto.sha224(string),
    krypto.sha256(string),
    krypto.sha384(string),
    krypto.sha512(string),

    krypto.ripemd128(string),
    krypto.ripemd160(string),
    krypto.ripemd256(string),
    krypto.ripemd320(string),
]

test.map(x => console.log(x))