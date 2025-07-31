#ifndef LIBRO_HEADER
#define LIBRO_HEADER
#include "cartaceo.h"
using std::string;

class Libro : public Cartaceo{
    private:
        int annoStampa;
        string casaEditrice;
    
    public:
        Libro(string titolo, int anno, string immagine, string scrittore, int annoStampa, string casaEditrice);
        virtual ~Libro() = default;

        int getAnnoStampa() const;
        string getCasaEditrice() const;

        void setAnnoStampa(const int &updAnnoStampa);
        void setCasaEditrice(const string &updCasaEditrice);
};

#endif