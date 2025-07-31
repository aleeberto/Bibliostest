#include "serieTv.h"
using std::string;

SerieTv::SerieTv(string titolo, int anno, string immagine, int numEpisodi, int numStagioni, int durataMediaEp, bool inCorso, string ideatore, string casaProduttrice) :
    Series(titolo, anno, immagine, numEpisodi, numStagioni, durataMediaEp, inCorso), ideatore(ideatore), casaProduttrice(casaProduttrice){}

string SerieTv::getIdeatore() const{
    return ideatore;
}

string SerieTv::getCasaProduttrice() const{
    return casaProduttrice;
}

void SerieTv::setIdeatore(const string &updIdeatore){
    ideatore = updIdeatore;
}

void SerieTv::setCasaProduttrice(const string &updCasaProduttrice){
    casaProduttrice = updCasaProduttrice;
}