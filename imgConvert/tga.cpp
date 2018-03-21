#include "main.h"
#include "image.h"
#include <map>

bool decodeTga2(image* img, FILE* pfile, const TGAHeader& fileHeader)
{
    const auto depth = fileHeader.imgSpec.pixelSize/8;
    for (uint32_t i = 0; i < img->height(); ++i)
    {
        for (uint32_t j = 0; j < img->width(); ++j)
        {
            colora clr = { 0, 0, 0, 255 };
            unsigned char pColor[4] = { 0 };
            fread(pColor, depth, 1, pfile);
            clr = getColor(pColor, depth);
            img->setColor(j, i, clr);
        }
    }
    return true;
}

bool decodeTga10(image* img, FILE* pfile, const TGAHeader& fileHeader)
{
    const uint8_t depth = fileHeader.imgSpec.pixelSize / 8;
    uint64_t idx = 0;
    colora clr = { 0,0,0,255 };
    uint8_t pkgLength = 0, header = 0; //header������־λ�Ͱ�����

    while (idx < img->width()*img->height())
    {
        fread(&header, 1, 1, pfile);

        pkgLength = (header & 0x7F) + 1;
        if (header & 0x80)//RLE
        {
            unsigned char pColor[4] = { 0 };
            fread(pColor, depth, 1, pfile);

            clr = getColor(pColor, depth);
            
            for (uint8_t k = 0; k < pkgLength; ++k) // ���ظ�����ɫ���ݶ��д�벻ͬ����ɫ���ݣ���ѹ
            {
                img->setColor(idx++, clr);
            }
        }
        else
        {
            // ��ȡ����������ɫ���ݣ���ȡdepth * pkgLength���ֽ�
            unsigned char pColor[4 * 128] = { 0 }; // �����ɫ�ֽ�����4������������128
            fread(pColor, depth, pkgLength, pfile);
            for (uint32_t k = 0; k < depth * pkgLength; k += depth)
            {
                clr = getColor(pColor+k, depth);
                img->setColor(idx++, clr);
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

    fread(&fileHeader, 1, 18, pfile); //��ȡ�ļ�ͷ
    img->width(fileHeader.imgSpec.width);
    img->height(fileHeader.imgSpec.height);
    img->reAlloc();//�����ڴ�
    
    //uint8_t* imageDescriptor = static_cast<uint8_t*>(malloc(fileHeader.IDLength * sizeof(char)));
    //fread(imageDescriptor, 1, fileHeader.IDLength, pfile); //��ȡͼ��������
    //����������Ϣ
    fseek(pfile, 18 + fileHeader.IDLength, SEEK_SET);
    
    uint8_t* colorData = nullptr;
    if (1 == fileHeader.colorMapType) //ʹ����ɫ��
    {
        colorData = new uint8_t[fileHeader.colorMspec.colorMapLength * fileHeader.colorMspec.colorMapEntrySize];
        fread(colorData, 1, fileHeader.colorMspec.colorMapLength * fileHeader.colorMspec.colorMapEntrySize, pfile); //��ȡ��ɫ��
    }

    map<uint8_t, bool(*)(image*, FILE*, const TGAHeader&)> decodeTga = { { 2,decodeTga2 },{ 10,decodeTga10 } };
    const auto f = decodeTga[fileHeader.imageType](img,pfile,fileHeader);

    fclose(pfile);
    pfile = nullptr;
    if (colorData)
    {
        delete[] colorData;
        colorData = nullptr;
    }
    return f;
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
    TGAHeader fileHeader = { 0,0,2,{0,0,0},{0,0,uint16_t(img->width()),uint16_t(img->height()),32,8} };
    fwrite(&fileHeader, 1, 18, pfile);

    for (auto i = 0; i < fileHeader.imgSpec.height; ++i)
    {
        for (auto j = 0; j < fileHeader.imgSpec.width; ++j)
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