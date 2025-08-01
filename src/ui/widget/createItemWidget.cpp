#include "createItemWidget.h"
#include <QScrollArea>
#include <QDebug>
#include <QFileDialog>
#include <QDir>
#include <QTimer>
#include <QAbstractScrollArea>
#include "../../utils/styleUtils.h"

CreateItemWidget::CreateItemWidget(QWidget *parent) : QWidget(parent), editingMedia(nullptr), editMode(false), mediaService(nullptr)
{
    // Create the main vertical layout (no extra margins)
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // --- Build the scrollable form area ---
    QWidget *formWidget = new QWidget();
    QVBoxLayout *formLayout = new QVBoxLayout(formWidget);
    formLayout->setContentsMargins(15, 15, 15, 15);
    formLayout->setSpacing(10);

    // Form content
    QLabel *label = new QLabel("Seleziona il tipo di oggetto:", formWidget);
    itemTypeCombo = new QComboBox(formWidget);
    itemTypeCombo->setFixedHeight(35);

    stackedFields = new QStackedWidget(formWidget);
    stackedFields->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);

    // Define fields for each media type
    QMap<QString, QStringList> mediaFields = {
        {"Serie Tv", {"Numero Episodi", "Numero Stagioni", "Durata Media Episodio (min)", "In Corso (true/false)", "Ideatore", "Casa produttrice"}},
        {"Anime", {"Numero Episodi", "Numero Stagioni", "Durata Media Episodio (min)", "In Corso (true/false)", "Disegnatore", "Studio Animazione"}},
        {"Film", {"Regista", "Attore protagonista", "Durata (min)"}},
        {"Libro", {"Scrittore", "Anno di stampa", "Casa Editrice"}},
        {"Manga", {"Scrittore", "Numero Libri", "Illustratore", "Concluso (true/false)"}},
        {"Cd", {"Artista", "Numero Tracce", "Durata Media Tracce (sec)"}}
    };

    // Create a form page for each media type
    for (const QString& type : mediaFields.keys()) {
        QWidget* page = new QWidget(formWidget);
        QVBoxLayout* pageLayout = new QVBoxLayout(page);
        pageLayout->setContentsMargins(5, 5, 5, 5);
        pageLayout->setSpacing(5);

        // Common fields for all media types
        addFieldWithPlaceholder(pageLayout, "Titolo:", "Inserisci il titolo");
        addFieldWithPlaceholder(pageLayout, "Immagine:", "Percorso dell'immagine");
        addFieldWithPlaceholder(pageLayout, "Anno:", "Anno di pubblicazione");

        // Type-specific fields
        for (const QString& field : mediaFields[type]) {
            addFieldWithPlaceholder(pageLayout, field + ":", "Inserisci " + field);
        }

        page->setLayout(pageLayout);
        stackedFields->addWidget(page);
        itemTypeCombo->addItem(type);
    }

    // Add widgets to the form layout (no button here)
    formLayout->addWidget(label);
    formLayout->addWidget(itemTypeCombo);
    formLayout->addWidget(stackedFields);
    // Do not add createButton here and no addStretch()

    // Create and configure QScrollArea for the form
    scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(formWidget);
    scrollArea->setFrameShape(QFrame::NoFrame);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scrollArea->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    scrollArea->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);

    // --- Build the fixed button area ---
    QWidget *buttonContainer = new QWidget(this);
    QHBoxLayout *buttonLayout = new QHBoxLayout(buttonContainer);
    buttonLayout->setContentsMargins(15, 15, 15, 15);
    buttonLayout->setSpacing(0);

    createButton = new QPushButton("✨ Crea Media", buttonContainer);
    createButton->setFixedHeight(48);
    createButton->setStyleSheet(StyleUtils::getPrimaryButtonStyle());

    buttonLayout->addStretch();
    buttonLayout->addWidget(createButton);
    buttonLayout->addStretch();

    // --- Assemble the main layout ---
    mainLayout->addWidget(scrollArea, 1); // stretch factor = 1 per far espandere la scroll area
    mainLayout->addWidget(buttonContainer, 0); // stretch factor = 0 per mantenere l'altezza fissa del bottone

    setLayout(mainLayout);
    
    // Imposta la policy di dimensionamento per occupare tutto lo spazio disponibile
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // Connections
    connect(itemTypeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &CreateItemWidget::onItemTypeChanged);
    connect(createButton, &QPushButton::clicked,
            this, &CreateItemWidget::onCreateButtonClicked);

    // Initialize view for the first media type
    onItemTypeChanged(0);
    
    // Initial scroll bar visibility check
    QTimer::singleShot(0, this, &CreateItemWidget::updateScrollBarVisibility);
}

