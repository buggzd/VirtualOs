#include<bits\stdc++.h>
#define delete_successful 1
#define delete_faile_path 2
#define delete_faile_using 3
#define create_successful 1
#define file_duplication 2
#include <QDebug>
#include "file.h"

using namespace std;
//全局变量
  struct FCB//文件控制块
    {
        //基本信息
        string  fileName;//文件名字
        int fileSize;//文件大小
        string  type;//文件类型：记录式文件还是流式文件
        int blocks_number;//文件所占用的块数
        string filecontent;
        /*
        //物理位置
        string  equipment_name;//存放文件的设备名
        int starting_disk_number;//文件的起始盘号

        int bytes_number;//文件所占用的字节数
        //逻辑结构

        string  if_length://定长记录还是变长记录；
        //物理结构
        string  type1;//文件是顺序文件，还是链式文件或索引文件
        //使用信息类
        string creat_time;//创建时间
        string last_revise_time//上一次修改时间
        */
        int  processing_num;//当前文件打开的进程数
        bool usings;//文件是否正被读入内存：true代表被读入
   };

typedef struct CSNode
    {
       string name;//数据文件名字或者目录名字
       bool  ifcatalog;//true代表是目录文件，FALSE代表是数据文件
       FCB FCB1;
       struct CSNode*firstchild,*nextsibling,*parent;//下一个兄弟节点
    }CSNode,*CSTree;



extern CSTree Root_directory;//根目录
extern bool ifdelete;//TRUE代表可以删除整科树，FALSE不可以

class catalog_management//目录管理
{


   public:
//地址格式：每一级文件之间用‘/’分开，根目录的名字：gen
   catalog_management();

   CSTree find( vector<char>addr /*文件地址*/);//寻找该文件所在目录位置

   //把string转为vector<char>
   vector<char> String2vector(string s);
   //给一个节点返回其目录，如gen/aa/cc不包含节点名字
   string getCatalog(CSTree pNode);

   void freeblock(string fileName,int fileLength);	//todo
   void Preorder2( CSTree pNode );//遍历删除该树在磁盘上的所有文件
   void Preorder1( CSTree pNode );//遍历判断该树的文件是否有被调入内存
   void delete_Catalogue(vector<char> addr);//删除目录 :多级文件
   void createCatalogue(vector<char> addr,string name);//创建空目录 空目录创建的地址（不包含空目录的名字），空目录的名字  调试成功
   void creatFile(file a,vector<char> addr);//为文件创建目录项 ,a代表文件，addr代表文件所在的地址，对应磁盘管理  调试成功
   void deleteFile(vector<char> addr);//删除文件  地址包含文件名字   调试成功
   int file_level(vector<char> addr); //返回最后一级文件所在层数   调试成功
   bool use_Catalogue (vector<char> addr);//地址包含文件的名字 ，进程要使用文件，若文件在内存里，返回true,不在返回false
   void Preorder3( CSTree pNode );
   void free_Catalogue (vector<char> addr); //若文件不在内存里面了，更改标志位
   void Preorder( CSTree pNode );
   void  lock_Catalogue(vector<char> addr);//若文件在内存里面更改标志位
   void Preorder5( CSTree pNode );


};

extern catalog_management catalog0;
