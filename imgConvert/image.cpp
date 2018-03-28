#include "image.h"
#include "main.h"
#include <map>

map<uint8_t, bool(*)(image*)> gfLoad = {{0, loadDefault}, {1, loadBmp}, {2, loadTga}, {3, loadPng}};
map<uint8_t, bool(*)(image*)> gfSave = {{0, saveDefault}, {1, saveBmp}, {2, saveTga}, {3, savePng}};

bool loadDefault(image* img)
{
    printf("%s unknown format.\n", img->name().c_str());
    return false;
}

bool saveDefault(image* img)
{
    printf("%s unknown format.\n", img->name().c_str());
    return false;
}

bool image::load()
{
    if (gfLoad[mImgType](this))
    {
        return true;
    }
    for (size_t i = 1; i < gfLoad.size(); ++i)
    {
        if (i == mImgType) continue;
        if (gfLoad[i](this))
        {
            type(i);
            return true;
        }
    }
    return false;
}

bool image::save()
{
    return gfSave[mImgType](this);
}

bool image::setColor(const uint32_t idx, colora& clr)
{
    if (idx < mWidth * mHeight)
    {
        mImgData[idx] = clr;
        return true;
    }
    return false;
}

bool image::setColor(const uint32_t x, const uint32_t y, colora& clr)
{
    if(y<mHeight && x<mWidth)
    {
        return setColor(y * mWidth + x, clr);
    }
    return false;
}

bool image::getColor(const uint32_t x, const uint32_t y, colora& clr)
{
    if (x < mWidth && y < mHeight)
    {
        clr = mImgData[y * mWidth + x];
        return true;
    }
    return false;
}

void image::reAlloc()
{
    mImgData.reserve(mHeight * mWidth);
    mImgData.resize(mImgData.capacity(), DEFAULTCOLOR);
}