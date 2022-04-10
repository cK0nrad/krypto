//https://tools.ietf.org/html/draft-eastlake-fnv-17

#include <string>
#include <iostream>
#include <cstring>
#include <iomanip>
#include <sstream>
#include <napi.h>
#include "../../check.hpp"

std::string FNV164_MAIN(uint8_t *message, size_t messageLength)
{
    uint64_t prime = 0x00000100000001B3;
    uint64_t offset = 0xcbf29ce484222325;

    for (size_t i = 0; i < messageLength; i++)
    {
        offset *= prime;
        offset ^= message[i];
    }

    std::ostringstream md5Message;
    md5Message << std::hex << std::setfill('0') << std::setw(16) << offset;
    return md5Message.str();
}

Napi::Value FNV164(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();
    if (!check(info))
        return env.Null();

    std::string arg0 = info[0].As<Napi::String>();
    Napi::String num = Napi::String::New(env, FNV164_MAIN((uint8_t *)(arg0.c_str()), arg0.length()));
    return reinterpret_cast<Napi::Value &&>(num);
}
