#ifndef SCREENSHOT_H
#define SCREENSHOT_H

#include <QObject>
#include <QWidget>
#include <QLabel>
#include <QPixmap>
#include <QPushButton>
#include <QCheckBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMouseEvent>
#include <QDebug>

class ScreenShot : public QWidget
{
    Q_OBJECT
public:
    ScreenShot();


private:
    QPixmap originalPixmap;

    QLabel *screenshotLabel;
    QCheckBox *hideThisWindowCheckBox;
    QPushButton *newScreenshotButton;
    QPushButton *newScreenVedioButton;
    QPushButton *SaveButton;
    QPushButton *ReadButton;

private slots:
    void newScreenShot();
    void newScreenVedio();
    void onSaveClicked();
    void onReadClicked();
    void onStateChanged(int);
};

#endif // SCREENSHOT_H
