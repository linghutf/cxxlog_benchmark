# Linux基本操作

## 文件查找

### 1.find,grep常用于文件匹配查找

#### 用法: find dir condition action
  - dir:       要搜索的目录及其子目录,默认当前目录
  - condition: 要搜索的文件的特征
  - action:    对符合条件的处理

```bash
find . -name "xxx" -type f -exec file {} \;      对所有xxx文件查看文件信息
find . -name "[a-z][a-z][0-9].md" -print         查看所有以两个小写字母开头，一个数字结尾的markdown文件
find . -size +1000000c -print                    查看当前目录所有大于1M的文件
find . -size 100c -print                         查看所有大小为100byte的文件
find . -maxdepth 2 -name "*.cpp"                 在当前目录及其一级子目录查找cpp后缀文件
find . -mtime -2 -type f                         查看最近2天内改动过的文件
find . -amin +30 -type d                         查看30分钟之前存取的目录

find . -type f|grep -e \\w+.md$                  使用正则表达式查看所有英文名的md文件

find . -path "cache" -o -path "tmp" -prune -o |egrep .cpp$ 
                                                 在当前目录下排除caheche,tmp两个目录查找cpp文件
```

### 2.locate 文件定位
#### 用法:常用于系统安装的工具定位
   - 依赖于mlocate.db数据库
   - updatedb生成

```bash
locate clang                                     查看clang命令路径
locate -i fileinfo.cpp                           忽略大小写,查看fileinfo.cpp所在位置
```

### 3.其他
1. `whereis`只用于程序名查找
2. `which` 在`PATH`定义的环境变量中搜索程序命令的位置
3. `type`  区分命令是shell自带还是外部命令,外部命令使用`-p`和`which`相同

```bash
whereis make                                     查看make命令位置
which tcpdump                                    查看tcpdump位置
type cd                                          查看cd命令属性
```

## 目录相关
 - 创建目录mkdir
 - 切换目录cd
 - 缓存目录pushd/popd
 - 列出目录ls

```bash
mkdir -p ~/test/{a,b}                            创建test下a,b两个子目录，若不存在test,则创建
cd ~/test                                        进入主目录下的test目录
pushd .                                          暂存当前目录
popd                                             切换到暂存的栈顶的目录
ls test                                          列出test下的文件和目录
```

## 文件相关
 - touch  更新文件
 - rm     删除文件
 - cp     复制文件
 - mv     移动文件
 - rename 批量重命名
 - chmod  改变文件权限
 - chown  改变文件所有者

```bash
touch test.cpp                                   若存在test.cpp，则更行创建时间，否则创建改文件
rm -rf test                                      删除test目录及所有子目录和文件，-f不需确认，强制删除
cp test.cc ./test/                               复制test.cc到test目录下，保持文件名
mv test.cpp test.cc                              移动test.cpp到test.cc,相当于重命名文件
rename <被替换的字符串> <替换成的字符串> <替换模式>
```

## 文件编辑
 - cat         显示文件内容
 - more/less   按页显示文件内容
 - sort        文件排序
 - uniq        文件去重
 - head/tail   显示文件前/后几行
 - cut         文件分割
 - wc          统计文件内容行，字符，单词数
 - vim         vim编辑器
 - awk         行处理器
 - sed         文件流处理

