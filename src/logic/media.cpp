#include "media.h"
using std::string;

Media::Media(string titolo, int anno, string immagine): titolo(titolo), anno(anno), immagine(immagine){}

string Media::getTitolo() const{
    return titolo;
}

int Media::getAnno() const{
    return anno;
}

string Media::getImmagine() const{
    return immagine;
}

void Media::setTitolo(const string &updTitolo){
    titolo = updTitolo;
}

void Media::setAnno(const int &updAnno){
    anno = updAnno;
}

void Media::setImmagine(const string &updImmagine){
    immagine = updImmagine;
}