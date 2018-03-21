#pragma once
#ifndef _UTILITY_H_
#define _UTILITY_H_
#include <cstdint>
#include <string>
#include "type.h"
using namespace std;

//获取后缀名
string getExt(const string& filename);

//根据后缀获取图像格式
uint8_t getImgType(const string& fileExt);

//RGBA<->BGRA
void changeRGBMod(colora& clr);

//获取颜色
colora getColor(const uint8_t* pColor, const uint8_t pixelSize);
#endif