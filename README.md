# 此库是创新创业实践学期项目作业整理

# 完成人员：张硕 账户名称：ShuoZhangzs

# 项目简介：

完成了基于三个算法的相关实验：

sm3：实现了基本算法、生日攻击、长度拓展攻击 （完成人：张硕）

sm4：实现了基本算法、算法的软件优化 （完成人：张硕）

sha256：实现了基本算法、以sha256为基础的零知识证明 （完成人：张硕）

# 已完成的项目清单：

__已完成__：  
1.implement the naïve birthday attack of reduced SM3  
2.implement length extension attack for SM3, SHA256, etc  
3.实现sm4算法和算法优化  
4.Write a circuit to prove that your CET6 grade is larger than 425.   
a. Your grade info is like (cn_id, grade, year, sig_by_moe). These grades are published as commitments onchain by MoE.   
b. When you got an interview from an employer, you can prove to them that you have passed the exam without letting them know the exact grade.  
The commitment scheme used by MoE is SHA256-based.commit = SHA256(cn_id, grade, year, sig_by_moe, r)
