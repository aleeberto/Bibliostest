#ifndef CD_HEADER
#define CD_HEADER
#include "media.h"
using std::string;

class Cd : public Media{
    private:
        string artista;
        int numTracce;
        int durataMedTracce;
    
    public:
        Cd(string titolo, int anno, string immagine, string artista, int numTracce, int durataMedTracce);
        virtual ~Cd() = default;

        string getArtista() const;
        int getNumTracce() const;
        int getDurataMedTracce() const;
        
        void setArtista(const string &updArtista);
        void setNumTracce(const int &updNumTracce);
        void setDurataMedTracce(const int &updDurataMedTracce);
};

#endif