#ifndef RIGHTLAYOUTWIDGET_H
#define RIGHTLAYOUTWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QVector>
#include "../../logic/media.h"
#include "../../services/jsonService.h"
#include "../../services/uiService.h"

class RightLayoutWidget : public QWidget
{
    Q_OBJECT

public:
    explicit RightLayoutWidget(QWidget *parent = nullptr);
    ~RightLayoutWidget();

    void setMediaCollection(const QVector<Media*>& collection);
    void setJsonService(JsonService* service);
    void setUIService(UIService* service);
    void displayMediaByCategory(const QString &category, const QString &searchText = "");
    void clearLayout();
    void showCreateItemWidget(QWidget* createWidget);

signals:
    void mediaEditRequested(Media* media);
    void mediaDeleteRequested(Media* media);

private:
    void addMediaCardToLayout(Media* media);

    QVBoxLayout *mainLayout;
    QVector<Media*> mediaCollection;
    JsonService *jsonService;
    UIService *uiService;
    QString currentCategory;
    QString currentSearchText;
};

#endif // RIGHTLAYOUTWIDGET_H
