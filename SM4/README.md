# __SM4有关实现__
## __1.sm4基础代码的实现(SM4.cpp sm4_单位8bit.cpp)__
  sm4基础代码实现了两种输入方式，一个是4bit一组输入，如下代码
  ```
  unsigned int plaintext[32] = { 0x0,0x1,0x2,0x3,0x4,0x5,0x6,0x7,0x8,0x9,0xa,0xb,0xc,0xd,0xe,0xf,0xf,0xe,0xd,0xc,0xb,0xa,0x9,0x8,0x7,0x6,0x5,0x4,0x3,0x2,0x1,0x0 };
  ```
  
  一个是8bit一组输入，如下代码
  ```
  	unsigned int MK[16] = { 0x01,0x23,0x45,0x67,0x89,0xab,0xcd,0xef,0xfe,0xdc,0xba,0x98,0x76,0x54,0x32,0x10 };
  ```
  
  检验两组效率实际上没有差距。
  两个代码除了输入方式没有区别，都按照sm4的流程走下来。
  在执行的时候，都是设置的运行多遍，通过修改time或times来决定加密多少次,如下表示加密100000次
  ```
  for (int times = 0; times < 100000; times++)
  ```
  
  ![图片](https://user-images.githubusercontent.com/105595225/181192952-57b7ed59-4c4c-4d4e-9f55-55d5707532d5.png)
  上图是执行100000次的运行结果，加密100000×16字节使用了0.946s，计算得sm4的基础实现速率约为1691331字节/秒
  
## __2.sm4代码优化实现(sm4_单位8bit优化.cpp)__
  sm4代码的优化，在这里使用了循环展开和SIMD指令。
  循环展开多见于在for循环中，将step++变成step+=more，进而减少判断（是否出循环）的时间和并行度。例如
  ```
  for (int j = 0; j < 31; j+=2)
		{
			lb[j] = XBbin[j] ^ XBbin[(j + 2) % 32] ^ XBbin[(j + 10) % 32] ^ XBbin[(j + 18) % 32] ^ XBbin[(j + 24) % 32];
			lb[j + 1] = XBbin[j + 1] ^ XBbin[(j + 3) % 32] ^ XBbin[(j + 11) % 32] ^ XBbin[(j + 19) % 32] ^ XBbin[(j + 25) % 32];
		}
  ```
  
  SIMD指令多用于同时使用一些互不干扰的变量时，例如同时取4个plaintext[i]
  ```
  for (int i = 0; i < 4; i++)//SIMD优化
	{
		__m128i tppl = _mm_loadu_epi32(&plaintext[i * 4]);
		_mm_storeu_epi32(&X[i], tppl);
	}
  ```
  
  运行时，也是改变main函数中的times来改变加密次数,如下是加密100000次
  ```
  for (int times = 0; times < 100000; times++)
  ```
  
  运行100000次的结果如下图
  ![图片](https://user-images.githubusercontent.com/105595225/181196968-b7dcab56-7c8c-427f-9511-31f8c2bddfd2.png)
  加密100000×16个字节时间约为0.734，加密效率约为2179837字节/秒
