#include "notepad.h"
#include "ui_notepad.h"


//Costruttore di default
Notepad::Notepad(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Notepad)
{
    ui->setupUi(this);
    //All'interno del costruttore ho deciso di settare una dimensione minima per rispettare un
    //aspetto 16:9 che consenta una corretta visualizzazione del campo di scrittura e dei vari menu
    this->setMinimumSize(QSize(640, 360));
    //Setto il plain text come central widget in modo che la sua dimensione scali correttamente con
    //la finestra
    this->setCentralWidget(ui->textEdit);
    //Setto le variabili di istanza
    _fileName = "";
    _fetcher = new fetcherDialog(this);
    _changedBefore = false;
    //Aggiungo una connect al costruttore che mi consente di ripulire la schermata di testo quando
    //La dialog viene chiusa
    connect(_fetcher, SIGNAL(cleaningRequest()), this, SLOT(cleaningHandler()));
}

//Distruttore di default
Notepad::~Notepad()
{
    delete ui;
    delete _fetcher;
}

//Definizione dello slot per l'action new.
//faccio il salvataggio solamente nal caso in cui il nome del file non sia vuoto, altrimenti
//le modifiche saranno perse.
//cancello il fileName e ripulisco la box di testo.
void Notepad::on_actionNew_triggered()
{
    if(_fileName != ""){

        save();
    }
    _fileName = "";
    ui->textEdit->clear();
}

//Definizione dello slot per l'action open.
//Ho utilizzato la libreria QFileDialog messa a disposizione da qt per richiamare direttamente le
//interfacce del sistema operativo per la selezione e apertura del file presente nel file system.
//Se la open fallisce (open in sola lettura) viene mostrato un QMessageBox di errore e poi si
//ritorna al chiamante, quindi nulla di quello che e' presente nella mainwindow viene modificato.
//Altrimenti carico il file nello spazio di testo riga per riga.
void Notepad::on_actionOpen_triggered()
{
    QString tmp = _fileName;
    _fileName = QFileDialog::getOpenFileName(this, "open");
    QFile file(_fileName);
    if(!file.open(QFile::ReadOnly | QFile::Text)){

        QMessageBox::warning(this, "Errore", "Problema durante l'apertura del file: " + file.errorString());
        _fileName = tmp;
        return;
    }
    QTextStream in(&file);
    ui->textEdit->clear();
    while(!in.atEnd()){

        QString line = in.readLine();
        ui->textEdit->appendPlainText(line);
    }
   file.close();
}

//Definizione dello slot per l'azione save.
//Questo slot non fa nulla se non, settare il nome del file se questo e' vuoto e successivamente
//chiama la save
void Notepad::on_actionSave_triggered()
{
    if(_fileName == ""){
        //Anche in questo caso, come per la open, ho utilizzato le QFileDialog offerte da qt
        _fileName = QFileDialog::getSaveFileName(this, "save as");
     }
    save();
}

//Definizione dello slot per l'azione save.
//Simile allo slot per la save solo che il nome del file viene richiesto forzatamente all'utente.
void Notepad::on_actionSave_As_triggered()
{
    _fileName = QFileDialog::getSaveFileName(this, "save as");
    save();
}

//Funzione privata save
//Questa funzione privata esegue una serie di operazioni simili alla open.
//L'unica cosa che deve fare e' provare ad aprire il file, se il tentativo ha successo allora
//viene scritto il contenuto del testo dentro un file txt, altrimenti viene prodotta una
//QMessageBox che comunica all'utente l'errore.
void Notepad::save(){
    QFile file(_fileName);
    if(!file.open(QFile::WriteOnly | QFile::Text)){

        QMessageBox::warning(this, "Errore", "Non sono riuscito a salvare il file: " + file.errorString());
        return;
    }
    QTextStream out(&file);
    QString plainText = ui->textEdit->toPlainText();
    out << plainText;
    file.close();
}

//Definizione dello slot per l'operazione di ricerca
//Lo slot si occupa di mostrare la finestra di dialogo per compiere la ricerca.
//Nella mia implementazione ho volutamente deciso di implementare una finestra di dialogo non
//modale perche', nel caso di testi molto lunghi, la finestra non modale non consente di scrollare
//verso il basso nella mainwindow; rendendo di fatto la ricerca un'operazione inutile.
//Al suo interno vi e' poi la connect che viene richiamata quando la finestra manda il testo
//scritto, il segnale inviato triggera lo slot catchPhrase che si occupera' dell'effettiva ricerca.
//La finestra di dialogo viene ripulita quando eseguo una nuova ricerca e quando esco dalla
//finestra di dialogo.
void Notepad::on_actionFetcher_triggered()
{
    //emetto questo segnale che consente di resettare la finestra di dialogo per evitare di mantenere
    //i valori di vecchie ricerche
    emit resetRequest();
    _fetcher->show();
    connect(_fetcher, SIGNAL(sendTextToFecth(bool,QString)), this, SLOT(catchPhrase(bool,QString)));
    ui->textEdit->moveCursor(QTextCursor::End);
    connect(ui->textEdit, SIGNAL(cursorPositionChanged()), this, SLOT(cleaningHandler()));
}

