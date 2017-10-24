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
    m_Status = FIND_FRAME_START;
    m_ReceiveCount = 0;
    m_SendCount = 0;
    m_Plotter = new Plotter;
    ui->displayArea->setBackgroundRole(QPalette::Dark);   //displayArea对象的背景色设为Dark，能拉动的框
    ui->displayArea->setWidget(m_Plotter);     //将画布添加到scrollArea中
    ui->displayArea->widget()->setMinimumSize(500,300); //scrollArea初始化大小设为800*600
    PlotSettings settings;
    settings.minX = 0.0;
    settings.maxX = 100.0;
    settings.minY = 0.0;
    settings.maxY = 120.0;
    m_Plotter->setPlotSettings(settings);
    IsShowCh1 = false;
    IsShowCh2 = false;
    IsShowCh3 = false;
    setWindowTitle(tr("串口助手"));

    //发送数据
    m_AutoSendTimer = new QTimer(this);
    m_AutoSendTimer->setInterval(5000);
    connect(m_AutoSendTimer, SIGNAL(timeout()), this, SLOT(sendPortData()));

    //保存数据
    m_AutoSaveTimer = new QTimer(this);
    m_AutoSaveTimer->setInterval(5000);
    connect(m_AutoSaveTimer, SIGNAL(timeout()), this, SLOT(savePortData()));

    //200ms更新曲线
    m_PlotUpdateTimer=new QTimer(this);
    m_PlotUpdateTimer->setInterval(200);
    connect(m_PlotUpdateTimer,SIGNAL(timeout()),this,SLOT(showPlot()));

        flappybridd  = new FlappyBrid();
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
               switch (m_ReceiveDataFormat) {
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
           disconnect(m_SerialPort, SIGNAL(readyRead()), this, SLOT(receivePortData()));
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
        switch(m_ReceiveDataFormat){
        case Utf_8:
        {
            QString tempDataNormal = QString(temp);
            ui->receiveText->insertPlainText(tempDataNormal);
            break;
        }
        case AscII:
        {
            QString tempDataAscII = QString(temp).toLatin1();
            ui->receiveText->insertPlainText(tempDataAscII);
            break;
        }
        case Binary:
        {
            QString tempDataHex = tool::ByteArrayToHexStr(temp);
            QString tempDataBinary = tool::StrHexToStrBin(tempDataHex);
            ui->receiveText->insertPlainText(tempDataBinary);
            break;
        }
        case Octal:
            break;
        case Decimal:
        {
            QString tempDataHex = tool::ByteArrayToHexStr(temp);
            int Decimal = tool::StrHexToDecimal(tempDataHex);
            ui->receiveText->insertPlainText(QString::number(Decimal));
            break;
        }
        case Hexadecimal:
        {
            QString tempDataHex = tool::ByteArrayToHexStr(temp);
            ui->receiveText->insertPlainText(tempDataHex);
            break;
        }
        }
        m_ReceiveCount = m_ReceiveCount + temp.size();
        ui->label_8->setText(QString("接收:%1 字节").arg(m_ReceiveCount));
        ui->receiveText->insertPlainText("\n");
        ui->receiveText->moveCursor(QTextCursor::End);
    }
}
//解析数据的函数，有待进一步改善
void MainWindow::decodeData(QByteArray temp)
{
    int len = temp.length();
    for(int i=0;i<len;i++)
   {
        uint8_t byte=temp[i];
        if(m_Status == FIND_FRAME_START && byte == FRAME_START)// 看是否在找帧开始符，0x55。
        {
            m_Status = NEED_FRAME_HEAD;
            continue;
        }
        else if(m_Status == NEED_FRAME_HEAD)
        {
                       // 找到0x55后，下一个必须是0xAA，否则在次寻找0x55。
                       if(byte == FRAME_HEAD)
                           m_Status = NEED_FRAME_NUM;
                       else
                           m_Status = FIND_FRAME_START;
                       continue;
        }
                   else if(m_Status == NEED_FRAME_NUM)
                   {
                       // 找到0x55后，下一个必须是0xAA，否则在次寻找0x55。
                       if(byte <= FRAME_MAX_NUMBER)
                         {
                           m_DataNum = byte;
                           m_Status = NEED_FRAME_LENGTH;
                         }
                       else
                           m_Status = FIND_FRAME_START;
                       continue;
                   }
                   else if(m_Status == NEED_FRAME_LENGTH)
                   {
                           if(byte < FRAME_MAX_LENGTH)
                           {
                              m_DataLength = byte;
                              m_Status = DATA_START;

                           }
                            else
                            { // 帧错误，寻找下一帧。
                                m_Status = FIND_FRAME_START;
                             }
                           continue;
                   }
                   // [0,DATA_LEN_MAX-1]就表示正在读数据。
                   else if(m_Status < FRAME_MAX_LENGTH)
                   {
                       m_Buff[m_Status] = byte;
                       m_Status ++;

                       // 判断读完没有。
                       if(m_Status == m_DataLength + 1)
                       {
                           //帧接收完了，处理。
                          int sum =0;
                        for(int j=0;j<m_DataLength;j++)
                            sum += m_Buff[j];
                        if(sum==m_Buff[m_DataLength])//最后一位数据，是和校验
                        {
//                           value1 = (float)((my_buff[0]<<8)+my_buff[1])/100;
//                           value2 = (float)((my_buff[2]<<8)+my_buff[3])/100;
//                           value3 = (float)((my_buff[4]<<8)+my_buff[5])/100;
//                           Data_send(my_dataLength,my_buff,my_dataNum);

//                           ui->checkBox_4->setText(QString("通道 1：%1").arg(value1));
//                           ui->checkBox_5->setText(QString("通道 2：%1").arg(value2));
//                           ui->checkBox_6->setText(QString("通道 3：%1").arg(value3));
                        }   // 寻找下一帧。
                           m_Status = FIND_FRAME_START;
                       }
                   }
    }
}
/*************************************发送串口数据*********************************************/
void MainWindow::sendPortData()
{
    QString str = ui->sendText->text();
    if (str == "")
    {
        ui->sendText->setFocus();
        return;
    }//发送数据为空
    if (!m_SerialPort->isOpen())
    {
        return;
    }//串口没有打开
    QByteArray outData = str.toLatin1();
    int size = outData.size();
    switch(m_SendDataFormat){
    case Utf_8:
    case AscII:
    case Binary:
    case Octal:
    case Decimal:
        break;
    case Hexadecimal:
    {
        outData = tool::HexStrToByteArray(str);
        break;
    }
    }
    size = outData.size();
    m_SerialPort->write(outData);
    ui->receiveText->append(QString("发送:%1").arg(str));
    m_SendCount = m_SendCount + size;
    ui->label_7->setText(QString("发送:%1 字节").arg(m_SendCount));

//    if (IsAutoClear)
//    {
//        //ui->SendTextEdit->setCurrentIndex(-1);
//       // ui->SendTextEdit->setFocus();
//        ui->sendText->clear();
//    }
}

