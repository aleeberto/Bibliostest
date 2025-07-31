#include "jsonService.h"
#include <QFile>
#include <QJsonDocument>
#include <QDebug>
#include "../logic/film.h"
#include "../logic/serieTv.h"
#include "../logic/anime.h"
#include "../logic/libro.h"
#include "../logic/manga.h"
#include "../logic/cd.h"

JsonService::JsonService(QObject *parent) : QObject(parent) {}

bool JsonService::loadFromFile(const QString &filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Impossibile aprire il file:" << filePath;
        return false;
    }
    
    QByteArray jsonData = file.readAll();
    file.close();
    
    QJsonDocument doc = QJsonDocument::fromJson(jsonData);
    if (doc.isNull()) {
        qWarning() << "Formato JSON non valido";
        return false;
    }
    
    mediaArray = doc.object()["media"].toArray();
    return true;
}

bool JsonService::saveToFile(const QString &filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly)) {
        qWarning() << "Impossibile salvare il file:" << filePath;
        return false;
    }
    
    QJsonObject rootObject;
    rootObject["media"] = mediaArray;
    
    QJsonDocument doc(rootObject);
    file.write(doc.toJson(QJsonDocument::Indented));
    file.close();
    
    return true;
}

QVector<Media*> JsonService::getAllMedia() const {
    QVector<Media*> result;
    
    for (const QJsonValue &value : mediaArray) {
        Media *media = jsonToMedia(value.toObject());
        if (media) {
            result.append(media);
        }
    }
    
    return result;
}

void JsonService::addMedia(Media *media) {
    if (!media) return;
    mediaArray.append(mediaToJson(media));
}

bool JsonService::removeMedia(const QString &title) {
    for (int i = 0; i < mediaArray.size(); ++i) {
        QJsonObject obj = mediaArray[i].toObject();
        if (obj["titolo"].toString().compare(title, Qt::CaseInsensitive) == 0) {
            mediaArray.removeAt(i);
            return true;
        }
    }
    return false;
}

Media* JsonService::findMedia(const QString &title) const {
    for (const QJsonValue &value : mediaArray) {
        QJsonObject obj = value.toObject();
        if (obj["titolo"].toString().compare(title, Qt::CaseInsensitive) == 0) {
            return jsonToMedia(obj);
        }
    }
    return nullptr;
}

void JsonService::clearAll() {
    mediaArray = QJsonArray();
}

QString JsonService::getMediaTypeName(Media* media) const {
    return MediaTypeUtils::getMediaTypeName(media);
}

Media* JsonService::jsonToMedia(const QJsonObject &jsonObj) const {
    std::string titolo = jsonObj["titolo"].toString().toStdString();
    int anno = jsonObj["anno"].toInt();
    std::string immagine = jsonObj["immagine"].toString().toStdString();
    QString type = jsonObj["type"].toString();
    
    if (type == "Film") {
        return new Film(
            titolo, anno, immagine,
            jsonObj["regista"].toString().toStdString(),
            jsonObj["attoreProtagonista"].toString().toStdString(),
            jsonObj["durata"].toInt()
        );
    }
    else if (type == "Serie Tv") {
        return new SerieTv(
            titolo, anno, immagine,
            jsonObj["numEpisodi"].toInt(),
            jsonObj["numStagioni"].toInt(),
            jsonObj["durataMediaEp"].toInt(),
            jsonObj["inCorso"].toBool(),
            jsonObj["ideatore"].toString().toStdString(),
            jsonObj["casaProduttrice"].toString().toStdString()
        );
    }
    else if (type == "Anime") {
        return new Anime(
            titolo, anno, immagine,
            jsonObj["numEpisodi"].toInt(),
            jsonObj["numStagioni"].toInt(),
            jsonObj["durataMediaEp"].toInt(),
            jsonObj["inCorso"].toBool(),
            jsonObj["disegnatore"].toString().toStdString(),
            jsonObj["studioAnimazione"].toString().toStdString()
        );
    }
    else if (type == "Libro") {
        return new Libro(
            titolo, anno, immagine,
            jsonObj["scrittore"].toString().toStdString(),
            jsonObj["annoStampa"].toInt(),
            jsonObj["casaEditrice"].toString().toStdString()
        );
    }
    else if (type == "Manga") {
        return new Manga(
            titolo, anno, immagine,
            jsonObj["scrittore"].toString().toStdString(),
            jsonObj["illustratore"].toString().toStdString(),
            jsonObj["numLibri"].toInt(),
            jsonObj["concluso"].toBool()
        );
    }
    else if (type == "Cd") {
        return new Cd(
            titolo, anno, immagine,
            jsonObj["artista"].toString().toStdString(),
            jsonObj["numTracce"].toInt(),
            jsonObj["durataMedTracce"].toInt()
        );
    }
    
    return nullptr;
}

