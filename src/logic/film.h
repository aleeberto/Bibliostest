#ifndef FILM_HEADER
#define FILM_HEADER
#include "media.h"
using std::string;

class Film : public Media{
    private:
        string regista;
        string attoreProtagonista;
        int durata;
    
    public:
        Film(string titolo, int anno, string immagine, string regista, string attoreProtagonista, int durata);
        virtual ~Film() = default;

        string getRegista() const;
        string getAttoreProtagonista() const;
        int getDurata() const;

        void setRegista(const string &updRegista);
        void setAttoreProtagonista(const string &updAttoreProtagonista);
        void setDurata(const int &durata);
};

#endif