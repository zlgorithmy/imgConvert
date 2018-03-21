#pragma once
#ifndef _TYPE_H_
#define _TYPE_H_
#include <cstdint>

#pragma pack(1)
typedef struct colorMapSpecification
{
    //��һ��Ԫ�ص�����ֵ���൱����ɫ����ʼλ��
    uint16_t colorMapOrigin;

    //��ɫ�������Ԫ���ܸ���
    uint16_t colorMapLength;

    //ÿ��Ԫ�ش�С������15λͼ�����λ��0����16λͼ��洢
    uint8_t colorMapEntrySize;
} colorMSpec;

typedef struct imageSpecification
{
    //����ԭ��xֵ
    uint16_t originX;

    //����ԭ��yֵ
    uint16_t originY;

    //���
    uint16_t width;

    //�߶�
    uint16_t height;

    //������� ��ȡֵ8��16��24��32
    uint8_t pixelSize;

    //ͼ������
    //bit3-bit0 ͼ��alpha��ռλ�� tga16Ϊ0000��0001��tga24Ϊ0000��tga32Ϊ1000
    //bit5-bit4 ͼ������ԭ��λ�� 00���£�01���£�10���ϣ�11����
    //bit7-bit6 ����ֵ00
    uint8_t imageDescriptor;
} imgSpec;

typedef struct TGAFileHeader //size=18
{
    //offset=0 size=1 �ֶ�6 image ID�ĳ��ȣ�0��ʾû���ֶ�6
    uint8_t IDLength;

    //offset=1 size=1 ʹ�õ���ɫ���ͣ�0->��ʹ����ɫ�� 1->ʹ����ɫ��
    uint8_t colorMapType;

    //offset=2 size=1 ʹ�õ�ͼ������ 0->û��ͼ�����ݣ�
    //1->δѹ������ɫ��ͼ�� 
    //2->δѹ�������ͼ��
    //3->δѹ���ĺڰ�ͼ��
    //9->RLEѹ������ɫ��ͼ�� 
    //10->RLEѹ�������ͼ��
    //11->RLEѹ���ĺڰ�ͼ��
    uint8_t imageType;

    //offset=3 size=5 ��ɫ��˵��
    colorMSpec colorMspec;

    //offset=8 size=10
    //ͼ��˵��
    imgSpec imgSpec;
} TGAHeader;

#define WIDTHBYTES(bits) (((bits)+31)/32*4)

//λͼ�ļ�ͷ��Ϣ�ṹ����
//���в������ļ�������Ϣ�����ڽṹ����ڴ�ṹ������Ҫ�Ǽ��˵Ļ���������ȷ��ȡ�ļ���Ϣ��

typedef struct tagBITMAPFILEHEADER {

    uint16_t bfType;//�̶�Ϊ0x4d42
    uint32_t bfSize; //�ļ���С
    uint16_t bfReserved1; //�����֣�������
    uint16_t bfReserved2; //�����֣�ͬ��
    uint32_t bfOffBits; //ʵ��λͼ���ݵ�ƫ���ֽ�������ǰ�������ֳ���֮��
} BITMAPFILEHEADER;


//��ϢͷBITMAPINFOHEADER��Ҳ��һ���ṹ���䶨�����£�

typedef struct tagBITMAPINFOHEADER {
    //public:
    uint32_t biSize; //ָ���˽ṹ��ĳ��ȣ�Ϊ40
    int32_t biWidth; //λͼ��
    int32_t biHeight; //λͼ��
    int16_t biPlanes; //ƽ������Ϊ1
    int32_t biBitCount; //������ɫλ����������1��2��4��8��16��24���µĿ�����32
    int32_t biCompression; //ѹ����ʽ��������0��1��2������0��ʾ��ѹ��
    int32_t biSizeImage; //ʵ��λͼ����ռ�õ��ֽ���
    int32_t biXPelsPerMeter; //X����ֱ���
    int32_t biYPelsPerMeter; //Y����ֱ���
    int32_t biClrUsed; //ʹ�õ���ɫ�������Ϊ0�����ʾĬ��ֵ(2^��ɫλ��)
    int32_t biClrImportant; //��Ҫ��ɫ�������Ϊ0�����ʾ������ɫ������Ҫ��
} BITMAPINFOHEADER;


//��ɫ��Palette����Ȼ�������Ƕ���Щ��Ҫ��ɫ���λͼ�ļ����Եġ�24λ��32λ�ǲ���Ҫ��ɫ��ġ�
//���ƺ��ǵ�ɫ��ṹ���������ʹ�õ���ɫ������

typedef struct tagRGBQUAD {
    //public:
    uint8_t red; //����ɫ�ĺ�ɫ����
    uint8_t green; //����ɫ����ɫ����
    uint8_t blue; //����ɫ����ɫ����
    uint8_t alpha; //����ֵ alpha
} RGBQUAD, colora;
#pragma pack()
#endif