### 本章支持的语法

```
 statements: statement
      |      statement statements
      ;

 statement: 'print' expression ';'
      |     'int'   identifier ';'
      |     identifier '=' expression ';'
      ;

 identifier: T_IDENT
      ;
```

### 解析流程

scan() 扫描第一个 token, statements() 根据 token 类型调用不同的解析函数, 例如:
- 如果 token 是 'int', 则调用 int_statement()
- 如果 token 是 T_IDENT, 则调用 assignment_statement()
- 如果 token 是 'print', 则调用 print_statement()

处理完当前 statement 后, 判断 token 是否是 'EOF', 如果不是, 则继续根据 token 类型调用不同的解析函数

**int**:
- 确认并消费 'int' 关键字
- 注册全局变量到符号表
- 生成汇编代码`format(".comm\t%s,8,8", varName)`, 分配内存空间
- 确认并消费 ';' 分号

**T_IDENT**:
- 确认并消费 T_IDENT 标识符
- 确认并消费 '=' 赋值运算符
- 解析后续表达式生成 AST 树
- 将 AST 树翻译并生成汇编代码
- 确认并消费 ';' 分号

**print**:
- 确认并消费 'print' 关键字
- 解析后续表达式生成 AST 树
- 将 AST 树翻译并生成汇编代码
- 确认并消费 ';' 分号