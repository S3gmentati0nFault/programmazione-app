#include "project.h"
#include <iostream>
#include <string>
#include <ostream>
#include <cmath>

/** Classe punto
 * 
 * Classe per eseguire test piu' complessi rispetto a quelli eseguiti sui tipi
 * base
 */
class Punto{
//variabili di istanza
  int x, y;

public:
  /** Costruttore di default
   */
  Punto() : x(0), y(0) {}
  /** Costruttore non di default
   * 
   * Unico costruttore che inizializza un punto inserendo due coordinate
   */
  Punto(int i, int j) : x(i), y(j) {}
  /** Copyconstructor
   * 
   * inizializza un punto variabile per variabile
   */
  Punto(const Punto &other) : x(other.x), y(other.y) {}
  /** Distruttore
   */
  ~Punto() {}

  /** Operatore di assegnamento
   */
  Punto &operator=(const Punto &other) {

    x = other.x;
    y = other.y;
    return *this;
  }
  /** Operatore di uguaglianza
   */
  bool operator==(const Punto &other) const {

    return (x == other.x) && (y == other.y);
  }
  /** Operatore di disuguaglianza
   */
  bool operator!=(const Punto &other) const {

    return !(*this == other);
  }
  /** Getter ascissa
   */
  int getX() const {

    return x;
  }
  /** Getter ordinata
   */
  int getY() const {

    return y;
  }
  /** Forward definition dell'operatore<<
   */
  friend std::ostream &operator<<(std::ostream &os, const Punto &p);
};

/** Funzione di stampa per i punti
 */
std::ostream &operator<<(std::ostream &os, const Punto &p){

  os << "x: " << p.x << " " << "y: " << p.y;
  return os;
}

/** definizione Classe Non Trivial
 * 
 * Definizione di una funzione di una certa complessita' (contiene un array
 * allocato dinamicamente di tipi custom), che fornira' l'infrastruttura per
 * costruire i test piu' difficili
 */
class ClasseNonTrivial{
  /** Definizione della struttura Alunno per i test
   * 
   * @member id la stringa che contiene nome e cognome
   * @member media un double contenente la media dei voti agli esami
   */
  struct Alunno{
    std::string id;
    double media;

    /** Costruttore di default
     */
    Alunno() : id(""), media(0.0){}

    /** Costruttore non di default
     * 
     * associo a ogni campo il valore preso in input
     */
    Alunno(std::string i, double m) : id(i), media(m) {}

    /** Copyconstructor
     * 
     * Associo a ogni campo il valore corretto preso da other
     */
    Alunno(const Alunno &other) : id(other.id), media(other.media) {}

    /** Distruttore di default
     */
    ~Alunno() {}

    /** Operatore di assegnamento
     * 
     * Assegno a ogni campo di this il corretto valore preso da other
     */
    Alunno &operator=(const Alunno &other) {
      if(this != &other){

        id = other.id;
        media = other.media;
      }
      return *this;
    }

    /** Operatore di uguaglianza
     */
    bool operator==(const Alunno &other) {
      if(id != other.id){

        return false;
      }
      return true;
    }

    /** Operatore di disuguaglianza
     */
    bool operator!=(const Alunno &other) {
      return(*this == other);
    }

    /** Setter per media
     */
    void setMedia(double m) {
      assert(m >= 0.0 && m <= 33.0);

      media = m;
    }
    friend std::ostream &operator<<(std::ostream &os, const Alunno &a);
  };

  Alunno *_alunni;
  int _dim, _current;
  bool _isFull;

public:

  /** Costruttore di default
   */
  ClasseNonTrivial() : 
  _alunni(nullptr), _dim(0), _current(0), _isFull(false) {}

  /** Primo costruttore non di default
   * 
   * Inizializza gli array secondo una dimensione specificata in input
   */
  explicit ClasseNonTrivial(int d) :
  _alunni(nullptr), _dim(0), _current(0), _isFull(false) {
    assert(d >= 0);
    Alunno alunnoDefault("", 0.0);
    try{

      _alunni = new Alunno[d];
      _dim = d;
      for(int i = 0; i < _dim; ++i){

        _alunni[i] = alunnoDefault;
      }
      if(_dim == 0){

        _isFull = true;
      }
    }
    catch(...){

      delete[] _alunni;
      _alunni = nullptr;
      std::cout << "Errore in fase di allocazione di memoria" << std::endl;
      throw;
    }
  }

