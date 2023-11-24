#include <iostream>
#include <gdal/gdal.h>

int main(int argc, char *argv[])
{
    std::cout << "Base application message" << std::endl;

    std::cout << "Color GCI_MagentaBand: " << GDALGetColorInterpretationName(GCI_MagentaBand) << std::endl;;

    return EXIT_SUCCESS;
}