/*************************************保存串口数据*********************************************/
void MainWindow::savePortData()
{
    QString tempData = ui->receiveText->toPlainText();
    if (tempData=="")
    {
        return;
    }//如果没有内容则不保存
    QDateTime now = QDateTime::currentDateTime();
    QString name = now.toString("yyyyMMddHHmmss");
    QString fileName = name+".txt";

    QFile file(fileName);
    file.open(QFile::WriteOnly | QIODevice::Text);
    QTextStream out(&file);
    out<<tempData;
    file.close();
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
    bool IsAutoSave = (arg1 == 0 ? false : true);
    if (IsAutoSave)
    {
        this->m_AutoSaveTimer->start();
    }
    else
    {
        this->m_AutoSaveTimer->stop();
    }
    ui->autoSaveTime->setEnabled(IsAutoSave);
}

void MainWindow::on_autoSaveTime_currentIndexChanged(int index)
{
     m_AutoSaveTimer->setInterval(ui->autoSaveTime->currentText().toInt() * 1000);
}

void MainWindow::on_autoSend_stateChanged(int arg1)
{
    bool IsAutoSend = (arg1 == 0 ? false : true);
    if (IsAutoSend)
    {
        this->m_AutoSendTimer->start();
    }
    else
    {
        this->m_AutoSendTimer->stop();
    }
    ui->autoSendTime->setEnabled(IsAutoSend);
}

