#include <cstdio>
#include <napi.h>
#include <iostream>
#include "check.cpp"
#include "main.h"

Napi::Object Init(Napi::Env env, Napi::Object exports)
{
    exports.Set(Napi::String::New(env, "md2"), Napi::Function::New(env, MD2));
    exports.Set(Napi::String::New(env, "md4"), Napi::Function::New(env, MD4));
    exports.Set(Napi::String::New(env, "md5"), Napi::Function::New(env, MD5));
    exports.Set(Napi::String::New(env, "md5x2"), Napi::Function::New(env, MD5X2));
    exports.Set(Napi::String::New(env, "md5x3"), Napi::Function::New(env, MD5X3));
    exports.Set(Napi::String::New(env, "md5x4"), Napi::Function::New(env, MD5X4));
    exports.Set(Napi::String::New(env, "md5x5"), Napi::Function::New(env, MD5X5));

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