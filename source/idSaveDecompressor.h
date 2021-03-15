#pragma once
#define WIN32_LEAN_AND_MEAN
#define SAFE_SPACE 64
#include <iostream>
#include <vector>
#include <WinSock2.h> // Additional dependencies: ws2_32.lib - for ntohl
#include <Windows.h>
#include <cstdint>
#include "murmur.h"

typedef unsigned char byte;
typedef unsigned char uint8;
typedef signed short int16;
typedef unsigned short uint16;
typedef signed int int32;
typedef unsigned int uint;
typedef unsigned int uint32;
typedef unsigned __int64 uint64;
typedef signed __int64 int64;

typedef int WINAPI OodLZ_CompressFunc(
    int codec, uint8* src_buf, size_t src_len, uint8* dst_buf, int level,
    void* opts, size_t offs, size_t unused, void* scratch, size_t scratch_size);

typedef int WINAPI OodLZ_DecompressFunc(
    uint8* src_buf, int src_len, uint8* dst, size_t dst_size, int fuzz, int crc, int verbose,
    uint8* dst_base, size_t e, void* cb, void* cb_ctx, void* scratch, size_t scratch_size, int threadPhase);

OodLZ_CompressFunc* OodLZ_Compress;
OodLZ_DecompressFunc* OodLZ_Decompress;

const int idchunksize = 131072; // game splits data into 131072 byte chunks before compressing