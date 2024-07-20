#ifndef PROJECT_H
#define PROJECT_H

#include <algorithm>
#include <iostream>
#include <cassert>
#include <ostream>
#include <cstddef> //Inclusione per ptrdiff_t

/**@author Alessandro Biagiotti [869014]
 * 
 * Classe SparseMatrix
 * 
 * La classe definisce il comportamento di una Sparse Matrix, ovvero di una
 * matrice che contiene un numero di dati minore della quantita' di 
 * informazioni che possono essere effettivamente inserite al suo interno.
 * L'implementazione e' stata fatta tramite una lista non ordinata.
 */
template <typename T>
class SparseMatrix{

public:
  /**@brief Definizione di dim_type
   * 
   * Definizione di un tipo di comodo per le dimensioni e per avere controllo
   * sull'input inserito dall'utente (le quantita' negative genereranno errori
   * a runtime o in compilazione).
   */
  typedef int dim_type;

private:
  /**@brief Definizione della struct di element
   * 
   * Struct impiegata per custodire il tipo templato, un element verra' poi
   * wrappato in una struct node che e' definita sotto
   */
  struct element{
    //dati membro
    T value;
    dim_type row, col;

    /** Costruttore di default
     * 
     */
    element() : row(0), col(0) {}
    /** Unico costruttore non di default
     * 
     * Utilizzato per andare a creare gli oggetti di tipo element con cui 
     * riempire i nodi della lista.
     * Sono costretto a controllare che i dati presi in ingresso siano non
     * negativi tramite una assert perche' altrimenti esplode tutto.
     */
    element(const T &v, dim_type r, dim_type c) : 
    row(0), col(0) {
      assert(r >= 0 && c >= 0);
      value = v;
      row = r;
      col = c;
    }
    /** Copyconstructor
     * 
     */
    element(const element &other) : 
    value(other.value), row(other.row), col(other.col) {}

    /** Distruttore di default
     */
    ~element() {}

    /** Operatore di uguaglianza
     * 
     * Controllo il self-assignement e poi faccio l'assegnamento membro a
     * membro
     */
    element &operator=(const element &other()) {
      if(this != &other){
        value = other.value;
        row = other.row;
        col = other.col;
      }
      return *this;
    }

    /** Getter per il numero di riga dell'elemento
     */
    dim_type getRow() const {

      return row;
    }

    /** Getter per il numero di colonna dell'elemento
     */
    dim_type getCol() const {

      return col;
    }

    /** Getter per il valore nell'elemento
     */
    T getValue() const {

      return value;
    }

    /** Ridefinizione della swap per element
     */
    element &swap(element &other) {
      std::swap(this->value, other.value);
      std::swap(this->row, other.row);
      std::swap(this->col, other.col);
    }
  };

  /** Definizione della struttura del nodo
   * 
   * Necessaria componente per il corretto funzionamento della lista, 
   * implementazione minimale costituita solamente da un element (che e' il 
   * core e un puntatore a nodo che e' il prossimo elemento da considerare)
   */
  struct node{
    element core;
    node *next;

    /** Costruttore di default
     * 
     * Quando un nodo viene costruito richiamo il costruttore di default di
     * element, probabilmente non e' particolarmente utile nel caso in esame
     * poiche' ogni nodo della matrice avra' sempre un valore, che sia definito
     * dall'utente o un valore di default
     */
    node() : core(), next(nullptr) {}
    /** Unico costruttore non di default
     * 
     * Mi viene passato direttamente un element gia' assemblato e lo chiudo 
     * dentro un nodo
     */
    node(const element &c) : core(c), next(nullptr) {}
    /** Copyconstructor
     * 
     * Copia dei dati membro a membro
     */
    node(const node &other) : core(other.core), next(other.next) {}

    /** Distruttore
     */
    ~node() {}

    /** Operatore di uguaglianza
     */
    node &operator=(const node &other) {
      if(other != this){

        this->core = other.core;
        this->next = other.next;
      }
      return *this;
    }

    /** Ridefinizione della swap per lavorare con i nodi
     */
    node &swap(node &other) {
      this->core.swap(other.core);
      std::swap(this->next, other.next);
    }
  };

public:
  
