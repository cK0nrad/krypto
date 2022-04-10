#include <string>
#include <iostream>
#include <cstring>
#include <iomanip>
#include <sstream>
#include "CRC.hpp"
#include <napi.h>
#include "../../check.hpp"

std::string CRC32_MAIN(uint8_t *message, size_t messageLength)
{
    uint32_t crc32 = 0xFFFFFFFF;

    for (size_t i = 0; i < messageLength; ++i)
        crc32 = (crc32 >> 8) ^ CRC32_lookup[(crc32 ^ message[i]) & 0xFF];

    crc32 ^= 0xFFFFFFFF;
    std::ostringstream md5Message;
    md5Message << std::hex << std::setfill('0') << std::setw(8) << (crc32);
    return md5Message.str();
}

Napi::Value CRC32(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();
    if (!check(info))
        return env.Null();

    std::string arg0 = info[0].As<Napi::String>();
    Napi::String num = Napi::String::New(env, CRC32_MAIN((uint8_t *)(arg0.c_str()), arg0.length()));
    return reinterpret_cast<Napi::Value &&>(num);
}