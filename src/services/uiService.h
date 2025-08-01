#ifndef UISERVICE_H
#define UISERVICE_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QPixmap>
#include "../logic/media.h"
#include "mediaTypeUtils.h"

class UIService : public QObject
{
    Q_OBJECT

public:
    explicit UIService(QObject *parent = nullptr);

    // Formattazione dati per UI
    QString formatMediaTitle(Media* media) const;
    QString formatMediaYear(Media* media) const;
    QString formatMediaType(Media* media) const;
    QStringList formatMediaDetails(Media* media) const;
    
    // Gestione immagini
    QPixmap loadMediaImage(const std::string& imagePath) const;
    QPixmap createImagePlaceholder(const std::string& originalPath) const;
    
    // Utility per UI
    QString formatBooleanValue(bool value, const QString& trueText = "true", const QString& falseText = "false") const;
    QString formatDuration(int minutes, const QString& unit = "min") const;
    QString formatNumber(int number) const;
    
    // Validazione path immagini
    QString resolveImagePath(const QString& relativePath) const;
    bool isValidImagePath(const QString& path) const;

private:
    QString createDebugInfo(const QString& originalPath, const QString& resolvedPath) const;
};

#endif // UISERVICE_H
