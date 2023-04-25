**Nume: Sampetru Mario**

**Grupă: 311CAb**

## Load balancer. Tema 2.

### Descriere:

Comanda **INIT_LOAD_BALANCER**
* Aceasta este comanda incipienta, in cadrul careia este alocata memoria pentru load balancer si sunt initializate campurile structurii acesteia.

Comanda **LOADER_ADD_SERVER**
* Comanda in urma careia serverele sunt puse in cercul imaginar. Inainte de adaugare verific daca este nevoie de o realocare pentru extinderea cercului. Apoi, pe masura ce adaug servere, verific si daca este nevoie de rabalansare, adica daca dimensiunea dictionarului server-ului sursa este diferita de zero. Gasesc pozitia unde ar trebui sa rebalansez obiectele, ca mai apoi sa aiba loc mutarea obiectelor.

Comanda **LOADER_ADD_SERVER**
* Determin pozitia server-ului care trebuie eliminat, folosind cautare binara. Apoi, similar cu functia de adaugare, determin pozitia server-ului unde ar trebui mutate obiectele si efectuez aceasta actiune. In final, mut server-ele pe ultimele pozitii si le eliberez memoria.

Comanda **LOADER_STORE**
* Aflu pozitia unde ar trebui sa adaug un nou obiect, apoi realizez adaugarea efectiva prin intermediul functiei "server_store", care pune in dictionarul server-ului aferent perechea cheie-valoare.

Comanda **LOADER_RETRIEVE**
* Gasesc pozitia server-ului care contine hash-ul cheii primite ca parametru, folosind cautare binara. Apoi, apelez functia "server_retrieve", ce extrage din dictionarul serever-ului valoarea.

Comanda **FREE_LOAD_BALANCER**
* Realizeaza dealocarea tuturor resurselor si permite terminarea in conditii optime ale programului.