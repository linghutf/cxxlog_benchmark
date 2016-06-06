# SHELL相关操作

## 1.注释`#`,如`#!/bin/bash`

## 2.变量
- 首字符为字母
- 中间不能有空格，以下划线连接
- 不能使用其他标点符号
- 区分大小写
- 单引号'  忽略到右引号的所有特殊字符
- 双引号"" 忽略到右引号的大多数特殊字符，除$,\,``以外
- 反引号``,命令替换，同$()

```bash
test=world     #等号两边不能有空格
echo "hello $test"
```

### 变量定义

```bash
declare -r         readonly,只读变量
        -i         integer,定义整数
        -a         array,定义数组
        -f         function,显示自定义函数
        -x         export current define,导出当前定义

unset n            清空变量
```

### 变量赋值

```bash
- ${n:-val}        return val if n is not set,else null
- ${n:+val}        return val if n is set,else null
- ${n:?val}        return n if n is set,else return val
- ${n-val}         return val if n is not set,else null
- ${n=val}         return val if n is not set,then n is set val,else return n
- ${n:=val}        return val if n is not set or "",then n is set val,else return n
- ${n+val}         return val if n is not set or "",else return n
- ${n:+val}        同上
- ${n?val}         print val in stderr and exit if n is not set or “”
```

### 字符串

```bash
${#str}               字符串长度
${str:pos[:len]}      提取从pos位置开始的子串，可选长度len
${str/sub[/rep]}      返回str中从头开始,sub替换一次成rep的字符串,str不变
${str//sub[/rep]}     返回str中所有sub替换成rep的字符串,str不变，rep可选
${str/%sub[/rep]}     返回从str末尾,sub替换一次成rep的字符串,str不变
```

### 数组

```bash
a=(1 2 3)             定义数组
${#a[@]}              数组长度
${a[@]}               访问数组
echo ${a[0]}          访问元素
unset a               删除数组
unset a[1]            删除元素,后面的元素不会前移
a[1]=100              元素赋值
${a[@]:1:4}           数组分片a[1],a[2],a[3]
${a[@]/2/10}          替换所有符合条件的元素
```

## 3.流程控制

### 选择语句

- `[]`可以替代test命令来检查cond的真假
- `[[]]`和`[]`的区别:
  - `[[]]`能够使用&& || <>
  - `[]`为shekk内置命令，所有在其中的表达式都是其命令行参数,<,>必须转义
  - `[[]]`是关键字,会进行算术扩展,如`[[ if $a>1+1 ]]`合法
  
```bash
if cond1; then
    ...
elif cond2; then
    ...
else
    ...
fi
```

### 分支选择

- cond可以使用`[[:lower:]]`,`[[:upper:]]`,`[0-9]`,`-d|-debug`这种匹配模式

```bash
case $var in
"cond1")
    ...
    ;;
"cond2")
    ...
    ;;
*)
    ...
    ;;
esac

```

### select选择

- 如果是有效选择，则$name是list中相应的值，无效选择则是null
- 常用if判断是否是有效选择

```bash
select name [in list]
do
    statement that can use $name...
done
```

### 比较关系

- 整数

```bash
-eq       相等
-ne       不等
-gt       大于
-ge       大于等于
-lt       小于
-le       小于等于
```
- 布尔运算

```bash
!         非
-o        或
-a        与
```

- 字符串

```bash
=         相等
!=        不等
-z        字符串为空或长度0返回true
-n        字符串长度大于0返回true
str       str不为空,[ str ]返回true
```

- 文件操作

```bash
符合则返回true

-b        块设备
-c        字符设备
-S        socket
-L        符号链接
-d        目录
-f        普通文件
-g        文件设置SGID位
-k        文件设置Sticky
-p        具名管道
-u        文件设置SUID
-r        可读
-w        可写
-x        可执行
-s        文件不为空
-e        文件(包括目录)存在
-O        文件拥有者
-G        文件拥有者的组
-N        文件自上次读取被修改过
```

### 循环语句

- for循环

```bash
for var in (list);do
    command
    command
    ...
done

for((i=0;i<=10;++i));do
    command
    ...
done
```

- while循环

```bash
while cond;do
    command
done

done

example:
i=100
while [[ $i -gt 0 ]];do
    command
    let "i--"
done
```

- until循环

```bash
until cond;do
    command
done
```

## 4.函数

- 参数$@,${1..9}
- 返回值只能为true/false
- 只是一串命令的集合
- 调用func_name $param1 $param2

```bash
定义:
function add()
{
    let "res = a + b"
}

调用:
add $a $b
echo $res
```

- 调用外部脚本函数
```bash
source other.sh
other_func_name
```

## 5.命令行参数
- `$@`为参数列表
- `$#`为参数个数
- 可以使用shift处理参数，向前移除参数
- $0保留为脚本名称

## 6.调试
- `echo`打印
- sex -x/+x 
- bash -x 
- trap/tee命令
- 日志输出
- 专业调试工具bashdb