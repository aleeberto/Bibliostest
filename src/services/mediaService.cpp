#include "mediaService.h"
#include <QDebug>
#include <QFile>
#include <QList>
#include <QLineEdit>
#include <stdexcept>
#include "../logic/film.h"
#include "../logic/serieTv.h"
#include "../logic/anime.h"
#include "../logic/libro.h"
#include "../logic/manga.h"
#include "../logic/cd.h"

MediaService::MediaService(QObject *parent) : QObject(parent), jsonService(nullptr)
{
}

MediaService::~MediaService()
{
    clearMediaCollection();
}

void MediaService::setMediaCollection(const QVector<Media*>& collection)
{
    clearMediaCollection();
    mediaCollection = collection;
    updateJsonService();
    emit collectionChanged();
}

QVector<Media*> MediaService::getMediaCollection() const
{
    return mediaCollection;
}

void MediaService::clearMediaCollection()
{
    for (Media* media : mediaCollection) {
        delete media;
    }
    mediaCollection.clear();
    updateJsonService();
    emit collectionChanged();
}

bool MediaService::addMedia(Media* media)
{
    if (!media) return false;
    
    mediaCollection.append(media);
    updateJsonService();
    emit mediaAdded(media);
    emit collectionChanged();
    return true;
}

bool MediaService::updateMedia(Media* oldMedia, Media* newMedia)
{
    if (!oldMedia || !newMedia) return false;
    
    int index = mediaCollection.indexOf(oldMedia);
    if (index == -1) return false;
    
    mediaCollection[index] = newMedia;
    delete oldMedia;
    updateJsonService();
    emit mediaUpdated(oldMedia, newMedia);
    emit collectionChanged();
    return true;
}

bool MediaService::deleteMedia(Media* media)
{
    if (!media) return false;
    
    int index = mediaCollection.indexOf(media);
    if (index == -1) return false;
    
    mediaCollection.removeAt(index);
    delete media;
    updateJsonService();
    emit mediaDeleted(media);
    emit collectionChanged();
    return true;
}

Media* MediaService::findMediaByTitle(const QString& title) const
{
    for (Media* media : mediaCollection) {
        if (QString::fromStdString(media->getTitolo()) == title) {
            return media;
        }
    }
    return nullptr;
}

bool MediaService::loadFromFile(const QString& filePath)
{
    if (!jsonService) return false;
    
    currentFilePath = filePath;
    bool success = jsonService->loadFromFile(filePath);
    if (success) {
        setMediaCollection(jsonService->getAllMedia());
        emit dataLoaded();
    }
    return success;
}

bool MediaService::saveToFile(const QString& filePath)
{
    if (!jsonService) return false;
    
    currentFilePath = filePath;
    bool success = jsonService->saveToFile(filePath);
    if (success) {
        emit dataSaved();
    }
    return success;
}

void MediaService::setJsonService(JsonService* service)
{
    jsonService = service;
}

QVector<Media*> MediaService::filterByCategory(const QString& category) const
{
    if (category == "Tutti") {
        return mediaCollection;
    }
    
    QVector<Media*> filtered;
    for (Media* media : mediaCollection) {
        QString mediaType = MediaTypeUtils::getMediaTypeName(media);
        if (mediaType == category) {
            filtered.append(media);
        }
    }
    return filtered;
}

QVector<Media*> MediaService::searchByTitle(const QString& searchText) const
{
    if (searchText.isEmpty()) {
        return mediaCollection;
    }
    
    QVector<Media*> filtered;
    for (Media* media : mediaCollection) {
        QString title = QString::fromStdString(media->getTitolo());
        if (title.contains(searchText, Qt::CaseInsensitive)) {
            filtered.append(media);
        }
    }
    return filtered;
}

QVector<Media*> MediaService::filterByCategoryAndSearch(const QString& category, const QString& searchText) const
{
    QVector<Media*> categoryFiltered = filterByCategory(category);
    
    if (searchText.isEmpty()) {
        return categoryFiltered;
    }
    
    QVector<Media*> filtered;
    for (Media* media : categoryFiltered) {
        QString title = QString::fromStdString(media->getTitolo());
        if (title.contains(searchText, Qt::CaseInsensitive)) {
            filtered.append(media);
        }
    }
    return filtered;
}

