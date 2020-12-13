#include <cstdio>
#include <napi.h>
#include <iostream>
#include "./check.cpp"
#include "./hashMethod/MD/MD2.cc"
#include "./hashMethod/MD/MD4.cc"
#include "./hashMethod/MD/MD5.cc"
#include "./hashMethod/SHA/SHA-1.cc"
#include "./hashMethod/SHA/SHA-256.cc"

Napi::Object Init(Napi::Env env, Napi::Object exports)
{
    exports.Set(Napi::String::New(env, "md2"), Napi::Function::New(env, MD2));
    exports.Set(Napi::String::New(env, "md4"), Napi::Function::New(env, MD4));
    exports.Set(Napi::String::New(env, "md5"), Napi::Function::New(env, MD5));
    exports.Set(Napi::String::New(env, "sha1"), Napi::Function::New(env, SHA1));
    exports.Set(Napi::String::New(env, "sha256"), Napi::Function::New(env, SHA256));
    return exports;
}

NODE_API_MODULE(addon, Init);