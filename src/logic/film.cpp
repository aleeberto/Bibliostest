#include "film.h"
using std::string;

Film::Film(string titolo, int anno, string immagine, string regista, string attoreProtagonista, int durata) : 
    Media(titolo, anno, immagine), regista(regista), attoreProtagonista(attoreProtagonista), durata(durata){}

string Film::getRegista() const{
    return regista;
}

string Film::getAttoreProtagonista() const{
    return attoreProtagonista;
}

int Film::getDurata() const{
    return durata;
}


void Film::setRegista(const string &updRegista){
    regista = updRegista;
}

void Film::setAttoreProtagonista(const string &updAttoreProtagonista){
    attoreProtagonista = updAttoreProtagonista;
}

void Film::setDurata(const int &updDurata){
    durata = updDurata;
}