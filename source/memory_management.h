#include<iostream>
#include<stdio.h>
#include<vector>
#include<string>
#include<stdlib.h>
#include "file.h"
#include <qdebug.h>

#define MEM_SIZE 256 //内存大小 (KB)
#define MEM_START 0  //起始位置
#define DEFAULT_PROCESS_SIZE 32//进程大小
#define BLOCKNUM 64//内存块数（每块4KB）
#define MAX 15 //最大文件块数

using namespace std;

class memory_management{

    public: void InitBlock();//初始化内存块
            int AllocBlock(struct file f);//分配内存块
            void RecycleBlock(struct file f);//回收内存块
            struct inblock* SearchBlock ();//寻找空闲块
            int FIFO(struct file f);//先进先出置换页面
            float percent();//占用内存百分比

};
    typedef struct inblock{//空闲内存块定义
    int id;//块号
    int size;//大小
    int addr;//内存块起始位置
    int state;//用来标记内存块是否被占用，-1表示空闲，1表示已使用
    string User;//占用当前块的文件
    struct inblock *next;//指向下一块的指针
    int visit;//被访问标记
    string content;//块中存放内容
    }inblock,*list;


    extern vector<int>oldrecord;//记录换出页的页号
    extern vector<int>newrecord;//记录换进页的页号
    extern int mem_size;//占用内存大小（KB）
    extern int block_num;//剩余块的数量
    extern struct inblock *block_head;//内存中指向空闲块链表的头指针
    extern memory_management memory_management0;