void CreateItemWidget::resizeEvent(QResizeEvent* event)
{
    QWidget::resizeEvent(event);
    updateScrollBarVisibility();
}

void CreateItemWidget::updateScrollBarVisibility()
{
    if (!scrollArea || !scrollArea->widget()) {
        return;
    }
    
    // Get the content height and viewport height
    int contentHeight = scrollArea->widget()->sizeHint().height();
    int viewportHeight = scrollArea->viewport()->height();
    
    // Only show scroll bar when content exceeds viewport
    if (contentHeight <= viewportHeight) {
        scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    } else {
        scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    }
}

void CreateItemWidget::addFieldWithPlaceholder(QVBoxLayout* layout, const QString& labelText, const QString& placeholder)
{
    if (labelText == "Immagine:") {
        // Special case for image path field with browse button
        QLabel* label = new QLabel(labelText);
        QHBoxLayout* hLayout = new QHBoxLayout();
        QLineEdit* input = new QLineEdit();
        input->setPlaceholderText(placeholder);
        input->setMinimumHeight(30);
        input->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

        QPushButton* browseButton = new QPushButton("Browse");
        browseButton->setFixedHeight(30);
        browseButton->setFixedWidth(70);

        hLayout->addWidget(input);
        hLayout->addWidget(browseButton);

        layout->addWidget(label);
        layout->addLayout(hLayout);

        // Connect browse button to open file dialog
        connect(browseButton, &QPushButton::clicked, this, [input, this]() {
            QString filePath = QFileDialog::getOpenFileName(
                this,
                tr("Seleziona immagine"),
                QDir::homePath(),
                tr("Immagini (*.png *.jpg *.jpeg *.bmp *.gif)")
            );
            if (!filePath.isEmpty()) {
                input->setText(filePath);
            }
        });
    } else {
        QLabel* label = new QLabel(labelText);
        QLineEdit* input = new QLineEdit();
        input->setPlaceholderText(placeholder);
        input->setMinimumHeight(30);
        input->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

        layout->addWidget(label);
        layout->addWidget(input);
    }
}

Media* CreateItemWidget::createMediaItem() const
{
    int index = itemTypeCombo->currentIndex();
    QWidget* currentPage = stackedFields->widget(index);
    QList<QLineEdit*> fields = currentPage->findChildren<QLineEdit*>();
    QString type = itemTypeCombo->currentText();

    if (!mediaService) {
        QMessageBox::critical(const_cast<QWidget*>(parentWidget()),
            "Errore",
            "MediaService non inizializzato");
        return nullptr;
    }

    return mediaService->createMediaFromFields(type, fields, const_cast<QWidget*>(static_cast<const QWidget*>(this)));
}

void CreateItemWidget::onCreateButtonClicked()
{
    // Prevent multiple rapid clicks
    if (!createButton->isEnabled()) {
        return;
    }
    
    createButton->setEnabled(false);
    
    Media* newItem = createMediaItem();
    if (newItem) {
        if (editMode && editingMedia) {
            // Modalità modifica
            emit itemUpdated(editingMedia, newItem);
            clearEditMode();
        } else {
            // Modalità creazione
            emit itemCreated(newItem);
        }

        // Clear all fields
        QWidget* currentPage = stackedFields->currentWidget();
        if (currentPage) {
            QList<QLineEdit*> fields = currentPage->findChildren<QLineEdit*>();
            for (QLineEdit* field : fields) {
                if (field) {
                    field->clear();
                }
            }
        }
    }
    
    // Re-enable button after a short delay to prevent rapid clicks
    QTimer::singleShot(500, this, [this]() {
        if (createButton) {
            createButton->setEnabled(true);
        }
    });
}

void CreateItemWidget::onItemTypeChanged(int index)
{
    stackedFields->setCurrentIndex(index);
    // Update scroll bar visibility when content changes
    QTimer::singleShot(0, this, &CreateItemWidget::updateScrollBarVisibility);
}

void CreateItemWidget::setEditMode(Media* media)
{
    if (!media) return;
    
    editingMedia = media;
    editMode = true;
    
    // Cambia il testo del bottone
    createButton->setText("🔄 Aggiorna Media");
    createButton->setStyleSheet(StyleUtils::getWarningButtonStyle());
    
    // Precompila i campi con i dati del media
    populateFieldsFromMedia(media);
}

void CreateItemWidget::clearEditMode()
{
    editingMedia = nullptr;
    editMode = false;
    
    // Ripristina il testo del bottone
    createButton->setText("✨ Crea Media");
    createButton->setStyleSheet(StyleUtils::getPrimaryButtonStyle());
}

