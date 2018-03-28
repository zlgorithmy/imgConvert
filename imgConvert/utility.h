#pragma once
#ifndef _UTILITY_H_
#define _UTILITY_H_
#include <cstdint>
#include <string>
#include "type.h"
using namespace std;
#define DEFAULTCOLOR {0,0,0,255}
//获取filename后缀名
string getExt(const string& filename);

//改变filename后缀为fileExt
string setExt(string& filename, const string& fileExt);

//根据后缀获取图像格式
uint8_t getImgType(const string& fileExt);

//RGBA<->BGRA
void changeRGBMod(colora& clr);

//获取颜色
colora getColor(const uint8_t* pColor, const uint8_t pixelSize);
colora getColor(const uint8_t* pColor, const uint8_t pixelSize, bool clrmap, const vector<colora>& colorMap);
#endif