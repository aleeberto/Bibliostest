#include "mainWindow.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QMessageBox>
#include <QScrollArea>
#include <QLabel>
#include <QDebug>
#include <QDir>
#include <QPixmap>
#include <QPainter>
#include <QCoreApplication>
#include "../utils/styleUtils.h"
#include "../utils/mediaTypeUtils.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    // Window configuration with modern styling
    this->setStyleSheet(StyleUtils::getMainWindowStyle());
    setWindowTitle("Bibliotheca Procurator");
    setMinimumSize(1000, 700);

    QWidget *centralWidget = new QWidget(this);
    centralWidget->setStyleSheet(StyleUtils::getContentAreaStyle());
    setCentralWidget(centralWidget);

    // Main layout
    QHBoxLayout *mainLayout = new QHBoxLayout(centralWidget);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // Left widget - Menu and categories
    QWidget *leftWidget = new QWidget();
    leftWidget->setStyleSheet(StyleUtils::getSidebarStyle());
    leftWidget->setFixedWidth(280);

    leftLayout = new QVBoxLayout(leftWidget);
    leftLayout->setContentsMargins(5, 10, 5, 10);
    leftLayout->setSpacing(5);

    // Top menu
    topMenu = new TopMenuWidget(this);
    leftLayout->addWidget(topMenu);

    // Search bar
    setupSearchBar();

    // Categories
    setupCategoryButtons();

    leftLayout->addStretch();

    // Right widget - Main content
    rightLayoutWidget = new RightLayoutWidget(this);
    
    // Scroll area for right content
    QScrollArea *rightScrollArea = new QScrollArea(this);
    rightScrollArea->setWidgetResizable(true);
    rightScrollArea->setWidget(rightLayoutWidget);
    rightScrollArea->setFrameShape(QFrame::NoFrame);
    rightScrollArea->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // Add widgets to main layout
    mainLayout->addWidget(leftWidget);
    mainLayout->addWidget(rightScrollArea, 1);

    // Initialize services
    jsonService = new JsonService(this);
    mediaService = new MediaService(this);
    mediaService->setJsonService(jsonService);
    
    uiService = new UIService(this);
    rightLayoutWidget->setUIService(uiService);

    // Set minimum window size
    setMinimumSize(1150, 600);

    // Create item widget (hidden by default)
    createItemWidget = new CreateItemWidget(this);
    createItemWidget->setObjectName("CreateItemWidget");
    createItemWidget->setMediaService(mediaService);
    createItemWidget->hide();

    // Load initial data
    if (QFile::exists(currentJsonPath)) {
        loadMediaData(currentJsonPath);
    }

    // Connections
    connect(topMenu, &TopMenuWidget::uploadRequested, this, &MainWindow::handleUploadRequest);
    connect(topMenu, &TopMenuWidget::createRequested, this, &MainWindow::showCreateItemWidget);
    connect(topMenu, &TopMenuWidget::exportRequested, this, &MainWindow::handleExportRequest);
    connect(createItemWidget, &CreateItemWidget::itemCreated, this, &MainWindow::onMediaItemCreated);
    connect(createItemWidget, &CreateItemWidget::itemUpdated, this, &MainWindow::onMediaItemUpdated);
    connect(rightLayoutWidget, &RightLayoutWidget::mediaEditRequested, this, &MainWindow::onMediaEditRequested);
    connect(rightLayoutWidget, &RightLayoutWidget::mediaDeleteRequested, this, &MainWindow::onMediaDeleteRequested);
}

MainWindow::~MainWindow()
{
    // MediaService gestisce automaticamente la pulizia della collezione
    delete mediaService;
    delete jsonService;
}

