# HeatEquationSolver

## Discretizzazione e riordinamento del dominio

### Task 1

Input: Numero intero N

Output: `coords.txt` e `connectivity.txt`

- Generazione `coords.txt`: Due cicli for annidati, uno su i e uno su j generano tutte le coppie (i,j). xi e yj si trovano di conseguenza. Ogni punto lo rappresento con una struct `punto {i, j, x, y}` e li accumolo in un `vector<punto>`. L'indice n aumenta di 1 ogni volta che aggiungo un elemento al vector. 

- Generazione `connectivity.txt`: Con un ciclo for sul `vector<punto>` controllo per ogni punto (i,j) se esistono (cioè se sono dentro il range 1,...,N) gli adiacenti (i, j+/-1) e (i+/-1, 1) e li organizzo in struct `arco {n1, n2}` che accumolo in un `vector<arco>`. L'indice e è equivalente a n nel punto precedente con la differenza che parte da zero.

  Problemi:

  - In questo modo conto due volte ogni arco, devo trovare un modo per escludere i doppioni. 
  - n1 è l'indice di iterazione del ciclo, il problema è trovare n2 senza dover ricontrollare tutto il `vector<punto>` per vedere in che punto è il punto adiacente.
  Sembrerebbe valido n(i,j) = (i -1)*N + (j -1), quindi posso risalire a n2 direttamente dalle coordinate spaziali.

### Task 2

Input: `coords.txt` e `connectivity.txt`

Output: `ordering.txt`

- Ordinamento _nested dissection_: Definisco una funzione ricorsiva che prende in input `vector<punto>` e un booleano che rappresenta la direzione su cui tagliare. La funzione deve fare queste cose in ordine:

  - Controlla quanti elementi ci sono nel vector, se 1 restituisce l'indice n di quel punto, se più di 1 continuo.
  - Non sono sicuro di come siano ordinati i punti nel vector quindi nel dubbio li ordina lungo la coordinata di taglio con `std::sort`. 
  - All'interno del vettore trova il valore mediano della coordinata di taglio, quindi ho il separatore.
  - Fa il taglio e ottengo: `V1`, `V2` e `VS`.
  - Definisce `lista1` e `lista2` richiamando la funzione in se stessa, questa volta con input `V1` e `V2` e con il booleano nel valore opposto.
  - Restituisce `lista1 + lista2 + VS`

  Se tutto va bene la funzione esegue il taglio fino a che non incontra un vector con un singolo elemento, a quel punto quella chiamata si interrompe e restituisce una lista con l'unico indice. Questo valore restituito diventa `lista1` (o 2) nel livello superiore.

  Problemi:

  - Non sono sicuro che funzioni.
  - Se invece di un vettore con un singolo elemento ne trova uno vuoto non so se causa problemi.
  - Se il numero di elementi del vettore è pari non ne trovo uno esattamente in mezzo. Non saprei quale scegliere tra i due centrali.
  - Il vettore lo riordino per trovare il mediano, però forse rischio di perdere informazioni sugli indici n originali, quindi potrebbe essere necessario estendere la struct `punto` per includere informazioni sull'indice n.

La funzione così non restituisce `ordering.txt`nel formato richiesto. L'indice m sarà la posizione nella lista finale restituita dalla funzione e l'indice n sarà il valore nella lista.

## Soluzione dell'equazione

### Task 3

Input: `connectivity.txt`, `ordering.txt`, `coords.txt`, u0 per il punto facoltativo

Output: `A.txt`, `rhs.txt`

- Il codice deve poter operare sull'ordinamento naturale o sull'ordinamento _nested-dissection_ a scelta dell'utente. Posso fare che il codice produce sempre i risultati per entrambi gli ordinamenti, oppure implementare un flag che governa in che modalità operare. Potrebbe essere utile un `std::unordered_map` che mappi gli indici n negli indici m corrispondenti basandosi su `ordering.txt`.
- Costruzione di A e rhs: 

  -Con un ciclo for sui punti di `coords.txt`, per ogni punto:

    - traduco l'indice n in m se la flag lo richiede.
    - Scrivo in `A.txt` "n n -4k/h**2" e costruisco la diagonale.
    - Scrivo in `rhs.txt` "f(x, y)" con x,y ricavati secondo la formula di task 1 a partire da i,j.
  - Con un ciclo for per ogni arco (n1,n2) in `connectivity.txt`:
  
    - traduco n1,n2 in m1,m2 se il flag lo richiede.
    - Scrivo "m1  m2  κ/h²" su `A.txt`.

  Problemi:
  - Per ora ho considerato u0=0. Per u0 diverse il problema è che mi servono informazioni sui punti di bordo che in `connectivity.txt` avevo escluso perché non erano richieste, quindi o modifico la task 1 per includere le informazioni sui punti di bordo oppure devo ripetere il procedimento della task 1 senza escludere i punti del bordo. 
