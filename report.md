# HeatEquationSolver

## Discretizzazione e riordinamento del dominio

### Task 1
Input: Numero intero N
Output: `coords.txt` e `connectivity.txt`


+ Generazione `coords.txt`: Due cicli for annidati, uno su i e uno su j generano tutte le coppie (i,j). xi e yj si trovano di conseguenza. Ogni punto lo rappresento con una struct `punto {i, j, x, y}` e li accumolo in un `vector<punto>`. L'indice n aumenta di 1 ogni volta che aggiungo un elemento al vector. 

+ Generazione `connectivity.txt`: Con un ciclo for sul `vector<punto>` controllo per ogni punto (i,j) se esistono (cioè se sono dentro il range 1,...,N) gli adiacenti (i, j+/-1) e (i+/-1, 1) e li organizzo in struct `arco {n1, n2}` che accumolo in un `vector<arco>`.
In questo modo conto due volte ogni arco, devo trovare un modo per escludere i doppioni. 
