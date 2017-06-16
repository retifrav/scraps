#include <stdio.h>
#include <stdlib.h>
using namespace std;

int factorial(int fact)
 {
  int x, y; x = y = 1;
  for(x=1; x<=fact; x++) y = y * x;
  return y;
 }
 
 int combo(int n, int i)
 {
  int cmb = 0;
  cmb = factorial(n) / (factorial(i)*factorial(n-i));
  return cmb;
 } 

int main()
{
 long a;
 a = combo(6, 2);
 printf("%d\n\n",a);
 system("PAUSE");
 return 0;
}
