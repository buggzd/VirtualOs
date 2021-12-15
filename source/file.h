#ifndef FILE_H
#define FILE_H
#include <string>
using namespace std;
struct file
    {
        string fileName;//文件名字
        int fileSize;//文件大小
        string filecontent;//文件内容
        string fileType;//文件类型
        int blocknumber;//文件所占块数
    };

#endif // FILE_H
