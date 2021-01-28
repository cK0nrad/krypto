//https://tools.ietf.org/html/draft-eastlake-fnv-17

#include <string>
#include <iostream>
#include <cstring>
#include <iomanip>
#include <sstream>
#include "FNV1A.h"

/*
[A,n^3][B,n^2][C,n^1][D,n^0] *
[E,n^3][F,n^2][G,n^1][H,n^0]  =

(n^6)[(AE)]
(n^5)[(BE) + (AF)]
(n^4)[(AG) + (BF) + (CE)]

(n^3)[(DE) + (CF) + (BG) + (AH)]
(n^2)[(DF) + (CG) + (BH)]
(n^1)[(CH) + (DG)]
(n^0)[(DH)]
*/

std::string FNV1A512_MAIN(uint8_t *message, size_t messageLength)
{
    uint64_t offset[] = {
        0xB86DB0B1171F4416, 0xDCA1E50F309990AC, 0xAC87D059C9000000, 0x0000000000000D21,
        0xE948F68A34C192F6, 0x2EA79BC942DBE7CE, 0x182036415F56E34B, 0xAC982AAC4AFE9FD9};
    uint64_t prime[] = {
        0x0000000000000000, 0x0000000000000000, 0x0000000001000000, 0x0000000000000000,
        0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000157};

    for (size_t i = 0; i < messageLength; i++)
    {
        offset[7] ^= message[i];
        mul512(&prime[0], &offset[0]);
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
    return md5Message.str();
}

Napi::Value FNV1A512(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();
    if (!check(info))
        return env.Null();

    std::string arg0 = info[0].As<Napi::String>();
    Napi::String num = Napi::String::New(env, FNV1A512_MAIN((uint8_t *)(arg0.c_str()), arg0.length()));
    return reinterpret_cast<Napi::Value &&>(num);
}
