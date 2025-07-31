#include "cartaceo.h"
using std::string;

Cartaceo::Cartaceo(string titolo, int anno, string immagine, string scrittore) : 
    Media(titolo, anno, immagine), scrittore(scrittore){}

string Cartaceo::getScrittore() const{
    return scrittore;
}

void Cartaceo::setScrittore(const string &updScrittore){
    scrittore = updScrittore;
}