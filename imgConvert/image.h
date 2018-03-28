#pragma once
#ifndef _IMAGE_H_
#define _IMAGE_H_

#include <string>
#include <vector>
#include "utility.h"

using namespace std;

class image
{
public:
    image()=delete;
    image operator=(const image&) = delete;
    ~image(){}

    explicit image(const string& path): mFilepath(path), mWidth(0), mHeight(0), mImgType(0)
    {
        mImgData.reserve(0);
        mImgType = getImgType(getExt(path));
    }

    void rename(const string& newPath)
    {
        name(newPath);
        type(getImgType(getExt(newPath)));
    }
    void name(const string& newPath) { mFilepath = newPath; }
    void width(const uint32_t& width) { mWidth = width; }
    void height(const uint32_t& height) { mHeight = height; }
    void type(const uint8_t& type) { mImgType = type; }
    
    string name()const { return mFilepath; }
    uint32_t width()const { return mWidth; }
    uint32_t height()const { return mHeight; }
    uint8_t type()const { return mImgType; }

    bool load();
    bool save();

    //设置(x,y)的颜色,若(x,y)超出范围返回 false
    bool setColor(const uint32_t x, const uint32_t y, colora& clr);
    bool setColor(const uint32_t idx, colora& clr);

    bool getColor(const uint32_t x, const uint32_t y, colora& clr);
    void reAlloc();

private:
    string mFilepath;
    uint32_t mWidth;
    uint32_t mHeight;
    uint8_t mImgType;

    vector<colora> mImgData;
};

#endif
//end of file image.h