void MainWindow::on_autoSendTime_currentIndexChanged(int index)
{
    m_AutoSendTimer->setInterval(ui->autoSendTime->currentText().toInt() * 1000);
}

void MainWindow::on_receiveDataFormat_currentIndexChanged(int index)
{
    m_ReceiveDataFormat = index;
}

void MainWindow::on_sendDataFormat_currentIndexChanged(int index)
{
    m_SendDataFormat = index;
}
void MainWindow::on_clearCountBtn_clicked()
{
    m_SendCount = 0;
    m_ReceiveCount = 0;
    ui->label_7->setText("发送:0 字节");
    ui->label_8->setText("接收:0 字节");
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
    ui->receiveText->clear();
}

void MainWindow::on_saveDataBtn_clicked()
{
    savePortData();
}
/**********************************串口数据管理  end************************************************/
/**********************************曲线绘制设置  start************************************************/
void MainWindow::showPlot()
{
    QVector<QPointF> points0;
    QVector<QPointF> points1;
    QVector<QPointF> points2;
    for(int x=0;x<=100;x++)
    {
        if(IsShowCh1)  {
            points0.append(QPointF(x, uint(qrand()) % 120));
        }
        else  {
            points0.append(QPointF(x, 0));
        }
        if(IsShowCh2)  {
            points1.append(QPointF(x, uint(qrand()) % 120));
        }
        else  {
            points1.append(QPointF(x, 0));
        }
        if(IsShowCh3)  {
            points2.append(QPointF(x, uint(qrand()) % 120));
        }
        else  {
            points2.append(QPointF(x, 0));
        }
    }
    m_Plotter->setCurveData(0, points0);
    m_Plotter->setCurveData(1, points1);
    m_Plotter->setCurveData(2, points2);
}

void MainWindow::on_startShowBtn_clicked()
{
    if (ui->startShowBtn->text() == "停止绘制")
    {
        m_PlotUpdateTimer->stop();
        ui->startShowBtn->setText("开始绘制");
    }
    else
    {
        m_PlotUpdateTimer->start();
        ui->startShowBtn->setText("停止绘制");
    }

}

void MainWindow::on_aisle1_stateChanged(int arg1)
{
    IsShowCh1=(arg1==0?false:true);
}

void MainWindow::on_aisle2_stateChanged(int arg1)
{
    IsShowCh2=(arg1==0?false:true);
}

void MainWindow::on_aisle3_stateChanged(int arg1)
{
    IsShowCh3=(arg1==0?false:true);
}

void MainWindow::on_aisle4_stateChanged(int arg1)
{
}

/**********************************快乐小鸟  start************************************************/
void MainWindow::on_flappyBridBtn_clicked()
{
     if(m_SerialPort->isOpen())
     {
         m_SerialPort->close();
         disconnect(m_SerialPort, SIGNAL(readyRead()), this, SLOT(receivePortData()));
         ui->openBtn->setText(tr("打开串口"));
     }
     if (ui->startShowBtn->text() == "停止绘制")
     {
         m_PlotUpdateTimer->stop();
         ui->startShowBtn->setText("开始绘制");
     }
     flappybridd->show();
}
/**********************************快乐小鸟 end************************************************/
