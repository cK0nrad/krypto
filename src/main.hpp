#ifndef MAIN_INCLUDE
Napi::Value MD2(const Napi::CallbackInfo &info);
Napi::Value MD4(const Napi::CallbackInfo &info);
Napi::Value MD5(const Napi::CallbackInfo &info);
Napi::Value MD5X2(const Napi::CallbackInfo &info);
Napi::Value MD5X3(const Napi::CallbackInfo &info);
Napi::Value MD5X4(const Napi::CallbackInfo &info);
Napi::Value MD5X5(const Napi::CallbackInfo &info);

Napi::Value SHA1(const Napi::CallbackInfo &info);
Napi::Value SHA224(const Napi::CallbackInfo &info);
Napi::Value SHA256(const Napi::CallbackInfo &info);
Napi::Value SHA384(const Napi::CallbackInfo &info);
Napi::Value SHA512(const Napi::CallbackInfo &info);

Napi::Value RIPEMD128(const Napi::CallbackInfo &info);
Napi::Value RIPEMD160(const Napi::CallbackInfo &info);
Napi::Value RIPEMD256(const Napi::CallbackInfo &info);
Napi::Value RIPEMD320(const Napi::CallbackInfo &info);

Napi::Value HAVAL3_128(const Napi::CallbackInfo &info);
Napi::Value HAVAL3_160(const Napi::CallbackInfo &info);
Napi::Value HAVAL3_192(const Napi::CallbackInfo &info);
Napi::Value HAVAL3_224(const Napi::CallbackInfo &info);
Napi::Value HAVAL3_256(const Napi::CallbackInfo &info);

Napi::Value HAVAL4_128(const Napi::CallbackInfo &info);
Napi::Value HAVAL4_160(const Napi::CallbackInfo &info);
Napi::Value HAVAL4_192(const Napi::CallbackInfo &info);
Napi::Value HAVAL4_224(const Napi::CallbackInfo &info);
Napi::Value HAVAL4_256(const Napi::CallbackInfo &info);

Napi::Value HAVAL5_128(const Napi::CallbackInfo &info);
Napi::Value HAVAL5_160(const Napi::CallbackInfo &info);
Napi::Value HAVAL5_192(const Napi::CallbackInfo &info);
Napi::Value HAVAL5_224(const Napi::CallbackInfo &info);
Napi::Value HAVAL5_256(const Napi::CallbackInfo &info);

Napi::Value TIGER3_128(const Napi::CallbackInfo &info);
Napi::Value TIGER3_160(const Napi::CallbackInfo &info);
Napi::Value TIGER3_192(const Napi::CallbackInfo &info);

Napi::Value TIGER4_128(const Napi::CallbackInfo &info);
Napi::Value TIGER4_160(const Napi::CallbackInfo &info);
Napi::Value TIGER4_192(const Napi::CallbackInfo &info);

Napi::Value FNV132(const Napi::CallbackInfo &info);
Napi::Value FNV164(const Napi::CallbackInfo &info);

Napi::Value FNV1A32(const Napi::CallbackInfo &info);
Napi::Value FNV1A64(const Napi::CallbackInfo &info);
Napi::Value FNV1A128(const Napi::CallbackInfo &info);
Napi::Value FNV1A256(const Napi::CallbackInfo &info);
Napi::Value FNV1A512(const Napi::CallbackInfo &info);
Napi::Value FNV1A1024(const Napi::CallbackInfo &info);

Napi::Value ADLER32(const Napi::CallbackInfo &info);
Napi::Value CRC32(const Napi::CallbackInfo &info);

#define MAIN_INCLUDE
#endif