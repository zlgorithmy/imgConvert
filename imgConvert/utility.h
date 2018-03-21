#pragma once
#ifndef _UTILITY_H_
#define _UTILITY_H_
#include <cstdint>
#include <string>
#include "type.h"
using namespace std;

//��ȡ��׺��
string getExt(const string& filename);

//���ݺ�׺��ȡͼ���ʽ
uint8_t getImgType(const string& fileExt);

//RGBA<->BGRA
void changeRGBMod(colora& clr);

//��ȡ��ɫ
colora getColor(const uint8_t* pColor, const uint8_t pixelSize);
#endif