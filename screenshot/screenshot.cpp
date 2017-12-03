#include "screenshot.h"

ScreenShot::ScreenShot()
{
    screenshotLabel = new QLabel(this);
    QVBoxLayout *vLayout = new QVBoxLayout();
    vLayout->addWidget(screenshotLabel);
    hideThisWindowCheckBox = new QCheckBox(tr("隐藏窗口"),this);
    hideThisWindowCheckBox->setMaximumHeight(60);
    vLayout->addWidget(hideThisWindowCheckBox);
    QHBoxLayout *hLayout = new QHBoxLayout();
    newScreenshotButton = new QPushButton(tr("截图"), this);
    newScreenVedioButton = new QPushButton(tr("录制"), this);
    SaveButton = new QPushButton(tr("保存"), this);
    ReadButton = new QPushButton(tr("读取"), this);
    hLayout->addWidget(newScreenshotButton);
    hLayout->addWidget(newScreenVedioButton);
    hLayout->addWidget(SaveButton);
    hLayout->addWidget(ReadButton);
    vLayout->addLayout(hLayout);
    this->setLayout(vLayout);

    connect(this->newScreenshotButton, SIGNAL(clicked()), this, SLOT(newScreenShot()));
    connect(this->newScreenVedioButton, SIGNAL(clicked()), this, SLOT(newScreenVedio()));
    connect(this->SaveButton, SIGNAL(clicked()), this, SLOT(onSaveClicked()));
    connect(this->ReadButton, SIGNAL(clicked()), this, SLOT(onReadClicked()));
    connect(this->hideThisWindowCheckBox, SIGNAL(stateChanged(int)), this, SLOT(onStateChanged(int)));

}

void ScreenShot::newScreenShot()
{
    qDebug()<<"截图";
    QScreen *screen = QGuiApplication::primaryScreen();
    if (const QWindow *window = windowHandle())
        screen = window->screen();
    if (!screen)
        return;
//    if(isHide)
//    {
//        this->hide();
//    }
    originalPixmap = screen->grabWindow(0);


    m_ScreenCanvas = new ScreenCanvas();     //创建画布
    m_ScreenCanvas->setBackgroundPixmap(originalPixmap);  //传递全屏背景图片
    connect(m_ScreenCanvas, SIGNAL(shotScreenFinish(QPixmap)), this, SLOT(getShotScreenPixmap(QPixmap)));
}

void ScreenShot::getShotScreenPixmap(QPixmap pixmap)
{
    screenshotLabel->setPixmap(pixmap.scaled(screenshotLabel->size(),
                                                     Qt::KeepAspectRatio,
                                                     Qt::SmoothTransformation));

}

void ScreenShot::newScreenVedio()
{
    qDebug()<<"录屏";
}

void ScreenShot::onSaveClicked()
{
    qDebug()<<"保存";
}

void ScreenShot::onReadClicked()
{
    qDebug()<<"读取";
}

void ScreenShot::onStateChanged(int state)
{
    qDebug()<<"隐藏"<<state;
    isHide = (state == 0 ? false : true);
}
