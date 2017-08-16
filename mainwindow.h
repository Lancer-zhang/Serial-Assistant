#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSerialPort/QtSerialPort>
#include <QDebug>
#include "constant.h"
#include "structrue.h"
#include "tool.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void receivePortData();

    void on_serialPort_currentIndexChanged(int index);
    void on_BaudRate_currentIndexChanged(int index);
    void on_dataBits_currentIndexChanged(int index);
    void on_checkBits_currentIndexChanged(int index);
    void on_stopBits_currentIndexChanged(int index);

    void on_autoSave_stateChanged(int arg1);
    void on_autoSaveTime_currentIndexChanged(int index);
    void on_autoSend_stateChanged(int arg1);
    void on_autoSendTime_currentIndexChanged(int index);
    void on_receiveDataFormat_currentIndexChanged(int index);
    void on_sendDataFormat_currentIndexChanged(int index);
    void on_clearCountBtn_clicked();
    void on_stopDisplayBtn_clicked();
    void on_clearDataBtn_clicked();
    void on_saveDataBtn_clicked();

    void on_openBtn_clicked();

private:
    Ui::MainWindow *ui;
    QSerialPort *m_SerialPort;
    int m_DataFormat;
    int m_ReceiveCount;

    bool IsShow;//是否显示数据
    bool IsDebug;//是否启用调试,接收到数据后模拟发送数据
    bool IsAutoClear;//是否自动清空
    bool IsHex;

    void handleReceivedata(QByteArray temp);
    void decodeData(QByteArray temp);
    void sendPortData();
};

#endif // MAINWINDOW_H
