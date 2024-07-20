#include "fetcherdialog.h"
#include "ui_fetcherdialog.h"
#include <QColor>
#include <QColorDialog>

fetcherDialog::fetcherDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::fetcherDialog)
{
    ui->setupUi(this);
    //La finestra di dialogo non puo' essere ridimensionata, perche' ha poco senso
    this->setFixedSize(QSize(303, 159));
    //Inserisco due connect che attendono l'arrivo dei segnali definiti all'interno dell'header
    connect(this->parent(), SIGNAL(badInsertion()), this, SLOT(badInsertionHandler()));
    connect(this->parent(), SIGNAL(fetcherFailure()), this, SLOT(failureHandler()));
    //Inserisco una connect che attende l'arrivo del segnale di reset per la chiamata alla finestra
    connect(this->parent(), SIGNAL(resetRequest()), this, SLOT(uiReset()));
}

fetcherDialog::~fetcherDialog()
{
    delete ui;
}

//Definizione dello slot per la find
//Questo slot non fa nulla se non fare l'emit del segnale che inoltra i dati presenti nella
//finestra di dialogo e ripulisce la label nella finestra così da evitare messaggi fuorvianti
void fetcherDialog::on_findPushButton_clicked()
{
    ui->messageLabel->clear();
    emit sendTextToFecth(ui->matchCaseBox->isChecked(), ui->lineEdit->text());
}

//Definizione dello slot di reset
//Questo slot riporta la dialog in uno stato di default con plain text pronto a ricevere e il box
//non checkato
void fetcherDialog::uiReset(){
    reset();
}

//Definizione dell'handler per le bad insertions
//La laber presente al di sotto degli elementi della dialog mostrera' come effetto il testo in
//rosso bad text input for search.
void fetcherDialog::badInsertionHandler(){
    ui->messageLabel->setStyleSheet("QLabel { color : red; }");
    ui->messageLabel->setText("Bad text input for search");
}

//Definizione dell'handler per le ricerche fallite
//Cosa molto simile all'handler soprastante
void fetcherDialog::failureHandler(){
    ui->messageLabel->setStyleSheet("QLabel { color : red; }");
    ui->messageLabel->setText("Word not found");
}

//Handler per la pressione del bottone di chiusura
//Questo handler lancia un segnale di cleaningRequest, che verra' catturato dalla mainwindow e
//triggherera' la pulizia (cancellazione di eventuali evidenziazioni) del text edit della main
//window. E porta poi alla chiusura della dialog
void fetcherDialog::on_closePushButton_clicked()
{
    emit cleaningRequest();
    this->close();
}

//Funzione di reset per la finestra di dialogo
void fetcherDialog::reset() {
    ui->messageLabel->setText("");
    ui->matchCaseBox->setCheckState(Qt::Unchecked);
    ui->lineEdit->setText("");
}

