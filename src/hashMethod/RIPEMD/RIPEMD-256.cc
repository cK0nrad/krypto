/*
https://homes.esat.kuleuven.be/~bosselae/ripemd/rmd128.txt
*/

#include <string>
#include <iostream>
#include <cstring>
#include <iomanip>
#include <sstream>
#include <vector>

#ifndef LEFTROTATE
#define LEFTROTATE(A, N) (((A) << (N)) | ((A) >> (32 - (N))))
#endif
#ifndef endia
#define endia(X) ((((X)&0xFF) << 24) | (((X)&0xFF00) << 8) | (((X)&0xFF0000) >> 8) | (((X)&0xFF000000) >> 24))
#endif

std::string RIPEMD256_MAIN(uint8_t *message, size_t messageLength)
{
    //Where to add size
    size_t shiftingPosition = (messageLength / 4);
    size_t shiftingAmount = 8 * (messageLength % 4);

    //Let x=MessageLength % 64 (modulo 64), If x=0, we pad with 64 0 bytes. If x !=0, then we check if x > 56. If it's the case we pad with n=64-(x-56) or if it's smaller than 56 n = 56-x
    size_t paddingLength = 448;
    if ((messageLength * 8) % 512 != 0)
    {
        if (((messageLength * 8) % 512) > 448)
        {
            paddingLength = 512 - (((messageLength * 8) % 512) - 448);
        }
        else
        {
            paddingLength = 448 - ((messageLength * 8) % 512);
        }
    }

    //Add the shifting to the padding to get full 32bit size, bcs we will add 0x80 in the same last one if the message length%32!=0
    size_t newMessageLength = (messageLength / 4) + ((paddingLength + shiftingAmount) / 32);
    size_t paddedMessageLength = newMessageLength + 2;

    //new array for the new message
    std::vector<uint32_t> paddedMessage(paddedMessageLength, 0);
    memcpy(&paddedMessage[0], message, messageLength);

    //Padding
    paddedMessage[shiftingPosition] += (0x80 << shiftingAmount);

    //Appending initial length
    paddedMessage[paddedMessageLength - 1] = ((messageLength >> 29) & 0xFFFFFFFF);
    paddedMessage[paddedMessageLength - 2] = ((messageLength << 3) & 0xFFFFFFFF);

    uint32_t H0 = 0x67452301;
    uint32_t H1 = 0xEFCDAB89;
    uint32_t H2 = 0x98BADCFE;
    uint32_t H3 = 0x10325476;
    uint32_t H4 = 0x76543210;
    uint32_t H5 = 0xFEDCBA98;
    uint32_t H6 = 0x89ABCDEF;
    uint32_t H7 = 0x01234567;
    uint32_t A, B, C, D, Ap, Bp, Cp, Dp, T, f, fp, K, Kp;

    for (size_t offset = 0; offset < (paddedMessageLength / 16); offset++)
    {
        A = H0;
        B = H1;
        C = H2;
        D = H3;
        Ap = H4;
        Bp = H5;
        Cp = H6;
        Dp = H7;

        for (size_t i = 0; i < 64; i++)
        {
            if (i < 16)
            {
                K = 0x00000000,
                Kp = 0x50A28BE6;
                f = (B ^ C ^ D);
                fp = (Bp & Dp) | (Cp & ~(Dp));
            }
            else if (i < 32)
            {
                K = 0x5A827999,
                Kp = 0x5C4DD124;
                f = (B & C) | (~B & D);
                fp = (Bp | ~(Cp)) ^ Dp;
            }
            else if (i < 48)
            {
                K = 0x6ED9EBA1,
                Kp = 0x6D703EF3;
                f = (B | ~C) ^ D;
                fp = (Bp & Cp) | (~(Bp)&Dp);
            }
            else if (i < 64)
            {
                K = 0x8F1BBCDC,
                Kp = 0x00000000;
                f = (B & D) | (C & ~(D));
                fp = (Bp ^ Cp ^ Dp);
            }

            T = LEFTROTATE((A + f + paddedMessage[16 * offset + rRIP[i]] + K), sRIP[i]);
            A = D;
            D = C;
            C = B;
            B = T;

            T = LEFTROTATE((Ap + fp + paddedMessage[16 * offset + rpRIP[i]] + Kp), spRIP[i]);
            Ap = Dp;
            Dp = Cp;
            Cp = Bp;
            Bp = T;

            if (i == 15)
            {
                T = A;
                A = Ap;
                Ap = T;
            }
            else if (i == 31)
            {
                T = B;
                B = Bp;
                Bp = T;
            }
            else if (i == 47)
            {
                T = C;
                C = Cp;
                Cp = T;
            }
            else if (i == 63)
            {
                T = D;
                D = Dp;
                Dp = T;
            }
        }
        H0 += A;
        H1 += B;
        H2 += C;
        H3 += D;
        H4 += Ap;
        H5 += Bp;
        H6 += Cp;
        H7 += Dp;
    }

    std::ostringstream md5Message;
    md5Message << std::hex << std::setfill('0') << std::setw(8) << endia(H0);
    md5Message << std::hex << std::setfill('0') << std::setw(8) << endia(H1);
    md5Message << std::hex << std::setfill('0') << std::setw(8) << endia(H2);
    md5Message << std::hex << std::setfill('0') << std::setw(8) << endia(H3);
    md5Message << std::hex << std::setfill('0') << std::setw(8) << endia(H4);
    md5Message << std::hex << std::setfill('0') << std::setw(8) << endia(H5);
    md5Message << std::hex << std::setfill('0') << std::setw(8) << endia(H6);
    md5Message << std::hex << std::setfill('0') << std::setw(8) << endia(H7);
    return md5Message.str();
}

Napi::Value RIPEMD256(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();
    if (!check(info))
        return env.Null();

    std::string arg0 = info[0].As<Napi::String>();
    Napi::String num = Napi::String::New(env, RIPEMD256_MAIN((uint8_t *)(arg0.c_str()), arg0.length()));
    return reinterpret_cast<Napi::Value &&>(num);
}
