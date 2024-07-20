#ifndef FETCHERDIALOG_H
#define FETCHERDIALOG_H

#include <QDialog>

namespace Ui {
class fetcherDialog;
}

class fetcherDialog : public QDialog
{
    Q_OBJECT

public:
    explicit fetcherDialog(QWidget *parent = nullptr);
    ~fetcherDialog();

//Sezione di definizione dei segnali
signals:
    //Questo e' il segnale che utilizzo per mandare la stringa e le opzioni
    //dalla finestra di dialogo alla mainwindow
    void sendTextToFecth(bool, QString);
    //Questo segnale consente alla finestra di dialogo di comunicare la
    //necessita' di ripulire il text edit presente nella mainwindow. Il
    //segnale e' utilizzato in fase di chiusura
    void cleaningRequest();

//Sezione di definizione degli slot privati
private slots:
    //slot di gestione della find
    void on_findPushButton_clicked();
    //handler per l'inserzione di stringhe cattive (null string e void string)
    //l'ho fatto per creare una separazione con il fallimento perche' la parola
    //non e' stata trovata
    void badInsertionHandler();
    //handler per il fallimento della ricerca
    void failureHandler();
    //handler per il reset della finestra di dialogo quando si vuole
    //reinserire qualcosa
    void uiReset();
    //slot di gestione della chiusura
    void on_closePushButton_clicked();

private:
    Ui::fetcherDialog *ui;

    //funzioni private
    //La funzione che resetta la finestra di dialogo
    void reset();
};

#endif // FETCHERDIALOG_H
