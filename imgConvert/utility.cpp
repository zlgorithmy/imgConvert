#include <cstdint>
#include <string>
#include <map>
#include "image.h"
using namespace std;

//获取后缀名
string getExt(const string& filename)
{
    return filename.substr(filename.find_last_of('.') + 1);
}

//根据后缀获取图像格式
uint8_t getImgType(const string& fileExt)
{
    map<std::string, uint8_t> imgType
    { { "bmp",1 },{ "BMP",1 },
    { "tga",2 },{ "TGA",2 },
    { "png",3 },{ "PNG",3 } };
    return imgType[fileExt];
}

void changeRGBMod(colora& clr)
{
    const auto b = clr.blue;
    clr.blue = clr.red;
    clr.red = b;

    clr.alpha = 255;
}

colora getColor(const uint8_t* pColor, const uint8_t pixelSize)
{
    colora clr = { 0,0,0,255 };
    switch (pixelSize)
    {
        case 2:
            clr.blue = (pColor[0] & 0x1F) << 3;
            clr.green = (((pColor[1] & 3) << 3) | ((pColor[0] & 0xE0) >> 5)) << 3;
            clr.red = (pColor[1] & 0x7C) << 1;
            clr.alpha = pColor[1] & 0x80;
            break;
        case 4:
            clr.alpha = pColor[3];
        case 3:
            clr.blue = pColor[0];
            clr.green = pColor[1];
            clr.red = pColor[2];
            break;
        default:
            break;
    }
    return clr;
}