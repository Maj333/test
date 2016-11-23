
#include <vector>
#include <iostream>
#include <set>

#define MX 10000

using namespace std;

int byc[MX][MX];//przebyte wierzcholki
vector<int>t[MX];//graf
vector<int>wynik;//kolejne wierzcholki w cylku

int dfs(int v)
{
    while(!t[v].empty())
    {
        int w = t[v].back();
        t[v].pop_back();
        if(byc[v][w] == 0)
        {
            byc[v][w]=1;
            byc[w][v]=1;
            dfs(w);
            wynik.push_back(w);
        }
   
    }
}

//graf nieskierowany
int main(int argc, char *argv[])
{
    int n , m,po;//n - wieszcholkow   m - krawedzi po - poczatek cyku Eulera
    cin >> n >> m;//wprwoadzamy dane
    for(int i=1; i<=m; i++)       // wprowadz wierzcholki i krawedzie
    {
        int a, b;//polaczenia 
        cout << "Krawedz " << i << ": ";//ktora z kolei krawedz
        cin >> a >> b;//polaczenie a i b
        t[a].push_back(b);
        t[b].push_back(a);
    }

    cout << "od ktorego wierzchołka zaczac ? : ";//start cyklu Eulera
    cin >> po; 
    cout << "\n\n" << po << " ";
    dfs(po);
    for(int i = 0 ; i < m;i++)
    {
        cout << wynik.back() << " ";
        wynik.pop_back();
    }

    system("PAUSE");
    return EXIT_SUCCESS;
}



