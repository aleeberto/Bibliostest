#include "topMenuWidget.h"
#include <QHBoxLayout>
#include "../../utils/styleUtils.h"

TopMenuWidget::TopMenuWidget(QWidget* parent) : QWidget(parent) {
    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->setContentsMargins(16, 16, 16, 16);
    layout->setSpacing(12);

    // Pulsante Upload con testo moderno
    QPushButton* btnUpload = new QPushButton("📁 Carica", this);
    btnUpload->setToolTip("Carica una biblioteca esistente");
    btnUpload->setStyleSheet(StyleUtils::getSecondaryButtonStyle());
    btnUpload->setFixedHeight(44);

    // Pulsante Export con testo moderno  
    QPushButton* btnSave = new QPushButton("💾 Esporta", this);
    btnSave->setToolTip("Esporta la biblioteca corrente");
    btnSave->setStyleSheet(StyleUtils::getPrimaryButtonStyle());
    btnSave->setFixedHeight(44);

    // Pulsante Create con testo moderno
    QPushButton* btnCreate = new QPushButton("➕ Nuovo", this);
    btnCreate->setToolTip("Crea un nuovo elemento");
    btnCreate->setStyleSheet(StyleUtils::getSecondaryButtonStyle());
    btnCreate->setFixedHeight(44);

    // Connessioni dei pulsanti
    connect(btnUpload, &QPushButton::clicked, this, &TopMenuWidget::uploadRequested);
    connect(btnSave, &QPushButton::clicked, this, &TopMenuWidget::exportRequested);
    connect(btnCreate, &QPushButton::clicked, this, &TopMenuWidget::createRequested);

    layout->addWidget(btnUpload);
    layout->addWidget(btnSave);
    layout->addWidget(btnCreate);
    layout->addStretch();

    this->setLayout(layout);
}
