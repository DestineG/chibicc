## 本章新增支持语法

``` c
void func(){
    statements;
}
```

**主要改动:**
- 在 Token 中添加 T_VOID 类型，在 AST 中添加 A_FUNCTION 类型
- 在 keyword() 中添加对 void 关键字的识别
- 在 statements.c 中添加 function statement 处理逻辑
- 在 gen.c | cgen.c 添加 function 处理逻辑
- 改动汇编前导生成函数

## 测试

**测试方法**
``` bash
cd 11_Function_pt1
make clean
make
make test
```

**测试结果**
``` bash
--- 运行结果如下 ---
./build/out
36
10
25
--- 测试完成 ---
```