#pragma once
#ifndef _UTILITY_H_
#define _UTILITY_H_
#include <cstdint>
#include <string>
#include "type.h"
using namespace std;
#define DEFAULTCOLOR {0,0,0,255}
//��ȡfilename��׺��
string getExt(const string& filename);

//�ı�filename��׺ΪfileExt
string setExt(string& filename, const string& fileExt);

//���ݺ�׺��ȡͼ���ʽ
uint8_t getImgType(const string& fileExt);

//RGBA<->BGRA
void changeRGBMod(colora& clr);

//��ȡ��ɫ
colora getColor(const uint8_t* pColor, const uint8_t pixelSize);
colora getColor(const uint8_t* pColor, const uint8_t pixelSize, bool clrmap, const vector<colora>& colorMap);
#endif