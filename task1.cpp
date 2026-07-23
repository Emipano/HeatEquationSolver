#include <vector>
#include <fstream>

using namespace std;

struct punto {
    int n, i, j;
    double x, y;
};

int main() {

    int N = 4;
    double h = 1.0/(N+1);
    vector<punto> punti;
    int n=0;

    // generazione griglia
    for (int i=1; i<=N; i++) {  //i e j da 1 a N per escudere i punti di bordo
        for (int j=1; j<=N; j++) {

            punto P; // creo nuovo punto
            
            P.n = n;
            P.i = i;
            P.j = j;
            P.x = i*h;
            P.y = j*h;

            punti.push_back(P); // aggiungi a vector

            n++; // aggiorna indice
        }
    }

    // scrittura su file coords.txt
    ofstream file("coords.txt");
    for (const punto& p : punti) {
        file << p.n << " " << p.i << " " << p.j << " " << p.x << " " << p.y << endl;
    }
    file.close();
    

    return 0;
}