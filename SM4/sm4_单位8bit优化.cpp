#include<iostream>
using namespace std;
#include<time.h>
#include<intrin.h>
int S[16][16] =
{
	{0xd6,0x90,0xe9,0xfe,0xcc,0xe1,0x3d,0xb7,0x16,0xb6,0x14,0xc2,0x28,0xfb,0x2c,0x05},
	{0x2b,0x67,0x9a,0x76,0x2a,0xbe,0x04,0xc3,0xaa,0x44,0x13,0x26,0x49,0x86,0x06,0x99},
	{0x9c,0x42,0x50,0xf4,0x91,0xef,0x98,0x7a,0x33,0x54,0x0b,0x43,0xed,0xcf,0xac,0x62},
	{0xe4,0xb3,0x1c,0xa9,0xc9,0x08,0xe8,0x95,0x80,0xdf,0x94,0xfa,0x75,0x8f,0x3f,0xa6},
	{0x47,0x07,0xa7,0xfc,0xf3,0x73,0x17,0xba,0x83,0x59,0x3c,0x19,0xe6,0x85,0x4f,0xa8},
	{0x68,0x6b,0x81,0xb2,0x71,0x64,0xda,0x8b,0xf8,0xeb,0x0f,0x4b,0x70,0x56,0x9d,0x35},
	{0x1e,0x24,0x0e,0x5e,0x63,0x58,0xd1,0xa2,0x25,0x22,0x7c,0x3b,0x01,0x21,0x78,0x87},
	{0xd4,0x00,0x46,0x57,0x9f,0xd3,0x27,0x52,0x4c,0x36,0x02,0xe7,0xa0,0xc4,0xc8,0x9e},
	{0xea,0xbf,0x8a,0xd2,0x40,0xc7,0x38,0xb5,0xa3,0xf7,0xf2,0xce,0xf9,0x61,0x15,0xa1},
	{0xe0,0xae,0x5d,0xa4,0x9b,0x34,0x1a,0x55,0xad,0x93,0x32,0x30,0xf5,0x8c,0xb1,0xe3},
	{0x1d,0xf6,0xe2,0x2e,0x82,0x66,0xca,0x60,0xc0,0x29,0x23,0xab,0x0d,0x53,0x4e,0x6f},
	{0xd5,0xdb,0x37,0x45,0xde,0xfd,0x8e,0x2f,0x03,0xff,0x6a,0x72,0x6d,0x6c,0x5b,0x51},
	{0x8d,0x1b,0xaf,0x92,0xbb,0xdd,0xbc,0x7f,0x11,0xd9,0x5c,0x41,0x1f,0x10,0x5a,0xd8},
	{0x0a,0xc1,0x31,0x88,0xa5,0xcd,0x7b,0xbd,0x2d,0x74,0xd0,0x12,0xb8,0xe5,0xb4,0xb0},
	{0x89,0x69,0x97,0x4a,0x0c,0x96,0x77,0x7e,0x65,0xb9,0xf1,0x09,0xc5,0x6e,0xc6,0x84},
	{0x18,0xf0,0x7d,0xec,0x3a,0xdc,0x4d,0x20,0x79,0xee,0x5f,0x3e,0xd7,0xcb,0x39,0x48}
};
void enc(unsigned int plaintext[], unsigned int rk[][4], int Y[][4])
{
	int X[4][4] = { 0 };
	unsigned int XA[4] = { 0 };
	unsigned int XB[4] = { 0 };
	unsigned int XBbin[32] = { 0 };
	unsigned int FX[4] = { 0 };
	int hang = 0, lie = 0;
	unsigned int lb[32] = { 0 };
	unsigned int lbhex[4] = { 0 };

	for (int i = 0; i < 4; i++)//SIMD优化
	{
		__m128i tppl = _mm_loadu_epi32(&plaintext[i * 4]);
		_mm_storeu_epi32(&X[i], tppl);
	}
	

	for (int i = 0; i < 32; i++)
	{
		//XA=X1^X2^X3^rk//SIMD优化
		__m128i temp1 = _mm_xor_si128(_mm_loadu_epi32(&X[1]), _mm_loadu_epi32(&X[2]));
		__m128i temp2 = _mm_xor_si128(_mm_loadu_epi32(&X[3]), _mm_loadu_epi32(&rk[i]));
		_mm_storeu_epi32(&XA, _mm_xor_si128(temp1, temp2));

		for (int j = 0; j < 4; j++)
		{
			hang = XA[j] / 16;
			lie = XA[j] % 16;
			XB[j] = S[hang][lie];

			for (int k = 7; k >= 0; k--)
			{
				XBbin[8 * j + k] = XB[j] % 2;
				XB[j] = XB[j] / 2;
			}
		}

		//循环展开优化
		for (int j = 0; j < 31; j+=2)
		{
			lb[j] = XBbin[j] ^ XBbin[(j + 2) % 32] ^ XBbin[(j + 10) % 32] ^ XBbin[(j + 18) % 32] ^ XBbin[(j + 24) % 32];
			lb[j + 1] = XBbin[j + 1] ^ XBbin[(j + 3) % 32] ^ XBbin[(j + 11) % 32] ^ XBbin[(j + 19) % 32] ^ XBbin[(j + 25) % 32];
		}


		//循环展开优化
		for (int j = 0; j < 3; j+=2)
		{
			int _j8 = j * 8;
			lbhex[j] = lb[_j8] * 128 + lb[_j8 + 1] *64+ lb[_j8 + 2] * 32 + lb[_j8 + 3] * 16 + lb[_j8 + 4] *8 + lb[_j8 + 5] * 4 + lb[_j8 + 6] * 2 + lb[_j8 + 7];
			FX[j] = X[0][j] ^ lbhex[j];

			int _j_1_8 = (j+1) * 8;
			lbhex[j+1] = lb[_j_1_8] * 128 + lb[_j_1_8 + 1] * 64 + lb[_j_1_8 + 2] * 32 + lb[_j_1_8 + 3] * 16 + lb[_j_1_8 + 4] * 8 + lb[_j_1_8 + 5] * 4 + lb[_j_1_8 + 6] * 2 + lb[_j_1_8 + 7];
			FX[j+1] = X[0][j+1] ^ lbhex[j+1];
		}

		//SIMD
		for (int j = 0; j < 3; j++)
		{
			__m128i xjj = _mm_loadu_epi32(&X[j + 1]);
			_mm_storeu_epi32(&X[j], xjj);
		}
		__m128i fx = _mm_loadu_epi32(&FX);
		_mm_storeu_epi32(&X[3], fx);
	}
	for (int i = 3; i >= 0; i--)//SIMD优化
	{
		__m128i tpx = _mm_loadu_epi32(&X[i]);
		_mm_storeu_epi32(&Y[3 - i], tpx);
	}
}