  /** Costruttore di default
   */
  SparseMatrix() : 
  _default(), _head(nullptr), _rows(0), _cols(0) {}

  /** Costruttore non di default
   * 
   * @param d valore di default templato
   * @param r numero massimo di righe
   * @param c numero massimo di colonne
   * 
   * Questo e' il costruttore principale, quello di default non verra'
   * mai usato, se la consegna mi e' chiara, perche' non c'e' modo di definire
   * a priori il valore di default, essendo un tipo templato. Anche in questo 
   * caso faccio i debiti controlli, tramite assert, per evitare che mi vengano
   * passate delle dimensioni negative per la matrice
   */
  SparseMatrix(const T &d, dim_type r, dim_type c) : 
  _head(nullptr), _rows(0), _cols(0) {
    assert(r >= 0 && c >= 0);
    _default = d;
    _rows = r;
    _cols = c;
  }
  /** Copyconstructor
   * 
   * Per questo copyconstructor vado a costruire una nuova matrice 
   * aggiungendo, elemento dopo elemento, i costituenti di other.
   */
  SparseMatrix(const SparseMatrix &other) : 
  _head(nullptr), _rows(0), _cols(0) {
    SparseMatrix tmp;
    node *i;

    try{

      tmp._rows = other._rows;
      tmp._cols = other._cols;
      tmp._default = other._default;
      for(i = other._head; i != nullptr; i = i->next){

        tmp.set(i->core.getValue(), i->core.getRow(), i->core.getCol());
      }
      this->swap(tmp);
    }
    catch(...){

      std::cout << "Errore in fase di costruzione" << std::endl;
      tmp._rows = 0;
      tmp._cols = 0;
      tmp.clear();
      throw;
    }
  }
  /** Distruttore
   * 
   * In fase di distruzione faccio una chiamata alla funzione privata clear
   * che mi ripulisce l'intera lista in modo da evitare memory leaks
   */
  ~SparseMatrix() {
    clear();
    _head = nullptr;
  }

  /** Operatore assegnamento
   */
  SparseMatrix &operator=(const SparseMatrix &other) {
    if(this != &other){

      SparseMatrix tmp(other);
      swap(tmp);
    }
    return *this;
  }

  /** Funzione set
   * 
   * @param v valore effettivo da inserire
   * @param r numero di riga in cui fare l'inserimento
   * @param c numero di colonna in cui fare l'inserimento
   * 
   * La funzione, dopo aver fatto un controllo sulla correttezza dei dati in
   * input (non devono essere negativi e non devono essere out of buonds) 
   * costruisce un oggetto di tipo element e, dopo aver controllato se esiste
   * gia' un elemento nella posizione indicata, procede all'inserimento.
   * Per fare il controllo sopra citato viene impiegata una funzione privata 
   * di cui parlo tra poco.
   */
  void set(const T &v, dim_type r, dim_type c) {
    assert(r < _rows && c < _cols);
    element e(v, r, c);
    node *newNode;
    node *tmp = posCheck(r, c);

    try{
    
      newNode = new node(e);
    }
    catch(...){

      delete newNode;
      std::cout << "Errore durante l'allocazione di memoria" << std::endl;
      throw;
    }
    if(_head == nullptr){

      _head = newNode;
      return;
    }
    else{
      if(tmp != nullptr){

        if(tmp == _head && newNode->core.getCol() == _head->core.getCol() && newNode->core.getRow() == _head->core.getRow()){

          replace(nullptr, newNode);
        }
        else{

          replace(tmp, newNode);
        }
        return;
      }
      else{

        newNode->next = _head;
        _head = newNode;
      }
    }
  }

  /** Funzione che restituisce il numero di righe nella matrice
   */
  dim_type rows() const {

    return _rows;
  }

  /** Funzione che restituisce il numero di colonne nella matrice
   */
  dim_type cols() const {

    return _cols;
  }

  /** Funzione che restituisce il valore di default
   */
  T getDefault() const {

    return _default;
  }

