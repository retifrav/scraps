#include "makeArgcArgv.h"

int main(int argc, char *argv[])
{
    QString line;
    line = "copy -c 50 -i \"f1,12\" -o \"f2,20\"";
    int newArgc = 0;
    char **newArgv = NULL;

    newArgv = makeArgsFromString(line, &newArgc, newArgv);

    qDebug() << "There are" << newArgc << "arguments:";
    for(int x = 0; x < 8; x++) {qDebug() << newArgv[x];}

    // delete newArgv
    int i = 0;
    while (newArgv[i]) {delete newArgv[i]; i++;}
    delete newArgv;

    return 0;
}
