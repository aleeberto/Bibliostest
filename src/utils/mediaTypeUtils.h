#ifndef MEDIATYPEUTILS_H
#define MEDIATYPEUTILS_H

#include <QString>
#include "../logic/media.h"

class MediaTypeUtils
{
public:
    static QString getMediaTypeName(Media* media);
    static QString getMediaTypeDisplayName(Media* media);
    
    // Utility per conversioni comuni
    static QString getMediaTitle(Media* media);
    static QString getMediaImage(Media* media);
    static int getMediaYear(Media* media);
    
private:
    MediaTypeUtils() = default; // Classe utility statica
};

#endif // MEDIATYPEUTILS_H