  /** Definizione del funtore di accesso ai dati
   * 
   * @return restituisco una copia del valore anziche' il reference al valore
   * perche' si tratta di un metodo di lettura
   * 
   * Il funtore di accesso mi consente di ripescare i valori presenti
   * all'interno della matrice, posto che la posizione sia valida, e li passa
   * al chiamante per copia.
   * Se la posizione e' occupata restituisce il valore, se la posizione e'
   * libera restituisce il valore di default.
   */
  T operator()(dim_type r, dim_type c) const {
    assert(r < _rows && c < _cols && r >= 0 && c >= 0);
    node *tmp = _head;

    while(tmp != nullptr){

      if(tmp->core.getRow() == r && tmp->core.getCol() == c){

        return tmp->core.getValue();
      }
      tmp = tmp->next;
    }
    return _default;
  }

  /** funzione numberOfElements
   * 
   * Una funzione che restituisce la quantita' di elementi presenti nella matrice
   */
  dim_type numberOfElements() const {
    dim_type length = 0;
    for(node *tmp = _head; tmp != nullptr; tmp = tmp->next){

      length++;
    }
    return length;
  }

  /** Funzione printList di stampa con iteratori
   * 
   * Tale funzione di stampa serve solamente per fare debugging, ho scelto il
   * formato con posizione scritta sopra e valore scritto sotto per renderla
   * piu' comprensibile quando si lavora con gli oggetti
   */
  void printList() const {

    for(const_iterator i = begin(); i != end(); ++i){

      std::cout << i->row << "\t" << i->col << std::endl;
      std::cout << i->value << std::endl;
    }
  }

  /** Funzione globale di stampa per il tipo SparseMatrix
   * 
   * La funzione stampa in modo tale che tutto sia formattato in modo carino
   * (un quadrato), evitando di inserire un ultimo a capo cosi che sia l'
   * utente a formattare come piu' gli piace.
   */
  friend std::ostream &operator<<(std::ostream &os, const SparseMatrix<T> &sm) {
    for(SparseMatrix<T>::dim_type i = 0; i < sm.rows(); ++i){

      for(SparseMatrix<T>::dim_type j = 0; j < sm.cols(); ++j) {

        os << sm(j, i) << '\t';
      }
      if(i != sm.rows() - 1) {
      
        os << std::endl;
      }
    }
    return os;
  }


private:
  
  //variabili di istanza
  node *_head;
  T _default;
  dim_type _rows, _cols;

  //metodi privati di SparseMatrix
  /** Funzione di pulizia
   * 
   * Questa funzione si occupa di liberare la memoria legata alla lista
   * facendo la delete di ogni nodo.
   */
  void clear() {
    node *tmp = _head;
    node *tmptmp;
    
    while(tmp != nullptr){

      tmptmp = tmp->next;
      delete tmp;
      tmp = tmptmp;
    }
  }

  /** Funzione per il controllo delle posizioni
   * 
   * @return Se la posizione e' occupata restituisco un puntatore all'elemento 
   * della lista che precede quello a cui mi devo riferire, altrimenti 
   * restituisco nullptr
   * 
   * Questa funzione privata e' un'alternativa all'operatore di accesso ed e'
   * necessaria per controllare se una posizione sia occupata o meno. Il valore 
   * restituito puo' essere: 1) nullptr se la posizione che cerco e' vuota, 
   * 2) un puntatore al nodo che precede il nodo che occupa la posizione che 
   * sto cercando, 3) un puntatore alla testa, se il nodo che occupa tale 
   * posizione e' in testa
   */
  node *posCheck(dim_type r, dim_type c) const {
    node *tmp = _head;
    if(tmp != nullptr){

      if(tmp->core.getRow() == r && tmp->core.getCol() == c){

        return tmp;
      }
      while(tmp->next != nullptr){

        if(tmp->next->core.getRow() == r && tmp->next->core.getCol() == c){

          return tmp;
        }
        tmp = tmp->next;
      }
    }
    return nullptr;
  }

  /** Ridefinizione della swap per lavorare con le SparseMatrix
   */
  SparseMatrix &swap(SparseMatrix &other) {
    std::swap(_head, other._head);
    std::swap(_default, other._default);
    std::swap(_rows, other._rows);
    std::swap(_cols, other._cols);
    return *this;
  }

