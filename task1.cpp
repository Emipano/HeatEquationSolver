#include <vector>
#include <fstream>

using namespace std;

struct punto {
    int n, i, j;
    double x, y;
};

struct arco {
    int e, n1, n2;
};

int main() {

    // generazione griglia

    int N = 4;
    double h = 1.0/(N+1);
    vector<punto> punti;
    int n = 0;


    for (int i=1; i<=N; i++) {  //i e j da 1 a N per escudere i punti di bordo

        for (int j=1; j<=N; j++) {

            punto P; // creo nuovo punto
            
            P.n = n;
            P.i = i;
            P.j = j;
            P.x = i*h;
            P.y = j*h;

            punti.push_back(P); // aggiungo a vector

            n++; // aggiorna indice
        }
    }

    // scrittura su file
    ofstream fileCoord("coords.txt");

    for (const punto& p : punti) {

        fileCoord << p.n << " " << p.i << " " << p.j << " " << p.x << " " << p.y << endl;
    }
    fileCoord.close();


    // generazione archi

    int di[] = {1, -1, 0, 0};
    int dj[] = {0, 0, 1, -1};
    vector<arco> archi;
    int e = 0;

    for (const punto& p : punti) {

        for (int k=0; k<4; k++) {

            int iv = p.i + di[k];
            int jv = p.j + dj[k];

            if (iv >= 1 && iv <= N && jv >= 1 && jv <= N) {

                int nv = (iv - 1)*N + (jv - 1);

                if (p.n<nv) {
                    
                arco A;

                A.e = e;
                A.n1 = p.n;
                A.n2 = nv;

                archi.push_back(A); // aggiungo a vector

                e++;  // aggiorna indice
                }
            }
        }
    }

    // scrittura su file
    ofstream fileConn("connectivity.txt");

    for (const arco& a : archi) {

        fileConn << a.e << " " << a.n1 << " " << a.n2 << endl;
    }
    fileConn.close();

    return 0;
}