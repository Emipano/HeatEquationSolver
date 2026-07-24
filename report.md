# HeatEquationSolver

## Discretizzazione e riordinamento del dominio

### Task 1

Input: Numero intero N

Output: `coords.txt` e `connectivity.txt`

- Generazione `coords.txt`: Due cicli for annidati, uno su i e uno su j generano tutte le coppie (i,j). xi e yj si trovano di conseguenza. Ogni punto lo rappresento con una struct `punto {n, i, j, x, y}` e li accumolo in un `vector<punto>`. L'indice n aumenta di 1 ogni volta che aggiungo un elemento al vector. Uso ofstream per scrivere sul file di testo.

- Generazione `connectivity.txt`: Con un ciclo for sul `vector<punto>` controllo per ogni punto (i,j) se esistono (cioè se sono dentro il range 1,...,N) gli adiacenti (i, j+/-1) e (i+/-1, j) e li organizzo in struct `arco {e, n1, n2}` che accumolo in un `vector<arco>`. L'indice n1 è l'indice di iterazione del ciclo, per trovare n2 uso la formula n(i,j) = (i -1)*N + (j -1), usando iv,jv del punto vicino. L'indice e è equivalente a n nel punto precedente. Per evitare di fare 4 cicli diversi per ogni direzione in cui c'è un punto adiacente faccio un ciclo interno su un indice k=0,...,3; e utilizzo due liste per gli spostamenti dx e dy, per definire gli adiacenti come (i+dx[k], j+dy[k]). Per escludere i doppioni dovrebbe bastare imporre una condizione `if (n1 < n2)`.

  Note:

  - Per ora N è un int fisso, per la task 5 se voglio automatizzare lo devo rendere un argomento che posso passare da terminale (o `subprocess.run`).

### Task 2

Input: `coords.txt`

Output: `ordering.txt`

- Ordinamento _nested dissection_: Definisco una funzione ricorsiva che prende in input `vector<punto>` e un booleano che rappresenta la direzione su cui tagliare. La funzione deve fare queste cose in ordine:

  - Controlla quanti elementi ci sono nel vector, se 1 restituisce l'indice n di quel punto, se più di 1 continuo. Se 0 non deve andare al caso ricorsivo.
  - Non sono sicuro di come siano ordinati i punti nel vector quindi nel dubbio li ordina lungo la coordinata di taglio con `std::sort`. 
  - All'interno del vettore trova il valore mediano della coordinata di taglio, quindi ho il separatore.
  - Fa il taglio e ottengo: `V1`, `V2` e `VS`.
  - Definisce `lista1` e `lista2` richiamando la funzione in se stessa, questa volta con input `V1` e `V2` e con il booleano nel valore opposto.
  - Restituisce `lista1 + lista2 + VS`

  Se tutto va bene la funzione esegue il taglio fino a che non incontra un vector con un singolo elemento, a quel punto quella chiamata si interrompe e restituisce una lista con l'unico indice. Questo valore restituito diventa `lista1` (o 2) nel livello superiore.

  Note:

  - Nella parte del taglio ho dovuto riscrivere due volte tutta la logica (una volta per il taglio su x e una volta per il taglio su y), c'è sicuramente un modo per scriverlo più pulito, se lo trovo lo implento.

La funzione così non restituisce `ordering.txt`nel formato richiesto. L'indice m sarà la posizione nella lista finale restituita dalla funzione e l'indice n sarà il valore nella lista.

## Soluzione dell'equazione

### Task 3

Input: `connectivity.txt`, `ordering.txt`, `coords.txt`, u0 per il punto facoltativo

Output: `A.txt`, `rhs.txt`