int MediaService::getMediaCount() const
{
    return mediaCollection.size();
}

bool MediaService::isEmpty() const
{
    return mediaCollection.isEmpty();
}

void MediaService::updateJsonService()
{
    if (jsonService) {
        jsonService->clearAll();
        for (Media* media : mediaCollection) {
            jsonService->addMedia(media);
        }
    }
}

Media* MediaService::createMediaFromFields(const QString& type, const QList<QLineEdit*>& fields, QWidget* parent)
{
    // Validate all fields are filled
    for (QLineEdit* field : fields) {
        if (field->text().isEmpty()) {
            QMessageBox::warning(parent,
                "Attenzione",
                QString("Il campo '%1' è obbligatorio!").arg(field->placeholderText()));
            field->setFocus();
            return nullptr;
        }
    }

    try {
        if (type == "Serie Tv") {
            bool inCorso;
            QString inCorsoStr = fields[6]->text().toLower();
            if (inCorsoStr == "true" || inCorsoStr == "1") inCorso = true;
            else if (inCorsoStr == "false" || inCorsoStr == "0") inCorso = false;
            else throw std::invalid_argument("Valore non valido per 'In Corso'");

            return new SerieTv(
                fields[0]->text().toStdString(),
                fields[2]->text().toInt(),
                fields[1]->text().toStdString(),
                fields[3]->text().toInt(),
                fields[4]->text().toInt(),
                fields[5]->text().toInt(),
                inCorso,
                fields[7]->text().toStdString(),
                fields[8]->text().toStdString()
            );
        }
        else if (type == "Anime") {
            bool inCorso;
            QString inCorsoStr = fields[6]->text().toLower();
            if (inCorsoStr == "true" || inCorsoStr == "1") inCorso = true;
            else if (inCorsoStr == "false" || inCorsoStr == "0") inCorso = false;
            else throw std::invalid_argument("Valore non valido per 'In Corso'");

            return new Anime(
                fields[0]->text().toStdString(),
                fields[2]->text().toInt(),
                fields[1]->text().toStdString(),
                fields[3]->text().toInt(),
                fields[4]->text().toInt(),
                fields[5]->text().toInt(),
                inCorso,
                fields[7]->text().toStdString(),
                fields[8]->text().toStdString()
            );
        }
        else if (type == "Film") {
            return new Film(
                fields[0]->text().toStdString(),
                fields[2]->text().toInt(),
                fields[1]->text().toStdString(),
                fields[3]->text().toStdString(),
                fields[4]->text().toStdString(),
                fields[5]->text().toInt()
            );
        }
        else if (type == "Libro") {
            return new Libro(
                fields[0]->text().toStdString(),
                fields[2]->text().toInt(),
                fields[1]->text().toStdString(),
                fields[3]->text().toStdString(),
                fields[4]->text().toInt(),
                fields[5]->text().toStdString()
            );
        }
        else if (type == "Manga") {
            bool concluso;
            QString conclusoStr = fields[6]->text().toLower();
            if (conclusoStr == "true" || conclusoStr == "1") concluso = true;
            else if (conclusoStr == "false" || conclusoStr == "0") concluso = false;
            else throw std::invalid_argument("Valore non valido per 'Concluso'");

            return new Manga(
                fields[0]->text().toStdString(),
                fields[2]->text().toInt(),
                fields[1]->text().toStdString(),
                fields[3]->text().toStdString(),
                fields[4]->text().toStdString(),
                fields[5]->text().toInt(),
                concluso
            );
        }
        else if (type == "Cd") {
            return new Cd(
                fields[0]->text().toStdString(),
                fields[2]->text().toInt(),
                fields[1]->text().toStdString(),
                fields[3]->text().toStdString(),
                fields[4]->text().toInt(),
                fields[5]->text().toInt()
            );
        }
    }
    catch (const std::exception& e) {
        QMessageBox::critical(parent,
            "Errore",
            QString("Dati non validi: %1").arg(e.what()));
        return nullptr;
    }
    catch (...) {
        QMessageBox::critical(parent,
            "Errore",
            "Si è verificato un errore sconosciuto durante la creazione del media");
        return nullptr;
    }

    return nullptr;
}
