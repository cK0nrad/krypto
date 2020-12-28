#ifndef GENERAL
#define LEFTROTATE(A, N) (((A) << (N)) | ((A) >> (32 - (N))))
#define RIGHTROTATE(X, N) ((X >> N) | (X << ((sizeof(X) << 3) - (N))))
#define ENDIAN(X) ((((X)&0xFF) << 24) | (((X)&0xFF00) << 8) | (((X)&0xFF0000) >> 8) | (((X)&0xFF000000) >> 24))
#define GENERAL
#endif

#ifndef HAVAL
#define HAHVAL_F1(x6, x5, x4, x3, x2, x1, x0) (((x1) & ((x0) ^ (x4))) ^ ((x2) & (x5)) ^ ((x3) & (x6)) ^ (x0))
#define HAHVAL_F2(x6, x5, x4, x3, x2, x1, x0) (((x2) & (((x1) & (~(x3))) ^ ((x4) & (x5)) ^ (x6) ^ (x0))) ^ (((x4) & ((x1) ^ (x5))) ^ ((x3) & (x5)) ^ (x0)))
#define HAHVAL_F3(x6, x5, x4, x3, x2, x1, x0) (((x3) & (((x1) & (x2)) ^ (x6) ^ (x0))) ^ ((x1) & (x4)) ^ ((x2) & (x5)) ^ (x0))
#define HAHVAL_F4(x6, x5, x4, x3, x2, x1, x0) (((x4) & (((x5) & ~(x2)) ^ (((x3) & ~(x6)) ^ (x1) ^ (x6) ^ (x0)))) ^ ((x3) & (((x1) & (x2)) ^ (x5) ^ (x6))) ^ ((x2) & (x6)) ^ (x0))
#define HAHVAL_F5(x6, x5, x4, x3, x2, x1, x0) (((x0) & (((x1) & (x2) & (x3)) ^ ~(x5))) ^ ((x1) & (x4)) ^ ((x2) & (x5)) ^ ((x3) & (x6)))
#define HAVAL
#endif