  /** Copyconstructor
   * 
   * Il copyconstructor ricrea gli array di other in this allocando la memoria
   * dinamicamente
   */
  ClasseNonTrivial(const ClasseNonTrivial &other) {
    try{

      _alunni = new Alunno[other._dim];
      for(int i = 0; i < other._current; ++i){

        _alunni[i] = other._alunni[i];
      }
      _current = other._current;
      _dim = other._dim;
      _isFull = other._isFull;
    }
    catch(...){

      delete[] _alunni;
      _alunni = nullptr;
      std::cout << "Errore in fase di allocazione di memoria" << std::endl;
      _current = 0;
      _dim = 0;
      _isFull = false;
      throw;
    }
  }

  /** Distruttore di default
   */
  ~ClasseNonTrivial() {
    delete[] _alunni;
    _alunni = nullptr;
  }

  /** Operatore assegnamento
   * 
   * controlla l'autoassegnamento e poi fa una chiamata al copyconstructor per
   * una variabile temporanea cosi' che sia poi il compilatore ad occuparsi 
   * della distruzione automatica dell'istanza
   */
  ClasseNonTrivial &operator=(const ClasseNonTrivial &other) {
    if(this != &other){

      ClasseNonTrivial tmp(other);
      this->swap(tmp);
    }
    return *this;
  }

  /** Ridefinizione dell'operatore []
   * 
   * L'operatore [] mi restituisce il pos-esimo elemento nell'array di alunni 
   */
  Alunno &operator[](int pos) const {
    assert(pos >= 0 && pos < _dim);

    return _alunni[pos];
  }

  /** Operatore ==
   */
  bool operator==(const ClasseNonTrivial &other) const {

    if(_isFull == other._isFull){

      if(_dim == other._dim){

        if(_current == other._current){

          for(int i = 0; i < _current; ++i){

            if(_alunni[i] != other._alunni[i]){

              return false;
            }
          }
        }
        else{

          return false;
        }
      }
      else{

        return false;
      }
    }
    else{

      return false;
    }
    return true;    
  }
  /** Operatore !=
   */
  bool operator!=(const ClasseNonTrivial &other) const {

    return !(*this == other);
  }

  /** Funzione add per l'aggiunta di un elemento
   * 
   * La funzione add va ad inserire nella posizione _current la coppia di 
   * valori (double e stringa) passati in ingresso, dopo aver controllato
   * che il valore della media sia non negativo
   */
  void add(double media, std::string nome_cognome){
    assert(media > 0.0 && media < 34.0);
    for(int i = 0; i < _current; ++i){

      if(_alunni[i].id == nome_cognome){

        _alunni[i].media = media;
        return;
      }
    }
    if(!_isFull){

      _alunni[_current].media = media;
      _alunni[_current].id = nome_cognome;
      if(_dim == _current + 1){

        _isFull = true;
      }
      _current++;
    }
  }

  /** Definizione funzione swap
   * 
   * funzione che scambia gli elementi di this variabile per variabile con l'
   * oggetto passato in input
   */
  void swap(ClasseNonTrivial &other) {

    std::swap(_alunni, other._alunni);
    std::swap(_dim, other._dim);
    std::swap(_current, other._current);
    std::swap(_isFull, other._isFull);
  }

  /** Getter per current
   */
  int getCurr() const {
    return _current;
  }

  /** Getter per dim
   */
  int getDim() const {
    return _dim;
  }

  /** Getter per isFull
   */
  bool checkStatus() const {

    return _isFull;
  }

  friend std::ostream &operator<<(std::ostream &os, const ClasseNonTrivial &cnt);
};

std::ostream &operator<<(std::ostream &os, const ClasseNonTrivial &cnt) {

  for(int i = 0; i < cnt.getCurr(); ++i){

    os << cnt[i].id << "\t" << cnt[i].media << std::endl;
  }
  return os;
}

/** Definizione del funtore is_even
 * 
 * Funtore che controlla se un valore intero e' pari
 */
struct is_even{

  bool operator()(int a) const {
    if(a % 2 == 0) {

      return true;
    }
    return false;
  }
};

