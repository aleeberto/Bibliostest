#include "cd.h"
using std::string;

Cd::Cd(string titolo, int anno, string immagine, string artista, int numTracce, int durataMedTracce) : 
    Media(titolo, anno, immagine), artista(artista), numTracce(numTracce), durataMedTracce(durataMedTracce){}

string Cd::getArtista() const{
    return artista;
}

int Cd::getNumTracce() const{
    return numTracce;
}

int Cd::getDurataMedTracce() const{
    return durataMedTracce;
}

void Cd::setArtista(const string &updArtista){
    artista = updArtista;
}

void Cd::setNumTracce(const int &updNumTracce){
    numTracce = updNumTracce;
}

void Cd::setDurataMedTracce(const int &updDurataMedTracce){
    durataMedTracce = updDurataMedTracce;
}