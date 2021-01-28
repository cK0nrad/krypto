//https://tools.ietf.org/html/draft-eastlake-fnv-17

#include <string>
#include <iostream>
#include <cstring>
#include <iomanip>
#include <sstream>
#include "FNV1A.h"

/*
n=2^32

[A,n^1][B,n^0]*
[C,n^1][D,n^0] =

(n^2)[(AC)]
(n^1)[(AD)+(BC)]
(n^0)[(BD)]

*/

std::string FNV1A128_MAIN(uint8_t *message, size_t messageLength)
{
    uint64_t prime[] = {0x0000000001000000, 0x000000000000013B};
    uint64_t offset[] = {0x6C62272E07BB0142, 0x62B821756295C58D};

    for (size_t i = 0; i < messageLength; i++)
    {
        offset[1] ^= message[i];
        mul128(&prime[0], &offset[0]);
    }

    std::ostringstream md5Message;
    md5Message << std::hex << std::setfill('0') << std::setw(16) << offset[0];
    md5Message << std::hex << std::setfill('0') << std::setw(16) << offset[1];
    return md5Message.str();
}

Napi::Value FNV1A128(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();
    if (!check(info))
        return env.Null();

    std::string arg0 = info[0].As<Napi::String>();
    Napi::String num = Napi::String::New(env, FNV1A128_MAIN((uint8_t *)(arg0.c_str()), arg0.length()));
    return reinterpret_cast<Napi::Value &&>(num);
}