/** Definizione del funtore is_odd
 * 
 * Funtore che controlla se un valore intero e' dispari
 */
struct is_odd{

  bool operator()(int a) const {

    if(a % 2 != 0){

      return true;
    }
    return false;
  }
};

/** Definzione del funtore is greater_than_2
 * 
 * Funtore che controlla se la distanza di un punto dall'origine (0,0) e'
 * maggiore di 2
 */
struct distance_grater_than_2{

  bool operator()(const Punto &p) const {
    double distance = sqrt(pow(p.getX(), 2) + pow(p.getY(), 2));

    if(distance > 2){

      return true;
    }
    return false;
  }
};

/** Definzione del funtore is greater_than_15
 * 
 * Funtore che controlla se la distanza di un punto dall'origine (0,0) e'
 * maggiore di 15
 */
struct distance_grater_than_15{

  bool operator()(const Punto &p) const {
    double distance = sqrt(pow(p.getX(), 2) + pow(p.getY(), 2));

    if(distance > 15){

      return true;
    }
    return false;
  }
};

/** Funtore difficolta_corso
 * 
 * Questo funtore, totalmente non rappresentativo della realta', stima la 
 * complessita' di un corso sulla base della media degli studenti che attendono
 * le lezioni
 */
struct difficolta_corso {

  bool operator()(const ClasseNonTrivial &cnt) const {
    int corsoFacile = 0;
    for(int i = 0; i < cnt.getCurr(); ++i){

      if(cnt[i].media >= 25.0){

        corsoFacile++;
      }
    }
    if(corsoFacile >= 10){

      return true;
    }
    return false;
  }
};

/** Funtore di affluenza
 * 
 * Il funtore di affluenza controlla se una classe / corso ha effettivamente
 * raggiunto la capienza massima o meno
 */
struct affluenza {

  bool operator()(const ClasseNonTrivial &cnt) const {

    return cnt.checkStatus();
  }
};

/** Funtore capienza massima pari
 */
struct capienza_massima_pari {

  bool operator()(const ClasseNonTrivial &cnt) const {

    if(cnt.getDim() % 2 == 0){

      return true;
    }
    return false;
  }
};

/** Funtore capienza massima dispari
 */
struct capienza_massima_dispari {

  bool operator()(const ClasseNonTrivial &cnt) const {

    if(cnt.getDim() % 2 == 1){

      return true;
    }
    return false;
  }
};

/** Funtore oggetti_non_default
 * 
 * Funtore che riconosce gli oggetti non di default presenti nella matrice, gli
 * oggetti di default inseriti dall'utente saranno comunque flaggati come
 * default
 */
struct oggetti_non_default {

  bool operator()(const ClasseNonTrivial &cnt) const {

    if(cnt.getDim() > 0){

      return true;
    }
    return false;
  }
};

/** Funtore oggetti_default
 * 
 * Funtore che riconosce gli oggetti di default presenti nella matrice, come
 * per il precedente gli oggetti di default inseriti dall'utente sono comunque
 * flaggati come default
 */
struct oggetti_default {

  bool operator()(const ClasseNonTrivial &cnt) const {

    if(cnt.getDim() == 0){

      return true;
    }
    return false;
  }
};

/** Test per le matrici vuote
 * 
 * Test di utilita' dubbia che mi sono sentito di fare per controllare che le
 * funzioni non avessero un comportamento inatteso in questo caso.
 */
void test_matrice_vuota(){
  SparseMatrix<ClasseNonTrivial> M;
  ClasseNonTrivial aula1(3);
  aula1.add(27.0, "Elena Biagiotti");
  aula1.add(29.8, "Filippo Loi");
  aula1.add(17.9, "Simone Ditrani");

  //Stampa delle informazioni riguardo alla matrice
  std::cout << M.getDefault() << std::endl;
  std::cout << M.rows() << std::endl;
  std::cout << M.cols() << std::endl;
  std::cout << M.numberOfElements() << std::endl;

  //tentativo di set di un elemento
  //M.set(aula1, 0, 0);

  //Stampa del contenuto della lista tramite iteratori
  //che non stampa nulla perche' la lista e' vuota
  for(SparseMatrix<ClasseNonTrivial>::const_iterator i = M.begin(); i != M.end(); ++i){

    std::cout << i->row << "\t" << i->col << std::endl << i->value << std::endl;
  }
}

