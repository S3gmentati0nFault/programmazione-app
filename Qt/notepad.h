#ifndef NOTEPAD_H
#define NOTEPAD_H

#include "fetcherdialog.h"
#include <QMainWindow>
#include <QFileDialog>
#include <QFile>
#include <QMessageBox>
#include <QTextDocument>
#include <QTextCursor>
#include <QTextCharFormat>
#include <QTextStream>
#include <iostream>

QT_BEGIN_NAMESPACE
namespace Ui { class Notepad; }
QT_END_NAMESPACE

class Notepad : public QMainWindow
{
    Q_OBJECT

public:
    Notepad(QWidget *parent = nullptr);
    ~Notepad();

//Sezione di definizione per segnali
signals:
    //Ho definito questo segnale per far sapere alla finestra di dialogo che
    //l'input non andava bene (a.k.a. stringa nulla o stringa vuota), si
    //sarebbe potuto fare un controllo, con la stessa efficacia, all'interno
    //della dialog.
    void badInsertion();
    //Questo segnale e' quello che comunica alla finestra di dialogo che cio'
    //che si sta cercando non e' stato trovato
    void fetcherFailure();
    //Utilizzo questo segnale per resettare la ui, e' un modo piu' semplice di
    //andare a resettarla in tutti gli specifici casi.
    void resetRequest();

//Sezione di definizione di slot pubblici
public slots:
    //Ho definito questo slot per consentire la comunicazione della parola e
    //delle impostazioni (match case) da parte della finestra di dialogo
    void catchPhrase(bool matchCase, QString word);

//Sezione di definizione di slot privati
private slots:
    //slot per l'azione New
    void on_actionNew_triggered();
    //slot per l'azione Open
    void on_actionOpen_triggered();
    //slot per l'azione Save
    void on_actionSave_triggered();
    //slot per l'azione Save as
    void on_actionSave_As_triggered();
    //slot per l'azione di ricerca
    void on_actionFetcher_triggered();

    void cleaningHandler();

private:
    //Variabili di istanza
    Ui::Notepad *ui;
    //utilizzo _fileName per salvare il nome del file corrente
    QString _fileName;
    //mantengo un puntatore a un'istanza della finestra di dialogo di ricerca
    //salvato cosi da poter richiamare all'occorrenza la finestra tramite
    //connect
    fetcherDialog *_fetcher;
    //una variabile che ho deciso di utilizzare per rendere leggermente piu'
    //efficace il tutto
    bool _changedBefore;

    //funzioni private
    //La funzione save() si occupera' delle operazioni di salvataggio su file.
    //L'ho creata per evitare di avere codice replicato tra save e save as
    void save();
    //La funzione cleaner() si occupera' di ripulire la finestra di testo dalle
    //evidenziazioni
    void cleaner();
};
#endif // NOTEPAD_H
