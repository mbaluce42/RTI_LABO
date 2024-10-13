#include "mainwindowclientbookencoder.h"
#include "ui_mainwindowclientbookencoder.h"
#include <signal.h>
#include "../Librairies/socket.h"
#include "unistd.h"
#include <QInputDialog>
#include <QMessageBox>
#include <iostream> 
#include <sstream>
#include <iomanip>//pour setprecision
using namespace std;
string portServeur="50000";
string ipServeur="127.0.0.1";

int sClient=0;
int IdBook=1;

void HandlerSIGINT(int s) ;

//***** Gestion du protocole SMOP ***********************************

//*******************************************************************
int OBEP_Op(char* requete, string& resultat);
   
//***** Echange de données entre client et serveur ******************
int Echange(char *requete, char *reponse);



MainWindowClientBookEncoder::MainWindowClientBookEncoder(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindowClientBookEncoder)
{
    ui->setupUi(this);
    ::close(2);

    //this->setFixedSize(1068, 301);

    // Configuration de la table des employes (Personnel Garage)
    ui->tableWidgetEncodedBooks->setColumnCount(9);
    ui->tableWidgetEncodedBooks->setRowCount(0);
    QStringList labelsTableEmployes;
    labelsTableEmployes << "Id" << "Titre" << "Auteur" << "Sujet" << "ISBN" << "Pages" << "Année" << "Prix" << "Stock";
    ui->tableWidgetEncodedBooks->setHorizontalHeaderLabels(labelsTableEmployes);
    ui->tableWidgetEncodedBooks->horizontalHeader()->setVisible(true);
    ui->tableWidgetEncodedBooks->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidgetEncodedBooks->verticalHeader()->setVisible(false);
    ui->tableWidgetEncodedBooks->horizontalHeader()->setStyleSheet("background-color: lightyellow");
    int columnWidths[] = {35, 250, 200, 200, 150, 50, 50, 50, 40};
    for (int col = 0; col < 9; ++col)
        ui->tableWidgetEncodedBooks->setColumnWidth(col, columnWidths[col]);

    this->logoutOk();

    // Exemples d'utilisation (à supprimer)
    /*this->addTupleTableBooks(1,"Les Thanatonautes","Bernard Werber","Science-Fiction","978-2253139225",505,1999,9.7f,3);
    this->addTupleTableBooks(6,"Dune","Frank Herbert","Science-Fiction","978-2266320481",929,2021,11.95f,13);
    this->addTupleTableBooks(13,"Le silence des agneaux","Thomas Harris","Thriller","978-2266208949",377,2015,7.7f,17);

    this->addComboBoxAuthors("Bernard Werber");
    this->addComboBoxAuthors("Dan Brown");

    this->addComboBoxSubjects("Roman");
    this->addComboBoxSubjects("Science-fiction");*/


    // Armement des signaux
    struct sigaction A;
    A.sa_flags = 0;
    sigemptyset(&A.sa_mask);
    A.sa_handler = HandlerSIGINT;
    if (sigaction(SIGINT, &A, NULL) == -1)
    {
        perror("Erreur de sigaction");
        printf("Arret du client.\n");
        exit(1);
    }
    // Connexion sur le serveur

    sClient = ClientSocket((char*)ipServeur.c_str(), (char*)portServeur.c_str());

    if (sClient == -1)
    {
        printf("Erreur de ClientSocket\n");
        perror("Erreur de ClientSocket");
        exit(1);
    }
    printf("Connecte sur le serveur.\n");

    string requette ="GET_AUTHORS#";
    string reponse;
    if(OBEP_Op((char*)requette.c_str(),reponse) == -1)
    {
        this->dialogError("Erreur",reponse);
        exit(1);
    }
    else
    {

        istringstream is(reponse);
        string ligne;
        
        while (getline(is, ligne, '\n'))
        {
            istringstream is2(ligne);
            string id, nom, prenom, date;
            getline(is2, id, ';');
            getline(is2, nom, ';');
            getline(is2, prenom, ';');
            getline(is2, date, '\n');

            addComboBoxAuthors(prenom+" "+nom);
        }
    }


    requette ="GET_SUBJECTS#";
    if(OBEP_Op((char*)requette.c_str(),reponse) == -1)
    {
        this->dialogError("Erreur",reponse);
        exit(1);
    }
    else
    {

        istringstream is(reponse);
        string ligne;
        printf("Reponse: %s\n", reponse.c_str());
        
        while (getline(is, ligne, '\n'))
        {
            istringstream is2(ligne);
            string id, nom;
            getline(is2, id, ';');
            getline(is2, nom, '\n');

            addComboBoxSubjects(nom);
        }
    }
    
}

