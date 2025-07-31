#include "mediaTypeUtils.h"
#include "../logic/film.h"
#include "../logic/serieTv.h"
#include "../logic/anime.h"
#include "../logic/libro.h"
#include "../logic/manga.h"
#include "../logic/cd.h"

QString MediaTypeUtils::getMediaTypeName(Media* media)
{
    if (!media) return "";
    
    if (dynamic_cast<Film*>(media)) return "Film";
    if (dynamic_cast<SerieTv*>(media)) return "Serie Tv";
    if (dynamic_cast<Anime*>(media)) return "Anime";
    if (dynamic_cast<Libro*>(media)) return "Libri";
    if (dynamic_cast<Manga*>(media)) return "Manga";
    if (dynamic_cast<Cd*>(media)) return "Cd";
    
    return "Sconosciuto";
}

QString MediaTypeUtils::getMediaTypeDisplayName(Media* media)
{
    if (!media) return "";
    
    if (dynamic_cast<Film*>(media)) return "Film";
    if (dynamic_cast<SerieTv*>(media)) return "Serie TV";
    if (dynamic_cast<Anime*>(media)) return "Anime";
    if (dynamic_cast<Libro*>(media)) return "Libro";
    if (dynamic_cast<Manga*>(media)) return "Manga";
    if (dynamic_cast<Cd*>(media)) return "CD";
    
    return "Tipo Sconosciuto";
}

QString MediaTypeUtils::getMediaTitle(Media* media)
{
    if (!media) return "";
    return QString::fromStdString(media->getTitolo());
}

QString MediaTypeUtils::getMediaImage(Media* media)
{
    if (!media) return "";
    return QString::fromStdString(media->getImmagine());
}

int MediaTypeUtils::getMediaYear(Media* media)
{
    if (!media) return 0;
    return media->getAnno();
}