int FK[4][4] =
{
	{0xa3,0xb1,0xba,0xc6},
	{0x56,0xaa,0x33,0x50},
	{0x67,0x7d,0x91,0x97},
	{0xb2,0x70,0x22,0xdc}
};
int CK[32][8] =
{
	{0x00,0x07,0x0e,0x15},
	{0x1c,0x23,0x2a,0x31},
	{0x38,0x3f,0x46,0x4d},
	{0x54,0x5b,0x62,0x69},
	{0x70,0x77,0x7e,0x85},
	{0x8c,0x93,0x9a,0xa1},
	{0xa8,0xaf,0xb6,0xbd},
	{0xc4,0xcb,0xd2,0xd9},
	{0xe0,0xe7,0xee,0xf5},
	{0xfc,0x03,0x0a,0x11},
	{0x18,0x1f,0x26,0x2d},
	{0x34,0x3b,0x42,0x49},
	{0x50,0x57,0x5e,0x65},
	{0x6c,0x73,0x7a,0x81},
	{0x88,0x8f,0x96,0x9d},
	{0xa4,0xab,0xb2,0xb9},
	{0xc0,0xc7,0xce,0xd5},
	{0xdc,0xe3,0xea,0xf1},
	{0xf8,0xff,0x06,0x0d},
	{0x14,0x1b,0x22,0x29},
	{0x30,0x37,0x3e,0x45},
	{0x4c,0x53,0x5a,0x61},
	{0x68,0x6f,0x76,0x7d},
	{0x84,0x8b,0x92,0x99},
	{0xa0,0xa7,0xae,0xb5},
	{0xbc,0xc3,0xca,0xd1},
	{0xd8,0xdf,0xe6,0xed},
	{0xf4,0xfb,0x02,0x09},
	{0x10,0x17,0x1e,0x25},
	{0x2c,0x33,0x3a,0x41},
	{0x48,0x4f,0x56,0x5d},
	{0x64,0x6b,0x72,0x79}

};


