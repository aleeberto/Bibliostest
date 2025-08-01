#include "rightLayoutWidget.h"
#include "../../utils/styleUtils.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QScrollArea>
#include <QPushButton>

RightLayoutWidget::RightLayoutWidget(QWidget *parent) : QWidget(parent), jsonService(nullptr), uiService(nullptr)
{
    setStyleSheet(
        "QWidget { background-color: #f5f5f5; }"
        "QScrollArea { border: none; background: transparent; }"
    );

    mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    currentCategory = "Tutti";
    currentSearchText = "";
}

RightLayoutWidget::~RightLayoutWidget()
{
    clearLayout();
}

void RightLayoutWidget::setMediaCollection(const QVector<Media*>& collection)
{
    mediaCollection = collection;
}

void RightLayoutWidget::setJsonService(JsonService* service)
{
    jsonService = service;
}

void RightLayoutWidget::setUIService(UIService* service)
{
    uiService = service;
}

void RightLayoutWidget::displayMediaByCategory(const QString &category, const QString &searchText)
{
    currentCategory = category;
    currentSearchText = searchText.toLower();
    
    clearLayout();

    for (Media* media : mediaCollection) {
        QString mediaType = uiService ? uiService->getMediaTypeName(media) : 
                           (jsonService ? jsonService->getMediaTypeName(media) : "");
        QString mediaTitle = QString::fromStdString(media->getTitolo()).toLower();

        if ((category == "Tutti" || mediaType == category) &&
            (currentSearchText.isEmpty() || mediaTitle.contains(currentSearchText))) {
            addMediaCardToLayout(media);
        }
    }

    mainLayout->addStretch();
}

void RightLayoutWidget::clearLayout()
{
    QLayoutItem* item;
    while ((item = mainLayout->takeAt(0)) != nullptr) {
        if (item->widget()) {
            QWidget* widget = item->widget();
            // Non eliminare il CreateItemWidget, solo rimuoverlo dal layout
            if (widget->objectName() != "CreateItemWidget") {
                widget->deleteLater();
            } else {
                // Per il CreateItemWidget, solo nascondilo e rimuovi il parent
                widget->hide();
                widget->setParent(nullptr);
            }
        }
        delete item;
    }
}

void RightLayoutWidget::showCreateItemWidget(QWidget* createWidget)
{
    // Controlla se il widget è già presente nel layout
    if (createWidget->parent() == this && mainLayout->indexOf(createWidget) != -1) {
        // Il widget è già presente, non fare nulla
        return;
    }
    
    clearLayout();
    
    // Assicurati che il widget non abbia già un parent
    if (createWidget->parent() && createWidget->parent() != this) {
        createWidget->setParent(nullptr);
    }
    
    createWidget->setParent(this);
    createWidget->show();
    createWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    mainLayout->addWidget(createWidget, 1); // stretch factor = 1 per occupare tutto lo spazio
    // Rimuovo addStretch() per permettere al widget di espandersi completamente
}

void RightLayoutWidget::addMediaCardToLayout(Media* media)
{
    if (!media || !uiService) return;
    
    QWidget *mediaCard = new QWidget();
    mediaCard->setStyleSheet(
        "QWidget {"
        "  border: 1px solid #ddd;"
        "  border-radius: 5px;"
        "  margin: 5px;"
        "  background: white;"
        "}"
    );

    QHBoxLayout *cardLayout = new QHBoxLayout(mediaCard);
    cardLayout->setContentsMargins(10, 10, 10, 10);

    // Image
    QLabel *imgLabel = new QLabel();
    imgLabel->setPixmap(uiService->loadMediaImage(media->getImmagine()));
    imgLabel->setAlignment(Qt::AlignTop);

    // Details
    QWidget *detailsWidget = new QWidget();
    QVBoxLayout *detailsLayout = new QVBoxLayout(detailsWidget);
    detailsLayout->setAlignment(Qt::AlignTop);

    // Usa UIService per formattare i dati
    QLabel *titleLabel = new QLabel(uiService->formatMediaTitle(media));
    titleLabel->setStyleSheet("font-size: 14px;");

    QLabel *yearLabel = new QLabel(uiService->formatMediaYear(media));
    QLabel *typeLabel = new QLabel(uiService->formatMediaType(media));

    detailsLayout->addWidget(titleLabel);
    detailsLayout->addWidget(yearLabel);
    detailsLayout->addWidget(typeLabel);

    // Aggiungi i dettagli specifici del tipo di media
    QStringList details = uiService->formatMediaDetails(media);
    for (const QString& detail : details) {
        detailsLayout->addWidget(new QLabel(detail));
    }

    detailsLayout->addStretch();

    // --- Buttons area ---
    QWidget *buttonsWidget = new QWidget();
    QVBoxLayout *buttonsLayout = new QVBoxLayout(buttonsWidget);
    buttonsLayout->setAlignment(Qt::AlignTop);
    buttonsLayout->setSpacing(10);

    // Edit button
    QPushButton *editButton = new QPushButton(this);
    editButton->setFixedHeight(44);
    editButton->setToolTip("Modifica questo elemento");
    editButton->setIcon(QIcon("../resources/icon/edit.png"));
    editButton->setStyleSheet(StyleUtils::getItemButtonStyle());


    // Delete button
    QPushButton *deleteButton = new QPushButton(this);
    deleteButton->setFixedHeight(44);
    deleteButton->setToolTip("Elimina questo elemento");
    deleteButton->setIcon(QIcon("../resources/icon/delete.png"));
    deleteButton->setStyleSheet(StyleUtils::getItemButtonStyle());

    buttonsLayout->addWidget(editButton);
    buttonsLayout->addWidget(deleteButton);
    buttonsLayout->addStretch();

    // Connect buttons to signals
    connect(editButton, &QPushButton::clicked, this, [this, media]() {
        emit mediaEditRequested(media);
    });

    connect(deleteButton, &QPushButton::clicked, this, [this, media]() {
        emit mediaDeleteRequested(media);
    });

    cardLayout->addWidget(imgLabel);
    cardLayout->addWidget(detailsWidget, 1);
    cardLayout->addWidget(buttonsWidget);

    mainLayout->addWidget(mediaCard);
}
