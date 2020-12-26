/*
https://homes.esat.kuleuven.be/~bosselae/ripemd/rmd320.txt
*/

#include <string>
#include <iostream>
#include <cstring>
#include <iomanip>
#include <sstream>
#include <vector>
#include "RIPEMD.h"

std::string RIPEMD320_MAIN(uint8_t *message, size_t messageLength)
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
    uint32_t H4 = 0xC3D2E1F0;
    uint32_t H5 = 0x76543210;
    uint32_t H6 = 0xFEDCBA98;
    uint32_t H7 = 0x89ABCDEF;
    uint32_t H8 = 0x01234567;
    uint32_t H9 = 0x3C2D1E0F;

    uint32_t A, B, C, D, E, Ap, Bp, Cp, Dp, Ep, T, f, fp, K, Kp;

    for (size_t offset = 0; offset < (paddedMessageLength / 16); offset++)
    {
        A = H0;
        B = H1;
        C = H2;
        D = H3;
        E = H4;
        Ap = H5;
        Bp = H6;
        Cp = H7;
        Dp = H8;
        Ep = H9;

        for (size_t i = 0; i < 80; i++)
        {
            if (i < 16)
            {
                K = 0x00000000,
                Kp = 0x50A28BE6;
                f = (B ^ C ^ D);
                fp = Bp ^ (Cp | ~(Dp));
            }
            else if (i < 32)
            {
                K = 0x5A827999,
                Kp = 0x5C4DD124;
                f = (B & C) | (~B & D);
                fp = (Bp & Dp) | (Cp & ~(Dp));
            }
            else if (i < 48)
            {
                K = 0x6ED9EBA1,
                Kp = 0x6D703EF3;
                f = (B | ~C) ^ D;
                fp = (Bp | ~Cp) ^ Dp;
            }
            else if (i < 64)
            {
                K = 0x8F1BBCDC,
                Kp = 0x7A6D76E9;
                f = (B & D) | (C & ~(D));
                fp = (Bp & Cp) | (~Bp & Dp);
            }
            else if (i < 80)
            {
                K = 0xA953FD4E,
                Kp = 0x00000000;
                f = B ^ (C | ~(D));
                fp = (Bp ^ Cp ^ Dp);
            }

            T = LEFTROTATE((A + f + paddedMessage[16 * offset + RIP_r[i]] + K), RIP_s[i]) + E;
            A = E;
            E = D;
            D = LEFTROTATE(C, 10);
            C = B;
            B = T;

            T = LEFTROTATE((Ap + fp + paddedMessage[16 * offset + RIP_rp[i]] + Kp), RIP_sp[i]) + Ep;
            Ap = Ep;
            Ep = Dp;
            Dp = LEFTROTATE(Cp, 10);
            Cp = Bp;
            Bp = T;

            if (i == 15)
            {
                T = B;
                B = Bp;
                Bp = T;
            }
            else if (i == 31)
            {
                T = D;
                D = Dp;
                Dp = T;
            }
            else if (i == 47)
            {
                T = A;
                A = Ap;
                Ap = T;
            }
            else if (i == 63)
            {
                T = C;
                C = Cp;
                Cp = T;
            }
            else if (i == 79)
            {
                T = E;
                E = Ep;
                Ep = T;
            }
        }

        H0 += A;
        H1 += B;
        H2 += C;
        H3 += D;
        H4 += E;
        H5 += Ap;
        H6 += Bp;
        H7 += Cp;
        H8 += Dp;
        H9 += Ep;
    }

    std::ostringstream md5Message;
    md5Message << std::hex << std::setfill('0') << std::setw(8) << ENDIAN(H0);
    md5Message << std::hex << std::setfill('0') << std::setw(8) << ENDIAN(H1);
    md5Message << std::hex << std::setfill('0') << std::setw(8) << ENDIAN(H2);
    md5Message << std::hex << std::setfill('0') << std::setw(8) << ENDIAN(H3);
    md5Message << std::hex << std::setfill('0') << std::setw(8) << ENDIAN(H4);
    md5Message << std::hex << std::setfill('0') << std::setw(8) << ENDIAN(H5);
    md5Message << std::hex << std::setfill('0') << std::setw(8) << ENDIAN(H6);
    md5Message << std::hex << std::setfill('0') << std::setw(8) << ENDIAN(H7);
    md5Message << std::hex << std::setfill('0') << std::setw(8) << ENDIAN(H8);
    md5Message << std::hex << std::setfill('0') << std::setw(8) << ENDIAN(H9);
    return md5Message.str();
}

Napi::Value RIPEMD320(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();
    if (!check(info))
        return env.Null();

    std::string arg0 = info[0].As<Napi::String>();
    Napi::String num = Napi::String::New(env, RIPEMD320_MAIN((uint8_t *)(arg0.c_str()), arg0.length()));
    return reinterpret_cast<Napi::Value &&>(num);
}
