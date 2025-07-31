#ifndef MANGA_HEADER
#define MANGA_HEADER
#include "cartaceo.h"
using std::string;

class Manga : public Cartaceo{
    private:
        string illustratore;
        int numLibri;
        bool concluso;
        
    public:
        Manga(string titolo, int anno, string immagine, string scrittore, string illustratore, int numLibri, bool concluso);
        virtual ~Manga() = default;

        string getIllustratore() const;
        int getNumLibri() const;
        bool getConcluso() const;
        
        void setIllustratore(const string &updIllustartore);
        void setNumLibri(const int &upddNumLibri);
        void setConcluso(const bool &updConcluso);
};

#endif