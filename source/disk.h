#include<iostream>
#include<fstream>
#include<math.h>
#include<stdio.h>
#include"file.h"
using namespace std;
#define max 900

typedef struct block {//内存块
    int flag;//用来标记内存块是否被占用
    int num;//相邻空闲块组成一个空闲区，各个空闲区链成一个链表，
    //指针指向空闲区的第一个空闲块，num表示当前空闲区有多少个空闲块
    string User;//占用当前块的文件名
    int id;//块的序号
    int size;//块的大小
    string content;//块内内容
    struct block* next;//指向下一空闲块的指针
    struct block* nextarea;//指向下一空闲区的指针
}block, * list;
class diskmanage
{//磁盘管理类
private:


public:
    struct block B[124];
    fstream fr;//文件流
    list F, L,D,DF;//F为空闲区链表的头指针,L为空闲块链表的头指针,D为兑换区头指针
    int Block[900];//用来显示块有没有被使用，1代表被占用
    void initial(list&L,list&F,list&D,list&DF) //初始化空闲区，空闲块链表
    {
        F = new block;
        list Last = new block;
        F->next = NULL;
        Last = F;
        L = new block;
        L->next = NULL;
        D = new block;
        D->next = NULL;
        DF = new block;
        DF->next = NULL;
        int sum = 0;
        int i = 0;
        list p;
        p = L;
        list q = D;
        while (sum <900)
        {
            list s = new block;
            s->next = NULL;
            s->User = "";
            s->flag = -1;
            s->id = i;
            s->size = 4;
            s->content = "";
            Block[i] = 0;
            p->next = s;
            //p->nextarea = s;
            p = p->next;
            sum++;
            i++;
        }
        sum = 0;
        while (sum < 124)
        {
            list s = new block;
            s->next = NULL;
            s->User = "";
            s->flag = -1;
            s->id = i;
            s->size = 4;
            s->content = "";
            q->next = s;
            q = q->next;
            sum++;
            i++;
        }
        DF->nextarea = D->next;
        D->flag = -1;
        DF->nextarea->nextarea = NULL;
        DF->nextarea->num =10;
        F->flag = -1;
        F->nextarea = L->next;
        L->flag = -1;
        F->nextarea->nextarea = NULL;
        F->nextarea->num =20;
        F->flag = -1;

    }
    void display() //
    {
        list s = F;
        list q = L;
        list p = DF;
        while (s)
        {
            cout << s->flag << " " << s->id << " " << s->num << " " << s->User << endl;
            s = s->nextarea;

        }
        while (p)
        {
            cout << p->flag << " " << p->id << " " << p->num << " " << p->User << endl;
            p = p->nextarea;

        }
        while (q)
        {
            cout << q->flag << " " << q->id << " " << q->num << " " << q->User<<" "<<q->content << endl;
            q = q->next;

        }
    }
    int  datamanage(list&L,list&F ,struct file f,fstream &fo)//存入文件
    {
        /*fstream fi;
        file fs;
        fo.open("os.txt");
        if (!fo)
        {
            printf("errror!\n");
            exit(-1);
        }
        fo << f.blocknumber <<" "<< f.fileName<<" " << f.fileSize <<" " <<" ";
        for (int i = 0;i < f.blocknumber;i++)
        {
            fo << f.filecontent[i]<<" ";
        }
        fo.close();*/
        int sum = 0;
        int needy = ceil(f.fileSize / 4.0);//需要块数
        int st = needy;
        list p = F;
        list q = F;
        list o;
        list t;
        while (p->nextarea != NULL)
        {
            if (f.fileSize > (p->nextarea->num * 4))
            {
                sum = sum + p->nextarea->num * 4;
                p = p->nextarea;
            }
            else break;
        }
        if (p->nextarea == NULL && sum < f.fileSize)
        {
            cout << "Disk is not big enough!" << endl;
            return -1;
        }
        else if (p->nextarea != NULL && p->nextarea->num > needy)
        {
            int number;
            t = p->nextarea;
            o = p->nextarea;
            number = p->nextarea->num;
            for (int i = 0;i < needy;i++)
            {
                Block[t->id] = 1;
                t->flag = 1;
                t->num = 0;
                t->User = f.fileName;
                t->content = f.filecontent[f.blocknumber - st];
                t = t->next;
                st--;
            }
            t->num = number - needy;
            t->nextarea = p->nextarea->nextarea;
            p->nextarea = t;
            o->nextarea = NULL;
            o->num = 0;

        }
        else if (p->nextarea != NULL && p->nextarea->num == needy)
        {
            t = p->nextarea;
            o = p->nextarea;
            t->num = t->num - needy;
            for (int i = 0;i < needy;i++)
            {
                Block[t->id] = 1;
                t->nextarea = NULL;
                t->flag = 1;
                t->num = 0;
                t->User = f.fileName;
                t->content = f.filecontent[f.blocknumber - st];
                t = t->next;
                st--;
            }
            p->nextarea = o->nextarea;

        }
        else
        {

            //q = q->nextarea
            while (needy > 0)
            {
                t = q->nextarea;
                o = q->nextarea;
                if (t->num < needy)
                {
                    //needy = needy - t->num;
                    //s = needy;
                    t->num = 0;
                    q->nextarea = t->nextarea;
                    q = q->nextarea;
                    for (int i = 0;i < needy;i++)
                    {
                        Block[t->id] = 1;
                        t->nextarea = NULL;
                        t->flag = 1;
                        t->num = 0;
                        t->User = f.fileName;
                        t->content = f.filecontent[f.blocknumber - st];
                        t = t->next;
                        st--;
                    }
                    needy = needy - t->num;

                    o->nextarea = NULL;

                }
                else if (t->num == needy)
                {

                    t->num = 0;
                    q->nextarea = t->nextarea;
                    t->nextarea = NULL;
                    for (int i = 0;i < needy;i++)
                    {
                        Block[t->id] = 1;
                        t->nextarea = NULL;
                        t->flag = 1;
                        t->num = 0;
                        t->User = f.fileName;
                        t->content = f.filecontent[f.blocknumber - st];
                        t = t->next;
                        st--;
                    }

                    needy = needy - t->num;
                }
                else
                {

                    int n = t->num;
                    t->num = 0;
                    o = t->nextarea;
                    for (int i = 0;i < needy;i++)
                    {
                        Block[t->id] = 1;
                        t->nextarea = NULL;
                        t->flag = 1;
                        t->num = 0;
                        t->content = f.filecontent[f.blocknumber - st];
                        t->User = f.fileName;
                        t = t->next;
                        st--;
                    }
                    needy = 0;
                    q->nextarea = t->next;
                    t->next->num = n - needy;
                    t->next->nextarea = o;
                }

            }
        }
    }
    void deletefile(list& L, list& F, struct file f)//删除文件
    {
            int flag = 0;
            list p = L;
            list q = F;
            list t, o;
            while (p->next != NULL)
            {
                if (p->next->flag==1&&p->next->User!=f.fileName)
                {
                    p = p->next;
                }
                else
                {
                    int num;
                    num = p->next->num;
                    if (p->next->flag == -1)
                    {
                        for (int i = 0;i < num-1;i++)
                        {
                            p = p->next;
                        }
                    }
                    else if (p->flag == -1&&p->next->flag==1)
                    {
                        int s = 0;
                        t = p->next;
                        while (t->flag == 1 && t->User == f.fileName)
                        {
                            Block[t->id] = 0;
                            flag = flag++;
                            s = s + 1;
                            t->flag = -1;
                            t->User = "";
                            t->content = "";
                            t = t->next;
                        }
                        if (t->flag == 1 && t->User != f.fileName)
                        {
                            list x;
                            x = F;
                            if (p->flag == -1 && p != L)
                            {
                                 p->num = p->num + s;
                            }
                            else
                            {
                                while (x->nextarea)
                                {
                                    x = x->nextarea;
                                }
                                x->nextarea = p->next;
                                p->next->nextarea = NULL;
                                p->next->num = s;
                            }

                        }
                        else if (t->flag == -1)
                        {
                            list q;
                            //list r;
                            q = F;
                            while (q->nextarea != NULL)
                            {
                                if (q->nextarea != t)
                                {
                                    q = q->nextarea;
                                }
                                else break;
                            }
                            if (t->nextarea != NULL)
                            {
                                list z = t->nextarea;
                                int m = z->num;
                                for (int i = 0;i < m - 1;i++)
                                {
                                    z = z->next;
                                }

                                if (z == p)
                                {
                                    q->nextarea = t->nextarea;
                                    t->nextarea->num = t->nextarea->num + s + t->num;
                                    t->num = 0;
                                    t->nextarea = NULL;
                                }
                            }
                            list r = q;
                            int n = r->num;
                            for (int i = 0;i < n-1;i++)
                            {
                                r = r->next;
                            }
                            if (r == p)
                            {
                                q->nextarea = t->nextarea;
                                t->nextarea = NULL;
                                q->num = q->num + s + t->num;
                                t->num = 0;
                            }

                            else
                            {
                                list q;
                                q = F;
                                while (q->nextarea != NULL)
                                {
                                    if (q->nextarea != t)
                                    {
                                        q = q->nextarea;
                                    }
                                    else break;
                                }
                                q->nextarea = t->nextarea;
                                p->num = p->num + s + t->num;
                                t->num = 0;
                                t->nextarea = NULL;

                            }
                            flag++;
                        }
                        p = t;
                        //p = p->next;
                    }
                    else
                    {
                        int s = 0;
                        t = p->next;
                        while (t->flag == 1 && t->User ==f.fileName)
                        {
                            Block[t->id] = 0;
                            s = s + 1;
                            t->flag = -1;
                            t->User = "";
                            t->content = "";
                            t = t->next;
                        }
                        if (t->flag == -1 && t->User != f.fileName)
                        {
                            list q;
                            q = F;
                            while (q->nextarea != NULL)
                            {
                                if (q->nextarea != t)
                                {
                                    q = q->nextarea;
                                }
                                else break;
                            }
                            q->nextarea = p->next;
                            p->next->nextarea = t->nextarea;
                            p->next->num = s + t->num;
                            t->num = 0;
                            t->nextarea = NULL;
                        }
                        else
                        {
                            list q;
                            q = F;
                            while (q->nextarea != NULL)
                            {
                                q = q->nextarea;
                            }
                            q->nextarea = p->next;
                            p->next->num = s;
                            p->next->nextarea = NULL;

                        }
                        p = t;
                        flag++;
                    }
                    p = p->next;//

                }
            }
            if (flag == 0)
            {
                cout << "error!" << endl;
            }
        }
    int  writeexchangarea(struct block b[],block t) //写兑换区
    {
        int s = 0;
        for (int i = 0;i < 124;i++)
        {
            if (b[i].flag == -1)
            {
                b[i] = t;
                break;
            }
            else s++;
        }
        if (s > 124)
        {
            cout << "ERROR" << endl;
            return -1;
        }
        else return 1;
    }//交换区管理
    void readexchange(struct block b[]) //读兑换区
    {
        for (int i = 0;i < 124;i++)
        {
            cout << b[i].flag << " " << b[i].id << " " << b[i].User << " " << b[i].content << endl;
        }
    }
    void filebrowsing();//文件查看
    //void nit(list& L, list& F,fstream &fo)
    //{
    //
    //	file f;
    //	//fo >> noskipws;
    //
    //
    //	fo.open("os.txt");
    //	if (!fo)
    //	{
    //		printf("errror!\n");
    //		exit(-1);
    //	}
 //           fo >> f.blocknumber >> f.fileName >> f.fileSize ;
 //			for (int i = 0;i < f.blocknumber;i++)
    //		{
    //			fo >> f.filecontent[i];
    //		}
    //		fo.close();
    //		datamanage(L, F, f, fo);
    //
    //
    //

    //
    //}
};
