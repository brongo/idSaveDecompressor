//////////////////////////////
////   HERE BE DRAGONS   /////
//////////////////////////////

typedef uint64_t QWORD;
template<class T> T __ROL__(T value, int count) {
	const uint64_t nbits = sizeof(T) * 8;

	if (count > 0) {
		count %= nbits;
		T high = value >> (nbits - count);
		if (T(-1) < 0)
			high &= ~((T(-1) << count));
		value <<= count;
		value |= high;
	}
	else {
		count = -count % nbits;
		T low = value << (nbits - count);
		value >>= count;
		value |= low;
	}
	return value;
}

inline uint8_t __ROL1__(uint8_t value, int count) { return __ROL__(value, count); }
inline uint8_t __ROR1__(uint8_t value, int count) { return __ROL__(value, -count); }

inline uint16_t __ROL2__(uint16_t value, int count) { return __ROL__(value, count); }
inline uint16_t __ROR2__(uint16_t value, int count) { return __ROL__(value, -count); }

inline uint32_t __ROL4__(uint32_t value, int count) { return __ROL__(value, count); }
inline uint32_t __ROR4__(uint32_t value, int count) { return __ROL__(value, -count); }

inline uint64_t __ROL8__(uint64_t value, int count) { return __ROL__(value, count); }
inline uint64_t __ROR8__(uint64_t value, int count) { return __ROL__(value, -count); }

__int64 __fastcall sub_141997F40(int* a1, unsigned __int64 a2, int a3)
{
	int v3; // er9
	int v5; // edi
	int v6; // er11
	int v7; // ebx
	int v8; // ecx
	int v9; // edx
	int v10; // ebx
	int v11; // ecx
	int v12; // edx
	int v13; // ecx
	int v14; // edx
	unsigned int v15; // ecx

	v3 = *(int*)((char*)a1 + a2 - 8);
	v5 = *(int*)((char*)a1 + a2 - 4);
	v6 = *(int*)((char*)a1 + (a2 >> 1));
	v7 = v5 + __ROR4__(*(int*)((char*)a1 + (a2 >> 1) - 4), 12);
	v8 = __ROL4__((a3 + a2 - 862048943 * v6) ^ (461845907 * __ROL4__(-862048943 * v3, 15)), 13);
	v9 = v8 - 430675100 + v7 + 4 * v8;
	v10 = v3 + __ROR4__(v7, 3);
	v11 = __ROL4__(v9 ^ (461845907 * __ROL4__(-862048943 * *a1, 15)), 13);
	v12 = v11 - 430675100 + v10 + 4 * v11;
	v13 = __ROR4__(v5 + v10, 12);
	v14 = __ROL4__((461845907 * __ROL4__(-862048943 * (a3 ^ a1[1]), 15)) ^ v12, 13);
	v15 = -2048144789
		* ((v6 + 4 * v14 + v14 + v13 - 430675100) ^ ((unsigned int)(v6 + 4 * v14 + v14 + v13 - 430675100) >> 16));
	return (-1028477387 * (v15 ^ (v15 >> 13))) ^ ((-1028477387 * (v15 ^ (v15 >> 13))) >> 16);
}
#define	HIWORD(...)		static_cast<unsigned int>(static_cast<unsigned short>((__VA_ARGS__) >> 16) )
__int64 __fastcall sub_141998020(int* a1, unsigned __int64 a2, int a3)
{
	unsigned int v3; // ecx
	unsigned int v4; // ecx

	v3 = a3 ^ (5
		* (__ROL4__(
			(5
				* (__ROL4__(
					(461845907 * __ROL4__(-862048943 * (5 * a2 + *(int*)((char*)a1 + a2 - 4)), 15)) ^ (5 * (__ROL4__((5 * a2 + a3) ^ (461845907 * __ROL4__(-862048943 * (a2 + *a1), 15)), 13) - 86135020)),
					13)
					- 86135020)) ^ (461845907
						* __ROL4__(831494105 - 862048943 * *(int*)((char*)a1 + ((a2 >> 1) & 4)), 15)),
			13)
			- 86135020));
	v4 = -1028477387 * ((-2048144789 * (v3 ^ HIWORD(v3))) ^ ((-2048144789 * (v3 ^ HIWORD(v3))) >> 13));
	return v4 ^ HIWORD(v4);
}
__int64 __fastcall sub_141997EB0(__int64 a1, unsigned __int64 a2, int a3)
{
	unsigned __int64 v3; // rax
	int i; // er10
	int v5; // er9
	int v6; // er9
	int v7; // edx
	unsigned int v8; // edx

	v3 = 0i64;
	for (i = 9; v3 < a2; a3 = v6)
	{
		v5 = *(char*)(v3 + a1);
		++v3;
		v6 = v5 - 862048943 * a3;
		i ^= v6;
	}
	v7 = __ROL4__(
		(461845907 * __ROL4__(-862048943 * a3, 15)) ^ (5
			* (__ROL4__(
				i ^ (461845907 * __ROL4__(-862048943 * a2, 15)),
				13)
				- 86135020)),
		13);
	v8 = -1028477387
		* ((-2048144789 * ((5 * (v7 - 86135020)) ^ ((unsigned int)(5 * (v7 - 86135020)) >> 16))) ^ ((-2048144789
			* ((5 * (v7 - 86135020)) ^ ((unsigned int)(5 * (v7 - 86135020)) >> 16))) >> 13));
	return v8 ^ HIWORD(v8);
}

