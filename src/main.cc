#include <cstdio>
#include <napi.h>
#include <iostream>
#include "./check.cpp"
#include "./hashMethod/MD/MD2.cc"
#include "./hashMethod/MD/MD4.cc"
#include "./hashMethod/MD/MD5.cc"
#include "./hashMethod/SHA/SHA-1.cc"
#include "./hashMethod/SHA/SHA-224.cc"
#include "./hashMethod/SHA/SHA-256.cc"
#include "./hashMethod/SHA/SHA-384.cc"
#include "./hashMethod/SHA/SHA-512.cc"
#include "./hashMethod/RIPEMD/RIPEMD-128.cc"
#include "./hashMethod/RIPEMD/RIPEMD-160.cc"
#include "./hashMethod/RIPEMD/RIPEMD-256.cc"
#include "./hashMethod/RIPEMD/RIPEMD-320.cc"

Napi::Object Init(Napi::Env env, Napi::Object exports)
{
    exports.Set(Napi::String::New(env, "md2"), Napi::Function::New(env, MD2));
    exports.Set(Napi::String::New(env, "md4"), Napi::Function::New(env, MD4));
    exports.Set(Napi::String::New(env, "md5"), Napi::Function::New(env, MD5));

    exports.Set(Napi::String::New(env, "sha1"), Napi::Function::New(env, SHA1));
    exports.Set(Napi::String::New(env, "sha224"), Napi::Function::New(env, SHA224));
    exports.Set(Napi::String::New(env, "sha256"), Napi::Function::New(env, SHA256));
    exports.Set(Napi::String::New(env, "sha384"), Napi::Function::New(env, SHA384));
    exports.Set(Napi::String::New(env, "sha512"), Napi::Function::New(env, SHA512));

    exports.Set(Napi::String::New(env, "ripemd128"), Napi::Function::New(env, RIPEMD128));
    exports.Set(Napi::String::New(env, "ripemd160"), Napi::Function::New(env, RIPEMD160));
    exports.Set(Napi::String::New(env, "ripemd256"), Napi::Function::New(env, RIPEMD256));
    exports.Set(Napi::String::New(env, "ripemd320"), Napi::Function::New(env, RIPEMD320));
    return exports;
}

NODE_API_MODULE(addon, Init);