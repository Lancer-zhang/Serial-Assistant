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
#include <QScreen>
#include <QGuiApplication>
#include <QWindow>
#include <QApplication>
#include <QDesktopWidget>
#include "screencanvas.h"
#include <QTime>
#include <QClipboard>
#include <QFileDialog>
#include <QFile>
#include <QBuffer>

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

    ScreenCanvas *m_ScreenCanvas;
    bool isHide;
    QClipboard * m_Clipboard;

private slots:
    void newScreenShot();
    void newScreenVedio();
    void onSaveClicked();
    void onReadClicked();
    void onStateChanged(int);

    void getShotScreenPixmap(QPixmap pixmap);

signals:
    void hideApplication();
    void showApplication();
};

#endif // SCREENSHOT_H