- Il codice deve poter operare sull'ordinamento naturale o sull'ordinamento _nested-dissection_ a scelta dell'utente. Posso fare che il codice produce sempre i risultati per entrambi gli ordinamenti, oppure implementare un flag che governa in che modalità operare. Potrebbe essere utile un `std::unordered_map` che mappi gli indici n negli indici m corrispondenti basandosi su `ordering.txt`. Per comodità definisco una funzione che usa la map per tradurre da n a m se richiesto dal flag.
- Costruzione di A e rhs: 

  - Con un ciclo for sui punti di `coords.txt`, per ogni punto:

    - Ogni volta che devo includere un indice chiamo la funzione così sceglie da solo m o n in base al flag.
    - Scrivo in `A.txt` "n n -4k/h^2" e costruisco la diagonale.
    - Scrivo in `rhs.txt` solo la funzione f valutata nel punto corrente. Devo comunque cambiare l'ordinamento dei valori in rhs.txt se il flag lo richiede, per farlo accumolo prima in un vector, lo ordino per n crescente e poi lo scrivo nel file.
  - Con un ciclo for per ogni arco (n1,n2) in `connectivity.txt`:
  
    - Ogni volta che devo includere un indice chiamo la funzione così sceglie da solo m o n in base al flag.
    - Scrivo "n1  n2  k/h^2" e il simmetrico "n2  n1  k/h^2" su `A.txt`. (Se non avessi escluso i doppioni da connectivity.txt in task1 non sarebbe necessario aggiungere manualmente i simmetrici, ma ormai...)

  Note:
  - Come per la task 1 per ora N e fisso, se automatizzo la task 5 sarà da cambiare in un argomento da passare dal terminale.
  - Il flag che decide se usare nested dissection per ora è fisso, come per N se automatizzo la task 5 lo devo rendere passabile da terminale.
  - Ho chiamato la funzione di mappatura da n a m ogni volta che compariva un indice, col senno di poi sarebbe stato meglio definire l'indice con la funzione una sola volta e poi usare quello. Se ho tempo lo farò per migliorare la leggibilità.
  - Per ora ho considerato u0=0. Per u0 diverse il problema è che mi servono informazioni sui punti di bordo che in `connectivity.txt` avevo escluso perché non erano richieste, quindi o modifico la task 1 per includere le informazioni sui punti di bordo oppure devo ripetere il procedimento della task 1 senza escludere i punti del bordo. 

### Task 4

Input: `A.txt`, `rhs.txt`

Output: Array `x` soluzione del sistema lineare

- Costruzione matrice sparsa: Leggo `A.txt` con `np.loadtxt(..., unpack=True)` per ottenere tre liste `i`, `j`, `A[i,j]`. Con `scipy.sparse.coo_matrix()` costruisco la matrice in formato COO e con `.tocsc()`la converato in CSC.
- Leggo `rhs.txt` con `np.loadtxt`.
- Definisco la funzione `my_cholesky` come sul pdf e con `my_cholesky(-A)` ottengo L.
- Risolvo il sistema in due passaggi:

  - Prima L*y = -rhs, con `spsolve_triangular(..., lower=True)`
  - Poi L.T*x = y, con con `spsolve_triangular(..., lower=False)`
  - L'array x dovrebbe essere la soluzione del sistema.


### Task 5

- Grafici della soluzione: Devo plottare la soluzione in uscita dalla task 4. Il vettore `x` dovrebbe essere ordinato secondo l'indice `m`, quindi tramite `ordering.txt` e `coords.txt` devo ricondurmi alle coordinate spaziali `i,j` in modo che il plot abbia un senso fisico. Per realizzare il grafico faccio utilizzo delle `colormaps` di `matplotlib` in modo da rappresentare diverse temperature con diversi colori.
- È richiesto di ottenere le soluzioni per 6 valori di N per entrambi gli ordinamento, quindi dovrei eseguire un totale di 12 volte i codici delle task da 1 a 4 e segnarmi i valori richiesti (i tempi e il numero di entrate non-zero). Sembra tedioso da fare manualmente, potrei provare ad automatizzare il processo tramite la libreria di python `subprocess`, dovrei fare due cicli for, uno su N e uno interno sulla modalità di ordinamento. Probabilmente sarà necessario adattare i codici in C++, per esempio N deve essere un argomento del programma della task 1, così posso eseguirlo con N diversi senza ricompilarlo ogni volta. 
  - Non credo di poter usare `subprocess` per runnare una cella specifica di un notebook jupyter (ovvero la task 4), quindi probabilmente sarà necessario copiare la task 4 su un file .py separato.