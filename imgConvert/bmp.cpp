#include "main.h"
#include "image.h"

//1 4 8λͼ��ȡ��ɫ����
//datΪ�������ݣ�biBitCountΪ������ռλ��
vector<uint8_t> getIdx(uint8_t dat, const uint8_t biBitCount)
{
    vector<uint8_t> ret;
    uint8_t mask = 0; // 1λ 0x01; 4λ 0x0f; 8λ 0xff
    auto t = biBitCount;
    while (t--)mask = mask << 1 | 0x01;
    while (dat)
    {
        ret.push_back(dat & mask);
        dat >>= biBitCount;
    }
    return ret;
}

//��ȡ����ɫ���bmp
bool deCodeBmpClrmap(image* img, FILE* pfile, const BITMAPINFOHEADER& infoHeader, const BITMAPFILEHEADER& fileHeader)
{
    //��ȡ��ɫ������
    vector<colora> colorMap;
    fseek(pfile, 54, SEEK_SET); //��ɫ�����ݴ� 0x37(55)��ʼ
    const auto clrNo = infoHeader.biClrUsed ? infoHeader.biClrUsed : 0x01 << infoHeader.biBitCount;
    uint8_t pColor[4] = {0};
    colora clr = DEFAULTCOLOR;
    for (auto i = 0; i < clrNo; ++i)
    {
        fread(pColor, 4, 1, pfile);
        clr = getColor(pColor, 4);
        colorMap.push_back(clr);
    }

    //��ȡͼ������
    fseek(pfile, fileHeader.bfOffBits, SEEK_SET);
    const uint8_t b = 8 / infoHeader.biBitCount; //ÿ�ֽ����ص���  
    const uint32_t w = (infoHeader.biWidth + 31) /32 * 4; //ÿ���ֽ���
    for (uint32_t i = 0; i < img->height(); ++i)
    {
        for (uint32_t j = 0; j < w; ++j)
        {
            fread(&pColor, 1, 1, pfile);
            uint8_t k = 0;
            for (auto idx : getIdx(pColor[0], infoHeader.biBitCount))
            {
                clr = getColor(&idx, 4, true, colorMap);
                img->setColor(j * b + k++, i, clr);
            }
        }
    }
    return true;
}

//��ȡ���bmp 24��32λ
bool deCodeBmpData(image* img, FILE* pfile, const BITMAPINFOHEADER& infoHeader, const BITMAPFILEHEADER& fileHeader)
{
    fseek(pfile, fileHeader.bfOffBits, SEEK_SET);
    const uint8_t depth = infoHeader.biBitCount / 8; //�����ֽ���
    const auto offset = depth == 4 ? 0 : infoHeader.biWidth % 4;
    colora clr = DEFAULTCOLOR;
    unsigned char pColor[4] = { 0 };

    for (uint32_t i = 0; i < img->height(); ++i)
    {
        for (uint32_t j = 0; j < img->width(); ++j)
        {
            fread(pColor, 1, depth, pfile);
            clr = getColor(pColor, depth);
            img->setColor(j, i, clr);
        }
        fseek(pfile, offset, SEEK_CUR);
    }
    return true;
}

bool loadBmp(image* img)
{
    FILE* pfile = nullptr;
    const auto e = fopen_s(&pfile, img->name().c_str(), "rb");
    if (e)
    {
        printf("%s File open failed. errno_t:%d\n", img->name().c_str(), e);
        return false;
    }
    BITMAPFILEHEADER fileHeader;
    BITMAPINFOHEADER infoHeader;

    fread(&fileHeader, 1, sizeof(BITMAPFILEHEADER), pfile);
    if (fileHeader.bfType != 0x4D42)
    {
        printf("%s is not bmp file!\n", img->name().c_str());
        fclose(pfile);
        pfile = nullptr;
        return false;
    }
    printf("%s is bmp file!\n", img->name().c_str());
    fread(&infoHeader, 1, sizeof(BITMAPINFOHEADER), pfile);
    img->width(infoHeader.biWidth);
    img->height(infoHeader.biHeight);

    img->reAlloc();

    auto ret = false;
    if (infoHeader.biBitCount < 24)
    {
        ret = deCodeBmpClrmap(img, pfile, infoHeader, fileHeader);
    }
    else if (infoHeader.biBitCount)
    {
        ret = deCodeBmpData(img, pfile, infoHeader, fileHeader);
    }

    fclose(pfile);
    pfile = nullptr;
    return true;
}

//����Ϊ32λ���ɫ
bool saveBmp(image* img)
{
    FILE* pfile = nullptr;
    const auto e = fopen_s(&pfile, img->name().c_str(), "wb");
    if (e)
    {
        printf("%s File open failed. errno_t:%d\n", img->name().c_str(), e);
        return false;
    }
    const uint8_t headerSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
    const uint64_t dataSize = img->width() * img->height() * 4;
    BITMAPFILEHEADER fileHeader = {0x4d42, headerSize + dataSize, 0, 0, headerSize};
    fwrite(&fileHeader, sizeof(BITMAPFILEHEADER), 1, pfile);

    BITMAPINFOHEADER info = {40, img->width(), img->height(), 1, 32, 0, dataSize, 1, 1, 0, 0};
    fwrite(&info, sizeof(BITMAPINFOHEADER), 1, pfile);

    colora clr = DEFAULTCOLOR;
    for (uint32_t i = 0; i < img->height(); ++i)
    {
        for (uint32_t j = 0; j < img->width(); ++j)
        {
            img->getColor(j, i, clr);
            changeRGBMod(clr);
            fwrite(&clr, 4, 1, pfile);
        }
    }
    fclose(pfile);
    pfile = nullptr;
    return true;
}
