#include "uiService.h"
#include <QDir>
#include <QFileInfo>
#include <QCoreApplication>
#include <QPainter>
#include <QFont>
#include <QDebug>
#include "../logic/film.h"
#include "../logic/serieTv.h"
#include "../logic/anime.h"
#include "../logic/libro.h"
#include "../logic/manga.h"
#include "../logic/cd.h"

UIService::UIService(QObject *parent) : QObject(parent)
{
}

QString UIService::formatMediaTitle(Media* media) const
{
    if (!media) return "";
    return "<b>" + QString::fromStdString(media->getTitolo()) + "</b>";
}

QString UIService::formatMediaYear(Media* media) const
{
    if (!media) return "";
    return "Anno: " + QString::number(media->getAnno());
}

QString UIService::formatMediaType(Media* media) const
{
    if (!media) return "";
    return "Tipo: " + MediaTypeUtils::getMediaTypeName(media);
}

QStringList UIService::formatMediaDetails(Media* media) const
{
    QStringList details;
    if (!media) return details;
    
    if (auto film = dynamic_cast<Film*>(media)) {
        details << "Regista: " + QString::fromStdString(film->getRegista());
        details << "Protagonista: " + QString::fromStdString(film->getAttoreProtagonista());
        details << "Durata: " + formatDuration(film->getDurata());
    }
    else if (auto serie = dynamic_cast<SerieTv*>(media)) {
        details << "Stagioni: " + formatNumber(serie->getNumStagioni());
        details << "Episodi: " + formatNumber(serie->getNumEpisodi());
        details << "Durata episodio: " + formatDuration(serie->getDurataMediaEp());
        details << "Ideatore: " + QString::fromStdString(serie->getIdeatore());
        details << "Casa produttrice: " + QString::fromStdString(serie->getCasaProduttrice());
        details << "Stato: " + formatBooleanValue(serie->getInCorso(), "In corso", "Conclusa");
    }
    else if (auto anime = dynamic_cast<Anime*>(media)) {
        details << "Stagioni: " + formatNumber(anime->getNumStagioni());
        details << "Episodi: " + formatNumber(anime->getNumEpisodi());
        details << "Durata episodio: " + formatDuration(anime->getDurataMediaEp());
        details << "Disegnatore: " + QString::fromStdString(anime->getDisegnatore());
        details << "Studio: " + QString::fromStdString(anime->getStudioAnimazione());
        details << "Stato: " + formatBooleanValue(anime->getInCorso(), "In corso", "Conclusa");
    }
    else if (auto libro = dynamic_cast<Libro*>(media)) {
        details << "Scrittore: " + QString::fromStdString(libro->getScrittore());
        details << "Anno di Stampa: " + formatNumber(libro->getAnnoStampa());
        details << "Casa Editrice: " + QString::fromStdString(libro->getCasaEditrice());
    }
    else if (auto manga = dynamic_cast<Manga*>(media)) {
        details << "Scrittore: " + QString::fromStdString(manga->getScrittore());
        details << "Illustratore: " + QString::fromStdString(manga->getIllustratore());
        details << "Volumi: " + formatNumber(manga->getNumLibri());
        details << "Stato: " + formatBooleanValue(manga->getConcluso(), "Concluso", "In corso");
    }
    else if (auto cd = dynamic_cast<Cd*>(media)) {
        details << "Artista: " + QString::fromStdString(cd->getArtista());
        details << "Tracce: " + formatNumber(cd->getNumTracce());
        details << "Durata media: " + formatDuration(cd->getDurataMedTracce(), "sec");
    }
    
    return details;
}

QPixmap UIService::loadMediaImage(const std::string& imagePath) const
{
    QString imageQPath = QString::fromStdString(imagePath);
    QPixmap pixmap;
    
    // Risolvi il path dell'immagine
    QString resolvedPath = resolveImagePath(imageQPath);
    
    // Prova a caricare l'immagine
    if (pixmap.load(resolvedPath)) {
        return pixmap.scaled(120, 180, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    }
    
    // Se il caricamento fallisce, crea un placeholder
    return createImagePlaceholder(imagePath);
}

QPixmap UIService::createImagePlaceholder(const std::string& originalPath) const
{
    QPixmap pixmap(120, 180);
    pixmap.fill(QColor(240, 240, 240));
    
    QPainter painter(&pixmap);
    painter.setPen(Qt::darkGray);
    painter.setFont(QFont("Arial", 8));
    
    QString imageQPath = QString::fromStdString(originalPath);
    QString resolvedPath = resolveImagePath(imageQPath);
    QString debugInfo = createDebugInfo(imageQPath, resolvedPath);
    
    painter.drawText(pixmap.rect().adjusted(5, 5, -5, -5), 
                    Qt::AlignCenter | Qt::TextWordWrap, 
                    debugInfo);
    
    qDebug() << "Errore caricamento immagine:" << debugInfo;
    
    return pixmap;
}


QString UIService::formatBooleanValue(bool value, const QString& trueText, const QString& falseText) const
{
    return value ? trueText : falseText;
}

QString UIService::formatDuration(int minutes, const QString& unit) const
{
    return QString::number(minutes) + " " + unit;
}

QString UIService::formatNumber(int number) const
{
    return QString::number(number);
}

QString UIService::resolveImagePath(const QString& relativePath) const
{
    QFileInfo fileInfo(relativePath);
    if (fileInfo.isRelative()) {
        QString appDir = QCoreApplication::applicationDirPath();
        return QDir::cleanPath(appDir + QDir::separator() + relativePath);
    }
    return relativePath;
}

bool UIService::isValidImagePath(const QString& path) const
{
    QString resolvedPath = resolveImagePath(path);
    return QFileInfo::exists(resolvedPath);
}

QString UIService::createDebugInfo(const QString& originalPath, const QString& resolvedPath) const
{
    return QString("Impossibile caricare:\n%1\nPercorso risolto:\n%2")
           .arg(originalPath)
           .arg(resolvedPath);
}
