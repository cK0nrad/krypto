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
#include "./hashMethod/DJB2/DJB2.cc"
#include "./hashMethod/SDBM/SDBM.cc"
#include "./hashMethod/LOSELOSE/LOSELOSE.cc"

Napi::Object Init(Napi::Env env, Napi::Object exports)
{
    exports.Set(Napi::String::New(env, "djb2"), Napi::Function::New(env, DJB2));
    exports.Set(Napi::String::New(env, "sdbm"), Napi::Function::New(env, SDBM));
    exports.Set(Napi::String::New(env, "loselose"), Napi::Function::New(env, LOSELOSE));
    exports.Set(Napi::String::New(env, "md2"), Napi::Function::New(env, MD2));
    exports.Set(Napi::String::New(env, "md4"), Napi::Function::New(env, MD4));
    exports.Set(Napi::String::New(env, "md5"), Napi::Function::New(env, MD5));
    exports.Set(Napi::String::New(env, "sha1"), Napi::Function::New(env, SHA1));
    exports.Set(Napi::String::New(env, "sha224"), Napi::Function::New(env, SHA224));
    exports.Set(Napi::String::New(env, "sha256"), Napi::Function::New(env, SHA256));
    exports.Set(Napi::String::New(env, "sha384"), Napi::Function::New(env, SHA384));
    exports.Set(Napi::String::New(env, "sha512"), Napi::Function::New(env, SHA512));
    return exports;
}

NODE_API_MODULE(addon, Init);