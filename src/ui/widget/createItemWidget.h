#ifndef CREATEITEMWIDGET_H
#define CREATEITEMWIDGET_H

#include <QWidget>
#include <QComboBox>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QStackedWidget>
#include <QPushButton>
#include <QScrollArea>
#include <QResizeEvent>
#include <QMessageBox>
#include "../../logic/media.h"
#include "../../logic/series.h"
#include "../../logic/anime.h"
#include "../../logic/serieTv.h"
#include "../../logic/film.h"
#include "../../logic/libro.h"
#include "../../logic/manga.h"
#include "../../logic/cd.h"
#include "../../logic/cartaceo.h"
#include "../../services/mediaService.h"
#include "../../utils/mediaTypeUtils.h"

class CreateItemWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CreateItemWidget(QWidget *parent = nullptr);
    Media* createMediaItem() const;
    
    // Metodi per la modalità di modifica
    void setEditMode(Media* media);
    void clearEditMode();
    bool isInEditMode() const;
    
    // Metodo per impostare il MediaService
    void setMediaService(MediaService* service);

signals:
    void itemCreated(Media* newItem);
    void itemUpdated(Media* oldMedia, Media* newMedia);

public slots:
    void onItemTypeChanged(int index);

protected:
    void resizeEvent(QResizeEvent* event) override;

private slots:
    void onCreateButtonClicked();

private:
    void addFieldWithPlaceholder(QVBoxLayout* layout, const QString& labelText, const QString& placeholder);
    void updateScrollBarVisibility();
    void populateFieldsFromMedia(Media* media);
    QString getMediaTypeName(Media* media) const;

    QComboBox *itemTypeCombo;
    QStackedWidget *stackedFields;
    QPushButton *createButton;
    QScrollArea *scrollArea;
    
    // Variabili per la modalità di modifica
    Media* editingMedia;
    bool editMode;
    
    // MediaService reference
    MediaService* mediaService;
};

#endif // CREATEITEMWIDGET_H
