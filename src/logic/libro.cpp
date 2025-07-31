#include "libro.h"
using std::string;

Libro::Libro(string titolo, int anno, string immagine, string scrittore, int annoStampa, string casaEditrice) : 
    Cartaceo(titolo, anno, immagine, scrittore), annoStampa(annoStampa), casaEditrice(casaEditrice){}

int Libro::getAnnoStampa()const{
    return annoStampa;
}

string Libro::getCasaEditrice()const{
    return casaEditrice;
}

void Libro::setAnnoStampa(const int &updAnnoStampa){
    annoStampa = updAnnoStampa;
}

void Libro::setCasaEditrice(const string &updCasaEditrice){
    casaEditrice = updCasaEditrice;
}