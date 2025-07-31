#ifndef SERIES_HEADER
#define SERIES_HEADER
#include "media.h"
using std::string;

class Series : public Media{
    private:
        int numEpisodi;
        int numStagioni;
        int durataMediaEp;
        bool inCorso;

    public:
        Series(string titolo, int anno, string immagine, int numEpisodi, int numStagioni, int durataMediaEp, bool inCorso);
        virtual ~Series() = default;

        int getNumEpisodi() const;
        int getNumStagioni() const;
        int getDurataMediaEp() const;
        bool getInCorso() const;
        
        void setNumEpisodi(const int &updNumEpisodi);
        void setNumStagioni(const int &updNumStagioni);
        void setDurataMediaEp(const int &updDurataMediaEp);
        void setInCorso(const bool &updInCorso);
};

#endif