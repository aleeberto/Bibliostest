#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <QMap>
#include <QPushButton>
#include <QLineEdit>
#include "widget/topMenuWidget.h"
#include "widget/createItemWidget.h"
#include "widget/rightLayoutWidget.h"
#include "../services/jsonService.h"
#include "../services/mediaService.h"
#include "../services/uiService.h"
#include "../logic/media.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void showCreateItemWidget();
    void onMediaItemCreated(Media* newItem);
    void onMediaItemUpdated(Media* oldMedia, Media* newMedia);
    void onMediaEditRequested(Media* media);
    void onMediaDeleteRequested(Media* media);
    void handleUploadRequest();
    void handleExportRequest();
    void onSearchTextChanged(const QString& text);

private:
    void loadMediaData(const QString &filePath);
    void setupCategoryButtons();
    void setupSearchBar();

    TopMenuWidget* topMenu;
    CreateItemWidget* createItemWidget;
    RightLayoutWidget* rightLayoutWidget;
    QVBoxLayout *leftLayout;
    JsonService *jsonService;
    MediaService *mediaService;
    UIService *uiService;
    QMap<QString, QPushButton*> categoryButtons;
    QString currentJsonPath = "resources/data/data.json";
    QString currentCategory = "Tutti";
    QLineEdit *searchBar;
};

#endif // MAINWINDOW_H
