#include "mainwindowclientbookencoder.h"
#include "ui_mainwindowclientbookencoder.h"
#include <signal.h>
#include "../Librairies/socket.h"
#include "../Protocole/Prot_OBEP.h"
#include "unistd.h"
#include <QInputDialog>
#include <QMessageBox>
#include <iostream> 
#include <sstream>
#include <iomanip>//pour setprecision
using namespace std;
string portServeur="8080";
string ipServeur="192.168.139.88";

int sClient=0;
int IdBook=1;
string IdEmployeeConnected;
void HandlerSIGINT(int s) ;


MainWindowClientBookEncoder::MainWindowClientBookEncoder(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindowClientBookEncoder)
{
    ui->setupUi(this);
    //::close(2);

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
    if(OBEP_OpClient((char*)requette.c_str(),reponse,sClient) == -1)
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
    if(OBEP_OpClient((char*)requette.c_str(),reponse,sClient) == -1)
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
    
    /*requette ="GET_BOOKS#";
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
            string id, titre, auteur, sujet, isbn, pages, annee, prix, stock;
            getline(is2, id, ';');
            getline(is2, titre, ';');
            getline(is2, auteur, ';');
            getline(is2, sujet, ';');
            getline(is2, isbn, ';');
            getline(is2, pages, ';');
            getline(is2, annee, ';');
            getline(is2, prix, ';');
            getline(is2, stock, '\n');

            addTupleTableBooks(stoi(id),titre,auteur,sujet,isbn,stoi(pages),stoi(annee),stof(prix),stoi(stock));
        }
    }*/
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
    int res=OBEP_OpClient((char*)(requette.c_str()), resultat,sClient);
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
    int res=OBEP_OpClient((char*)(requette.c_str()), resultat,sClient);
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
    if(OBEP_OpClient((char*)requette.c_str(),resultat,sClient)==0)
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
    if(OBEP_OpClient((char*)requette.c_str(),resultat,sClient)==0)
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
    if(OBEP_OpClient((char*)requette.c_str(),resultat,sClient)==0)
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
    int res=OBEP_OpClient((char*)requette.c_str(),resultat,sClient);
    if(res==0)
    {
        this->dialogMessage("Login",resultat);
        this->loginOk();
        printf("Code retour: %d\n Resultat Message: %s\n", res,resultat.c_str());

        //rechere tous les livres encodés par l'utilisateur
        /*requette = "GETID_EMPLOYEE#"+login;
        string employeeId;
        if(OBEP_Op((char*)requette.c_str(),employeeId)==0)
        {
            istringstream is(employeeId);
            string id;
            getline(is, id, '\n');
            IdEmployeeConnected = id;
            requette="GET_ENCODEDBOOKSBYEMPLOYEE#"+id;
            string encodedBooksId;
            if(OBEP_Op((char*)requette.c_str(),encodedBooksId)==0)
            {
                istringstream is(encodedBooksId);
                string ligne;
                while (getline(is, ligne, '\n'))
                {
                    istringstream is2(ligne);
                
                    string id, employee_id, book_id, date;
                    getline(is2, id, ';'); //ignore l'id
                    getline(is2, employee_id, ';'); //ignore l'employee_id
                    getline(is2, book_id, ';');
                    getline(is2, date, '\n'); //ignore la date

                    string allBooks;
                    requette = "GET_BOOKBYID#"+book_id;
                    if(OBEP_Op((char*)requette.c_str(),allBooks)==0)
                    {
                        istringstream is3(allBooks);
                        string ligne;
                        string id, author, subject, title, isbn, pageCount, publishYear, price, stockQuantity;
                        
                        while (getline(is3, ligne, '\n'))
                        {
                            istringstream is4(ligne);
                            string id, titre, auteur, sujet, isbn, pages, annee, prix, stock;
                            getline(is4, id, ';');
                            getline(is4, titre, ';');
                            getline(is4, auteur, ';');
                            getline(is4, sujet, ';');
                            getline(is4, isbn, ';');
                            getline(is4, pages, ';');
                            getline(is4, annee, ';');
                            getline(is4, prix, ';');
                            getline(is4, stock, '\n');

                            addTupleTableBooks(IdBook,titre,auteur,sujet,isbn,stoi(pages),stoi(annee),stof(prix),stoi(stock));
                            IdBook++;

                        }
                        
                    }
                    else
                    {
                        dialogError("Erreur",allBooks);
                    
                    }
                }
                
            }
            else
            {
                dialogError("Erreur",encodedBooksId);
            }
        }*/
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
    int res=OBEP_OpClient((char*)"LOGOUT#",resultat,sClient);
    if(res==0)
    {
        this->dialogMessage("Logout",resultat);
        this->logoutOk();
        //delete liste des livres
        this->clearTableBooks();

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
    int res=OBEP_OpClient((char*)requette.c_str(),resultat,sClient);
    if(res==0)
    {
        this->dialogMessage("Succès",resultat);
        ::close(sClient);
        QApplication::exit(0);
    }
    else
    {
        this->dialogError("Erreur",resultat);
    }
}


void HandlerSIGINT(int s) 
{
    printf("\nArret du client.\n");
    string resultat;
    string requette = "LOGOUT#";
    int res=OBEP_OpClient((char*)requette.c_str(),resultat,sClient);
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

