#include <QSettings>
#include <QFile>
#include <QBitArray>
#include <QByteArray>
#include <QTextCodec>
#include "windows.h"

#define S_MARK -121    // 10000111 // системная метка
#define H_MARK -113    // 10001111 // метка рукопотрясения
#define H_RES_MARK -49 // 11001111 // ответное рукопотрясение
#define NAME_MARK -25  // 11100111 // метка имени файла
#define DATA_MARK -13  // 11110011 // метка содержимого файла
#define B_MARK -8      // обрыв, отмена
#define E_MARK -57     // ошибка
#define A_MARK -61     // всё отлично
#define NO_CHAR -128   // ничего не прочитано

#define MAX_ERRORS 3   // с какого числа ошибок оборвать передачу/приём

// --- кодирование и проверка закодированного -----------------------------------
// кодировать циклическим кодом
void EncodeCc (QBitArray *red)
{
    QBitArray pack = *red;
    for(int i=4; i>0; i--)
    {
        if (pack.testBit(i+2))
        {
            pack.setBit(i+2, false);
            if(pack.testBit(i)) pack.setBit(i, false);
            else pack.setBit(i, true);
            if(pack.testBit(i-1)) pack.setBit(i-1, false);
            else pack.setBit(i-1, true);
        }
    }
    red->setBit(2, pack.testBit(2));
    red->setBit(1, pack.testBit(1));
    red->setBit(0, pack.testBit(0));
}
// проверить закодированное
bool CheckCc (QBitArray *red){
    QBitArray pack = *red;
    for(int i=4; i>0; i--)
    {
        if (pack.testBit(i+2))
        {
            pack.setBit(i+2, false);
            if(pack.testBit(i)) pack.setBit(i, false);
            else pack.setBit(i, true);
            if(pack.testBit(i-1)) pack.setBit(i-1, false);
            else pack.setBit(i-1, true);
        }
    }
    if (!(pack.testBit(2)&&pack.testBit(1)&&pack.testBit(0))) return(true);
    return(false);
}
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

// --- отправка байта данных в порт --------------------------------------------
void SendByte(char ByteToSend, HANDLE PortHandle)
{
    QByteArray Input(&ByteToSend);
    QBitArray bits(8);
    bits.fill(false);

    for(int b=0; b<8; ++b) bits.setBit(b, Input[0]&(1<<b));

    QBitArray Apack(8);
    Apack.fill(false);
    QBitArray Upack(8);
    Upack.fill(false);

    for (int i=7; i>= 0; i--)
    {
         if(i<4) Upack.setBit(i+3, bits.testBit(i));
         else Apack.setBit((i-1), bits.testBit(i));
    }

    EncodeCc(&Apack);
    EncodeCc(&Upack);

    QByteArray Abyte = "";
    QByteArray Ubyte = "";
    for(int b=0; b<Apack.count(); ++b) Abyte[0] = (Abyte[0] | ((Apack[b]?1:0)<<(b%8)));
    for(int b=0; b<Upack.count(); ++b) Ubyte[0] = (Ubyte[0] | ((Upack[b]?1:0)<<(b%8)));
    CHAR AB = Abyte[0];
    CHAR UB = Ubyte[0];
    DWORD write_counter;
    WriteFile(PortHandle, &AB, 1, &write_counter, NULL);
    WriteFile(PortHandle, &UB, 1, &write_counter, NULL);
}
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

// --- получение байта ---------------------------------------------------------
bool GetFullByte(CHAR FirstHalFBits, HANDLE PortHandle, CHAR &FullByte)
{
    QByteArray FHByte(&FirstHalFBits);
    QBitArray FBits(8);
    FBits.fill(false);
    QBitArray FHBit(8);
    FHBit.fill(false);
    for(int b=0; b<8; ++b) FHBit.setBit(b, FHByte[0]&(1<<b));
    if(CheckCc(&FHBit)) {for (int i=0; i<=3; i++)
            FBits.setBit(i+4, FHBit.testBit(i+3));} else return false;

// вторая половина байта
    CHAR SHalFBits;
    DWORD readen;
    ReadFile(PortHandle, &SHalFBits, 1, &readen, NULL);
    QByteArray SHByte(&SHalFBits);
    QBitArray SHBit(8);
    SHBit.fill(false);
    for(int b=0; b<8; ++b) SHBit.setBit(b, SHByte[0]&(1<<b));
    if(CheckCc(&SHBit)) {for (int i = 0; i <= 3; i++)
            FBits.setBit(i, SHBit.testBit(i+3));} else return false;
    QByteArray Fbyte = "";
    for(int b=0; b<FBits.count(); ++b)
        Fbyte[0] = (Fbyte[0] | ((FBits[b]?1:0)<<(b%8)));
    FullByte = Fbyte[0];
    return true;
}
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