void MainWindow::setupCategoryButtons()
{
    QStringList categories = {"Tutti", "Film", "Serie Tv", "Anime", "Libri", "Manga", "Cd"};

    for (const QString &category : categories) {
        QPushButton *btn = new QPushButton(category);
        btn->setCheckable(true);
        btn->setChecked(category == "Tutti");
        QString categoryButtonStyle = QString(
            "QPushButton {"
            "  text-align: left;"
            "  padding: 12px 16px;"
            "  border: none;"
            "  border-radius: 8px;"
            "  margin: 2px 0px;"
            "  font-size: 14px;"
            "  font-weight: 500;"
            "  color: %1;"
            "  background-color: transparent;"
            "}"
            "QPushButton:hover {"
            "  background-color: %2;"
            "  color: %3;"
            "}"
            "QPushButton:checked {"
            "  background-color: %4;"
            "  color: white;"
            "  font-weight: 600;"
            "}"
        ).arg(StyleUtils::getTextSecondaryColor(), 
              StyleUtils::getSurfaceColor(), 
              StyleUtils::getTextColor(),
              StyleUtils::getPrimaryColor());
        
        btn->setStyleSheet(categoryButtonStyle);

        connect(btn, &QPushButton::clicked, this, [this, category]() {
            currentCategory = category;
            
            // Usa MediaService per filtrare i media
            QVector<Media*> filteredMedia = mediaService->filterByCategoryAndSearch(category, searchBar->text());
            rightLayoutWidget->setMediaCollection(filteredMedia);
            rightLayoutWidget->setJsonService(jsonService);
            rightLayoutWidget->displayMediaByCategory("Tutti", ""); // Mostra tutti i media filtrati

            // Update button states
            for (QPushButton *button : categoryButtons) {
                button->setChecked(button->text() == category);
            }
        });

        categoryButtons[category] = btn;
        leftLayout->addWidget(btn);
    }
}

void MainWindow::setupSearchBar()
{
    searchBar = new QLineEdit();
    searchBar->setPlaceholderText("Cerca per titolo...");
    searchBar->setClearButtonEnabled(true);
    searchBar->setStyleSheet(StyleUtils::getSearchBarStyle());
    searchBar->setFixedHeight(44);

    connect(searchBar, &QLineEdit::textChanged, this, &MainWindow::onSearchTextChanged);
    leftLayout->addWidget(searchBar);
}

void MainWindow::loadMediaData(const QString &filePath)
{
    if (mediaService->loadFromFile(filePath)) {
        currentJsonPath = filePath;
        
        // Aggiorna la visualizzazione con i media filtrati
        QVector<Media*> filteredMedia = mediaService->filterByCategoryAndSearch(currentCategory, searchBar->text());
        rightLayoutWidget->setMediaCollection(filteredMedia);
        rightLayoutWidget->setJsonService(jsonService);
        rightLayoutWidget->displayMediaByCategory("Tutti", "");
    }
}

void MainWindow::handleUploadRequest()
{
    QString filePath = QFileDialog::getOpenFileName(
        this,
        tr("Seleziona file JSON"),
        QDir::homePath(),
        tr("File JSON (*.json)")
    );
    
    if (!filePath.isEmpty()) {
        loadMediaData(filePath);
        QMessageBox::information(this, "Successo", "File caricato correttamente!");
    }
}

void MainWindow::handleExportRequest()
{
    QString filePath = QFileDialog::getSaveFileName(
        this,
        tr("Esporta biblioteca in JSON"),
        QDir::homePath() + "/biblioteca_export.json",
        tr("File JSON (*.json)")
    );
    
    if (!filePath.isEmpty()) {
        // Assicurati che il file abbia l'estensione .json
        if (!filePath.endsWith(".json", Qt::CaseInsensitive)) {
            filePath += ".json";
        }
        
        if (mediaService->saveToFile(filePath)) {
            QMessageBox::information(this, "Successo", 
                "Biblioteca esportata correttamente in:\n" + QDir::toNativeSeparators(filePath));
            
            // Chiedi se vuole aprire il file esportato come biblioteca corrente
            QMessageBox::StandardButton reply = QMessageBox::question(
                this,
                "Apri file esportato",
                "Vuoi aprire il file appena esportato come biblioteca corrente?",
                QMessageBox::Yes | QMessageBox::No,
                QMessageBox::No
            );
            
            if (reply == QMessageBox::Yes) {
                loadMediaData(filePath);
                QMessageBox::information(this, "Successo", "File caricato come biblioteca corrente!");
            }
        } else {
            QMessageBox::critical(this, "Errore",
                "Impossibile esportare i dati in:\n" + QDir::toNativeSeparators(filePath) +
                "\n\nAssicurati di avere i permessi di scrittura per questa posizione.");
        }
    }
}

void MainWindow::showCreateItemWidget()
{
    // Assicurati che il widget sia in modalità creazione
    createItemWidget->clearEditMode();
    rightLayoutWidget->showCreateItemWidget(createItemWidget);
}

