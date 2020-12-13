/*
https://www.ietf.org/rfc/rfc3174.txt
*/
#include <string>
#include <iostream>
#include <cstring>
#include <iomanip>
#include <sstream>
#include <vector>
#ifndef endia
#define endia(X) ((((X)&0xFF) << 24) | (((X)&0xFF00) << 8) | (((X)&0xFF0000) >> 8) | (((X)&0xFF000000) >> 24))
#endif

#define SHR(X, N) (X >> N)
#define ROTR(X, N) ((X >> N) | (X << ((sizeof(X) << 3) - N)))

#define Usigma0(X) (ROTR(X, 2) ^ ROTR(X, 13) ^ ROTR(X, 22))
#define Usigma1(X) (ROTR(X, 6) ^ ROTR(X, 11) ^ ROTR(X, 25))
#define Lsigma0(X) (ROTR(X, 7) ^ ROTR(X, 18) ^ SHR(X, 3))
#define Lsigma1(X) (ROTR(X, 17) ^ ROTR(X, 19) ^ SHR(X, 10))
#define Ch(X, Y, Z) ((X & Y) ^ (~X & Z))
#define Maj(X, Y, Z) ((X & Y) ^ (X & Z) ^ (Y & Z))

unsigned long kSHA256[64] = {
    0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5,
    0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
    0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
    0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
    0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc,
    0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
    0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7,
    0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
    0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
    0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
    0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3,
    0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
    0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5,
    0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
    0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
    0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2};

std::string SHA256_MAIN(uint8_t *message, size_t messageLength)
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
    size_t i;
    for (i = 0; i < messageLength; i++)
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
    uint32_t H0 = 0x6a09e667;
    uint32_t H1 = 0xbb67ae85;
    uint32_t H2 = 0x3c6ef372;
    uint32_t H3 = 0xa54ff53a;
    uint32_t H4 = 0x510e527f;
    uint32_t H5 = 0x9b05688c;
    uint32_t H6 = 0x1f83d9ab;
    uint32_t H7 = 0x5be0cd19;

    uint32_t A, B, C, D, E, F, G, H, T1, T2;
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
    md5Message << std::hex << std::setfill('0') << std::setw(8) << (H7);
    return md5Message.str();
}

Napi::Value SHA256(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();
    if (!check(info))
        return env.Null();

    std::string arg0 = info[0].As<Napi::String>();
    Napi::String num = Napi::String::New(env, SHA256_MAIN((uint8_t *)(arg0.c_str()), arg0.length()));
    return reinterpret_cast<Napi::Value &&>(num);
}