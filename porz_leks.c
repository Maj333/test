//Porzadek leksykograficzny
//
//Generowanie:
// - permutacji
// - podzbiorow
// - podziorow k-elementowych
// - slow n-elementowych
//
//www.algorytm.org
//(c) 2007 Rafal Swietlicki


#include "stdio.h"

int NextWord(char* Alf,char* slowo, shorthjkhjkhj n, short k)
{
        short i=k-1;
        short j=0;
        short s;
                
        while(i>=0 && slowo[i]==Alf[n-1])
           i--;
        if(i<0)
           return 0;
        while(Alf[j]!=slowo[i])
           j++;
        slowo[i]=Alf[j+1];
        for(s=i+1;s<k;s++)
           slowo[s]=*Alf;
        return 1;
}

void Slowa(char* Alf, short n, short k)
{
        char slowo[k+1];
        short i;
        FILE *p;
        
        for(i=0;i<k;i++)
           slowo[i]=*Alf;
           
        slowo[k]=0;
        p = fopen("slowa.txt", "w");
        fputs(slowo, p);
        fputs("\n", p);
        while(NextWord(Alf,slowo,n,k))
        {
            fputs(slowo, p);
            fputs("\n", p);
        }
        
        fclose(p);
}

void Podzbiory(char **Zbior, int ile)
{
        char Alf[2] = {'0','1'};
        char slowo[ile-1];
        short i;
        FILE *p;
        
        p = fopen("podzbiory.txt", "w");

        for(i=0;i<ile;i++)
           slowo[i]=*Alf;
        fputs("zbior pusty\n", p);
        while(NextWord(Alf,slowo,2,ile))
        {
                for(i=0;i<ile;i++)
                        if(slowo[i]=='1')
                        {
                            fputs(Zbior[i], p);
                            fputs(" ", p);
                        }
                fputs("\n", p);
        }
        
        fclose(p);
}

int NastPerm(short* perm, int ile)
{
        short i=ile-1;
        short min, indmin, j;
        short *a, *b;
        
        if(ile==1)
           return 0;

        while(i>0 && perm[i]<perm[i-1])
           i--;
        if(!i)
           return 0;
        i--;
        min=perm[i+1];
        indmin=i+1;
        for(j=i+2;j<ile;j++)
                if(perm[j]>perm[i] && perm[j]<min)
                {
                        min=perm[j];
                        indmin=j;
                }
        perm[indmin]=perm[i];
        perm[i]=min;
        a=&perm[i+1];
        b=&perm[ile-1];
        while(a<b)
        {
                min=*a; *a=*b; *b=min;
                a++; b--;
        }
        return 1;
}

void Permutacje(char **zbior, short n)
{
        short tab[n];
        short i;
        FILE *p;
        
        p = fopen("permutacje.txt", "w");
        for(i=0;i<n;i++)
           tab[i]=i;
        do
        {
                for(i=0;i<n;i++)
                {
                    fputs(zbior[tab[i]], p);
                    fputs(" ", p);
                }
                fputs("\n", p);
        }
        while(NastPerm(tab,n));
        
        fclose(p);
}

int NastKomb(short* slowo, short n)
{
        short i, j, pom;
        
        if(n==1)
           return 0;
        i=n-2;
        while(i>=0 && !(slowo[i]==0 && slowo[i+1]==1))
           i--;
        if(i<0)
           return 0;
        slowo[i]=1;
        slowo[i+1]=0;
        if(slowo[n-1] || i==n-2)
           return 1;
        i+=2;
        if(!slowo[i])
           return 1;
        j=n-1;
        while(i<j)
        {
                pom=slowo[i];
                slowo[i]=slowo[j];
                slowo[j]=pom;
                i++;
                j--;
        }
        return 1;
}

void Podzbiory_k_elementowe(char **zbior, short n, short k)
{
        short i;
        short tab[n];
        FILE *p;
        
        p = fopen("podzbkelemnt.txt", "w");
        for(i=1;i<=k;i++)
                tab[n-i]=1;
        for(i=k+1;i<=n;i++)
                tab[n-i]=0;
        do
        {
                for(i=0;i<n;i++)
                        if(tab[i])
                        {
                            fputs(zbior[i], p);
                            fputs(" ", p);
                        }
                fputs("\n", p);
        }
        while(NastKomb(tab,n));
        
        fclose(p);
}

int main()
{
        char *zb[]={"ala","ma","malego","kotka"};
        char Alfabet[2]={'0','1'};
                
        Podzbiory_k_elementowe(zb,4,2);
        Permutacje(zb,4);
        Podzbiory(zb,4);
        Slowa(Alfabet,2,4);
        return 0;
}
