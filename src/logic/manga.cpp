#include "manga.h"
using std::string;

Manga::Manga(string titolo, int anno, string immagine, string scrittore, string illustratore, int numLibri, bool concluso) :
    Cartaceo(titolo, anno, immagine, scrittore), illustratore(illustratore), numLibri(numLibri), concluso(concluso){}

string Manga::getIllustratore() const {
    return illustratore;
}

int Manga::getNumLibri() const {
    return numLibri;
}

bool Manga::getConcluso() const {
    return concluso;
}

void Manga::setIllustratore(const string &updIllustratore) {
    illustratore = updIllustratore;
}

void Manga::setNumLibri(const int &updNumLibri) {
    numLibri = updNumLibri;
}

void Manga::setConcluso(const bool &updConcluso) {
    concluso = updConcluso;
}