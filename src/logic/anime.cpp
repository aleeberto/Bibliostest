#include "anime.h"
using std::string;

Anime::Anime(string titolo, int anno, string immagine, int numEpisodi, int numStagioni, int durataMediaEp, bool inCorso, string disegnatore, string studioAnimazione) :
    Series(titolo, anno, immagine, numEpisodi, numStagioni, durataMediaEp, inCorso), disegnatore(disegnatore), studioAnimazione(studioAnimazione){}

string Anime::getDisegnatore() const{
    return disegnatore;
}

string Anime::getStudioAnimazione() const{
    return studioAnimazione;
}

void Anime::setDisegnatore(const string &updDisegnatore){
    disegnatore = updDisegnatore;
}

void Anime::setStudioAnimazione(const string &updStudioAnimazione){
    studioAnimazione = updStudioAnimazione;
}