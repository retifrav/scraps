#include "makeArgvArgcfromFile.h"

int main(int argc, char *argv[])
{
    QFile *file = new QFile("incoming.txt");
    readScenarioFile(file);
    
    return 0;
}
