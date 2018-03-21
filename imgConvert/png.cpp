#include "image.h"
#include "libpng/libpng.h"
#include "main.h"
#define PNG_BYTES_TO_CHECK 4

bool loadPng(image* img)
{
    FILE* pfile = nullptr;
    const auto e = fopen_s(&pfile, img->name().c_str(), "rb");
    if (e)
    {
        printf("load %s failed.error:%d\n", img->name().c_str(), e);
        return false;
    }

    char buf[PNG_BYTES_TO_CHECK];

    auto pngPtr = png_create_read_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0);
    auto infoPtr = png_create_info_struct(pngPtr);

    setjmp(png_jmpbuf(pngPtr)); // 这句很重要

    int temp = fread(buf, 1, PNG_BYTES_TO_CHECK, pfile);
    temp = png_sig_cmp(reinterpret_cast<png_const_bytep>(buf), static_cast<png_size_t>(0), PNG_BYTES_TO_CHECK);

    /*检测是否为png文件*/
    if (temp)
    {
        printf("%s is not png file.\n", img->name().c_str());
        fclose(pfile);
        pfile = nullptr;
        return false;
    }

    rewind(pfile);
    png_init_io(pngPtr, pfile);
    png_read_png(pngPtr, infoPtr, PNG_TRANSFORM_EXPAND, 0);

    /*获取宽度，高度，位深，颜色类型*/
    const int channels = png_get_channels(pngPtr, infoPtr); /*获取通道数*/
    auto bitDepth = png_get_bit_depth(pngPtr, infoPtr); /* 获取位深 */
    const int colorType = png_get_color_type(pngPtr, infoPtr); /*颜色类型*/

    const auto rowPointers = png_get_rows(pngPtr, infoPtr);
    img->width(png_get_image_width(pngPtr, infoPtr));//设置宽度
    img->height(png_get_image_height(pngPtr, infoPtr));//设置高度

    img->reAlloc();
    if (channels == 4 || colorType == PNG_COLOR_TYPE_RGB_ALPHA)
    {
        for (uint32_t i = 0; i < img->height(); ++i)
        {
            for (uint32_t j = 0; j < img->width(); ++j)
            {
                colora clr;
                clr.red = rowPointers[i][j * 4]; // red
                clr.green = rowPointers[i][j * 4+1]; // green
                clr.blue = rowPointers[i][j * 4+2]; // blue
                clr.alpha = rowPointers[i][j * 4+3]; // alpha

                if (!img->setColor(j, img->height()-1-i, clr))
                {
                    printf("loadpng error.\n");
                    return false;
                }
            }
        }
    }
    if (channels == 3)
    {
        for (uint32_t i = 0; i < img->height(); ++i)
        {
            for (uint32_t j = 0; j < img->width(); ++j)
            {
                colora clr;
                clr.red = rowPointers[i][j * 3]; // red
                clr.green = rowPointers[i][j * 3 + 1]; // green
                clr.blue = rowPointers[i][j * 3 + 2]; // blue
                clr.alpha = 255; // alpha

                if (!img->setColor(j, img->height() - 1 - i, clr))
                {
                    printf("loadpng error.\n");
                    return false;
                }
            }
        }
    }

    png_destroy_read_struct(&pngPtr, &infoPtr, nullptr);
    return true;
}

bool savePng(image* img)
{
    FILE* pfile = nullptr;
    const auto e = fopen_s(&pfile, img->name().c_str(), "wb");
    if (e)
    {
        printf("[write_png_file] File %s could not be opened for writing:%d\n", img->name().c_str(), e);
        return false;
    }

    /* initialize stuff */
    const auto pngPtr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);

    if (!pngPtr)
    {
        printf("[write_png_file] png_create_write_struct failed");
        return false;
    }
    const auto infoPtr = png_create_info_struct(pngPtr);
    if (!infoPtr)
    {
        printf("[write_png_file] png_create_info_struct failed");
        return false;
    }
    if (setjmp(png_jmpbuf(pngPtr)))
    {
        printf("[write_png_file] Error during init_io");
        return false;
    }
    png_init_io(pngPtr, pfile);


    /* write header */
    if (setjmp(png_jmpbuf(pngPtr)))
    {
        printf("[write_png_file] Error during writing header");
        return false;
    }

    png_set_IHDR(pngPtr, infoPtr, img->width(), img->height(),
        8, PNG_COLOR_TYPE_RGB_ALPHA, PNG_INTERLACE_NONE,
        PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

    png_write_info(pngPtr, infoPtr);

    /* write bytes */
    if (setjmp(png_jmpbuf(pngPtr)))
    {
        printf("[write_png_file] Error during writing bytes");
        return false;
    }

    const auto rowPointers = static_cast<png_bytepp>(malloc(img->height() * sizeof(png_bytep)));
    for (uint32_t i = 0; i < img->height(); ++i)
    {
        rowPointers[i] = static_cast<png_bytep>(malloc(sizeof(png_byte) * img->width() * 4));
        for (uint32_t j = 0; j < img->width(); ++j)
        {
            colora clr = { 0,0,0,0 };
            
            if(!img->getColor(j, img->height()-1-i, clr))
            {
                for (uint32_t k = 0; k <= i; k++)
                    free(rowPointers[k]);
                free(rowPointers);

                fclose(pfile);
                pfile = nullptr;
                return false;
            }
            rowPointers[i][j * 4] = clr.red; // red
            rowPointers[i][j * 4 + 1] = clr.green; // green
            rowPointers[i][j * 4 + 2] = clr.blue;  // blue
            rowPointers[i][j * 4 + 3] = clr.alpha; // clr.alpha; // alpha
        }
    }
    png_write_image(pngPtr, rowPointers);

    /* end write */
    if (setjmp(png_jmpbuf(pngPtr)))
    {
        printf("[write_png_file] Error during end of write");
        return false;
    }
    png_write_end(pngPtr, nullptr);

    /* cleanup heap allocation */
    for (uint32_t j = 0; j < img->height(); j++)
        free(rowPointers[j]);
    free(rowPointers);

    fclose(pfile);
    pfile = nullptr;
    return true;
}