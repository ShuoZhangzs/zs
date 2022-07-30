# 有关sha256和零知识证明

## 1.基础的sha256的代码实现(sha256.cpp)

__基本过程__

sha256的算法过程与sm3杂凑函数过程相似，都是由padding和iteration组成，iteration又有消息拓展和压缩的过程，在sha256中，消息拓展是将16字拓展为64字，通过线性运算和模加实现，压缩函数也是一系列的线性运算。

__函数说明__

填充函数padding（）：补一个1和一堆0使得长度为448mod512bit，最后64bit填写消息原本二进制长度。因为输入的是字符串类型，所以先将其转换成ASCII码表示，再将ASCII转换成16进制，然后就可以进行补充了。

迭代函数iteration（）：迭代函数首先计算迭代次数，也就是消息填充后是512bit的多少倍（因为是以4bit为一个单位，所以这里是除以128  ```int n = str.size() / 128;```）初始的h是给定的。在每一次迭代中，有消息扩展和压缩两个步骤，两个步骤之后，哈希h得到了更新。

消息拓展函数extension（）：是将16个字拓展成64个字，拓展的算法也是线性运算。

压缩函数compress（）：ABCDEFGH 8个字由传进来的h赋值，然后再进行64轮的线性运算，得到新的ABCDEFGH值，对于得到的新的值，将其加在（模加）传入的h上，就是新的h。


__运行时改变message的内容即可__

```
string message = "lovesong";
```

__以上输入的运行结果是：__


![图片](https://user-images.githubusercontent.com/105595225/181880101-92f72a19-1a5d-4b42-96b5-8f7b8be736e8.png)

## 2.以sha256为基础验证cet6成绩大于等于425，但不让对方知道自己的真实成绩（prove_grade_qualified.cpp）

__基本原理__
一共有三方：证明者me 验证者Emploer 第三方MoE。
第三方有一个chain，要存储所有考生信息。
在第三方妄图存储考生成绩信息时，发放给考生一个秘密号码secret_number，假设考生成绩为n，那么考生的成绩签名(sig_by_moe)就是对secret_number做n次哈希，这里采用sha256作为哈希函数，sig_by_moe是公开的。
考生拥有的secret_number是秘密的，在需要向人证明时，先私下对其做n-425次哈希，将此时的哈希值交给验证者Emploer，只要验证者做了425次哈希之后得到了跟sig_by_moe相同的结果，则可以零知识证明本人成绩合格。

__代码实现时__
，先随机生成一个字符串，作为secret_number

```
string secret_number = random_string(rand() % 20 + 1);
```

MoE对其做grade次哈希sig_by_moe得到并公开

```
string sig_by_moe = to_string(grade);
for (int times = 0; times < grade; times++)
{
	string paddind_result = padding(sig_by_moe);
	sig_by_moe= iteration(paddind_result);
}
```

考生对secret_number做grade-425次哈希并交给验证者

```
string message = to_string(grade);
for (int t = 0; t < grade - 425; t++)
{
  string paddind_result = padding(message);
	message = iteration(paddind_result);
}
```

验证者做425次哈希并与sig_by_moe对比，相等则验证成功

```
for (int t = 0; t < 425; t++)
{
	string paddind_result = padding(message);
	message = iteration(paddind_result);
}
if (message == sig_by_moe)
{
	cout << "证明成绩合格" << endl;
}
```

__代码可以直接运行
运行截图为：__

![图片](https://user-images.githubusercontent.com/105595225/181880456-e6a00600-6c73-4d10-a9b6-45aadc222e79.png)