QJsonObject JsonService::mediaToJson(Media *media) const {
    QJsonObject jsonObj;
    
    if (dynamic_cast<Film*>(media)) jsonObj["type"] = "Film";
    else if (dynamic_cast<SerieTv*>(media)) jsonObj["type"] = "Serie Tv";
    else if (dynamic_cast<Anime*>(media)) jsonObj["type"] = "Anime";
    else if (dynamic_cast<Libro*>(media)) jsonObj["type"] = "Libri";
    else if (dynamic_cast<Manga*>(media)) jsonObj["type"] = "Manga";
    else if (dynamic_cast<Cd*>(media)) jsonObj["type"] = "Cd";
    else jsonObj["type"] = "Altro";
    
    jsonObj["titolo"] = QString::fromStdString(media->getTitolo());
    jsonObj["anno"] = media->getAnno();
    jsonObj["immagine"] = QString::fromStdString(media->getImmagine());
    
    if (auto film = dynamic_cast<Film*>(media)) {
        jsonObj["regista"] = QString::fromStdString(film->getRegista());
        jsonObj["attoreProtagonista"] = QString::fromStdString(film->getAttoreProtagonista());
        jsonObj["durata"] = film->getDurata();
    }
    else if (auto serie = dynamic_cast<SerieTv*>(media)) {
        jsonObj["numEpisodi"] = serie->getNumEpisodi();
        jsonObj["numStagioni"] = serie->getNumStagioni();
        jsonObj["durataMediaEp"] = serie->getDurataMediaEp();
        jsonObj["inCorso"] = serie->getInCorso();
        jsonObj["ideatore"] = QString::fromStdString(serie->getIdeatore());
        jsonObj["casaProduttrice"] = QString::fromStdString(serie->getCasaProduttrice());
    }
    else if (auto anime = dynamic_cast<Anime*>(media)) {
        jsonObj["numEpisodi"] = anime->getNumEpisodi();
        jsonObj["numStagioni"] = anime->getNumStagioni();
        jsonObj["durataMediaEp"] = anime->getDurataMediaEp();
        jsonObj["inCorso"] = anime->getInCorso();
        jsonObj["disegnatore"] = QString::fromStdString(anime->getDisegnatore());
        jsonObj["studioAnimazione"] = QString::fromStdString(anime->getStudioAnimazione());
    }
    else if (auto libro = dynamic_cast<Libro*>(media)) {
        jsonObj["scrittore"] = QString::fromStdString(libro->getScrittore());
        jsonObj["annoStampa"] = libro->getAnnoStampa();
        jsonObj["casaEditrice"] = QString::fromStdString(libro->getCasaEditrice());
    }
    else if (auto manga = dynamic_cast<Manga*>(media)) {
        jsonObj["scrittore"] = QString::fromStdString(manga->getScrittore());
        jsonObj["illustratore"] = QString::fromStdString(manga->getIllustratore());
        jsonObj["numLibri"] = manga->getNumLibri();
        jsonObj["concluso"] = manga->getConcluso();
    }
    else if (auto cd = dynamic_cast<Cd*>(media)) {
        jsonObj["artista"] = QString::fromStdString(cd->getArtista());
        jsonObj["numTracce"] = cd->getNumTracce();
        jsonObj["durataMedTracce"] = cd->getDurataMedTracce();
    }
    
    return jsonObj;

}