MainWindowClientBookEncoder::~MainWindowClientBookEncoder() {
    delete ui;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///// Fonctions utiles Table des livres encodés (ne pas modifier) ////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MainWindowClientBookEncoder::addTupleTableBooks(int id,
                                                     string title,
                                                     string author,
                                                     string subject,
                                                     string isbn,
                                                     int pageCount,
                                                     int publishYear,
                                                     float price,
                                                     int stockQuantity)
{
    int nb = ui->tableWidgetEncodedBooks->rowCount();
    nb++;
    ui->tableWidgetEncodedBooks->setRowCount(nb);
    ui->tableWidgetEncodedBooks->setRowHeight(nb-1,10);

    // id
    QTableWidgetItem *item = new QTableWidgetItem;
    item->setFlags(item->flags() & ~Qt::ItemIsSelectable);
    item->setTextAlignment(Qt::AlignCenter);
    item->setText(QString::number(id));
    ui->tableWidgetEncodedBooks->setItem(nb-1,0,item);

    // title
    item = new QTableWidgetItem;
    item->setFlags(item->flags() & ~Qt::ItemIsSelectable);
    item->setText(QString::fromStdString(title));
    ui->tableWidgetEncodedBooks->setItem(nb-1,1,item);

    // author
    item = new QTableWidgetItem;
    item->setFlags(item->flags() & ~Qt::ItemIsSelectable);
    item->setTextAlignment(Qt::AlignCenter);
    item->setText(QString::fromStdString(author));
    ui->tableWidgetEncodedBooks->setItem(nb-1,2,item);

    // subject
    item = new QTableWidgetItem;
    item->setFlags(item->flags() & ~Qt::ItemIsSelectable);
    item->setTextAlignment(Qt::AlignCenter);
    item->setText(QString::fromStdString(subject));
    ui->tableWidgetEncodedBooks->setItem(nb-1,3,item);

    // isbn
    item = new QTableWidgetItem;
    item->setFlags(item->flags() & ~Qt::ItemIsSelectable);
    item->setTextAlignment(Qt::AlignCenter);
    item->setText(QString::fromStdString(isbn));
    ui->tableWidgetEncodedBooks->setItem(nb-1,4,item);

    // pageCount
    item = new QTableWidgetItem;
    item->setFlags(item->flags() & ~Qt::ItemIsSelectable);
    item->setTextAlignment(Qt::AlignCenter);
    item->setText(QString::number(pageCount));
    ui->tableWidgetEncodedBooks->setItem(nb-1,5,item);

    // publishYear
    item = new QTableWidgetItem;
    item->setFlags(item->flags() & ~Qt::ItemIsSelectable);
    item->setTextAlignment(Qt::AlignCenter);
    item->setText(QString::number(publishYear));
    ui->tableWidgetEncodedBooks->setItem(nb-1,6,item);

    // price
    item = new QTableWidgetItem;
    item->setFlags(item->flags() & ~Qt::ItemIsSelectable);
    item->setTextAlignment(Qt::AlignCenter);
    item->setText(QString::number(price));
    ui->tableWidgetEncodedBooks->setItem(nb-1,7,item);

    // stockQuantity
    item = new QTableWidgetItem;
    item->setFlags(item->flags() & ~Qt::ItemIsSelectable);
    item->setTextAlignment(Qt::AlignCenter);
    item->setText(QString::number(stockQuantity));
    ui->tableWidgetEncodedBooks->setItem(nb-1,8,item);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MainWindowClientBookEncoder::clearTableBooks() {
    ui->tableWidgetEncodedBooks->setRowCount(0);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///// Fonctions utiles des comboboxes (ne pas modifier) //////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MainWindowClientBookEncoder::addComboBoxAuthors(string author){
    ui->comboBoxAuthors->addItem(QString::fromStdString(author));
}

string MainWindowClientBookEncoder::getSelectionAuthor() const {
    return ui->comboBoxAuthors->currentText().toStdString();
}

void MainWindowClientBookEncoder::clearComboBoxAuthors() {
    ui->comboBoxAuthors->clear();
}

void MainWindowClientBookEncoder::addComboBoxSubjects(string subject){
    ui->comboBoxSubjects->addItem(QString::fromStdString(subject));
}

string MainWindowClientBookEncoder::getSelectionSubject() const {
    return ui->comboBoxSubjects->currentText().toStdString();
}

void MainWindowClientBookEncoder::clearComboBoxSubjects() {
    ui->comboBoxSubjects->clear();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///// Fonction utiles de la fenêtre (ne pas modifier) ////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
string MainWindowClientBookEncoder::getTitle() const {
    return ui->lineEditTitle->text().toStdString();
}

string MainWindowClientBookEncoder::getIsbn() const {
    return ui->lineEditIsbn->text().toStdString();
}

int MainWindowClientBookEncoder::getPageCount() const {
    return ui->spinBoxPageCount->value();
}

float MainWindowClientBookEncoder::getPrice() const {
    return ui->doubleSpinBoxPrice->value();
}

int MainWindowClientBookEncoder::getPublishYear() const {
    return ui->spinBoxPublishYear->value();
}

int MainWindowClientBookEncoder::getStockQuantity() const {
    return ui->spinBoxStockQuantity->value();
}

void MainWindowClientBookEncoder::loginOk() {
    ui->pushButtonClear->setEnabled(true);
    ui->pushButtonAddBook->setEnabled(true);
    ui->pushButtonAddAuthor->setEnabled(true);
    ui->pushButtonAddSubject->setEnabled(true);
    ui->actionLogin->setEnabled(false);
    ui->actionLogout->setEnabled(true);
}

void MainWindowClientBookEncoder::logoutOk() {
    ui->pushButtonClear->setEnabled(false);
    ui->pushButtonAddBook->setEnabled(false);
    ui->pushButtonAddAuthor->setEnabled(false);
    ui->pushButtonAddSubject->setEnabled(false);
    ui->actionLogin->setEnabled(true);
    ui->actionLogout->setEnabled(false);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///// Fonctions permettant d'afficher des boites de dialogue (ne pas modifier) ///////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MainWindowClientBookEncoder::dialogMessage(const string& title,const string& message) {
   QMessageBox::information(this,QString::fromStdString(title),QString::fromStdString(message));
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MainWindowClientBookEncoder::dialogError(const string& title,const string& message) {
   QMessageBox::critical(this,QString::fromStdString(title),QString::fromStdString(message));
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
string MainWindowClientBookEncoder::dialogInputText(const string& title,const string& question) {
    return QInputDialog::getText(this,QString::fromStdString(title),QString::fromStdString(question)).toStdString();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
int MainWindowClientBookEncoder::dialogInputInt(const string& title,const string& question) {
    return QInputDialog::getInt(this,QString::fromStdString(title),QString::fromStdString(question));
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///// Fonctions gestion des boutons et items de menu (TO DO) /////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MainWindowClientBookEncoder::on_pushButtonAddAuthor_clicked() 
{
    printf("Ajout d'un auteur en cours...\n");

    string nom = this->dialogInputText("Nouvel auteur","Nom ?");
    string prenom = this->dialogInputText("Nouvel auteur","Prénom ?");
    string dateNaiss = this->dialogInputText("Nouvel auteur","Date de naissance (yyyy-mm-dd) ?");

    cout << "Nom de l'auteur : " << nom << endl;
    cout << "Prénom de l'auteur : " << prenom << endl;
    cout << "Date de naissance de l'auteur : " << dateNaiss << endl;

    nom[0]=toupper(nom[0]);
    prenom[0]=toupper(prenom[0]);

    string requette = "ADD_AUTHOR#"+nom+"#"+prenom+"#"+dateNaiss;
    string resultat;
    int res=OBEP_Op((char*)(requette.c_str()), resultat);
    if(res<0)
    {
        dialogError("Erreur",resultat);
    }
    else
    {
        dialogMessage("Succès",resultat);

        this->addComboBoxAuthors(prenom+" "+nom);
    }
}

void MainWindowClientBookEncoder::on_pushButtonAddSubject_clicked() 
{
    printf("Ajout d'un sujet en cours...\n");
    string nom;
    nom= this->dialogInputText("Nouveau sujet","Nom ?");
    cout << "Nom du sujet : " << nom << endl;


    string requette= "ADD_SUBJECT#"+nom;
    string resultat;
    int res=OBEP_Op((char*)(requette.c_str()), resultat);
    if(res<0)
    {
        dialogError("Erreur",resultat);
    }
    else
    {
        dialogMessage("Succès",resultat);
        this->addComboBoxSubjects(nom);
    }

}

void MainWindowClientBookEncoder::on_pushButtonAddBook_clicked() {
    cout << "title = " << this->getTitle() << endl;
    cout << "Isbn = " << this->getIsbn() << endl;
    cout << "PageCount = " << this->getPageCount() << endl;
    cout << "Price = " << this->getPrice() << endl;
    cout << "PublishYear = " << this->getPublishYear() << endl;
    cout << "Stock = " << this->getStockQuantity() << endl;

    cout << "selection auteur = " << this->getSelectionAuthor() << endl;
    cout << "selection sujet  = " << this->getSelectionSubject() << endl;
    string requette, resultat;
    string id_author, id_subject;

    string author = this->getSelectionAuthor(); //recup prenom et nom
    string subject = this->getSelectionSubject();//recup le nom
    string title = this->getTitle();
    string isbn = this->getIsbn();
    string pageCount = to_string(this->getPageCount());
    string price = to_string(this->getPrice());

    //remplace la virguile par un point et laise que 2 chiffres apres la virgule
    replace(price.begin(), price.end(), ',', '.');
    ostringstream oss; 
    oss<< fixed << setprecision(2) << price; // Formatage à 2 décimales
    price = oss.str();
    
    string publishYear = to_string(this->getPublishYear());
    string stockQuantity = to_string(this->getStockQuantity());

    requette = "GETID_AUTHOR#"+author;
    if(OBEP_Op((char*)requette.c_str(),resultat)==0)
    {
        istringstream is(resultat);
        string id;
        getline(is, id, '\n');
        id_author = id;
    }
    else
    {
        dialogError("Erreur",resultat);
        return;
    }
    

    requette = "GETID_SUBJECT#"+subject;
    if(OBEP_Op((char*)requette.c_str(),resultat)==0)
    {
        istringstream is(resultat);
        string id;
        getline(is, id, '\n');
        id_subject = id;
    }
    else
    {
        dialogError("Erreur",resultat);
        return;
    }

    requette = "ADD_BOOK#"+id_author+"#"+id_subject+"#"+title+"#"+isbn+"#"+pageCount+"#"+stockQuantity+"#"+price+"#"+publishYear;
    if(OBEP_Op((char*)requette.c_str(),resultat)==0)
    {
        dialogMessage("Succès",resultat);
        this->addTupleTableBooks(IdBook, title, author, subject, isbn, stoi(pageCount), stoi(publishYear), stof(price), stoi(stockQuantity));
        IdBook++;
    }
    else
    {
        dialogError("Erreur",resultat);
    }
}

void MainWindowClientBookEncoder::on_pushButtonClear_clicked() {
    ui->lineEditTitle->clear();
    ui->lineEditIsbn->clear();
    ui->spinBoxPageCount->setValue(0);
    ui->doubleSpinBoxPrice->setValue(0);
    ui->spinBoxPublishYear->setValue(0);
    ui->spinBoxStockQuantity->setValue(0);
}

void MainWindowClientBookEncoder::on_actionLogin_triggered() {
    string login = this->dialogInputText("Entrée en session","Login ?");
    string password = this->dialogInputText("Entrée en session","Password ?");

    printf("Tentative de login...\n");
    string resultat;
    string requette= "LOGIN#"+login+"#"+password;
    int res=OBEP_Op((char*)requette.c_str(),resultat);

    if(res==0)
    {
        this->dialogMessage("Login",resultat);
        this->loginOk();
        printf("Code retour: %d\n Resultat Message: %s\n", res,resultat.c_str());
    }
    else
    {
        this->dialogError("Erreur de login",resultat);
        printf("Code retour: %d\n Resultat Message: %s\n", res,resultat.c_str());
    }
    
}

void MainWindowClientBookEncoder::on_actionLogout_triggered() {
    
    string resultat;
    
    printf("Tentative de logout...\n");
    int res=OBEP_Op((char*)"LOGOUT#",resultat);
    if(res==0)
    {
        this->dialogMessage("Logout",resultat);
        this->logoutOk();
        printf("Code retour: %d\n Resultat Message: %s\n", res,resultat.c_str());
    }
    else
    {
        this->dialogError("Erreur de logout",resultat);
        printf("Code retour: %d\n Resultat Message: %s\n", res,resultat.c_str());
    }
}

void MainWindowClientBookEncoder::on_actionQuitter_triggered()
{
    
    string resultat;
    string requette = "LOGOUT#";
    int res=OBEP_Op((char*)requette.c_str(),resultat);
    if(res<0)
    {
        this->dialogError("Erreur",resultat);
    }
    else
    {
        this->dialogMessage("Succès",resultat);
        ::close(sClient);
        QApplication::exit(0);
    }
}


void HandlerSIGINT(int s) 
{
    printf("\nArret du client.\n");
    string resultat;
    string requette = "LOGOUT#";
    int res=OBEP_Op((char*)requette.c_str(),resultat);
    if(res<0)
    {
        printf("Erreur de logout: %s\n", resultat.c_str());
    }
    else
    {
        printf("Logout: %s\n", resultat.c_str());
        close(sClient);
        QApplication::exit(0);
    }
    close(sClient);
    exit(0);
}

//***** Gestion du protocole SMOP ***********************************
/*bool OBEP_Login(const char* user,const char* password)
{
    char requete[200], reponse[200];
    bool onContinue = true;
    // ***** Construction de la requete ********************* 
    sprintf(requete,"LOGIN#%s#%s",user,password);
    // ***** Envoi requete + réception réponse ************** 
    Echange(requete,reponse);
    // ***** Parsing de la réponse ************************** 
    char *ptr = strtok(reponse,"#"); // entête = LOGIN (normalement...) 
    ptr = strtok(NULL,"#"); // statut = ok ou ko
    if (strcmp(ptr, "ok") == 0 || strcmp(ptr, "OK") == 0)
        printf("Login OK.\n");
    else
    {
        ptr = strtok(NULL, "#"); // raison du ko
        printf("Erreur de login: %s\n", ptr);
        onContinue = false;
    }
    return onContinue;
}*/
//******************************************************************* 
/*void OBEP_Logout()
{
    char requete[200], reponse[200];
    int nbEcrits, nbLus;
    // ***** Construction de la requete ********************* 
    sprintf(requete,"LOGOUT");
    // ***** Envoi requete + réception réponse ************** 
    Echange(requete,reponse);
    // ***** Parsing de la réponse **************************
    // pas vraiment utile...
}*/

//*******************************************************************
int OBEP_Op(char* requete, string& resultat)
{
    char reponse[4048];
    int res;
    // *****Envoi requete + réception réponse **************
    res=Echange(requete, reponse);
    if(res<0)
    {
        return -1;
    }
    else
    {
        // *****Parsing de la réponse **************************
        istringstream is(reponse);
        string TypeRequete;
        getline(is, TypeRequete, '_'); // skip le type (add, get)
        if(TypeRequete == "ADD")
        {
            getline(is, TypeRequete, '#');//skip le type (Author, Subject, Book)

            string status;
            string message;
            getline(is, status, '#');//recupere le status de la requete
            getline(is, message, '\n'); //recupere le message de la requete
            resultat = message;
            if(status == "OK")
            {
                
            }
            else
            {
                return -1;
            }
        }
        else if(TypeRequete == "GET")
        {
            getline(is, TypeRequete, '#');//skip le type (Author, Subject, Book)
            string status;
            string messages;
            getline(is, status, '\n');//recupere le status de la requete
            getline(is, messages, '\n'); //recupere le message de la requete
            if(status == "OK")
            {
                resultat = messages + '\n';
                while (getline(is, messages, '\n'))
                {
                    resultat += messages;
                    resultat += '\n';
                }
                
            }
            else
            {
                
                istringstream is(status);
                getline(is, status, '#');//ignore le status

                getline(is, messages, '\n'); //recupere le message de la requete
                resultat = messages;
                return -1;
            }
        }
        else if(TypeRequete=="GETID")
        {
            getline(is, TypeRequete, '#');//skip le type (Author, Subject)
            string status;
            string messages;
            getline(is, status, '\n');//recupere le status de la requete
            getline(is, messages, '\n'); //recupere le message de la requete
            if(status == "OK")
            {
                resultat = messages;
            }
            else
            {
                istringstream is(status);
                getline(is, status, '#');//ignore le status

                getline(is, messages, '\n'); //recupere le message de la requete
                resultat = messages;
                return -1;
            }
        }
        else
        {
            istringstream is(reponse);
            string TypeRequete;
            getline(is, TypeRequete, '#'); // skip le type (Author, Subject, Book)
            
            if(TypeRequete == "LOGIN")
            {
                string status;
                getline(is, status, '#');
                string message;
                getline(is, message, '\n');
                resultat=message;
                if(status == "OK")
                {
                
                }
                else
                {
                    return -1;
                }
            }
            else if(TypeRequete == "LOGOUT")
            {
                string status;
                getline(is, status, '#');
                string message;
                getline(is, message, '\n');
                resultat=message;
                if(status == "OK")
                {
                
                }
                else
                {
                    return -1;
                }
            }
        }

    }

    printf("Reponse OBEP(return 0): %s\n", resultat.c_str());

    return 0;
}
//***** Echange de données entre client et serveur ******************
int Echange(char *requete, char *reponse)
{
    int nbEcrits, nbLus;
    // ***** Envoi de la requete ****************************
    if ((nbEcrits = SendSocket(sClient, requete, strlen(requete))) == -1)
    {
        perror("Erreur de Send");
        close(sClient);
        return -1;
    }
    // ***** Attente de la reponse **************************
    if ((nbLus = ReceiveSocket(sClient, reponse)) < 0)
    {
        perror("Erreur de Receive");
        close(sClient);
        return -2;
    }
    if (nbLus == 0)
    {
        printf("Serveur arrete, pas de reponse reçue...\n");
        close(sClient);
        return -3;
    }
    reponse[nbLus] = 0;

    return 0;
}