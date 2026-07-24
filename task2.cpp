#include <vector>
#include <fstream>
#include <algorithm>

using namespace std;

struct punto {
    int n, i, j;
    double x, y;
};

// funzione ricorsiva per nested dissection
vector<int> ordina(vector<punto> punti, bool dir_x) { // se dir_x è true taglia lungo x, altrimenti lungo y

    if (punti.size()<=1) { // caso base
        vector<int> indici;
        if (punti.size()==1) { // se c'è un punto lo metto nel vettore, se non c'è il vettore rimane vuoto
            indici.push_back(punti[0].n);
        }
        return indici;
    }

    if (punti.size()>1) { // caso ricorsivo
    
        // riordinamento lungo x o y
        if (dir_x) {
            sort(punti.begin(), punti.end(), [](punto a, punto b) {
            return a.x < b.x;   // ordina per x crescente
            });
        }
        else {
            sort(punti.begin(), punti.end(), [](punto a, punto b) {
            return a.y < b.y;  // ordina per y crescente
            });
        }

        // ricerca del separatore
        int indice_mediano = punti.size()/2;
        double separatore; 
        punto mediano = punti[indice_mediano];

        if (dir_x) {
            separatore = mediano.x;
        }
        else {
            separatore = mediano.y;
        }

        // taglio
        vector<punto> V1;
        vector<punto> V2;
        vector<punto> VS;

        for (const punto& p : punti) {
            if (dir_x) { // taglio su x
                if (p.x<separatore) {
                    V1.push_back(p);
                }
                if (p.x>separatore) {
                    V2.push_back(p);
                }
                if (p.x==separatore) {
                    VS.push_back(p);
                }
            }
            else {  //talgio su y
                if (p.y<separatore) {
                    V1.push_back(p);
                }
                if (p.y>separatore) {
                    V2.push_back(p);
                }
                if (p.y==separatore) {
                    VS.push_back(p);
                }
            }
        }
        // chiamata ricorsiva
        vector<int> lista1 = ordina(V1, !dir_x); // inverte direzione con !
        vector<int> lista2 = ordina(V2, !dir_x);

        vector<int> ordine = lista1;
        for (int x : lista2) {
            ordine.push_back(x);
        }
        for (const punto& p : VS) {
            ordine.push_back(p.n);
        }

        return ordine;
    }
}

int main() {

    // lettura coords.txt
    int n, i, j;
    double x, y;
    vector<punto> punti;

    ifstream fileCoord("coords.txt");

    while (fileCoord>>n>>i>>j>>x>>y) {

        punto P;

        P.n = n;
        P.i = i;
        P.j = j;
        P.x = x;
        P.y = y;

        punti.push_back(P);
    }

    // ordinamento
    vector<int> ordinamento = ordina(punti, true);

    // scrittura file ordering.txt
    ofstream fileOrder("ordering.txt");
    for (int m = 0; m < ordinamento.size(); m++) {
        fileOrder << m << " " << ordinamento[m] << endl;
    }
    fileOrder.close();

    return 0;
}