#include "catalogue.h"

#include <vector>

CSTree Root_directory;//根目录
bool ifdelete=true;//TRUE代表可以删除整科树，FALSE不可以
catalog_management catalog0;
catalog_management::catalog_management(){
    Root_directory=new CSNode;
    Root_directory->name="gen";
    Root_directory->ifcatalog=true;
    Root_directory->firstchild=nullptr;
    Root_directory->nextsibling=nullptr;
    Root_directory->parent=nullptr;

}
//string转vector
vector<char> catalog_management::String2vector(string s){
    vector<char> a;

            for(int i=0;i<s.size();i++)
            {
               a.push_back(s[i]);
            }

            return a;
}
//寻找该文件所在目录位置,
CSTree catalog_management::find(vector<char> addr/*文件地址*/) {
    vector<string> Catalogue_name;//各级目录的名字
    string a;
    for(int i=0;i<addr.size();i++)
    {
        if(addr[i] != '/')
        {
           a=(a+addr[i]).c_str();
        }
        else
        {

           Catalogue_name.push_back(a);
           a.clear();
        }
    }

    Catalogue_name.push_back(a);
    CSTree t;
    t = Root_directory;
    for(int i=0;i<Catalogue_name.size()-1;i++)
    {
        while(1)
        {
            if(t==NULL)
            {
                qDebug()<<"当前路径不存在"<<endl;
                break;
            }
            if(t->name==Catalogue_name[i])
            {
                t=t->firstchild;
                break;
            }
            t=t->nextsibling;
        }

    }
      while(1)
        {
           if(t==NULL)
            {
                qDebug()<<"当前路径不存在"<<endl;
                break;
            }
            if(t->name==Catalogue_name[Catalogue_name.size()-1])
            {
                break;
            }
            t=t->nextsibling;

        }

   return t;
}
//todo
void catalog_management::freeblock(string fileName,int fileLength){

}
void catalog_management::Preorder2( CSTree pNode )//遍历删除该树在磁盘上的所有文件
    {
        if (pNode == NULL)
        return;
       //freeblock(pNode->firstchild->FCB1.fileName,pNode->firstchild->FCB1.fileSize);
       Preorder2(pNode->firstchild);
       Preorder2(pNode->nextsibling);
    }

void catalog_management::Preorder1( CSTree pNode )//遍历判断该树的文件是否有被调入内存
    {
        if (pNode == NULL)
        return;
       if(pNode->FCB1.usings=true&&pNode->ifcatalog==false)
       {
         ifdelete=false;
         return;
       }
       Preorder1( pNode->firstchild );
       Preorder1( pNode->nextsibling);
    }


void catalog_management::delete_Catalogue(vector<char> addr)//删除目录 :多级文件
 {
     vector<string> Catalogue_name;//各级目录的名字
     string s;

     for(int i=0;i<addr.size();i++)
     {
         if(addr[i] != '/')
         {
            s=(s+addr[i]).c_str();
         }
         else
         {
            Catalogue_name.push_back(s);
            s.clear();
         }
     }
     Catalogue_name.push_back(s);
     CSTree t;
     t = Root_directory;
     for(int i=0;i<Catalogue_name.size()-1;i++)
     {
         while(1)
         {
             if(t==NULL)
             {
                 qDebug()<<"当前目录不存在"<<endl;
                 return ;
             }
             else if(t->name==Catalogue_name[i]&&i!=Catalogue_name.size()-2)
             {
                 t=t->firstchild;
                  break;
             }
             else if(t->name==Catalogue_name[i]&&i==Catalogue_name.size()-2)
             {
                 break;
             }
             t=t->nextsibling;
         }
     }
     if(t->firstchild->name==Catalogue_name[Catalogue_name.size()-1])
     {       Preorder1(t->firstchild);
             if(ifdelete==false)
             {
               qDebug()<<"此目录中有文件被调用，不可以被删除"<<endl;
               ifdelete=true;
               return ;
             }
             else
             {
                 //删除磁盘此节点下的多级文件
                // Preorder2(t->firstchild);

                 t->firstchild=t->firstchild->nextsibling;
                 qDebug()<<"删除目录成功"<<endl;
                 return  ;
         }

     }
     else
 {
         t=t->firstchild;
         while(1)
         {
            if(t==NULL)
             {
                 qDebug()<<"当前目录不存在"<<endl;
                 return ;
             }
             if(t->nextsibling->name==Catalogue_name[Catalogue_name.size()-1])
             {
                     Preorder1(t->nextsibling);
                    if(ifdelete==false)
                     {
                        qDebug()<<"此目录中有文件被调用，不可以被删除"<<endl;
                        ifdelete=true;
                        return ;
                      }
                      else
                      {
                         //删除磁盘此节点下的多级文件
                        // Preorder2(t->nextsibling);
                        //
                        t->nextsibling=t->nextsibling->nextsibling;
                       qDebug()<<"删除目录成功"<<endl;
                       return  ;
                      }


             }
             t=t->nextsibling;

         }


     }
 }
void catalog_management::createCatalogue(vector<char> addr,string name)//创建空目录 空目录创建的地址（不包含空目录的名字），空目录的名字  调试成功
 {
     CSTree s,t;
     s=find(addr);
     t=new CSNode;
     t->firstchild=NULL;
     t->nextsibling=NULL;
     t->name=name;
     t->ifcatalog=true;
     t->FCB1.usings=false;

    if(s->firstchild==NULL)
    {
         s->firstchild=t;
         t->parent=s;
    }
    else
    {
         s=s->firstchild;
         while(s->nextsibling!=NULL)
         {
         s=s->nextsibling;
         }
         s->nextsibling=t;
         t->parent=s->parent;
    }
 }