//Definizione dello slot catchPhrase
//Tale slot impiega i cursori per andare a cercare la parola word all'interno del documento
void Notepad::catchPhrase(bool matchCase, QString word){
    //Costruisco un puntatore a documento che mi serve per lavorare con i cursori definiti nella
    //libreria Qt
    QTextDocument *document = ui->textEdit->document();
    //Inizializzo un booleano che si occupera' di farmi sapere la risposta
    bool found = false;

    //Gestisco il caso in cui la parola non sia adeguata e rimbalzo al chiamante con una emit di
    //badInsertion
    if(word.isEmpty() || word.isNull()){

        emit badInsertion();
    }
    //Definisco i cursori
    QTextCursor highlighter(document);
    QTextCursor cursor(document);
    cursor.beginEditBlock();
    //Definisco una variabile che mi definisce il formato del carattere, cosi' da poter
    //effettivamente evidenziare il testo
    QTextCharFormat normal(highlighter.charFormat());
    QTextCharFormat highlighted = normal;
    //Questo if procede alla pulizia dell'editor di testo.
    //In una guida Qt proponevano l'utilizzo della undo dei cursori ma questo provocava dei
    //problemi qualora io andassi a fare una ricerca, dopodiche' modificassi il testo e
    //successivamente facessi una nuova ricerca (la undo eliminava le ultime modifiche al documento)
    if(_changedBefore){

        cleaner();
        highlighter.movePosition(QTextCursor::Start, QTextCursor::KeepAnchor);
    }
    highlighted.setBackground(Qt::yellow);
    //Faccio un ciclo di ricerca
    while(!highlighter.isNull() && !highlighter.atEnd()){
        //Setto le opzioni della find sulla base delle opzioni che mi vengono passate dalla dialog
        if(matchCase){

            highlighter = document->find(word, highlighter, {QTextDocument::FindCaseSensitively});
        }
        else{

            highlighter = document->find(word, highlighter);
        }
        if(!highlighter.isNull()){

            //Se qualcosa e' stato trovato setto il found a true, _changedBefore a true perche'
            //sono certo che qualcosa sia stato evidenziato
            found = true;
            _changedBefore = true;
            highlighter.mergeCharFormat(highlighted);
        }
    }
    cursor.endEditBlock();
    //Se non sono riuscito a trovare nulla faccio una emit di fetcherFailure()
    if(!found){

        emit fetcherFailure();
    }
}

//Il cleaingHandler non fa altro che chiamare il cleaner
//La pulizia della zona di editing del testo avviene quando:
//1) Cerco qualcosa di nuovo
//2) Chiudo la finestra di dialogo
//3) Quando cambia la posizione del cursore (al termine della ricerca il cursore viene messo in ultima
//posizione all'interno della text box
//Ho optato di aggiungere anche la ultima clean perche' ho scovato una situazione davvero bruttina
//in cui, se si fa una ricerca, e successivamente si va a scrivere esattamente dopo un'evidenziazione
//la sezione evidenziata si prolungava
void Notepad::cleaningHandler(){
    cleaner();
}

//La funzione si occupa di pulire lo spazio di testo nella mainwindow, o per prepararlo all'
//operazione di ricerca o per tornare a scrivere
void Notepad::cleaner(){
    //Prima di fare qualsiasi cosa mi fermo un attimo per controllare se e' effettivamente
    //necessario andare a ripulire tutto
    if(!_changedBefore){

        return;
    }
    //Definisco le variabili che mi sono necessarie per andare a ripulire il testo
    QTextDocument *document = ui->textEdit->document();
    QTextCursor highlighter(document);
    QTextCursor cursor(document);
    //apro la modifica
    cursor.beginEditBlock();
    QTextCharFormat normal(highlighter.charFormat());
    QTextCharFormat highlighted = normal;

    //setto il colore di highlighted a trasparente
    highlighted.setBackground(Qt::transparent);
    //sposto highlighter alla fine del documento e metto un'ancora in modo tale da selezionare
    //tutto il testo presente nella finestra
    highlighter.movePosition(QTextCursor::End, QTextCursor::KeepAnchor);
    //eseguo la merge tra cio' che e' selezionato e il formato di highlighted
    highlighter.mergeCharFormat(highlighted);
    //Quando eseguo quest'operazione cambio il flag a false perche' il testo e' stato ripulito
    _changedBefore = false;
    //chiudo la modifica
    cursor.endEditBlock();
}