/** test per i metodi fondamentali sul tipo base
 * 
 * Questa funzione esegue i metodi fondamentali su una matrice di interi
 */
void test_metodi_fondamentali_tipo_base() {
  //test costruttore di default
  SparseMatrix<int> M;
  std::cout << "Matrice M: " << std::endl << M << std::endl;

  //test inserimento con matrice di dimensione (0,0)
  //M.set(1, 0, 0);

  //test costruttore non di default
  SparseMatrix<int> M1(0, 10, 10);
  std::cout << "Matrice M1: " << std::endl << M1 << std::endl;
  M1.printList();

  //test inserimento in posizione illegale
  //M1.set(1, -1, -1);
  //M1.set(1, 100, 100);

  //test di inserimento
  M1.set(10, 3, 1);
  M1.set(0, 0, 0);
  M1.set(3, 1, 2);
  M1.set(70, 8, 0);
  M1.set(3, 4, 3);
  
  //test sovrascrittura e nuovo inserimento
  M1.set(21, 8, 0);
  M1.set(13, 7, 9);
  M1.set(1, 0, 0);

  //Controllo del corretto funzionamento dell'operatore di lettura di un
  //elemento in una certa posizione
  if(M1(8, 0) != 21){

    std::cout << "Errore dell'operatore di lettura, expected 21 ma M1(8, 0) = " << M1(8, 0) << std::endl;
    return;
  }
  if(M1(1, 1) != 0){

    std::cout << "Errore dell'operatore di lettura, expected default value ma M1(1, 1) = " << M1(1, 1) << std::endl;
    return;
  }
  //controllo il contenuto della lista
  std::cout << std::endl << std::endl;
  M1.printList();
  std::cout << std::endl << std::endl;

  //test lettura in posizione illegale
  //M1(-1, -1);
  //M1(100, 100);

  //test copyconstructor
  SparseMatrix<int> M2(M1);
  //test inserimento e sovrascrittura in M2
  M2.set(5, 6, 6);
  M2.set(42, 5, 3);
  M2.set(-21, 7, 9);
  std::cout << "Matrice M2: " << std::endl << M2 << std::endl;
  //controllo il contenuto della lista
  std::cout << std::endl << std::endl;
  M2.printList();
  std::cout << std::endl << std::endl;

  //test assegnamento a matrice con elementi
  SparseMatrix<int> M3(0, 2, 2);
  M3.set(3, 1, 1);
  std::cout << "Matrice M3: " << std::endl << M3 << std::endl;
  M3 = M2;
  //test inserimento e sovrascrittura in M3
  M3.set(8, 3, 0);
  M3.set(-11, 9, 9);
  M3.set(2, 6, 6);
  std::cout << "Matrice M3: " << std::endl << M3 << std::endl;
  //controllo il contenuto della lista
  std::cout << std::endl << std::endl;
  M3.printList();
  std::cout << std::endl << std::endl;
}

/** test per i metodi fondamentali sul tipo complesso
 * 
 * Questa funzione esegue i metodi fondamentali su una matrice di punti
 */
