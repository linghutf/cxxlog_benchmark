# 常见问题及解决方案

## YCM编译问题

### YCM启动崩溃问题

#### 1.启动一会儿就显示`SHUT DOWN`

1. `:YcmToggleLogs stderr` 查看日志
2. `/lib64/libc.so.6: version 'GLIBC_2.14' not found`
3. 查看GLIBC版本`strings /lib64/libc.so.6 |grep GLIBC_`发现glibc版本太低
4. 使用`-system-libclang`依旧出现`ice_*.hpp deprecated`问题

#### 2.解决办法:

1. 安装`python-devel.x86_64`消除`deprecated`问题 --- 无sudo权限
2. 下载编译新版GLIBC --- 无权限
3. 复制一份其他服务器的YCM到相应目录 --- 正在尝试

#### 3.问题来源:
- 没有使用`--system-libclang`时下载的是clang3.8编译器，而其依赖GLIBC2.14+,因此
使用旧版YCM即可