void catalog_management::creatFile(file a,vector<char> addr)//为文件创建目录项 ,a代表文件，addr代表文件所在的地址，对应磁盘管理  调试成功
 {

     CSTree s,t;
     t=new CSNode;
     t->firstchild=NULL;
     t->nextsibling=NULL;
     s=find(addr);
     t->name=a.fileName;
     t->ifcatalog=false;
     t->FCB1.fileSize=a.fileSize;
     t->FCB1.fileName=a.fileName;
     t->FCB1.type=a.fileType;
     t->FCB1.blocks_number=a.blocknumber;
     t->FCB1.filecontent=a.filecontent;
     t->FCB1.usings=false;
     if(s->firstchild==NULL)
     {
         s->firstchild=t;
         t->parent=s;
     }
     else
     {
         s=s->firstchild;
         while(s->nextsibling!=NULL)
         {
         s=s->nextsibling;
         }
         s->nextsibling=t;
         t->parent=s->parent;
     }

 }
void catalog_management::deleteFile(vector<char> addr)//删除文件  地址包含文件名字   调试成功
 {

    vector<string> Catalogue_name;//各级目录的名字
        string s;
        for(int i=0;i<addr.size();i++)
        {
            if(addr[i] != '/')
            {
               s=(s+addr[i]).c_str();
            }
            else
            {

               Catalogue_name.push_back(s);
               s.clear();
            }
        }

        Catalogue_name.push_back(s);
        CSTree t;
        t = Root_directory;
        for(int i=0;i<Catalogue_name.size()-1;i++)
        {
            while(1)
            {
                if(t==NULL)
                {
                    qDebug()<<"当前目录不存在此文件"<<endl;
                    return ;
                }
                else if(t->name==Catalogue_name[i]&&i!=Catalogue_name.size()-2)
                {
                    t=t->firstchild;
                    break;
                }
                else if(t->name==Catalogue_name[i]&&i==Catalogue_name.size()-2)
                {
                    break;
                }
                t=t->nextsibling;
            }
        }
        if(t->firstchild->name==Catalogue_name[Catalogue_name.size()-1])
        {
            if(t->firstchild->FCB1.usings==true)
            {
                qDebug()<<"文件正在被使用，不可被删除"<<endl;
                return ;
            }
            else
            {
                t->firstchild=t->firstchild->nextsibling;
                //freeblock(t->firstchild->FCB1.fileName,t->firstchild->FCB1.fileSize);//在磁盘删除文件
                qDebug()<<"删除成功"<<endl;
                return ;
            }

        }
        else
        {
            t=t->firstchild;
            while(1)
            {
               if(t==NULL)
                {
                    qDebug()<<"当前目录不存在此文件2"<<endl;
                    return ;
                }
                if(t->nextsibling->name==Catalogue_name[Catalogue_name.size()-1])
                {
                    if(t->nextsibling->FCB1.usings==true)
                    {
                        qDebug()<<"文件正在被使用，不可被删除"<<endl;
                        return ;
                    }
                    else
                    {
                        t->nextsibling=t->nextsibling->nextsibling;
                        //freeblock(t->firstchild->FCB1.fileName,t->firstchild->FCB1.fileSize);//在磁盘删除文件
                        qDebug()<<"删除成功"<<endl;
                        return ;
                    }

                }
                t=t->nextsibling;

            }


        }
 }
int catalog_management::file_level(vector<char> addr) //返回最后一级文件所在层数   调试成功
 {
    vector<string> Catalogue_name;//各级目录的名字
     string s;
     int n=-1;
     for(int i=0;i<addr.size();i++)
     {
         if(addr[i] != '/')
         {
            s=(s+addr[i]).c_str();
         }
         else
         {
            Catalogue_name.push_back(s);
            s.clear();
         }
     }
     Catalogue_name.push_back(s);
     CSTree t;
     t = Root_directory;
     for(int i=0;i<Catalogue_name.size();i++)
     {
         while(1)
         {
             if(t==NULL)
             {
                 qDebug()<<"当前路径不存在"<<endl;
                 break;
             }
             if(t->name==Catalogue_name[i])
             {
                 t=t->firstchild;
                 n++;
                 break;
             }
             t=t->nextsibling;
         }
     }


    return n;
 }
//

bool catalog_management::use_Catalogue (vector<char> addr)//地址包含文件的名字 ，进程要使用文件，若文件在内存里，返回true,不在返回false
 {                                     //需要进程去调度内存管理和磁盘管理
     CSTree s;
     s=find(addr);
     if(s->FCB1.usings==true) return true;
     else return false;
 }
//
void catalog_management::Preorder3( CSTree pNode )
 {
     if (pNode == NULL)
     return;
    pNode->FCB1.usings=false;
    Preorder3( pNode->firstchild );
    Preorder3( pNode->nextsibling );
 }
void catalog_management::free_Catalogue (vector<char> addr) //若文件不在内存里面了，更改标志位
{
     CSTree s;
     s=find(addr);
    Preorder3(s);
}
void catalog_management::Preorder( CSTree pNode )
 {
     if (pNode == NULL)
     return;
    pNode->FCB1.usings=true;
    Preorder( pNode->firstchild );
    Preorder( pNode->nextsibling );
 }
void  catalog_management::lock_Catalogue(vector<char> addr)//若文件在内存里面更改标志位
{
     CSTree s;
     s=find(addr);
     Preorder(s);
}
void catalog_management::Preorder5( CSTree pNode )
 {
     if (pNode == NULL)
     return;
     cout<<pNode->name<<"  ";
     Preorder5( pNode->firstchild );
     Preorder5( pNode->nextsibling );
 }