void MainWindow::onMediaItemCreated(Media* newItem)
{
    if (newItem) {
        // Aggiungi il media tramite MediaService
        if (mediaService->addMedia(newItem)) {
            // Salva automaticamente i dati nel file JSON
            if (mediaService->saveToFile(currentJsonPath)) {
                qDebug() << "Media salvato automaticamente in:" << currentJsonPath;
                
                // Mostra un messaggio di conferma all'utente
                QMessageBox::information(this, "Successo", 
                    "Media creato e salvato correttamente!\n" + 
                    MediaTypeUtils::getMediaTitle(newItem) + " è stato aggiunto alla biblioteca.");
            } else {
                QMessageBox::warning(this, "Attenzione",
                    "Media creato ma non è stato possibile salvarlo automaticamente.\n"
                    "Usa il pulsante 'Salva' per salvare manualmente i dati.");
            }
            
            // Aggiorna la visualizzazione con i media filtrati
            QVector<Media*> filteredMedia = mediaService->filterByCategoryAndSearch(currentCategory, searchBar->text());
            rightLayoutWidget->setMediaCollection(filteredMedia);
            rightLayoutWidget->displayMediaByCategory("Tutti", "");
        }
    }
}

void MainWindow::onSearchTextChanged(const QString& text)
{
    // Usa MediaService per filtrare i media
    QVector<Media*> filteredMedia = mediaService->filterByCategoryAndSearch(currentCategory, text);
    rightLayoutWidget->setMediaCollection(filteredMedia);
    rightLayoutWidget->displayMediaByCategory("Tutti", "");
}

void MainWindow::onMediaEditRequested(Media* media)
{
    if (!media) return;
    
    // Mostra il widget di creazione in modalità modifica
    rightLayoutWidget->showCreateItemWidget(createItemWidget);
    
    // Imposta la modalità di modifica con i dati del media
    createItemWidget->setEditMode(media);
}

void MainWindow::onMediaDeleteRequested(Media* media)
{
    if (!media) return;
    
    // Chiedi conferma all'utente
    QMessageBox::StandardButton reply = QMessageBox::question(
        this,
        "Conferma Eliminazione",
        QString("Sei sicuro di voler eliminare '%1'?\n\nQuesta operazione non può essere annullata.")
            .arg(MediaTypeUtils::getMediaTitle(media)),
        QMessageBox::Yes | QMessageBox::No,
        QMessageBox::No
    );
    
    if (reply == QMessageBox::Yes) {
        QString mediaTitle = MediaTypeUtils::getMediaTitle(media);
        
        // Elimina il media tramite MediaService
        if (mediaService->deleteMedia(media)) {
            // Salva automaticamente
            if (mediaService->saveToFile(currentJsonPath)) {
                qDebug() << "Media eliminato e salvato automaticamente:" << currentJsonPath;
                
                QMessageBox::information(this, "Successo", 
                    QString("'%1' è stato eliminato correttamente dalla biblioteca.")
                        .arg(mediaTitle));
                
                // Aggiorna la visualizzazione con i media filtrati
                QVector<Media*> filteredMedia = mediaService->filterByCategoryAndSearch(currentCategory, searchBar->text());
                rightLayoutWidget->setMediaCollection(filteredMedia);
                rightLayoutWidget->displayMediaByCategory("Tutti", "");
                
            } else {
                QMessageBox::critical(this, "Errore",
                    "Impossibile salvare le modifiche.\n"
                    "Il media è stato rimosso dalla visualizzazione ma potrebbe non essere salvato permanentemente.");
            }
        }
    }
}

void MainWindow::onMediaItemUpdated(Media* oldMedia, Media* newMedia)
{
    if (!oldMedia || !newMedia) return;
    
    // Aggiorna il media tramite MediaService
    if (mediaService->updateMedia(oldMedia, newMedia)) {
        // Salva automaticamente
        if (mediaService->saveToFile(currentJsonPath)) {
            qDebug() << "Media aggiornato e salvato automaticamente:" << currentJsonPath;
            
            QMessageBox::information(this, "Successo", 
                QString("'%1' è stato aggiornato correttamente!")
                    .arg(MediaTypeUtils::getMediaTitle(newMedia)));
            
            // Aggiorna la visualizzazione con i media filtrati
            QVector<Media*> filteredMedia = mediaService->filterByCategoryAndSearch(currentCategory, searchBar->text());
            rightLayoutWidget->setMediaCollection(filteredMedia);
            rightLayoutWidget->displayMediaByCategory("Tutti", "");
            
        } else {
            QMessageBox::critical(this, "Errore",
                "Impossibile salvare le modifiche.\n"
                "Il media è stato aggiornato nella visualizzazione ma potrebbe non essere salvato permanentemente.");
        }
    }
}
