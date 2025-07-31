#ifndef TOPMENU_HEADER
#define TOPMENU_HEADER

#include <QWidget>
#include <QPushButton>

class TopMenuWidget : public QWidget {
    Q_OBJECT
public:
    TopMenuWidget(QWidget* parent = nullptr);
    
signals:
    void uploadRequested();  // Carica un file JSON esistente
    void exportRequested();  // Esporta i dati in un nuovo file JSON
    void createRequested();  // Crea un nuovo media
};

#endif
