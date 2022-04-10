//https://tools.ietf.org/html/draft-eastlake-fnv-17

#include <string>
#include <iostream>
#include <cstring>
#include <iomanip>
#include <sstream>
#include "FNV1A.hpp"
#include <napi.h>
#include "../../check.hpp"

std::string FNV1A1024_MAIN(uint8_t *message, size_t messageLength)
{
    uint64_t offset[] = {
        0x0000000000000000, 0x005F7A76758ECC4D, 0x32E56D5A591028B7, 0x4B29FC4223FDADA1,
        0x6C3BF34EDA3674DA, 0x9A21D90000000000, 0x0000000000000000, 0x0000000000000000,
        0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x000000000004C6D7,
        0xEB6E73802734510A, 0x555F256CC005AE55, 0x6BDE8CC9C6A93B21, 0xAFF4B16C71EE90B3};
    uint64_t prime[] = {
        0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000,
        0x0000000000000000, 0x0000010000000000, 0x0000000000000000, 0x0000000000000000,
        0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000,
        0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x000000000000018D};

    for (size_t i = 0; i < messageLength; i++)
    {
        offset[15] ^= message[i];
        mul1024(&prime[0], &offset[0]);
    }

    std::ostringstream md5Message;
    md5Message << std::hex << std::setfill('0') << std::setw(16) << offset[0];
    md5Message << std::hex << std::setfill('0') << std::setw(16) << offset[1];
    md5Message << std::hex << std::setfill('0') << std::setw(16) << offset[2];
    md5Message << std::hex << std::setfill('0') << std::setw(16) << offset[3];
    md5Message << std::hex << std::setfill('0') << std::setw(16) << offset[4];
    md5Message << std::hex << std::setfill('0') << std::setw(16) << offset[5];
    md5Message << std::hex << std::setfill('0') << std::setw(16) << offset[6];
    md5Message << std::hex << std::setfill('0') << std::setw(16) << offset[7];
    md5Message << std::hex << std::setfill('0') << std::setw(16) << offset[8];
    md5Message << std::hex << std::setfill('0') << std::setw(16) << offset[9];
    md5Message << std::hex << std::setfill('0') << std::setw(16) << offset[10];
    md5Message << std::hex << std::setfill('0') << std::setw(16) << offset[11];
    md5Message << std::hex << std::setfill('0') << std::setw(16) << offset[12];
    md5Message << std::hex << std::setfill('0') << std::setw(16) << offset[13];
    md5Message << std::hex << std::setfill('0') << std::setw(16) << offset[14];
    md5Message << std::hex << std::setfill('0') << std::setw(16) << offset[15];
    return md5Message.str();
}

Napi::Value FNV1A1024(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();
    if (!check(info))
        return env.Null();

    std::string arg0 = info[0].As<Napi::String>();
    Napi::String num = Napi::String::New(env, FNV1A1024_MAIN((uint8_t *)(arg0.c_str()), arg0.length()));
    return reinterpret_cast<Napi::Value &&>(num);
}
