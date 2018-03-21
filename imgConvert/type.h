#pragma once
#ifndef _TYPE_H_
#define _TYPE_H_
#include <cstdint>

#pragma pack(1)
typedef struct colorMapSpecification
{
    //第一个元素的索引值，相当于颜色表起始位置
    uint16_t colorMapOrigin;

    //颜色表包含的元素总个数
    uint16_t colorMapLength;

    //每个元素大小，对于15位图像最高位置0，按16位图像存储
    uint8_t colorMapEntrySize;
} colorMSpec;

typedef struct imageSpecification
{
    //坐标原点x值
    uint16_t originX;

    //坐标原点y值
    uint16_t originY;

    //宽度
    uint16_t width;

    //高度
    uint16_t height;

    //像素深度 可取值8，16，24，32
    uint8_t pixelSize;

    //图像描述
    //bit3-bit0 图像alpha所占位数 tga16为0000或0001，tga24为0000，tga32为1000
    //bit5-bit4 图像坐标原点位置 00左下，01右下，10左上，11右上
    //bit7-bit6 保留值00
    uint8_t imageDescriptor;
} imgSpec;

typedef struct TGAFileHeader //size=18
{
    //offset=0 size=1 字段6 image ID的长度，0表示没有字段6
    uint8_t IDLength;

    //offset=1 size=1 使用的颜色类型，0->不使用颜色表 1->使用颜色表
    uint8_t colorMapType;

    //offset=2 size=1 使用的图像类型 0->没有图像数据，
    //1->未压缩的颜色表图像 
    //2->未压缩的真彩图像
    //3->未压缩的黑白图像
    //9->RLE压缩的颜色表图像 
    //10->RLE压缩的真彩图像
    //11->RLE压缩的黑白图像
    uint8_t imageType;

    //offset=3 size=5 颜色表说明
    colorMSpec colorMspec;

    //offset=8 size=10
    //图像说明
    imgSpec imgSpec;
} TGAHeader;

#define WIDTHBYTES(bits) (((bits)+31)/32*4)

//位图文件头信息结构定义
//其中不包含文件类型信息（由于结构体的内存结构决定，要是加了的话将不能正确读取文件信息）

typedef struct tagBITMAPFILEHEADER {

    uint16_t bfType;//固定为0x4d42
    uint32_t bfSize; //文件大小
    uint16_t bfReserved1; //保留字，不考虑
    uint16_t bfReserved2; //保留字，同上
    uint32_t bfOffBits; //实际位图数据的偏移字节数，即前三个部分长度之和
} BITMAPFILEHEADER;


//信息头BITMAPINFOHEADER，也是一个结构，其定义如下：

typedef struct tagBITMAPINFOHEADER {
    //public:
    uint32_t biSize; //指定此结构体的长度，为40
    int32_t biWidth; //位图宽
    int32_t biHeight; //位图高
    int16_t biPlanes; //平面数，为1
    int32_t biBitCount; //采用颜色位数，可以是1，2，4，8，16，24，新的可以是32
    int32_t biCompression; //压缩方式，可以是0，1，2，其中0表示不压缩
    int32_t biSizeImage; //实际位图数据占用的字节数
    int32_t biXPelsPerMeter; //X方向分辨率
    int32_t biYPelsPerMeter; //Y方向分辨率
    int32_t biClrUsed; //使用的颜色数，如果为0，则表示默认值(2^颜色位数)
    int32_t biClrImportant; //重要颜色数，如果为0，则表示所有颜色都是重要的
} BITMAPINFOHEADER;


//调色板Palette，当然，这里是对那些需要调色板的位图文件而言的。24位和32位是不需要调色板的。
//（似乎是调色板结构体个数等于使用的颜色数。）

typedef struct tagRGBQUAD {
    //public:
    uint8_t red; //该颜色的红色分量
    uint8_t green; //该颜色的绿色分量
    uint8_t blue; //该颜色的蓝色分量
    uint8_t alpha; //保留值 alpha
} RGBQUAD, colora;
#pragma pack()
#endif