bool CreateItemWidget::isInEditMode() const
{
    return editMode;
}

QString CreateItemWidget::getMediaTypeName(Media* media) const
{
    return MediaTypeUtils::getMediaTypeName(media);
}

void CreateItemWidget::populateFieldsFromMedia(Media* media)
{
    if (!media) return;
    
    // Determina il tipo di media e seleziona il combo box appropriato
    QString mediaType = getMediaTypeName(media);
    int typeIndex = itemTypeCombo->findText(mediaType);
    if (typeIndex != -1) {
        itemTypeCombo->setCurrentIndex(typeIndex);
        stackedFields->setCurrentIndex(typeIndex);
    }
    
    // Ottieni i campi della pagina corrente
    QWidget* currentPage = stackedFields->currentWidget();
    if (!currentPage) return;
    
    QList<QLineEdit*> fields = currentPage->findChildren<QLineEdit*>();
    if (fields.isEmpty()) return;
    
    // Campi comuni per tutti i media
    if (fields.size() > 0) fields[0]->setText(QString::fromStdString(media->getTitolo()));
    if (fields.size() > 1) fields[1]->setText(QString::fromStdString(media->getImmagine()));
    if (fields.size() > 2) fields[2]->setText(QString::number(media->getAnno()));
    
    // Campi specifici per tipo di media
    if (auto film = dynamic_cast<Film*>(media)) {
        if (fields.size() > 3) fields[3]->setText(QString::fromStdString(film->getRegista()));
        if (fields.size() > 4) fields[4]->setText(QString::fromStdString(film->getAttoreProtagonista()));
        if (fields.size() > 5) fields[5]->setText(QString::number(film->getDurata()));
    }
    else if (auto serie = dynamic_cast<SerieTv*>(media)) {
        if (fields.size() > 3) fields[3]->setText(QString::number(serie->getNumEpisodi()));
        if (fields.size() > 4) fields[4]->setText(QString::number(serie->getNumStagioni()));
        if (fields.size() > 5) fields[5]->setText(QString::number(serie->getDurataMediaEp()));
        if (fields.size() > 6) fields[6]->setText(serie->getInCorso() ? "true" : "false");
        if (fields.size() > 7) fields[7]->setText(QString::fromStdString(serie->getIdeatore()));
        if (fields.size() > 8) fields[8]->setText(QString::fromStdString(serie->getCasaProduttrice()));
    }
    else if (auto anime = dynamic_cast<Anime*>(media)) {
        if (fields.size() > 3) fields[3]->setText(QString::number(anime->getNumEpisodi()));
        if (fields.size() > 4) fields[4]->setText(QString::number(anime->getNumStagioni()));
        if (fields.size() > 5) fields[5]->setText(QString::number(anime->getDurataMediaEp()));
        if (fields.size() > 6) fields[6]->setText(anime->getInCorso() ? "true" : "false");
        if (fields.size() > 7) fields[7]->setText(QString::fromStdString(anime->getDisegnatore()));
        if (fields.size() > 8) fields[8]->setText(QString::fromStdString(anime->getStudioAnimazione()));
    }
    else if (auto libro = dynamic_cast<Libro*>(media)) {
        if (fields.size() > 3) fields[3]->setText(QString::fromStdString(libro->getScrittore()));
        if (fields.size() > 4) fields[4]->setText(QString::number(libro->getAnnoStampa()));
        if (fields.size() > 5) fields[5]->setText(QString::fromStdString(libro->getCasaEditrice()));
    }
    else if (auto manga = dynamic_cast<Manga*>(media)) {
        if (fields.size() > 3) fields[3]->setText(QString::fromStdString(manga->getScrittore()));
        if (fields.size() > 4) fields[4]->setText(QString::number(manga->getNumLibri()));
        if (fields.size() > 5) fields[5]->setText(QString::fromStdString(manga->getIllustratore()));
        if (fields.size() > 6) fields[6]->setText(manga->getConcluso() ? "true" : "false");
    }
    else if (auto cd = dynamic_cast<Cd*>(media)) {
        if (fields.size() > 3) fields[3]->setText(QString::fromStdString(cd->getArtista()));
        if (fields.size() > 4) fields[4]->setText(QString::number(cd->getNumTracce()));
        if (fields.size() > 5) fields[5]->setText(QString::number(cd->getDurataMedTracce()));
    }
}

void CreateItemWidget::setMediaService(MediaService* service)
{
    mediaService = service;
}
