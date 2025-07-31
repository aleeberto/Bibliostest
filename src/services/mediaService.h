#ifndef MEDIASERVICE_H
#define MEDIASERVICE_H

#include <QObject>
#include <QVector>
#include <QString>
#include <QList>
#include <QLineEdit>
#include <QWidget>
#include <QMessageBox>
#include "../logic/media.h"
#include "../utils/mediaTypeUtils.h"
#include "jsonService.h"

class MediaService : public QObject
{
    Q_OBJECT

public:
    explicit MediaService(QObject *parent = nullptr);
    ~MediaService();

    // Gestione collezione media
    void setMediaCollection(const QVector<Media*>& collection);
    QVector<Media*> getMediaCollection() const;
    void clearMediaCollection();

    // Operazioni CRUD
    bool addMedia(Media* media);
    bool updateMedia(Media* oldMedia, Media* newMedia);
    bool deleteMedia(Media* media);
    Media* findMediaByTitle(const QString& title) const;

    // Operazioni di persistenza
    bool loadFromFile(const QString& filePath);
    bool saveToFile(const QString& filePath);
    void setJsonService(JsonService* service);

    // Operazioni di filtro e ricerca
    QVector<Media*> filterByCategory(const QString& category) const;
    QVector<Media*> searchByTitle(const QString& searchText) const;
    QVector<Media*> filterByCategoryAndSearch(const QString& category, const QString& searchText) const;

    // Utility
    QString getMediaTypeName(Media* media) const;
    int getMediaCount() const;
    bool isEmpty() const;

    // Creazione media da campi UI
    Media* createMediaFromFields(const QString& type, const QList<QLineEdit*>& fields, QWidget* parent);

signals:
    void mediaAdded(Media* media);
    void mediaUpdated(Media* oldMedia, Media* newMedia);
    void mediaDeleted(Media* media);
    void collectionChanged();
    void dataLoaded();
    void dataSaved();

private:
    QVector<Media*> mediaCollection;
    JsonService* jsonService;
    QString currentFilePath;

    void updateJsonService();
};

#endif // MEDIASERVICE_H
