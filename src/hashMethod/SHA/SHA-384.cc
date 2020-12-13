/*
https://csrc.nist.gov/csrc/media/publications/fips/180/4/final/documents/fips180-4-draft-aug2014.pdf
*/
#include <string>
#include <iostream>
#include <cstring>
#include <iomanip>
#include <sstream>
#include <vector>

std::string SHA384_MAIN(uint8_t *message, size_t messageLength)
{
    //Where to add size
    size_t shiftingPosition = (messageLength / 8);
    size_t shiftingAmount = 8 * (messageLength % 8);

    //How many array entry to fill the message
    size_t paddingLength = 896;
    if ((messageLength * 8) % 1024 != 0)
    {
        if (((messageLength * 8) % 1024) > 896)
        {
            paddingLength = 512 - (((messageLength * 8) % 1024) - 896);
        }
        else
        {
            paddingLength = 896 - ((messageLength * 8) % 1024);
        }
    }

    //Total size of the message + padding + shiftingAmount + 2*64 entry for the length
    size_t newMessageLength = (messageLength / 8) + ((paddingLength + shiftingAmount) / 64);
    size_t paddedMessageLength = newMessageLength + 2;

    //Vector that contain message + padding + length
    std::vector<uint64_t> paddedMessage(paddedMessageLength, 0);
    for (size_t i = 0; i < messageLength; i++)
    {
        paddedMessage[((i + 8) / 8) - 1] += ((uint64_t)message[i] << (56 - (8 * (i % 8))));
    }

    //Append 0x80 at then end of the message (10000000)
    //Padding
    paddedMessage[shiftingPosition] += (((uint64_t)0x80) << (56 - shiftingAmount));

    //Appending initial length
    paddedMessage[paddedMessageLength - 1] = ((messageLength << 3) & 0xFFFFFFFFFFFFFFFF);
    paddedMessage[paddedMessageLength - 2] = ((messageLength >> 61) & 0xFFFFFFFFFFFFFFFF);

    //Hash calculation
    //For loop of 16 words per rounds.

    //Initialize register
    uint64_t H0 = 0xcbbb9d5dc1059ed8;
    uint64_t H1 = 0x629a292a367cd507;
    uint64_t H2 = 0x9159015a3070dd17;
    uint64_t H3 = 0x152fecd8f70e5939;
    uint64_t H4 = 0x67332667ffc00b31;
    uint64_t H5 = 0x8eb44a8768581511;
    uint64_t H6 = 0xdb0c2e0d64f98fa7;
    uint64_t H7 = 0x47b5481dbefa4fa4;

    uint64_t A, B, C, D, E, F, G, H, T1, T2;
    size_t i;
    uint64_t W[80] = {0};
    for (size_t offset = 0; offset < (paddedMessageLength / 16); offset++)
    {
        //Original message
        for (i = 0; i < 16; i++)
        {
            W[i] = (paddedMessage[offset * 16 + i]);
        }

        //Expension
        for (i = 16; i < 80; i++)
        {
            W[i] = (LLsigma1(W[i - 2]) + W[i - 7] + LLsigma0(W[i - 15]) + W[i - 16]);
        }

        A = H0;
        B = H1;
        C = H2;
        D = H3;
        E = H4;
        F = H5;
        G = H6;
        H = H7;

        for (i = 0; i < 80; i++)
        {
            T1 = H + LUsigma1(E) + Ch(E, F, G) + kSHA512[i] + W[i];
            T2 = LUsigma0(A) + Maj(A, B, C);

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

    return md5Message.str();
}

Napi::Value SHA384(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();
    if (!check(info))
        return env.Null();

    std::string arg0 = info[0].As<Napi::String>();
    Napi::String num = Napi::String::New(env, SHA384_MAIN((uint8_t *)(arg0.c_str()), arg0.length()));
    return reinterpret_cast<Napi::Value &&>(num);
}