cmake_minimum_required(VERSION 3.4.1)
 
project(littleGif)  #项目名
 
include_directories(include)  #头文件路径
 
aux_source_directory(src DIR_SOURCE) #源文件目录，DIR_SOURCE 为定义的变量
 
set(SRC_FILE_PATH ${DIR_SOURCE})  #设置环境变量，编译用到的源文件都要放到这里
 
add_executable(littleGif ${SRC_FILE_PATH})  #设置可执行源文件编译成的可执行文件名