```bash
cat -n test.cc                                   显示文件内容，行号
more/less                                        逐页显示文件内容

sort -k N                                        按照第k个字段排序(ASCII)
     -n                                          按照数字大小排序
     -r                                          逆序排列
     -u                                          去除重复项 排序文件

uniq -c                                          sort排序后，输出重复项个数
     -d                                          只输出重复行
     -i                                          忽略大小写
     -u                                          只输出不同行

head/tail -10                                    输出文件前/后10行
          -f xxx.log                             输出结果到文件

cut -d:                                          按照:分割行
    -f 2-5                                       输出分割行后的2-5字段
    
wc  -l                                           输出文件行数
    -c                                           输出文件字符数
    -w                                           输出单词数
    
vim test.cc                                      编辑test.cc
    h,j,k,l                                      左，下，上，右移动
    Nyy,Ndd,p                                    复制,剪切N行,粘贴
    d^,d$                                        删除光标到行首，行尾
    %s/xxx/yyy/N                                 替换xxx为yyy N次

awk NR,NF,$1~$NF                                 行号，字段数,第1-最后字段
    FS                                           字段分割定义
    print,printf                                 打印，格式化打印
    BEGIN,END                                    预定义，统计部分
    
sed [ -nefri] [action]
    -n                                           使用安静模式
    -e                                           直接在命令列模式上编辑
    -f                                           保存修改后的结果
    -r                                           使用基础正则表达式
    -i                                           直接修改文件
    action: [n1[,n2]]function
       n1,n2                                     非必需，一般是行号
    function:
       a                                         新增一行字符串在之后
       c                                         替换n1,n2行
       d                                         删除n1,n2行
       i                                         插入一行到之前
       p                                         列,与-n一起使用
       s                                         替换，与正则一起使用
```

## 网络调试
 - ping                                          检测与目标主机的连通性
 - netstat                                       查看本机网络通信状态
 - tcpdump                                       监听截获通讯端口数据
 - ifconfig                                      配置本机网络
 - scp                                           安全复制，可远程复制
 - rasync                                        多机同步

```bash
ping [ -cdtIW] destination
     -c                                          ping次数
     -d                                          启用SO_DEBUG
     -t                                          指定TTL生存时间
     -I                                          使用网络硬件Interface
     -W                                          指定超时时间
     
netstat [-anpt]
     -npt                                        查看所有TCP连接
     -npu                                        查看所有UDP连接
     -npa                                        查看所有sokcet连接
     -nl                                         处于LISTEN状态的连接
     
tcpdump [tcp|ip|icmp|arp|rarp] [ -ictwXn] 'port |and src host |and dst host'
     tcp,udp,ip,icmp,rap,rarp                    监听制定协议数据
     -i interface                                监听的网络interface,如lo
     -X                                          使用hex显示数据
     -n                                          不对地址自动转换到名字
     -c                                          抓取包的个数
     -t                                          不打印时间戳
     -w                                          保存监听数据到文件
     
     port                                        监听端口号
     src host,dst host                           监听源主机，目标主句

ifocnfig
     -a                                          列出所有网络interface
     interface [down|up]                         开关指定网络硬件接口
     interface 192.168.1.2                       配置IP地址

scp -P port user@host:/path/file1 user@host:/path/file2
                                                 远程复制文件
```

## 编译调试
 - nm                                            列出ELF文件中的符号
 - objdump                                       分析ELF文件
 - readelf                                       同上
 - c++file                                       显示c++程序编译后的原型
 - ldd                                           显示ELF依赖的动态库
 - strings                                       输出ELF中的字符串
 - strip                                         剪裁ELF,去除多余信息
 - gdb                                           调试程序

```bash
nm [ -CA] ELF_file
   -C                                            还原编译后的符号
   -A                                            解析所有传入的文件
   
objdump
   -t                                            显示所有符号的信息
   -T                                            显示所有动态链接的符号
   -d                                            反汇编可执行字段
   -D                                            反汇编所有字段
  
readelf
   -s                                            显示所有符号
   --dynamic                                     显示链接动态库信息
   --dyn-syms                                    显示动态符号表
   
gdb
   l                                             显示源代码
   b line-num                                    断点
   n                                             运行到下一行,step over
   s                                             step into
   c                                             continue
   p                                             打印变量
   disp                                          监视变量
   bt                                            显示堆栈
   thread                                        切换线程
   delete                                        删除断点
```

## 帮助查阅
 - man                                           显示命令的帮助文档,操作类似vim
 - info                                          参考文档较完整
 
```bash
 
 man 1(用户命令) 2(系统调用) 3(库函数) 
     4(特殊文件) 5(文件格式) 6(游戏)
     7(杂项)     8(超级用户可用命令的帮助)
```