  /** Funzione di rimpiazzo
   * 
   * @param prevToReplace e' il nodo che precede quello che deve essere
   * rimpiazzato
   * @param replacer e' il nodo rimpiazzante
   * 
   * Se prevToReplace e' nullptr allora significa che devo rimpiazzare la testa
   * altrimenti rimpiazzo il nodo puntato da prevToReplace->next.
   * Prima di eseguire il rimpiazzo mi occupo di liberare correttamente la 
   * memoria allocata
   */
  void replace(node *prevToReplace, node *replacer) {
    if(prevToReplace != nullptr){

      node *tmp = prevToReplace->next;
      prevToReplace->next = replacer;
      replacer->next = tmp->next;
      delete tmp;
    }
    else{

      node *tmp = _head->next;
      replacer->next = tmp;
      delete _head;
      _head = replacer;
    }
  }


/** Definizione di const_iterator
 * 
 * Utilizzo i const_iterator per:
 *  - Linearizzare la struttura dati e poterci iterare sopra (e' gia' lineare
 *    e ci potrebbero essere altri modi per lavorarci sopra ma gli iteratori 
 *    sono lo standard della programmazione ad oggetti)
 *  - Concedere all'utente un modo di scorrere gli elementi in sola lettura
 */
public:

  class const_iterator { 
  public:
    typedef std::forward_iterator_tag iterator_category;
    typedef element                         value_type;
    typedef ptrdiff_t                 difference_type;
    typedef const element*                  pointer;
    typedef const element&                  reference;

  
    const_iterator() : _current(nullptr) {}
    
    const_iterator(const const_iterator &other) : _current(other._current) {}

    const_iterator& operator=(const const_iterator &other) {
      _current = other._current;
      return *this;
    }

    ~const_iterator() {}

    // Ritorna il dato riferito dall'iteratore (dereferenziamento)
    reference operator*() const {
      return _current->core;
    }

    // Ritorna il puntatore al dato riferito dall'iteratore
    pointer operator->() const {
      return &(_current->core);
    }
    
    // Operatore di iterazione post-incremento
    const_iterator operator++(int) {
      const_iterator tmp(*this);
      _current = _current->next;
      return tmp;
    }

    // // Operatore di iterazione pre-incremento
    const_iterator& operator++() {
      _current = _current->next;
      return *this;
    }

    // Uguaglianza
    bool operator==(const const_iterator &other) const {
      return !(_current != other._current);
    }
    
    // Diversita'
    bool operator!=(const const_iterator &other) const {
      return _current != other._current;
    }

  private:
    //Dati membro
    const node *_current;

    // La classe container deve essere messa friend dell'iteratore per poter
    // usare il costruttore di inizializzazione.
    friend class SparseMatrix; // !!! Da cambiare il nome!

    // Costruttore privato di inizializzazione usato dalla classe container
    // tipicamente nei metodi begin e end
    const_iterator(const node *n) : _current(n) {}
    
    // !!! Eventuali altri metodi privati
    
  }; // classe const_iterator
  
  // Ritorna l'iteratore all'inizio della sequenza dati
  const_iterator begin() const {
    return const_iterator(_head);
  }
  
  // Ritorna l'iteratore alla fine della sequenza dati
  const_iterator end() const {
    return const_iterator(nullptr);
  }
};

/** Funzione evaluate
 * 
 * @param m matrice su cui deve essere eseguita la valutazione
 * @param pred funtore da valutare
 * 
 * @return restituisce il numero di elementi che rispettano il predicato
 * 
 * Funzione che valuta un predicato passato dall'utente su tutti i valori 
 * presenti nella matrice
 */
template <typename T, typename P>
typename SparseMatrix<T>::dim_type evaluate(const SparseMatrix<T> &m, const P &pred) {
  int count = 0;

  for(int i = 0; i < m.rows(); ++i){

    for(int j = 0; j < m.cols(); ++j){

      if(pred(m(i,j))){

        count++;
      }
    }
  }
  return count;
}

#endif