void test_metodi_fondamentali_tipo_complesso(){
  //Test costruzione e inserimento
  Punto defaultPoint;
  SparseMatrix<Punto> M1(defaultPoint, 6, 6);
  std::cout << "Matrice M1: " << std::endl << M1 << std::endl;

  Punto p1(10, 3), p2(3, 3), p3(0, 2), p4(7, 0), p5(9, 9), p6(6, 5), p7(2, 4), pErr(1, 1);
  M1.set(p1, 3, 1);
  M1.set(p2, 0, 0);
  M1.set(p3, 1, 2);
  M1.set(p4, 5, 0);
  M1.set(p5, 4, 3);

  //test sovrascrittura
  M1.set(p6, 1, 2);
  //test di stampa con iteratori
  std::cout << std::endl << std::endl;
  M1.printList();
  std::cout << std::endl << std::endl;

  //Test inserimento pErr in posizione illegale
  //M1.set(pErr, -1, -1);
  //M1.set(pErr, 100, 100);

  //controllo dell'operatore di lettura
  if(M1(3, 1) != p1){

    std::cout << "Errore dell'operatore di lettura, expected: " << p1 <<
    " recieved instead: " << M1(3, 1);
  }
  if(M1(3, 3) != defaultPoint){

    std::cout << "Errore dell'operatore di lettura, expected: " << defaultPoint <<
    " recieved instead: " << M1(3, 3);
  }

  //test di lettura in posizione illegale
  //M1(-1, -1);
  //M1(100, 100);

  Punto p8(10, 0), p9(7, 4), p10(2, 2);
  //test copyconstructor
  SparseMatrix<Punto> M2(M1);
  //test inserimento e sovrascrittura
  M2.set(p8, 3, 3);
  M2.set(p9, 1, 5);
  M2.set(p10, 4, 3);
  std::cout << "Matrice M2: " << std::endl << M2 << std::endl;

  //test di stampa con iteratori
  std::cout << std::endl << std::endl;
  M2.printList();
  std::cout << std::endl << std::endl;

  //test assegnamento a matrice non vuota
  SparseMatrix<Punto> M3(defaultPoint, 2, 2);
  M3.set(p7, 1, 1);
  std::cout << "Matrice M3: " << std::endl << M3 << std::endl;
  Punto p11(3, 2), p12(12, 21), p13(9, 3);
  M3 = M2;
  //test inserimento e sovrascrittura
  M3.set(p11, 1, 1);
  M3.set(p12, 1, 3);
  M3.set(p13, 5, 0);
  std::cout << "Matrice M3: " << std::endl << M3 << std::endl;
  //test di stampa con iteratori
  std::cout << std::endl << std::endl;
  M3.printList();
  std::cout << std::endl << std::endl;
}

/** test metodi fondamentali di tipo molto complesso
 * 
 * Questa funzione esegue il test dei metodi fondamentali su una matrice di 
 * valori Classe Non Trivial
 */
void test_metodi_fondamentali_tipo_molto_complesso(){
  ClasseNonTrivial aulaDefault;
  ClasseNonTrivial aula1(3);
  aula1.add(27.0, "Elena Biagiotti");
  aula1.add(29.8, "Filippo Loi");
  aula1.add(17.9, "Simone Ditrani");
  ClasseNonTrivial aula2(4);
  aula2.add(30.0, "Edoardo Samanna");
  aula2.add(16.0, "Alessandro Segalini");
  aula2.add(22.1, "Emanuele Bellini");
  aula2.add(25.3, "Lorenzo Auletta");
  ClasseNonTrivial aula3(2);
  aula3.add(29.34, "Rene Bwanika");
  aula3.add(27.5, "Nemanja Antonic");
  ClasseNonTrivial aula4(5);
  aula4.add(28.4, "Alice Arneodo");
  aula4.add(23.1, "Alessandra Atria");
  aula4.add(25.0, "Federica DiCatania");
  ClasseNonTrivial aula5(7);
  aula5.add(21.1, "Antonio Bosco");
  aula5.add(24.3, "Antonio Andreoli");
  aula5.add(26.3, "Stefano Annoni");
  ClasseNonTrivial aula6(10);
  aula6.add(24.0, "Michele Cossu");
  aula6.add(26.5, "Alessandro Biagiotti");
  aula6.add(27.34, "Elia Gaviraghi");
  aula6.add(30.0, "Sofia Amaru");
  aula6.add(26.6, "Luca Annese");
  aula6.add(31.1, "Fabio Cavalieri");
  ClasseNonTrivial aula7(3);
  aula7.add(23.0, "Eugenio Merini");
  aula7.add(30.0, "Giulia Granata");
  aula7.add(31.0, "Anna Breda");
  ClasseNonTrivial aula8(4);
  aula8.add(25.0, "Francesco Rossi");
  aula8.add(22.5, "Alice Andreoni");
  ClasseNonTrivial aula9(2);
  aula9.add(29.0, "Riccardo Fanizzi");
  aula9.add(29.0, "Andrea Bottinelli");
  ClasseNonTrivial aula10(10);
  aula10.add(30.0, "Susanna Negri");
  aula10.add(1.0, "Luca Comelli");
  aula10.add(22.0, "Alessandro Regalia");

  //test costruttore di default
  SparseMatrix<ClasseNonTrivial> M;
  std::cout << M << std::endl;

  //test costruttore non di default
  SparseMatrix<ClasseNonTrivial> M1(aulaDefault, 10, 10);
  M1.set(aula1, 3, 1);
  M1.set(aula2, 0, 0);
  M1.set(aula3, 1, 2);
  M1.set(aula4, 8, 0);
  M1.set(aula5, 4, 3);
  //std::cout << M1 << std::endl;

  //test sovrascrittura
  M1.set(aula6, 1, 2);
  //std::cout << M1(1, 2) << std::endl;

  //test copyconstructor
  SparseMatrix<ClasseNonTrivial> M2(M1);
  //test aggiunta e sovrascrittura
  M2.set(aula7, 5, 5);
  M2.set(aula8, 9, 8);
  M2.set(aula9, 0, 0);

  //Stampa con iteratori
  for(SparseMatrix<ClasseNonTrivial>::const_iterator i = M2.begin(); i != M2.end(); ++i){

    std::cout << i->row << "\t" << i->col << std::endl;
    std::cout << i->value << std::endl;
  }
  std::cout << std::endl << std::endl << std::endl;

  //test assegnamento
  M = M2;
  M.set(aula6, 3, 7);
  //test aggiunta e sovrascrittura
  M.set(aulaDefault, 5, 5);
  M.set(aulaDefault, 9, 8);
  M.set(aula10, 7, 1);
  for(SparseMatrix<ClasseNonTrivial>::const_iterator i = M.begin(); i != M.end(); ++i){

    std::cout << i->row << "\t" << i->col << std::endl;
    std::cout << i->value << std::endl;
  }
}

