#include <iostream>
#include <string>
#include <cmath>
#include<time.h>
using namespace std;

string bin_to_hex(string str)
{
	/*补充为4的倍数*/
	while (str.size() % 4 != 0)
	{
		str = "0" + str;
	}

	string hex = "";
	int temp = 0;
	for (int i = 0; i < str.size(); i += 4) //每4位bit为1个16进制
	{
		/*先转换为10十进制，然后判断是否大于10*/
		temp = (str[i] - '0') * 8 + (str[i + 1] - '0') * 4 + (str[i + 2] - '0') * 2 + (str[i + 3] - '0') * 1;
		if (temp < 10)
		{
			hex += to_string(temp);
		}
		else
		{
			hex += 'A' + (temp - 10);
		}
	}
	return hex;
}

string hex_to_bin(string str)
{
	string bin = "";
	string trans[16] = { "0000","0001","0010","0011","0100","0101","0110","0111","1000","1001","1010","1011","1100","1101","1110","1111" };
	for (int i = 0; i < str.size(); i++)
	{
		if (str[i] >= 'A' && str[i] <= 'F')
		{
			bin += trans[str[i] - 'A' + 10];
		}
		else
		{
			bin += trans[str[i] - '0'];
		}
	}
	return bin;
}

string dec_to_hex(int str)
{
	string hex = "";
	int temp = 0;
	while (str >= 1)
	{
		temp = str % 16;
		if (temp < 10 && temp >= 0)
		{
			hex = to_string(temp) + hex;
		}
		else
		{
			hex += ('A' + (temp - 10));
		}
		str = str / 16;
	}
	return hex;
}

string shift_left(string str, int step)
{
	string res = hex_to_bin(str);
	res = res.substr(step) + res.substr(0, step);
	return bin_to_hex(res);
}

/*32bit异或操作*/
string XOR(string str1, string str2)
{
	string bin1 = hex_to_bin(str1);
	string bin2 = hex_to_bin(str2);
	string res = "";
	for (int i = 0; i < bin1.size(); i++)
	{
		if (bin1[i] == bin2[i])
		{
			res += "0";
		}
		else {
			res += "1";
		}
	}
	return bin_to_hex(res);
}

/*32bit与操作*/
string AND(string str1, string str2)
{
	string bin1 = hex_to_bin(str1);
	string bin2 = hex_to_bin(str2);
	string res = "";
	for (int i = 0; i < bin1.size(); i++)
	{
		if (bin1[i] == '1' && bin2[i] == '1')
		{
			res += "1";
		}
		else
		{
			res += "0";
		}
	}
	return bin_to_hex(res);
}

/*32bit或操作*/
string OR(string str1, string str2)
{
	string bin1 = hex_to_bin(str1);
	string bin2 = hex_to_bin(str2);
	string res = "";
	for (int i = 0; i < bin1.size(); i++)
	{
		if (bin1[i] == '0' && bin2[i] == '0')
		{
			res += "0";
		}
		else {
			res += "1";
		}
	}
	return bin_to_hex(res);
}

/*32bit非操作*/
string NOT(string str)
{
	string bin1 = hex_to_bin(str);
	string res = "";
	for (int i = 0; i < bin1.size(); i++)
	{
		if (bin1[i] == '0') {
			res += "1";
		}
		else {
			res += "0";
		}
	}
	return bin_to_hex(res);
}

/*1 bit异或操作*/
char bit_xor(char str1, char str2)
{
	return str1 == str2 ? '0' : '1';
}

/*1 bit与操作*/
char bit_and(char str1, char str2)
{
	return (str1 == '1' && str2 == '1') ? '1' : '0';
}

/*在mod 2^32上的加法,str1 str2是32bit*/
string add_mod(string str1, string str2)
{
	string bin1 = hex_to_bin(str1);
	string bin2 = hex_to_bin(str2);
	char temp = '0';
	string res = "";
	for (int i = bin1.size() - 1; i >= 0; i--)
	{
		res = bit_xor(bit_xor(bin1[i], bin2[i]), temp) + res;
		if (bit_and(bin1[i], bin2[i]) == '1')
		{
			temp = '1';
		}
		else
		{
			if (bit_xor(bin1[i], bin2[i]) == '1')
			{
				temp = bit_and('1', temp);
			}
			else
			{
				temp = '0';
			}
		}
	}
	return bin_to_hex(res);
}

string P0(string str)
{
	return XOR(XOR(str, shift_left(str, 9)), shift_left(str, 17));
}

string P1(string str)
{
	return XOR(XOR(str, shift_left(str, 15)), shift_left(str, 23));
}

string T(int j)
{
	if (0 <= j && j <= 15) {
		return "79CC4519";
	}
	else {
		return "7A879D8A";
	}
}

