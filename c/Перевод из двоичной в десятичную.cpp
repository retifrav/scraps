#include <stdio.h>
#include <stdlib.h>
#include <math.h>
using namespace std;

int crc2digit(int *codecrc)
{
 char bin[] = "0000";
 int a, b, c[4] = {0,0,0,0}, *code = codecrc;
 for(a=0; a<4; a++) bin[a] = code[a]+48;
 b = atoi(bin);
 for(a=3; a>=0; a--) {
                      c[a] = b%10;
                      b=b/10;
                     }
 b = c[0]*pow(2,3)+c[1]*pow(2,2)+c[2]*pow(2,1)+c[3]*pow(2,0);
 return b;
}

int main()
{ 
 int crc[4] = {1,0,0,1};
 printf("%d\n\n",crc2digit(crc));
 
 system("PAUSE");
 return 0;
}