int main()
{

	
	unsigned int plaintext[16] = { 0 };
	unsigned int rk[32][4] = { 0 };
	unsigned int MK[16] = { 0x01,0x23,0x45,0x67,0x89,0xab,0xcd,0xef,0xfe,0xdc,0xba,0x98,0x76,0x54,0x32,0x10 };
	unsigned int K[4][4] = { 0 };
	int hang = 0, lie = 0;
	//32轮求密钥
	unsigned int KA[4] = { 0 };
	unsigned int KB[4] = { 0 };
	unsigned int KBbin[32] = { 0 };
	unsigned int lb[32] = { 0 };
	unsigned int lbhex[4] = { 0 };
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			K[i][j] = MK[i * 4 + j] ^ FK[i][j];
		}
	}
	for (int i = 0; i < 32; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			KA[j] = K[1][j] ^ K[2][j] ^ K[3][j] ^ CK[i][j];
			//A进S盒
			hang = KA[j] / 16;
			lie = KA[j] % 16;
			KB[j] = S[hang][lie];

			//B转换为2进制
			for (int k = 7; k >= 0; k--)
			{
				KBbin[8 * j + k] = KB[j] % 2;
				KB[j] = KB[j] / 2;
			}
		}


		//B移位异或
		for (int j = 0; j < 32; j++)
		{
			lb[j] = KBbin[j] ^ KBbin[(j + 13) % 32] ^ KBbin[(j + 23) % 32];
		}


		//rki=K0^lb


		for (int j = 0; j < 4; j++)
		{
			lbhex[j] = lb[j * 8] * 2 * 2 * 2 * 2 * 2 * 2 * 2 + lb[j * 8 + 1] * 2 * 2 * 2 * 2 * 2 * 2 + lb[j * 8 + 2] * 2 * 2 * 2 * 2 * 2 + lb[j * 8 + 3] * 2 * 2 * 2 * 2 + lb[j * 8 + 4] * 2 * 2 * 2 + lb[j * 8 + 5] * 2 * 2 + lb[j * 8 + 6] * 2 + lb[j * 8 + 7];
			rk[i][j] = K[0][j] ^ lbhex[j];
		}


		//K0=K1,K1=K2,K2=K3,K3=K4
		for (int j = 0; j < 3; j++)
		{
			for (int k = 0; k < 4; k++)
			{
				K[j][k] = K[j + 1][k];
			}
		}
		for (int j = 0; j < 4; j++)
		{
			K[3][j] = rk[i][j];
		}

	}


	int Y[4][4] = { 0 };
	clock_t start = clock();

	for (int times = 0; times < 10000; times++)
	{
		enc(plaintext, rk, Y);
	}

	clock_t end = clock();

	double timespend = ((double)end - (double)start) / CLOCKS_PER_SEC;
	cout << "加密10000时间为:" << timespend << " s" << endl; //加密10000时间为:0.072 s
	return 0;
}
