/*
https://csrc.nist.gov/csrc/media/publications/fips/180/4/final/documents/fips180-4-draft-aug2014.pdf
*/
#include <string>
#include <iostream>
#include <cstring>
#include <iomanip>
#include <sstream>
#include <vector>
#include "./SHA.h"

std::string SHA224_MAIN(uint8_t *message, size_t messageLength)
{
    //Where to add size
    size_t shiftingPosition = (messageLength / 4);
    size_t shiftingAmount = 8 * (messageLength % 4);

    //How many array entry to fill the message
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
    //Total size of the message + padding + shiftingAmount + 2*32 entry for the length
    size_t newMessageLength = (messageLength / 4) + ((paddingLength + shiftingAmount) / 32);
    size_t paddedMessageLength = newMessageLength + 2;

    //Vector that contain message + padding + length
    std::vector<uint32_t> paddedMessage(paddedMessageLength, 0);
    for (size_t i = 0; i < messageLength; i++)
    {
        paddedMessage[((i + 4) / 4) - 1] += (message[i] << (24 - (8 * (i % 4))));
    }

    //Append 0x80 at then end of the message (10000000)
    //Padding
    paddedMessage[shiftingPosition] += (0x80 << (24 - shiftingAmount));

    //Appending initial length
    paddedMessage[paddedMessageLength - 1] = ((messageLength << 3) & 0xFFFFFFFF);
    paddedMessage[paddedMessageLength - 2] = ((messageLength >> 29) & 0xFFFFFFFF);

    //Hash calculation
    //For loop of 16 words per rounds.

    //Initialize register
    uint32_t H0 = 0xc1059ed8;
    uint32_t H1 = 0x367cd507;
    uint32_t H2 = 0x3070dd17;
    uint32_t H3 = 0xf70e5939;
    uint32_t H4 = 0xffc00b31;
    uint32_t H5 = 0x68581511;
    uint32_t H6 = 0x64f98fa7;
    uint32_t H7 = 0xbefa4fa4;

    uint32_t A, B, C, D, E, F, G, H, T1, T2;
    size_t i;
    uint32_t W[64] = {0};

    for (size_t offset = 0; offset < (paddedMessageLength / 16); offset++)
    {
        //Original message
        for (i = 0; i < 16; i++)
        {
            W[i] = (paddedMessage[offset * 16 + i]);
        }

        //Expension
        for (i = 16; i < 64; i++)
        {
            W[i] = (Lsigma1(W[i - 2]) + W[i - 7] + Lsigma0(W[i - 15]) + W[i - 16]);
        }

        A = H0;
        B = H1;
        C = H2;
        D = H3;
        E = H4;
        F = H5;
        G = H6;
        H = H7;

        for (i = 0; i < 64; i++)
        {
            T1 = H + Usigma1(E) + Ch(E, F, G) + kSHA256[i] + W[i];
            T2 = Usigma0(A) + Maj(A, B, C);

            H = G;
            G = F;
            F = E;
            E = D + T1;
            D = C;
            C = B;
            B = A;
            A = T1 + T2;
        }

        H0 += A;
        H1 += B;
        H2 += C;
        H3 += D;
        H4 += E;
        H5 += F;
        H6 += G;
        H7 += H;
    }
    std::ostringstream md5Message;
    md5Message << std::hex << std::setfill('0') << std::setw(8) << (H0);
    md5Message << std::hex << std::setfill('0') << std::setw(8) << (H1);
    md5Message << std::hex << std::setfill('0') << std::setw(8) << (H2);
    md5Message << std::hex << std::setfill('0') << std::setw(8) << (H3);
    md5Message << std::hex << std::setfill('0') << std::setw(8) << (H4);
    md5Message << std::hex << std::setfill('0') << std::setw(8) << (H5);
    md5Message << std::hex << std::setfill('0') << std::setw(8) << (H6);
    return md5Message.str();
}

Napi::Value SHA224(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();
    if (!check(info))
        return env.Null();

    std::string arg0 = info[0].As<Napi::String>();
    Napi::String num = Napi::String::New(env, SHA224_MAIN((uint8_t *)(arg0.c_str()), arg0.length()));
    return reinterpret_cast<Napi::Value &&>(num);
}