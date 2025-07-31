#ifndef JSONSERVICE_HEADER
#define JSONSERVICE_HEADER

#include <QObject>
#include <QJsonArray>
#include <QJsonObject>
#include "../logic/media.h"
#include "../utils/mediaTypeUtils.h"

class JsonService : public QObject
{
    Q_OBJECT
public:
    explicit JsonService(QObject *parent = nullptr);
    
    bool loadFromFile(const QString &filePath);
    bool saveToFile(const QString &filePath);
    
    QVector<Media*> getAllMedia() const;
    void addMedia(Media *media);
    bool removeMedia(const QString &title);
    Media* findMedia(const QString &title) const;
    void clearAll();
    
    QString getMediaTypeName(Media* media) const;

private:
    QJsonArray mediaArray;
    
    Media* jsonToMedia(const QJsonObject &jsonObj) const;
    QJsonObject mediaToJson(Media *media) const;
};

#endif