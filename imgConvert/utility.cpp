#include <cstdint>
#include <string>
#include <map>
#include "image.h"
using namespace std;

//��ȡ��׺��
string getExt(const string& filename)
{
    const auto pos = filename.find_last_of('.') + 1;
    return pos?filename.substr(pos):"";
}

//�ı�filename��׺ΪfileExt
string setExt(string& filename, const string& fileExt)
{
    const auto pos = filename.find_last_of('.') + 1;
    filename = pos ? filename.substr(0, pos).append(fileExt) : filename.substr(0, filename.length()).append(".").append(fileExt);
    return filename;
}

//���ݺ�׺��ȡͼ���ʽ
uint8_t getImgType(const string& fileExt)
{
    map<std::string, uint8_t> imgType
    { { "bmp",1 },{ "BMP",1 },
    { "tga",2 },{ "TGA",2 },
    { "png",3 },{ "PNG",3 } };
    return imgType[fileExt];
}

//RGB<==>BGR
void changeRGBMod(colora& clr)
{
    const auto b = clr.blue;
    clr.blue = clr.red;
    clr.red = b;
}

//��ȡ��ɫ
colora getColor(const uint8_t* pColor, const uint8_t pixelSize)
{
    colora clr = DEFAULTCOLOR;
    switch (pixelSize)
    {
    case 1:
        clr = { pColor[0] ,pColor[0] ,pColor[0] ,255 };
        break;
    case 2:
        //0        1
        //gggbbbbb arrrrrgg
        clr.red = (pColor[1] & 0x7C) << 1;
        clr.green = (pColor[1] << 6 & 0xc0) | (pColor[0] >> 2 & 0x68);
        clr.blue = pColor[0] << 3 & 0xF8;

        //clr.alpha = (pColor[1] & 0x80) << 8;
        break;
    case 4:
        //clr.alpha = pColor[3];
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
//��ȡ��ɫ
colora getColor(const uint8_t* pColor, const uint8_t pixelSize, const bool clrmap,const vector<colora>& colorMap)
{
    colora clr = DEFAULTCOLOR;
    if (clrmap)
    {
        if (pColor[0] < colorMap.size())
        {
            return colorMap.at(pColor[0]);
        }
        return { 0,0,0,255 };
    }
    return getColor(pColor, pixelSize);
}