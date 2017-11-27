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
}
