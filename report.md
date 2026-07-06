# HeatEquationSolver

## Discretizzazione e riordinamento del dominio

### Task 1
Input: Numero intero N

Output: `coords.txt` e `connectivity.txt`


+ Generazione `coords.txt`: Due cicli for annidati, uno su i e uno su j generano tutte le coppie (i,j). xi e yj si trovano di conseguenza. Ogni punto lo rappresento con una struct `punto {i, j, x, y}` e li accumolo in un `vector<punto>`. L'indice n aumenta di 1 ogni volta che aggiungo un elemento al vector. 

+ Generazione `connectivity.txt`: Con un ciclo for sul `vector<punto>` controllo per ogni punto (i,j) se esistono (cioè se sono dentro il range 1,...,N) gli adiacenti (i, j+/-1) e (i+/-1, 1) e li organizzo in struct `arco {n1, n2}` che accumolo in un `vector<arco>`. L'indice e è equivalente a n nel punto precedente con la differenza che parte da zero.
    * In questo modo conto due volte ogni arco, devo trovare un modo per escludere i doppioni. 
    * n1 è l'indice di iterazione del ciclo, il problema è trovare n2 senza dover ricontrollare tutto il `vector<punto>` per vedere in che punto è il punto adiacente.

### Task 2
Input: `coords.txt` e `connectivity.txt`

Output: `ordering.txt`


+ Ordinamento _nested dissection_: Definisco una funzione ricorsiva che prende in input un vector e un booleano che rappresenta la direzione su cui tagliare. La funzione deve fare queste cose in ordine:
    * Non sono sicuro di come siano ordinati i punti nel vector quindi nel dubbio li ordino lungo la coordinata di taglio con `std::sort`. 
    * All'interno del vettore trovo il valore mediano della coordinata di taglio, quindi ho il separatore.
    * Faccio il taglio e ottengo: `V1`, `V2` e `VS`.
    * Imposta il booleano della direzione sul valore opposto.
    * Richiamo la funzione in se stessa, questa volta con input `V1` e `V2`. 
    * Deve in qualche modo fermarsi quando arriva a `V1`/`V2` contenenti un singolo elemento e aggiungere l'indice n di quell'elemento in due liste `lista1`/`lista2`.
    * Restituire `lista1 + lista2 + VS`

 
