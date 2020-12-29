#include <cstdio>
#include <napi.h>
#include <iostream>
#include "check.cpp"
#include "main.h"

Napi::Object Init(Napi::Env env, Napi::Object exports)
{
    //MD
    exports.Set(Napi::String::New(env, "md2"), Napi::Function::New(env, MD2));
    exports.Set(Napi::String::New(env, "md4"), Napi::Function::New(env, MD4));
    exports.Set(Napi::String::New(env, "md5"), Napi::Function::New(env, MD5));
    exports.Set(Napi::String::New(env, "md5x2"), Napi::Function::New(env, MD5X2));
    exports.Set(Napi::String::New(env, "md5x3"), Napi::Function::New(env, MD5X3));
    exports.Set(Napi::String::New(env, "md5x4"), Napi::Function::New(env, MD5X4));
    exports.Set(Napi::String::New(env, "md5x5"), Napi::Function::New(env, MD5X5));

    //SHA
    exports.Set(Napi::String::New(env, "sha1"), Napi::Function::New(env, SHA1));
    exports.Set(Napi::String::New(env, "sha224"), Napi::Function::New(env, SHA224));
    exports.Set(Napi::String::New(env, "sha256"), Napi::Function::New(env, SHA256));
    exports.Set(Napi::String::New(env, "sha384"), Napi::Function::New(env, SHA384));
    exports.Set(Napi::String::New(env, "sha512"), Napi::Function::New(env, SHA512));

    //RIPEMD
    exports.Set(Napi::String::New(env, "ripemd128"), Napi::Function::New(env, RIPEMD128));
    exports.Set(Napi::String::New(env, "ripemd160"), Napi::Function::New(env, RIPEMD160));
    exports.Set(Napi::String::New(env, "ripemd256"), Napi::Function::New(env, RIPEMD256));
    exports.Set(Napi::String::New(env, "ripemd320"), Napi::Function::New(env, RIPEMD320));

    //HAVAL
    exports.Set(Napi::String::New(env, "haval3_128"), Napi::Function::New(env, HAVAL3_128));
    exports.Set(Napi::String::New(env, "haval3_160"), Napi::Function::New(env, HAVAL3_160));
    exports.Set(Napi::String::New(env, "haval3_192"), Napi::Function::New(env, HAVAL3_192));
    exports.Set(Napi::String::New(env, "haval3_224"), Napi::Function::New(env, HAVAL3_224));
    exports.Set(Napi::String::New(env, "haval3_256"), Napi::Function::New(env, HAVAL3_256));

    exports.Set(Napi::String::New(env, "haval4_128"), Napi::Function::New(env, HAVAL4_128));
    exports.Set(Napi::String::New(env, "haval4_160"), Napi::Function::New(env, HAVAL4_160));
    exports.Set(Napi::String::New(env, "haval4_192"), Napi::Function::New(env, HAVAL4_192));
    exports.Set(Napi::String::New(env, "haval4_224"), Napi::Function::New(env, HAVAL4_224));
    exports.Set(Napi::String::New(env, "haval4_256"), Napi::Function::New(env, HAVAL4_256));

    exports.Set(Napi::String::New(env, "haval5_128"), Napi::Function::New(env, HAVAL5_128));
    exports.Set(Napi::String::New(env, "haval5_160"), Napi::Function::New(env, HAVAL5_160));
    exports.Set(Napi::String::New(env, "haval5_192"), Napi::Function::New(env, HAVAL5_192));
    exports.Set(Napi::String::New(env, "haval5_224"), Napi::Function::New(env, HAVAL5_224));
    exports.Set(Napi::String::New(env, "haval5_256"), Napi::Function::New(env, HAVAL5_256));

    //TIGER
    exports.Set(Napi::String::New(env, "tiger3_128"), Napi::Function::New(env, TIGER3_128));
    exports.Set(Napi::String::New(env, "tiger3_160"), Napi::Function::New(env, TIGER3_160));
    exports.Set(Napi::String::New(env, "tiger3_192"), Napi::Function::New(env, TIGER3_192));

    exports.Set(Napi::String::New(env, "tiger4_128"), Napi::Function::New(env, TIGER4_128));
    exports.Set(Napi::String::New(env, "tiger4_160"), Napi::Function::New(env, TIGER4_160));
    exports.Set(Napi::String::New(env, "tiger4_192"), Napi::Function::New(env, TIGER4_192));
    return exports;
}

NODE_API_MODULE(addon, Init);