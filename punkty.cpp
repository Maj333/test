//Program sprawdza, czy trzy punkty sa wspolliniowe
//Program pobrano ze strony www.algorytm.org
//Opracowal Michal Knasiecki

#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
struct wsp      //wspolrzedne punktow
		{
      	int x,y;
      };

void main(void)
{
int i,det; //wyznacznik macierzy
wsp punkty[3]; //tablica punktow
clrscr();
printf("Wprowadzanie wspolrzednych punktow.");
for (i=1;i<4;i++)
	{
	printf("\nPunkt #%i",i);
   printf("\nX= ");
   scanf("%i",&punkty[i-1].x);
   printf("\nY= ");
   scanf("%i",&punkty[i-1].y);
   }
//Oblicznie wyznacznika macierzy
det=punkty[0].x*punkty[1].y+punkty[1].x*punkty[2].y+punkty[2].x*punkty[0].y-
punkty[2].x*punkty[1].y-punkty[0].x*punkty[2].y-punkty[1].x*punkty[0].y;
if (det>0) printf("\nPunkt #3 lezy po lewej stronie wektora #1->#2");
else if (det<0) printf("\nPunkt #3 lezy po prawej stronie wektora #1->#2");
else printf("\nPunkty leza na wspolnej prostej");
getch();
}