// Main hash function
__int64 __fastcall murmur_hash(char* a1, unsigned __int64 size)
{
	__int64 result; // rax
	int v3; // esi
	int v4; // edi
	unsigned __int64 v5; // r15
	int v6; // ebp
	char* v7; // r14
	int v8; // ebx
	int v9; // er9
	int v10; // er11
	int v11; // er8
	int v12; // er10
	int v13; // ecx
	int v14; // ecx
	int v15; // edi
	int v16; // edx

	if (size > 0x18)
	{
		v3 = 5
			* (__ROL4__(
				(0x1B873593 * __ROL4__(0xCC9E2D51 * *(int*)&a1[size - 16], 15)) ^ (5
					* (__ROL4__(
						size ^ (0x1B873593 * __ROL4__(0xCC9E2D51 * *(int*)&a1[size - 4], 15)),
						13)
						- 0x52250EC)),
				13)
				- 86135020);
		v4 = 5
			* (__ROL4__(
				(0x1B873593 * __ROL4__(0xCC9E2D51 * *(int*)&a1[size - 12], 15)) ^ (5
					* (__ROL4__(
						(0xCC9E2D51 * size) ^ (461845907 * __ROL4__(0xCC9E2D51 * *(int*)&a1[size - 8], 15)),
						13)
						- 86135020)),
				13)
				- 86135020);
		v5 = (size - 1) / 0x14;
		v6 = __ROL4__(0xCC9E2D51 * size + 461845907 * __ROL4__(-862048943 * *(int*)&a1[size - 20], 15), 13) + 113;
		v7 = a1 + 8;
		do
		{
			v8 = *((int*)v7 + 1);
			v9 = *((int*)v7 - 2);
			v10 = *(int*)v7;
			v11 = *((int*)v7 + 2);
			v12 = *((int*)v7 - 1);
			v7 += 20;
			v13 = __ROL4__((v9 + v3) ^ (461845907 * __ROL4__(-862048943 * v8, 15)), 13);
			v3 = v13 - 430675100 + v11 + 4 * v13;
			v14 = __ROL4__((v12 + v4) ^ (461845907 * __ROL4__(-862048943 * v10, 15)), 13);
			v15 = v14 - 430675100 + v9 + 4 * v14;
			v16 = __ROL4__((v10 + v6) ^ (461845907 * __ROL4__(268538785 * v11 - 862048943 * v12, 15)), 13);
			v6 = v15 + v16 + v8 + 4 * (v16 - 107668775);
			v4 = v6 + v15;
			--v5;
		} while (v5);
		result = (unsigned int)(-862048943
			* __ROL4__(
				5
				* __ROL4__(
					-862048943
					* (__ROL4__(-862048943 * __ROR4__(v6, 11), 15)
						+ __ROL4__(
							5
							* (__ROL4__(v3 - 862048943 * __ROL4__(-862048943 * __ROR4__(v4, 11), 15), 13)
								- 86135020),
							15)),
					13)
				- 430675100,
				15));
	}
	else if (size > 0xC)
	{
		result = sub_141997F40((int*)a1, size, 0);
	}
	else if (size > 4)
	{
		result = sub_141998020((int*)a1, size, 0);
	}
	else
	{
		result = sub_141997EB0((__int64)a1, size, 0);
	}
	return result;
}