#include "main.h"
#include "image.h"

bool loadBmp(image* img)
{
    FILE* pfile = nullptr;
    const auto e = fopen_s(&pfile,img->name().c_str(), "rb");
    if (e)
    {
        printf("%s File open failed. errno_t:%d\n", img->name().c_str(), e);
        return false;
    }
    BITMAPFILEHEADER fileHeader;
    BITMAPINFOHEADER infoHeader;

    fread(&fileHeader, 1, 14, pfile);
    if (fileHeader.bfType != 0x4D42)
    {
        printf("file is not bmp file!");
        return false;
    }
    fread(&infoHeader, 1, sizeof(BITMAPINFOHEADER), pfile);
    img->width(infoHeader.biWidth);
    img->height(infoHeader.biHeight);
    
    img->reAlloc();
    const uint8_t depth = infoHeader.biBitCount / 8;
    for (uint32_t i = 0; i<img->height(); ++i)
    {
        for (uint32_t j = 0; j<img->width(); ++j)
        {
            unsigned char pColor[4] = { 0 };
            fread(pColor, depth, 1, pfile);

            auto clr = getColor(pColor, depth);
            clr.alpha = 255;
            if (!img->setColor(j, i, clr))
            {
                printf("loadBmp error.\n");
                return false;
            }
        }
    }
    
    fclose(pfile);
    pfile = nullptr;
    return true;
}

bool saveBmp(image* img)
{
    FILE* pfile = nullptr;
    const auto e = fopen_s(&pfile, img->name().c_str(), "wb");
    if (e)
    {
        printf("%s File open failed. errno_t:%d\n", img->name().c_str(), e);
        return false;
    }
    BITMAPFILEHEADER fileHeader;
    BITMAPINFOHEADER infoHeader;

    fileHeader.bfType = 'B' | (int('M') << 8u);
    fileHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
    fileHeader.bfSize = fileHeader.bfOffBits + img->width() * img->height() * 4;
    fileHeader.bfReserved1 = fileHeader.bfReserved2 = 0;

    fwrite(&fileHeader, sizeof(BITMAPFILEHEADER), 1, pfile);

    BITMAPINFOHEADER info;
    info.biSize = 40;
    info.biWidth = img->width();
    info.biHeight = img->height();
    info.biPlanes = 1;
    info.biBitCount = 32;
    info.biCompression = 0;
    info.biSizeImage = img->width() * img->height() * 4;
    info.biXPelsPerMeter = 1;
    info.biYPelsPerMeter = 1;
    info.biClrUsed = 0;
    info.biClrImportant = 0;

    fwrite(&info, sizeof(BITMAPINFOHEADER), 1, pfile);

    for (uint32_t i = 0; i < img->height(); ++i)
    {
        for (uint32_t j = 0; j < img->width(); ++j)
        {
            colora clr = { 0,0,0,255 };
            img->getColor(j, i, clr);
            changeRGBMod(clr);

            fwrite(&clr, 4, 1, pfile);
        }
    }
    fclose(pfile);
    pfile = nullptr;
    return true;
}