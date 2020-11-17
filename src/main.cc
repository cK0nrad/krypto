#include <cstdio>
#include <napi.h>
#include <iostream>
#include "./check.cpp"
#include "./hashMethod/MD/MD2.cc"
#include "./hashMethod/MD/MD4.cc"
#include "./hashMethod/MD/MD5.cc"

Napi::Object Init(Napi::Env env, Napi::Object exports)
{
    exports.Set(Napi::String::New(env, "md2"), Napi::Function::New(env, MD2));
    exports.Set(Napi::String::New(env, "md4"), Napi::Function::New(env, MD4));
    exports.Set(Napi::String::New(env, "md5"), Napi::Function::New(env, MD5));
    return exports;
}

NODE_API_MODULE(addon, Init);