string FF(string str1, string str2, string str3, int j)
{
	if (0 <= j && j <= 15)
	{
		return XOR(XOR(str1, str2), str3);
	}
	else if (j >= 16 && j <= 63)
	{
		return OR(OR(AND(str1, str2), AND(str1, str3)), AND(str2, str3));
	}
}

string GG(string str1, string str2, string str3, int j)
{
	if (0 <= j && j <= 15)
	{
		return XOR(XOR(str1, str2), str3);
	}
	else if (j >= 16 && j <= 63)
	{
		return OR(AND(str1, str2), AND(NOT(str1), str3));
	}
}

/*消息填充*/
string padding(string message)
{
	string padding_res = "";
	for (int i = 0; i < message.size(); i++)
	{
		padding_res += dec_to_hex((int)message[i]);
	}
	int length = padding_res.size() * 4;

	/*将比特1添加到消息末尾*/
	padding_res += "8";

	/*0 bit填充 成448 mod 512 */
	while (padding_res.size() % 128 != 112)
	{
		padding_res += "0";
	}

	/*64位bit串表示消息二进制长度*/
	string length_str = dec_to_hex(length);
	while (length_str.size() != 16)
	{
		length_str = "0" + length_str;
	}

	padding_res += length_str;
	return padding_res;
}

/*消息扩展*/
string extension(string str) /*16字 ——> 132字*/
{
	string W_132 = str;
	for (int i = 16; i < 68; i++) //W17-W68
	{
		W_132 += XOR(XOR(P1(XOR(XOR(W_132.substr((i - 16) * 8, 8), W_132.substr((i - 9) * 8, 8)), shift_left(W_132.substr((i - 3) * 8, 8), 15))), shift_left(W_132.substr((i - 13) * 8, 8), 7)), W_132.substr((i - 6) * 8, 8));
	}
	for (int i = 0; i < 64; i++) //W'1-W'64
	{
		W_132 += XOR(W_132.substr(i * 8, 8), W_132.substr((i + 4) * 8, 8));
	}
	return W_132;
}

/*压缩函数*/
string compress(string W, string Vi)
{
	string IV = Vi;
	string A = IV.substr(0, 8);
	string B = IV.substr(8, 8);
	string C = IV.substr(16, 8);
	string D = IV.substr(24, 8);
	string E = IV.substr(32, 8);
	string F = IV.substr(40, 8);
	string G = IV.substr(48, 8);
	string H = IV.substr(56, 8);

	string SS1 = "", SS2 = "", TT1 = "", TT2 = "";
	for (int j = 0; j < 64; j++)
	{
		SS1 = shift_left(add_mod(add_mod(shift_left(A, 12), E), shift_left(T(j), (j % 32))), 7);
		SS2 = XOR(SS1, shift_left(A, 12));
		TT1 = add_mod(add_mod(add_mod(FF(A, B, C, j), D), SS2), W.substr((j + 68) * 8, 8));
		TT2 = add_mod(add_mod(add_mod(GG(E, F, G, j), H), SS1), W.substr(j * 8, 8));
		D = C;
		C = shift_left(B, 9);
		B = A;
		A = TT1;
		H = G;
		G = shift_left(F, 19);
		F = E;
		E = P0(TT2);
	}
	return (A + B + C + D + E + F + G + H);
}

/*迭代*/
string iteration(string str)
{
	//迭代次数
	int n = str.size() / 128;

	string V = "7380166F4914B2B9172442D7DA8A0600A96F30BC163138AAE38DEE4DB0FB0E4E";
	string Bi = "";
	string W_132 = "";
	string ABCDEFGH = "";
	for (int i = 0; i < n; i++)
	{
		Bi = str.substr(i * 128, 128);

		//#1.消息扩展
		W_132 = extension(Bi);
		//#2.压缩函数
		ABCDEFGH = compress(W_132, V);

		V = XOR(V, ABCDEFGH);
	}
	return V;
}
string random_string(const int len)
{
	string str;                 
	for (int i = 0; i < len; i++)
	{
		str+='a' + rand() % 26;
	}
	return str; 
}

int main()
{
	clock_t start = clock();

	int n = 16;

	/*time是使碰撞概率达到50%的次数*/
	int times = 0.83 * pow(2, n / 2);
	for (int i = 0; i < times; i++)
	{
		string m1 = random_string(rand()%n+1);
		string paddind_result1 = padding(m1);
		string hash1 = iteration(paddind_result1);

		string m2 = random_string(rand()%n+1);
		string paddind_result2 = padding(m2);
		string hash2 = iteration(paddind_result2);
		if (hash1.substr(0, n) == hash2.substr(0, n))
		{
			cout << "第一个消息：" << m1 << endl;
			cout << "第二个消息:" << m2 << endl;
			cout << n << "bit 碰撞成功" << endl;
			break;
		}
	}

	clock_t end = clock();
	double timespend = ((double)end - (double)start) / CLOCKS_PER_SEC;
	cout << "时间消耗:" << timespend << endl;

	return 0;
}