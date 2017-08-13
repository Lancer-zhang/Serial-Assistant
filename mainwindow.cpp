#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtSerialPort/QtSerialPort>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) //获取可用的串口
    {
        ui->serialPort->addItem(info.portName());
    }

    m_SerialPort = new QSerialPort();
    setWindowTitle(tr("串口助手"));
}

MainWindow::~MainWindow()
{
    delete ui;
}
/*************************************打开串口**************************************************/
void MainWindow::on_openBtn_clicked()
{
    m_SerialPort->setPortName(ui->serialPort->currentText());                                                             //获取要打开的串口
       if(!m_SerialPort->isOpen())                                                                                                                               //如果之前是没有打开的则进行open动作
       {
           if(m_SerialPort->open(QIODevice::ReadWrite))                                                                                       //如果打开成功了，则打开的annuity置灰
           {
               /*******************************设置串口的默认参数**************************************/
               m_SerialPort->setBaudRate(ui->BaudRate->currentText().toInt()); //波特率
                //数据位
               switch (ui->dataBits->currentIndex()) {
               case 0:
                   m_SerialPort->setDataBits(QSerialPort::Data8);
                   break;
               case 1:
                   m_SerialPort->setDataBits(QSerialPort::Data7);
                   break;
               case 2:
                   m_SerialPort->setDataBits(QSerialPort::Data6);
                   break;
               case 3:
                   m_SerialPort->setDataBits(QSerialPort::Data5);
                   break;
               default:
                   break;
               }
               m_SerialPort->setFlowControl(QSerialPort::NoFlowControl);//无流控
               //停止位
               switch (ui->stopBits->currentIndex()) {
               case 0:
                   m_SerialPort->setStopBits(QSerialPort::OneStop);         //1位停止位
                   break;
               case 1:
                   m_SerialPort->setStopBits(QSerialPort::OneAndHalfStop);         //1.5位停止位
                   break;
               case 2:
                   m_SerialPort->setStopBits(QSerialPort::TwoStop);         //2位停止位
                   break;
               default:
                   m_SerialPort->setStopBits(QSerialPort::UnknownStopBits);
                   break;
               }
               //校验位
               switch (ui->checkBits->currentIndex()) {
               case 0:
                   m_SerialPort->setParity(QSerialPort::NoParity);          //无较验位
                   break;
               case 1:
                   m_SerialPort->setParity(QSerialPort::OddParity);          //奇较验位
                   break;
               case 2:
                   m_SerialPort->setParity(QSerialPort::EvenParity);          //偶较验位
                   break;
               default:
                   break;
               }
               m_SerialPort->setReadBufferSize(DATA_BUFFER);                    //设置数据缓冲区大小
               connect(m_SerialPort, SIGNAL(readyRead()), this, SLOT(readyReadSlot()));
               ui->openBtn->setText(tr("关闭串口"));
           }
           else
           {
               qDebug()<<m_SerialPort->errorString();
               this->statusBar()->showMessage(m_SerialPort->errorString());
           }
       }
       else                                          //如果串口处于打开的状态，则关闭
       {
           m_SerialPort->close();
           ui->openBtn->setText(tr("打开串口"));
       }
}
/*************************************串口设置  start*********************************************/
void MainWindow::on_serialPort_currentIndexChanged(int index)
{

}

void MainWindow::on_BaudRate_currentIndexChanged(int index)
{

}

void MainWindow::on_dataBits_currentIndexChanged(int index)
{

}

void MainWindow::on_checkBits_currentIndexChanged(int index)
{

}

void MainWindow::on_stopBits_currentIndexChanged(int index)
{

}
/*************************************串口设置  end************************************************/
/**********************************串口数据管理  start**********************************************/
void MainWindow::on_autoSave_stateChanged(int arg1)
{

}

void MainWindow::on_autoSaveTime_currentIndexChanged(int index)
{

}

void MainWindow::on_autoSend_stateChanged(int arg1)
{

}

void MainWindow::on_autoSendTime_currentIndexChanged(int index)
{

}

void MainWindow::on_receiveDataFormat_currentIndexChanged(int index)
{

}

void MainWindow::on_sendDataFormat_currentIndexChanged(int index)
{

}
void MainWindow::on_clearCountBtn_clicked()
{

}

void MainWindow::on_stopDisplayBtn_clicked()
{

}

void MainWindow::on_clearDataBtn_clicked()
{

}

void MainWindow::on_saveDataBtn_clicked()
{

}
/**********************************串口数据管理  end************************************************/

