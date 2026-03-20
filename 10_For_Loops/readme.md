## 本章新增支持语法

```
for(pre;cond;post){
    body
}
```

**主要改动:**
- 分别在 Token 添加 T_FOR
- 在 keyword() 添加对 for 的识别
- 将 compound_statement() 函数拆分出 single_statement()；single_statement 仅负责单条语句的处理，compound_statement 负责代码块的处理
- 在 statements.c 中添加对 for 代码块的处理

**原理**
将 for 循环转换为 while 循环，完全复用 while 的后端生成
```
for(pre;cond;post){
    body
}

等价于

pre
while(cond){
    body
    post
}
```
注：在 body 中没有 continue 语句的时候等价关系成立，若存在 continue 语句，则需要对该语句进行特殊处理，跳转的位置是在 post 的开始处

## 测试

**测试方法**
``` bash
cd 10_For_Loops
make clean
make
make test
```

**测试结果**
``` bash
--- 运行结果如下 ---
./build/out
36
72
10
25
55
4950
--- 测试完成 ---
```