#include "mainwindow.h"
#include "ui_mainwindow.h"

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
               switch (m_DataFormat) {
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
               connect(m_SerialPort, SIGNAL(readyRead()), this, SLOT(receivePortData()));
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


/*************************************读取串口数据*********************************************/
void MainWindow::receivePortData()
{
    //这个判断尤为重要,否则的话直接延时再接收数据,空闲时和会出现高内存占用
    if (m_SerialPort->bytesAvailable() <= 0)
    {
        return;
    }
    tool::Sleep(DELAY_100MS);//延时100毫秒保证接收到的是一条完整的数据,而不是脱节的
    QByteArray temp = m_SerialPort->readAll(); //读取串口缓冲区的所有数据给临时变量temp
    ui->receiveText->insertPlainText(temp); //将串口的数据显示在窗口的文本浏览器中
    handleReceivedata(temp);
}

void MainWindow::handleReceivedata(QByteArray temp)
{
    if(IsShow)
    {
        ui->receiveText->setTextColor(Qt::lightGray);
        ui->receiveText->insertPlainText(tr("接收: "));
        ui->receiveText->setTextColor(Qt::black);
        switch(m_DataFormat)
        {
        case Utf_8:
        {
            QString tempDataNormal = QString(temp);
            ui->receiveText->insertPlainText(tempDataNormal);
        }
            break;
        case AscII:
            break;
        case Binary:
            break;
        case Octal:
            break;
        case Decimal:
            break;
        case Hexadecimal:
        {
            QString tempDataHex = tool::ByteArrayToHexStr(temp);
            ui->receiveText->insertPlainText(tempDataHex);
        }
            break;
        }
        m_ReceiveCount = m_ReceiveCount + temp.size();
        ui->label_8->setText(QString("接收:%1 字节").arg(m_ReceiveCount));
        ui->receiveText->insertPlainText("\n");
        ui->receiveText->moveCursor(QTextCursor::End);
    }
}

void MainWindow::decodeData(QByteArray temp)
{
//    int len = temp.length();
//    for(int i=0;i<len;i++)
//   {
//        uint8_t byte=temp[i];
//        if(my_status == FINDING_55)
//                   {
//                       // 看是否在找帧开始符，0x55。
//                       if(byte == 0xAA)
//                           my_status = NEED_AA;
//                   }
//                   else if(my_status == NEED_AA)
//                   {
//                       // 找到0x55后，下一个必须是0xAA，否则在次寻找0x55。
//                       if(byte == 0xAA)
//                           my_status = NEED_NUM;
//                       else
//                           my_status = FINDING_55;
//                   }
//                   else if(my_status == NEED_NUM)
//                   {
//                       // 找到0x55后，下一个必须是0xAA，否则在次寻找0x55。
//                       if(byte == 0x01)
//                         {
//                           my_dataNum = byte;
//                           my_status = NEED_LENGTH;
//                         }
//                       else
//                           my_status = FINDING_55;
//                   }

//                   else if(my_status == NEED_LENGTH)
//                   {
//                           if(byte < DATA_LEN_MAX)
//                           {
//                              my_dataLength = byte;
//                              my_status = DATA_START;

//                           }
//                            else
//                            {
//                // 帧错误，寻找下一帧。
//                                my_status = FINDING_55;
//                             }
//                   }
//                   // [0,DATA_LEN_MAX-1]就表示正在读数据。
//                   else if(my_status < DATA_LEN_MAX)
//                   {
//                       my_buff[my_status] = byte;
//                       my_status ++;

//                       // 判断读完没有。
//                       if(my_status == my_dataLength+1)
//                       {
//                           //帧接收完了，处理。
//                          int sum =0;
//                        for(int j=0;j<my_dataLength;j++)
//                            sum += my_buff[j];
//                        if(sum==my_buff[my_dataLength])
//                        {
//                           value1 = (float)((my_buff[0]<<8)+my_buff[1])/100;
//                           value2 = (float)((my_buff[2]<<8)+my_buff[3])/100;
//                           value3 = (float)((my_buff[4]<<8)+my_buff[5])/100;
//                           Data_send(my_dataLength,my_buff,my_dataNum);

//                           ui->checkBox_4->setText(QString("通道 1：%1").arg(value1));
//                           ui->checkBox_5->setText(QString("通道 2：%1").arg(value2));
//                           ui->checkBox_6->setText(QString("通道 3：%1").arg(value3));
//                        }   // 寻找下一帧。
//                           my_status = FINDING_55;
//                       }
//                   }
//    }
}
/*************************************发送串口数据*********************************************/
void MainWindow::sendPortData()
{
    m_SerialPort->write(ui->sendText->text().toLatin1()); //以ASCII码形式将数据写入串口
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
    m_DataFormat = index;
}

void MainWindow::on_sendDataFormat_currentIndexChanged(int index)
{

}
void MainWindow::on_clearCountBtn_clicked()
{

}

void MainWindow::on_stopDisplayBtn_clicked()
{
    if (ui->stopDisplayBtn->text() == "停止显示")
    {
        IsShow = false;
        ui->stopDisplayBtn->setText("开始显示");
    }
    else
    {
        IsShow = true;
        ui->stopDisplayBtn->setText("停止显示");
    }
}

void MainWindow::on_clearDataBtn_clicked()
{

}

void MainWindow::on_saveDataBtn_clicked()
{

}
/**********************************串口数据管理  end************************************************/

