#include<iostream>
#include <string>
#include <cmath>
using namespace std;
string k = "428a2f9871374491b5c0fbcfe9b5dba53956c25b59f111f1923f82a4ab1c5ed5d807aa9812835b01243185be550c7dc372be5d7480deb1fe9bdc06a7c19bf174e49b69c1efbe47860fc19dc6240ca1cc2de92c6f4a7484aa5cb0a9dc76f988da983e5152a831c66db00327c8bf597fc7c6e00bf3d5a7914706ca63511429296727b70a852e1b21384d2c6dfc53380d13650a7354766a0abb81c2c92e92722c85a2bfe8a1a81a664bc24b8b70c76c51a3d192e819d6990624f40e3585106aa07019a4c1161e376c082748774c34b0bcb5391c0cb34ed8aa4a5b9cca4f682e6ff3748f82ee78a5636f84c878148cc7020890befffaa4506cebbef9a3f7c67178f2";
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
			hex += ('a' + (temp - 10));
		}
		str = str / 16;
	}
	return hex;
}
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
			hex += 'a' + (temp - 10);
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
		if (str[i] >= 'a' && str[i] <= 'f')
		{
			bin += trans[str[i] - 'a' + 10];
		}
		else
		{
			bin += trans[str[i] - '0'];
		}
	}
	return bin;
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
/*循环右移*/
string S(string str, int step)
{
	string res = hex_to_bin(str);
	int len = str.length() * 4;
	res = res.substr(len-step) + res.substr(0, len-step);
	return bin_to_hex(res);
}

/*右移*/
string R(string str, int step)
{
	string res = hex_to_bin(str);
	int len = str.length() * 4;
	string tmp = "";
	for (int i = 0; i < step; i++)
	{
		tmp += "0";
	}
	res = tmp + res.substr(0, len - step);
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

string P0(string str)
{
	return XOR(XOR(S(str, 7), S(str,18)), R(str, 3));
}
string P1(string str)
{
	return XOR(XOR(S(str, 17), S(str, 19)), R(str, 10));
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
string extension(string str) /*16字 ——> 64字*/
{
	string W_64 = str;
	for (int i = 16; i < 64; i++) 
	{
		W_64 += add_mod(add_mod(add_mod(W_64.substr((i - 7) * 8, 8), W_64.substr((i - 16) * 8, 8)), P1(W_64.substr((i - 2) * 8, 8))), P0(W_64.substr((i - 15) * 8, 8)));
	}
	
	return W_64;
}
/*压缩函数*/
string compress(string W, string hi)
{
	string IV = hi;
	string A = IV.substr(0, 8);
	string B = IV.substr(8, 8);
	string C = IV.substr(16, 8);
	string D = IV.substr(24, 8);
	string E = IV.substr(32, 8);
	string F = IV.substr(40, 8);
	string G = IV.substr(48, 8);
	string H = IV.substr(56, 8);


	string s0 = "", maj = "", t2 = "", s1 = "", ch = "", t1 = "";
	for (int j = 0; j < 64; j++)
	{
		s0 = XOR(XOR(S(A, 2), S(A, 13)), S(A, 22));
		maj = XOR(XOR(AND(A, B), AND(A, C)), AND(B, C));
		t2 = add_mod(s0, maj);
		s1 = XOR(XOR(S(E, 6), S(E, 11)), S(E, 25));
		ch = XOR(AND(E, F), AND(NOT(E), G));
		t1 = add_mod(add_mod(add_mod(add_mod(H, s1),ch),k.substr(j * 8, 8)),W.substr(j * 8, 8));
		H = G;
		G = F;
		F = E;
		E = add_mod(D, t1);
		D = C;
		C = B;
		B = A;
		A = add_mod(t1, t2);
	}
	return add_mod(IV.substr(0, 8), A) + add_mod(IV.substr(8, 8), B) + add_mod(IV.substr(16, 8), C) + add_mod(IV.substr(24, 8), D) + add_mod(IV.substr(32, 8), E) + add_mod(IV.substr(40, 8), F) + add_mod(IV.substr(48, 8), G) + add_mod(IV.substr(56, 8), H);
}
/*迭代*/
string iteration(string str)
{
	//迭代次数
	int n = str.size() / 128;
	string h = "6a09e667bb67ae853c6ef372a54ff53a510e527f9b05688c1f83d9ab5be0cd19";
	string Bi = "";
	string W_64 = "";
	string ABCDEFGH = "";
	for (int i = 0; i < n; i++)
	{
		Bi = str.substr(i * 128, 128);

		//#1.消息扩展
		W_64 = extension(Bi);
		//#2.压缩函数
		h=compress(W_64, h);
	}
	return h;
}
int main()
{
	string message = "lovesong";
	/*算法过程*/
	//#1.填充
	string paddind_result = padding(message);
	//#2.迭代过程
	string hash = iteration(paddind_result);


	cout << "杂凑值：" << endl;
	for (int i = 0; i < 8; i++) {
		cout << hash.substr(i * 8, 8) << "  ";
	}
	cout << endl;
}