/** Funzione per la valutazione di funtori semplici
 * 
 * Funzione che esegue la valutazione di alcuni funtori di base su 
 * una matrice di interi
 */
void test_funtori_base() {
  //test costruttore non di default
  SparseMatrix<int> M1(0, 10, 10);
  std::cout << "Matrice M1: " << std::endl << M1 << std::endl;

  //test di inserimento
  M1.set(10, 3, 1);
  M1.set(0, 0, 0);
  M1.set(3, 1, 2);
  M1.set(70, 8, 0);
  M1.set(3, 4, 3);

  //test funtore is_even
  is_even ie;
  std::cout << evaluate(M1, ie) << std::endl;
  //test funtore is_odd
  is_odd io;
  std::cout << evaluate(M1, io) << std::endl;
}

/** Funzione per la valutazione di funtori complessi
 * 
 * Funzione che esegue la valutazione di alcuni funtori di base su 
 * una matrice di punti
 */
void test_funtori_complessi() {
  //Test costruzione e inserimento
  Punto defaultPoint;
  SparseMatrix<Punto> M1(defaultPoint, 10, 10);
  std::cout << "Matrice M1: " << std::endl << M1 << std::endl;

  Punto p1(10, 3), p2(3, 3), p3(0, 2), p4(7, 0), p5(9, 9);
  M1.set(p1, 3, 1);
  M1.set(p2, 0, 0);
  M1.set(p3, 1, 2);
  M1.set(p4, 8, 0);
  M1.set(p5, 4, 3);
  M1.set(defaultPoint, 4, 4);

  //test funtore distance_greater_than_2
  distance_grater_than_2 dgt2;
  std::cout << evaluate(M1, dgt2) << std::endl;
  //test funtore distance_greater_than_15
  distance_grater_than_15 dgt15;
  std::cout << evaluate(M1, dgt15) << std::endl;
}

/** Funzione per la valutazione di funtori su oggetti complessi
 * 
 * La funzione valuta i funtori descritti sopra su oggetti di tipo 
 * ClasseNonStandard.
 */
