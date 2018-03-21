#include <cstdlib>
#include "image.h"

void test(const char* name)
{
    image img(name);
    img.load();
    img.rename("image/test/bmp.bmp");
    img.save();
    img.rename("image/test/tga.tga");
    img.save();
    img.rename("image/test/png.png");
    img.save();
}
int main(int argc, char** argv)
{
    test(argv[1]);
    system("pause");
    return 0;
}