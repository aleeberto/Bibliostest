#ifndef ANIME_HEADER
#define ANIME_HEADER
#include "series.h"
using std::string;

class Anime : public Series{
    private:
        string disegnatore;
        string studioAnimazione;

    public:
        Anime(string titolo, int anno, string immagine, int numEpisodi, int numStagioni, int durataMediaEp, bool inCorso, string disegnatore, string studioAnimazione);
        virtual ~Anime() = default;
    
        string getDisegnatore() const;
        string getStudioAnimazione() const;
        
        void setDisegnatore(const string &updDisegnatore);
        void setStudioAnimazione(const string &updStudioAnimazione);
};

#endif