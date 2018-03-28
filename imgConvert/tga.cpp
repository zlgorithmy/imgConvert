#include "main.h"
#include "image.h"
#include <map>

bool deCodeTga(image* img, FILE* pfile, const TGAHeader& fileHeader, const bool clrmap, const vector<colora>& colorMap)
{
    const auto depth = fileHeader.imgSpec.pixelSize / 8;
    const auto fh = fileHeader.imgSpec.imageDescriptor >> 5 & 1; //y原点坐标，0为下，1为上
    const auto fw = fileHeader.imgSpec.imageDescriptor >> 4 & 1; //x原点坐标，0为左，1为右
    colora clr = DEFAULTCOLOR;
    unsigned char pColor[4] = { 0 };

    for (uint32_t i = 0; i < img->height(); ++i)
    {
        for (uint32_t j = 0; j < img->width(); ++j)
        {
            fread(pColor, depth, 1, pfile);
            clr = getColor(pColor, depth, clrmap, colorMap);
            img->setColor(fw ? img->width() - 1 - j : j, fh ? img->height() - 1 - i : i, clr);
        }
    }
    return true;
}

bool deCodeRLe(image* img, FILE* pfile, const TGAHeader& fileHeader, const bool clrmap, const vector<colora>& colorMap)
{
    const uint8_t depth = fileHeader.imgSpec.pixelSize / 8;
    const auto fh = fileHeader.imgSpec.imageDescriptor >> 5 & 1; //y原点坐标，0为下，1为上
    const auto fw = fileHeader.imgSpec.imageDescriptor >> 4 & 1; //x原点坐标，0为左，1为右
    int64_t idxw = fw ? img->width() - 1 : 0;
    int64_t idxy = fh ? img->height() - 1 : 0;

    uint64_t idx = 0;
    colora clr = DEFAULTCOLOR;
    uint8_t pkgLength = 0, header = 0; //header包括标志位和包长度
    unsigned char pColor[4 * 128] = { 0 }; // 最大颜色字节数是4，最大包长度是128

    while (idxw < img->width() && idxy < img->height() && idxw >= 0 && idxy >= 0)
    {
        fread(&header, 1, 1, pfile);
        pkgLength = (header & 0x7F) + 1;
        if (header & 0x80) //RLE
        {
            fread(&pColor, depth, 1, pfile);
            clr = getColor(pColor, depth, clrmap, colorMap);
            for (uint8_t k = 0; k < pkgLength; ++k) // 将重复的颜色数据多次写入不同的颜色数据，解压
            {
                idx = idxy * img->width() + idxw;
                img->setColor(idx, clr);
                idxw += fw ? -1 : 1;
                if (idxw == -1 || idxw == img->width())
                {
                    idxw = fw ? img->width() - 1 : 0;
                    idxy += fh ? -1 : 1;
                    if (idxy == -1 || idxy == img->height())
                    {
                        break;
                    }
                }
            }
        }
        else// 读取接下来的颜色数据，读取depth * pkgLength个字节         
        {
            fread(pColor, depth, pkgLength, pfile);
            for (uint32_t k = 0; k < depth * pkgLength; k += depth)
            {
                idx = idxy * img->width() + idxw;
                clr = getColor(pColor + k, depth, clrmap, colorMap);
                img->setColor(idx, clr);
                idxw += fw ? -1 : 1;
                if (idxw == -1 || idxw == img->width())
                {
                    idxw = fw ? img->width() - 1 : 0;
                    idxy += fh ? -1 : 1;
                    if (idxy == -1 || idxy == img->height())
                    {
                        break;
                    }
                }
            }
        }
    }
    return true;
}

bool loadTga(image* img)
{
    FILE* pfile = nullptr;
    const auto e = fopen_s(&pfile, img->name().c_str(), "rb");
    if (e)
    {
        printf("%s File open failed. errno_t:%d\n", img->name().c_str(), e);
        return false;
    }
    TGAHeader fileHeader;

    fread(&fileHeader, 1, 18, pfile); //读取文件头
    if (fileHeader.imageType != 1 && fileHeader.imageType != 2 && fileHeader.imageType != 3 &&
        fileHeader.imageType != 9 && fileHeader.imageType != 10 && fileHeader.imageType != 11)
    {
        printf("%s is not tga file.\n", img->name().c_str());
        fclose(pfile);
        pfile = nullptr;
        return false;
    }
    printf("%s is tga file!\n", img->name().c_str());

    img->width(fileHeader.imgSpec.width);
    img->height(fileHeader.imgSpec.height);
    img->reAlloc(); //分配内存

    const auto imageDescriptor = static_cast<uint8_t*>(malloc(fileHeader.IDLength * sizeof(char)));
    fread(imageDescriptor, 1, fileHeader.IDLength, pfile); //读取图像描述、
    //跳过描述信息
    fseek(pfile, 18 + fileHeader.IDLength, SEEK_SET);

    vector<colora> colorMap;
    if (1 == fileHeader.colorMapType) //使用颜色表
    {
        const auto depth = fileHeader.colorMspec.colorMapEntrySize / 8;
        for (auto i = 0; i < fileHeader.colorMspec.colorMapLength; ++i)
        {
            unsigned char pColor[4] = {0};
            fread(pColor, depth, 1, pfile);

            auto clr = getColor(pColor, depth, false, colorMap);
            clr.alpha = 255;
            colorMap.push_back(clr);
        }
    }

    map<uint8_t, bool(*)(image*, FILE*, const TGAHeader&, bool clrmap, const vector<colora>&)> decodeTga = { {0, deCodeTga}, {1, deCodeRLe} };
    const auto ret = decodeTga[fileHeader.imageType>3](img, pfile, fileHeader, fileHeader.colorMapType, colorMap);

    fclose(pfile);
    pfile = nullptr;
    return ret;
}

bool saveTga(image* img)
{
    FILE* pfile = nullptr;
    const auto e = fopen_s(&pfile, img->name().c_str(), "wb");
    if (e)
    {
        printf("%s File open failed. errno_t:%d\n", img->name().c_str(), e);
        return false;
    }
    TGAHeader fileHeader = {0, 0, 2, {0, 0, 0}, {0, 0, uint32_t(img->width()), uint32_t(img->height()), 32, 8}};
    fwrite(&fileHeader, 1, 18, pfile);

    for (auto i = 0; i < fileHeader.imgSpec.height; ++i)
    {
        for (auto j = 0; j < fileHeader.imgSpec.width; ++j)
        {
            colora clr = DEFAULTCOLOR;
            img->getColor(j, i, clr);
            changeRGBMod(clr);

            fwrite(&clr, 4, 1, pfile);
        }
    }
    fclose(pfile);
    pfile = nullptr;
    return true;
}