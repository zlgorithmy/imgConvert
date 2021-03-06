#include <cstdlib>
#include "image.h"
#include <corecrt_io.h>
#include <iostream>
void convert(const string& p1, const string& p2)
{
    printf("converting ...\n");
    image img(p1);
    if (img.load())
    {
        img.rename(p2);
        const auto f = img.save();
        printf("convert %s to %s %s\n", p1.c_str(), p2.c_str(), f ? "success!" : "error!");
    }
}
void judgePath(const string& path)
{
    struct stat info;
    stat(path.c_str(), &info);
    if (S_IFDIR & info.st_mode)
        printf("%s is dir.", path.c_str());
}
void test()
{

    _finddata_t findData;

    const string ip = "E:\\Desktop\\22\\testdata\\tga\\";
    const string op = "E:\\Desktop\\22\\testdata\\result\\";
    const string type = "bmp";
    const auto handle = _findfirst((ip+"*").c_str(), &findData);    // 查找目录中的第一个文件
    if (handle == -1)
    {
        cout << "Failed to find first file!\n";
        return;
    }
    do
    {
        if (findData.attrib & _A_SUBDIR)    // 是否是子目录并且不为"."或".."
        {
            cout << findData.name << "\t<dir>\n";
        }
        else
        {
            cout << findData.name << "\t" << findData.size << endl;
            auto in = ip+findData.name;
            auto ot = op+findData.name;
            
            ot = ot.substr(0, ot.find_last_of('.') + 1).append(type);
            judgePath(in);
            convert(in, ot);
        }
    } while (_findnext(handle, &findData) == 0);    // 查找目录中的下一个文件

    cout << "Done!\n";
    _findclose(handle);    // 关闭搜索句柄
}

int main(const int argv, char** argc)
{
    test();
    /*
    if (argv<3)
    {
        printf("Please type img2png infilepath outfilepath\nAnd the path can't include Spaces.\n");
        return 0;
    }
    if (-1 == _access(argc[1], 0))
    {
        printf("%s not exists,please check.\n",argc[1]);
        return 0;
    }
    if (0 == strcmp(argc[1], argc[2]))
    {
        printf("Same name,no conversion.\n");
        return 0;
    }

    printf("converting ...\n");
    image img(argc[1]);
    if(img.load())
    {
        img.rename(argc[2]);
        const auto f = img.save();
        printf("convert %s to %s %s\n", argc[1], argc[2], f ? "success!" : "error!");
    }*/
    system("pause");
    return 0;
}
