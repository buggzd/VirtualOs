#include "memory_management.h"

vector<int>oldrecord(10);//记录换出页的页号
vector<int>newrecord(10);//记录换进页的页号
int mem_size=MEM_SIZE;//占用内存大小（KB）
int block_num=BLOCKNUM;//剩余块的数量
struct inblock *block_head=NULL;//内存中指向空闲块链表的头指针
memory_management memory_management0;

void memory_management::InitBlock(){
    for(int i=1;i<65;i++){//共64块空闲区
        inblock *temp;
        temp=new inblock;
        temp->id=i;
        temp->size=4;
        temp->next=NULL;
        temp->state=-1;
        temp->visit=0;
        if(block_head==NULL){
            block_head=temp;
            temp->addr=0;
        }
        else{
            block_head->next=temp;
            temp->addr+=4;
        }
        temp=temp->next;
    }
}

int memory_management::FIFO(struct file f){//先进先出置换页面算法
       struct inblock *temp,*b;
       int num=f.blocknumber-8;
       int n=9;
       int oldpage,newpage; //记录被置换出以及置换进的页号
       int ID;//记录文件f的第一个内存块的id
       while(num>=0){
          for(temp=block_head;temp!=NULL;temp=temp->next){
           if(temp->User==f.fileName&&temp->visit==0){
               b=temp;
               ID=b->id;
               oldpage=b->id;//被置换页的页号（块号）   页和块的大小相同
               newpage=ID+8;//新进入内存的页的页号
               oldrecord.push_back(oldpage);//记录换出页号
               newrecord.push_back(newpage);//记录换进页号
               temp->id=ID+8;
               n++;
               num--;
               temp->visit=1;
            }
          }
       }//多于8块的都要置换进来

       //换出页面存放在磁盘的兑换区中，调用磁盘的函数
       }

int memory_management::AllocBlock(struct file f){
       struct inblock *b,*temp;
       if(block_num<=0){
        qDebug()<<"内存已无空间！"<<endl;
        return 0;
       }
       else {
       if(f.blocknumber<=8){
            int n=f.blocknumber;
            int i=1;
            for(temp=block_head;temp!=NULL;temp=temp->next){
                    if(temp->state=-1&&f.blocknumber>0){
                       b=temp;
                       b->User=f.fileName;
                       b->state=1;//表示已使用
                       b->content=f.filecontent[i];//按照文件存在外存块中的内容进行赋值？（需要看磁盘怎么存放数据的）
                       n--;
                       i++;
                       }
                    temp=temp->next;
            }
            block_num-=8;
            mem_size-=32;
            qDebug()<<QString::fromStdString(f.fileName+" "+f.fileType+" ")<<QString::number(f.blocknumber)+f.fileSize; //展示文件信息
            qDebug()<<"内存分配成功！"<<endl;
            return 1;//固定内存块足够
            }
        else {
              FIFO(f);
              return 2;//固定内存块不够
           }
        }
}


void memory_management::RecycleBlock(struct file f){//回收内存
    for(inblock *temp=block_head;temp!=NULL;temp=temp->next){
    if(temp->User==f.fileName){
        temp->state=-1;
        temp->User="null";
        temp->content="";
        temp->visit=0;
      }
    }
}

struct inblock* memory_management::SearchBlock (){//寻找空闲块
       struct inblock *temp,*b;
       int m=0;
       for(temp=block_head;temp!=NULL;temp=temp->next){
           if(temp->state=-1){
               b=temp;
               m++;
               return b;
            }
       }
       if(m==0) qDebug()<<"当前已无空闲块！"<<endl;
}

float memory_management::percent(){//计算内存占用百分比
    float per=1-float(mem_size)/MEM_SIZE;
    return per;
}
