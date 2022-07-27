# __SM3的有关实现__

## __1.sm3基础代码（sm3.cpp）__
  算法过程大致包含两个步骤，一个是信息填充padding，一个是迭代过程，迭代过程又分为扩展和压缩过程。  
  padding将消息补充为512bit的倍数，迭代的次数依赖于padding的结果，即文本是多少倍的512bit，这将文本分为很多组，假设为n组，则需迭代n次。  
  在每次迭代中：  
    先将512bit（16个字）的消息进行扩展，扩展为132个字  
    ```
    W_132 = extension(Bi);
    ```  
    扩展之后进行消息的压缩，压缩涉及两个变量，一个是IV，一个是刚刚消息扩展的结果。  
    ```
    ABCDEFGH = compress(W_132, V);
    ```  
    IV第一次是固定的，假设要迭代很多次，之后的IV是上一次压缩的结果与上一次IV的异或  
    ```
    V = XOR(V, ABCDEFGH);
    ```  
  代码运行：  
    改变mian函数中的message值：如下输入一堆随机字母进行加密  
    ```
    string message = "fajdkfnnvjsdinfajdfnieruvbjnfxnvzjdfivaneiofnhrjkxcjsadnovnjafsnvjfb"; 
    ```  
  杂凑结果：  
    ![图片](https://user-images.githubusercontent.com/105595225/181201891-92754346-b2a4-4b14-ad21-3d2ec4164233.png)  

## __2.sm3长度拓展攻击(sm3_length_extension.cpp)__  
  长度拓展攻击关键在padding。  
    首先生成了一个随机的消息，长度在512bit以内（为了方便实现）。然后对它进行padding和哈希，IV使用的还是正常哈希的IV。  
    ```
    string message = random_string(10);	  
    string paddind_result1 = padding(message);  
    string hash1 = iteration(paddind_result1,IV);  
    cout <<"hash1:"<< hash1 << endl;  
    ```  
    然后再生成一个随机的附加消息，这个消息的IV是刚刚的hash1  
    ```
    string m = random_string(10);
    string padding_result2 = padding(m);
    string hash2 = iteration(padding_result2,hash1); 
    cout << "hash2:" << hash2 << endl;
    ```  
    最后，构造新的消息，这个消息内容是前两次padding之后的结果拼接。对其进行哈希，得到hash3  
    ```
    string attack_m = paddind_result1 + padding_result2;
    string hash3= iteration(attack_m, IV);
    cout << "hash3:" << hash3<< endl;
    ```      
    如果hash3=hash2则成功  
  此代码直接运行，运行结果为：  
  ![图片](https://user-images.githubusercontent.com/105595225/181204318-37126c39-a506-4231-bb2b-3b2e9a20aea4.png)  

## __3.sm3生日攻击(sm3_生日攻击.cpp)__  
  生日攻击，实际上只是用概率来指导散列冲突的发现。  
  要使碰撞概率达到50%，则需要k=0.83 * √n组哈希，SM3报文输出长度为256bit，所以对于SM3，如果尝试0.83*（2** 128）个报文，那它们之间正好有一个碰撞的概率为50%。  
  此实验只能达到少部分bit的碰撞。这里实现了前8bit的碰撞。  
  times是运行多少次会使得碰撞概率达到50%，这里times=0.83* （2 ** （n* 4/2））  
  运行：  
    修改n可以实现n * 4bit的碰撞，（因为n的单位是4bit）。  
  运行结果为：  
  ![图片](https://user-images.githubusercontent.com/105595225/181213553-a9ad963c-e059-4ffe-bbbc-e74e9e926ada.png)
  
  
  