void test_funtori_assurdi() {
  ClasseNonTrivial aulaDefault;
  ClasseNonTrivial aula1(3);
  aula1.add(27.0, "Elena Biagiotti");
  aula1.add(29.8, "Filippo Loi");
  aula1.add(17.9, "Simone Ditrani");
  ClasseNonTrivial aula2(4);
  aula2.add(30.0, "Edoardo Samanna");
  aula2.add(16.0, "Alessandro Segalini");
  aula2.add(22.1, "Emanuele Bellini");
  aula2.add(25.3, "Lorenzo Auletta");
  ClasseNonTrivial aula3(2);
  aula3.add(29.34, "Rene Bwanika");
  aula3.add(27.5, "Nemanja Antonic");
  ClasseNonTrivial aula4(5);
  aula4.add(28.4, "Alice Arneodo");
  aula4.add(23.1, "Alessandra Atria");
  aula4.add(25.0, "Federica DiCatania");
  ClasseNonTrivial aula5(7);
  aula5.add(21.1, "Antonio Bosco");
  aula5.add(24.3, "Antonio Andreoli");
  aula5.add(26.3, "Stefano Annoni");
  ClasseNonTrivial aula6(10);
  aula6.add(24.0, "Michele Cossu");
  aula6.add(26.5, "Alessandro Biagiotti");
  aula6.add(27.34, "Elia Gaviraghi");
  aula6.add(30.0, "Sofia Amaru");
  aula6.add(26.6, "Luca Annese");
  aula6.add(31.1, "Fabio Cavalieri");
  ClasseNonTrivial aula7(3);
  aula7.add(23.0, "Eugenio Merini");
  aula7.add(30.0, "Giulia Granata");
  aula7.add(31.0, "Anna Breda");
  ClasseNonTrivial aula8(4);
  aula8.add(25.0, "Francesco Rossi");
  aula8.add(22.5, "Alice Andreoni");
  ClasseNonTrivial aula9(2);
  aula9.add(29.0, "Riccardo Fanizzi");
  aula9.add(29.0, "Andrea Bottinelli");
  ClasseNonTrivial aula10(10);
  aula10.add(30.0, "Susanna Negri");
  aula10.add(1.0, "Luca Comelli");
  aula10.add(22.0, "Alessandro Regalia");
  SparseMatrix<ClasseNonTrivial> M(aulaDefault, 10, 10);
  M.set(aula1, 3, 1);
  M.set(aula2, 0, 0);
  M.set(aula3, 1, 2);
  M.set(aula4, 8, 0);
  M.set(aula5, 4, 3);  
  M.set(aula6, 6, 2);
  M.set(aula7, 5, 5);
  M.set(aula8, 9, 8);
  M.set(aula9, 1, 0);
  M.set(aula10, 7, 1);
  M.set(aulaDefault, 9, 9);
  assert(evaluate(M, difficolta_corso()) == 0);
  assert(evaluate(M, affluenza()) == 5);
  assert(evaluate(M, capienza_massima_pari()) == 96);
  assert(evaluate(M, capienza_massima_dispari()) == 4);
  assert(evaluate(M, oggetti_default()) == 90);
  assert(evaluate(M, oggetti_non_default()) == 10);
}

void test_classe_non_trivial() {
  //test costruttore di default
  ClasseNonTrivial aula0;
  std::cout << aula0 << std::endl;

  //test costruttore non di default
  ClasseNonTrivial aula(22);
  std::cout << aula << std::endl;

  //test inserimento
  aula.add(26.5, "Alessandro Biagiotti");
  aula.add(24.0, "Michele Cossu");
  aula.add(27.34, "Elia Gaviraghi");
  aula.add(30.0, "Lidia Losavio");
  aula.add(26.6, "Luca Annese");
  aula.add(31.1, "Fabio");
  std::cout << aula << std::endl;

  //test copyconstructor
  ClasseNonTrivial aula1(aula);

  //test inserimento nuovi dati e sovrascrittura
  aula1.add(28.1, "Lorenzo Guzzetti");
  aula1.add(31.1, "Lidia Losavio");

  //test operatori di uguaglianza e diversita'
  std::cout << (aula == aula1) << "\t" << (aula != aula1) << std::endl;

  //test operatore di assegnamento
  aula0 = aula1;

  //test inserimento nuovi dati e sovrascrittura
  aula0.add(29.8, "Alin Tomulet");
  aula0.add(33.0, "Emma Tidu");
  aula0.add(24.3, "Francesco Rossi");
  aula0.add(27.0, "Alessandro Biagiotti");
  //test operatori di uguaglianza e diversita'
  std::cout << (aula0 == aula1) << "\t" << (aula0 != aula1) << std::endl;
}

int main(){
  // test_matrice_vuota();
  test_metodi_fondamentali_tipo_base();
  // test_metodi_fondamentali_tipo_complesso();
  // test_metodi_fondamentali_tipo_molto_complesso();
  // test_funtori_base();
  // test_funtori_complessi();
  // test_funtori_assurdi();
  // test_classe_non_trivial();
  return 0;
}
