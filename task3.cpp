#include <vector>
#include <fstream>
#include <unordered_map>
#include <cmath>
#include <cstdlib>

using namespace std;

struct punto {
    int n, i, j;
    double x, y;
};

struct arco {
    int e, n1, n2;
};

// funzione per tradurre da n a m se richiesto
int map_nm(int indice_n, bool nested_dissection, unordered_map<int,int>& n_to_m) {
    if (nested_dissection) {
        return n_to_m[indice_n];
    }
    else {
        return indice_n;
    }
}

int main(int argc, char* argv[]) { // si deve passare N e ndTrue o ndFalse in base a come si vuole l'ordinamento: "./task3 N ndTrue" o "./task3 N ndFalse"

    int N = atoi(argv[1]); 
    double h = 1.0/(N+1);
    double K = 0.01;

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

    fileCoord.close();

    // lettura connectivity.txt
    int e, n1, n2;
    vector<arco> archi;

    ifstream fileConn("connectivity.txt");

    while (fileConn>>e>>n1>>n2) {

        arco A;

        A.e = e;
        A.n1 = n1;
        A.n2 = n2;

        archi.push_back(A);
    }
    
    fileConn.close();

    // mappatura n->m e lettura ordering.txt
    unordered_map<int, int> n_to_m;
    int m, n_ord;

    ifstream fileOrder("ordering.txt");
    while (fileOrder>>m>>n_ord) {
        n_to_m[n_ord] = m;
    }

    fileOrder.close();

    bool nested_dissection; // flag che governa che ordinamento usare
    string modalità = argv[2];
    if (modalità == "ndTrue") {
        nested_dissection = true;
    }
    else if (modalità == "ndFalse") {
        nested_dissection = false;
    }

    // generazione A.txt e rhs.txt
    vector<double> rhs(punti.size());

    ofstream fileA("A.txt");
    ofstream filerhs("rhs.txt");

    for (const punto& p : punti) {
        
        fileA << map_nm(p.n, nested_dissection, n_to_m) << " " << map_nm(p.n, nested_dissection, n_to_m) << " " << -4*K/(h*h) << endl;  // diagonale di A

        double f_val = exp(-10*(p.x*p.x + p.y*p.y)); // valutazione di f sul punto

        rhs[map_nm(p.n, nested_dissection, n_to_m)] = f_val; // creazione rhs ordinato secondo l'ordinamento deciso dal flag nested_dissection
    }

    for (double f : rhs) {
    filerhs << f << endl;
    }
    
    filerhs.close();

    for (const arco& a : archi) {
    fileA << map_nm(a.n1, nested_dissection, n_to_m) << " " << map_nm(a.n2, nested_dissection, n_to_m) << " " << K/(h*h) << endl;  // elementi fuori diagonale
    fileA << map_nm(a.n2, nested_dissection, n_to_m) << " " << map_nm(a.n1, nested_dissection, n_to_m) << " " << K/(h*h) << endl; 
    }
    
    fileA.close();


    return 0;
}