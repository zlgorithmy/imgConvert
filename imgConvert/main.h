#pragma once
#ifndef _MAIN_H_
#define _MAIN_H_
#include <string>

using namespace std;
class image;

bool loadDefault(image*);
bool loadTga(image*);
bool loadBmp(image*);
bool loadPng(image*);

bool saveDefault(image*);
bool saveTga(image*);
bool saveBmp(image*);
bool savePng(image*);

#endif
