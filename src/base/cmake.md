# cmake使用方法

## 1.基本结构

```
- project
 - includes
   - CMakeLists.txt
   - *.h
 - src
   - CMakeLists.txt
   - *.cc
 - CMakeLists.txt
```

1. 依赖CMakeLists.txt文件,项目主目标一个，主目录可指定包含的子目录
2. 在项目CMakeLists.txt中使用project指定项目名称，add_subdirectory添加子目录
3. 子目录CMakeLists.txt从父目录CMakeLists.txt继承设置

## 2.语法

- 注释: 行注释，以`#`开头

- 变量
 - 使用set命令显式定义及赋值
 - 在非if语句中，使用${}引用
 - if中直接使用变量名引用
 - 后续的set命令会清理变量原来的值
 
- command (args ...)  
 - 命令不分大小写
 - 参数使用空格分隔
 - 使用双引号引起参数中空格
 
- set(var a;b;c) <=> set(var a b c)  
 - 定义变量var并赋值为a;b;c这样一个string list
 
- add_executable(${var}) <=> add_executable(a b c)
 - 变量使用${xxx}引用

- 条件语句

```cmake
if(var) #var非empty,0,FALSE
    ...
elseif()
    ...
else()
    ...
endif()
```

- 循环语句

```cmake
第一种:
set(var a b c)
foreach(f ${var})
    ...
endforeach(f)

第二种:
while()
    ...
endwhile()
```

## 3.内部变量

```cmake

1. CMAKE_C_COMPILER/CMAKE_CXX_COMPILER                指定C/C++编译器
2. CMAKE_C_FLAGS/CMAKE_CXX_FLAGS                      设置C/C++编译参数
3. EXECUTABLE_OUTPUT_PATH                             可执行文件的存放路径
4. LIBRARY_OUTPUT_PATH                                库文件存放路径
5. CMAKE_BUILD_TYPE                                   编译类型(Debug,Release)
6. BUILD_SHARED_LIBS                                  编译动态链接库(ON,OFF)
```

## 4.命令(不分大小写)

```cmake

project( test )                     #指定项目名称,使用${test_SOURCE_DIR}表示项目根目录

include_directories                 #指定头文件的搜索路径

link_directories                    #指定动态链接库的搜索路径

add_subdirectory                    #包含子目录

add_executable                      #编译可执行程序，指定编译`add_executable(hello hello.cc header.cc)`

add_definitions                     #添加编译参数 `add_definitions("-Wall -g")`

target_link_libraries               #添加链接库 `target_link_libraries( demo ${BOOST_LIBRARIES} )`

add_library                         #编译成静态库 `add_library(demo demo.cc)`

add_custom_target                   #添加自定义编译目标

set_target_properties               #设置编译目标属性,OUTPUT_NAME,VERSION等

link_libraries(lib1 lib2 )          #所有编译目标链接相同的库
```

## 5.其他

```cmake
1. MESSAGE({SEND_ERROR|STATUS|FATAL_ERROR} "this is message")        #打印编译信息，FATAL_ERROR会停止编译

2. AUX_SOURCE_DIRECTORIES                                            #获得目录下所有源文件,添加新文件需要重新运行cmake .

3. FILE(GLOB SRC_FILES RELATIVE ${CMAKE_CURRENT_SOUCE_DIR} *.cpp)    #显式添加编译文件列表,添加新文件无需重新运行cmake .

4. FIND_LIBRARY(Boost COMPONENTS thread system log REQUIRED)         #查找并添加动态库(添加一部分)

5. OPTION 条件编译
   OPTION(DEBUG_mode "this is debug." ON)
   IF(DEBUG_mode)
        add_definitions(-DDEBUG)
   ENDIF()
   
6. ADD_DEPENDENCIES                                                  添加编译依赖项

7. SET_TARGET_PROPERTIES                                             设置目标文件目录

8. CMAKE_MINIMUM_REQUIRED(VERSION 2.6)                               设置cmake最小版本

9. LESS,GREATER,EQUAL                                                数字比较

10.STRLESS、STRGREATER、STREQUAL                                     字符串比较
```

## 6.C++ Server组使用范例

- *workspace*
  - CMakeLists.txt #根目录cmake file 
  - common_inc     #头文件
  - common_lib     #库文件
  
- *编译common库*
  - add_subdirectory(stt_common/branch/1.0)
  - add_subdirectory(stt_msgbus/branch/1.0_cluster)
  - add_subdirectory(stt_dy_common/trunk)

- *编译具体项目*
  - add_subdirectory(stt_dy_chatroom/trunk)