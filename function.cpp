#include "MainWindow.h"
#include "ui_mainwindow.h"
#include <QBarSet>
#include <QTimer>
void MainWindow::searchSerialPort()
{
    ui->setPort->clear();
    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        QSerialPort ser;
        ser.clear();
        ser.setPort(info);
        if(ser.open(QIODevice::ReadWrite))
        {
            ui->setPort->addItem(ser.portName());
            ser.close();
        }
    }
}


void MainWindow::openButton()
{
    if(ui->openButton->text()==tr("打开串口"))
    {
        //searchSerialPort();




        //设置串口名
        serial->setPortName(ui->setPort->currentText());
        //serial->setPortName(ui->setPort->currentText());


        //设置波特率
        serial->setBaudRate(ui->setBaudRate->currentText().toInt(),QSerialPort::AllDirections);

        //设置数据位数
        switch(ui->setDataBits->currentIndex())
        {
            case 0:serial->setDataBits(QSerialPort::Data8);break;
            case 1:serial->setDataBits(QSerialPort::Data7);break;
            case 2:serial->setDataBits(QSerialPort::Data6);break;
            case 3:serial->setDataBits(QSerialPort::Data5);break;
            default: break;
        }


        //设置奇偶校验
        switch(ui->setParitycheck->currentIndex())
        {
        case 0: serial->setParity(QSerialPort::NoParity); break;
        case 1: serial->setParity(QSerialPort::OddParity); break;
        case 2: serial->setParity(QSerialPort::EvenParity); break;
        default: break;
        }
        //设置停止位
        switch(ui->setStopBits->currentIndex())
        {
        case 0: serial->setStopBits(QSerialPort::OneStop); break;
        case 1: serial->setStopBits(QSerialPort::OneAndHalfStop); break;
        case 2: serial->setStopBits(QSerialPort::TwoStop); break;
        default: break;
        }
        //设置流控制
        serial->setFlowControl(QSerialPort::NoFlowControl);
        //serial->setFlowControl(QSerialPort::SoftwareControl);
        //关闭设置菜单使能
        //ui->PortBox->setEnabled(false);
        ui->setBaudRate->setEnabled(false);
        ui->setStopBits->setEnabled(false);
        ui->setParitycheck->setEnabled(false);
        ui->setDataBits->setEnabled(false);
        ui->setPort->setEnabled(false);
        ui->searchButton->setEnabled(false);

        //ui->SendButton->setEnabled(true);

        ui->openButton->setText(tr("关闭串口"));
        //连接信号槽
        QObject::connect(serial,SIGNAL(readyRead()),this,SLOT (Read_Data()),Qt::UniqueConnection);

        //打开串口
        serial->open(QIODevice::ReadWrite);
    }
    else
    {
        //关闭串口
        serial->clear();
        serial->close();
        //恢复设置使能
        ui->setBaudRate->setEnabled(true);
        ui->setStopBits->setEnabled(true);
        ui->setParitycheck->setEnabled(true);
        ui->setDataBits->setEnabled(true);
        ui->setPort->setEnabled(true);
        ui->searchButton->setEnabled(true);

        //ui->sendButton->setEnabled(false);

        ui->openButton->setText(tr("打开串口"));
        QObject::disconnect(serial,SIGNAL(readyRead()),this,SLOT (Read_Data()));
    }


    exitButtonClick();//命令小车退出接收模式.进入选择等待模式.
}
                    //QString str;
                    //QString valfirst = "#-_-#";
                    //QString vallast = "#^_^#";
                    //QString val;
                    //int indexFirst;
                    //int indexLast;
                    //double dou;

                    //if(RceiveStr.isNull())RceiveStr =ui->ReceivePlainTextEdit->toPlainText();
                    //RceiveStr+=QString::fromLocal8Bit(Rceivebuff);
                    //str=QString::fromLocal8Bit(Rceivebuff);

                    //indexFirst=str.indexOf(valfirst,0);
                    //indexLast=str.indexOf(vallast,0);
                    //val=str.mid(indexFirst+5,indexLast-5-indexFirst);

                    //dou=val.toDouble();
//读取接收到的数据
void MainWindow::Read_Data()
{
    QByteArray Rceivebuff;
    short k;
    Rceivebuff = serial->readAll();
    if(!Rceivebuff.isEmpty())
    {
        if((Rceivebuff[0]==(char)0xFE)&&(Rceivebuff[1]==(char)0x01)&&(Rceivebuff[2]==(char)0xFE)&&(Rceivebuff[3]==(char)0x01))
        {
                k=((Rceivebuff[5])<<8)+(unsigned char)Rceivebuff[6];
                set0->replace(0,(int)k);
                k=((Rceivebuff[7])<<8)+(unsigned char)Rceivebuff[8];
                set0->replace(1,(int)k);
                k=((Rceivebuff[9])<<8)+(unsigned char)Rceivebuff[10];
                set0->replace(2,(int)k);
                Bar->append(set0);

                k=((Rceivebuff[11])<<8)+(unsigned char)Rceivebuff[12];
                airset0->replace(0,(int)k);
                airBar->append(airset0);

                RceiveStr.clear();         
        }
        else
        {
            if(RceiveStr.isNull())RceiveStr =ui->ReceivePlainTextEdit->toPlainText();
            RceiveStr+=QString::fromLocal8Bit(Rceivebuff);
            ui->ReceivePlainTextEdit->clear();
            ui->ReceivePlainTextEdit->appendPlainText(RceiveStr);
            RceiveStr.clear();
        }
    }
    Rceivebuff.clear();
}
//void MainWindow::StartfreshenButtonClick()
//{
//    if(ui->freshenButton->text()==tr("开始刷新"))
//    {
//        ui->backButton->setEnabled(false);
//        ui->LeftButton->setEnabled(false);
//        ui->rightButton->setEnabled(false);
//        ui->forwardButton->setEnabled(false);
//        ui->stopButton->setEnabled(false);

//        timer->start(100);
//        ui->freshenButton->setText(tr("暂停刷新"));
//    }
//    else
//    {
//        ui->backButton->setEnabled(true);
//        ui->LeftButton->setEnabled(true);
//        ui->rightButton->setEnabled(true);
//        ui->forwardButton->setEnabled(true);
//        ui->stopButton->setEnabled(true);

//        timer->stop();
//        ui->freshenButton->setText(tr("开始刷新"));
//    }
//    //freshenButtonClick();
//}

void MainWindow::clearButtonClick()
{
    ui->ReceivePlainTextEdit->clear();
}

////刷新
//void MainWindow::freshenButtonClick()
//{

//    QByteArray buf;

//    ui->checkBox_1->setText("nrf发送模式");
//    ui->checkBox_2->setText("nrf接收模式");

//    buf.append(0xFE);
//    buf.append(0x01);
//    buf.append(0xFE);
//    buf.append(0x01);

//    buf.append(0x13);
//    buf.append(0x13);
//    buf.append(0x13);
//    buf.append(0x13);
//    serial->write(buf);
//}

void MainWindow::leftButtonClick()
{
    QByteArray buf;
    if((ui->checkBox_1->isChecked())&&!(ui->checkBox_2->isChecked()))
    {
        buf.append(0xFE);
        buf.append(0x01);
        buf.append(0xFE);
        buf.append(0x01);

        buf.append(0x01);
        buf.append(0x01);
        buf.append(0x01);
        buf.append(0x01);
        serial->write(buf);
    }
    else if(!(ui->checkBox_1->isChecked())&&(ui->checkBox_2->isChecked()))
    {
        buf.append(0xFE);
        buf.append(0x01);
        buf.append(0xFE);
        buf.append(0x01);

        buf.append(0x09);
        buf.append(0x09);
        buf.append(0x09);
        buf.append(0x09);
        serial->write(buf);
    }
    else if((ui->checkBox_1->isChecked())&&(ui->checkBox_2->isChecked()))
    {
        buf.append(0xFE);
        buf.append(0x01);
        buf.append(0xFE);
        buf.append(0x01);

        buf.append(0x0E);
        buf.append(0x0E);
        buf.append(0x0E);
        buf.append(0x0E);
        serial->write(buf);
    }

}
void MainWindow::rightButtonClick()
{
    QByteArray buf;
    if((ui->checkBox_1->isChecked())&&!(ui->checkBox_2->isChecked()))
    {
        buf.append(0xFE);
        buf.append(0x01);
        buf.append(0xFE);
        buf.append(0x01);

        buf.append(0x03);
        buf.append(0x03);
        buf.append(0x03);
        buf.append(0x03);
        serial->write(buf);
    }
    else if(!(ui->checkBox_1->isChecked())&&(ui->checkBox_2->isChecked()))
    {
        buf.append(0xFE);
        buf.append(0x01);
        buf.append(0xFE);
        buf.append(0x01);

        buf.append(0x0B);
        buf.append(0x0B);
        buf.append(0x0B);
        buf.append(0x0B);
        serial->write(buf);
    }
    else if((ui->checkBox_1->isChecked())&&(ui->checkBox_2->isChecked()))
    {
        buf.append(0xFE);
        buf.append(0x01);
        buf.append(0xFE);
        buf.append(0x01);

        buf.append(0x10);
        buf.append(0x10);
        buf.append(0x10);
        buf.append(0x10);
        serial->write(buf);
    }

}
void MainWindow::forwarduttonClick()
{
    QByteArray buf;
    if((ui->checkBox_1->isChecked())&&!(ui->checkBox_2->isChecked()))
    {
        buf.append(0xFE);
        buf.append(0x01);
        buf.append(0xFE);
        buf.append(0x01);

        buf.append(0x04);
        buf.append(0x04);
        buf.append(0x04);
        buf.append(0x04);
        serial->write(buf);
    }
    else if(!(ui->checkBox_1->isChecked())&&(ui->checkBox_2->isChecked()))
    {
        buf.append(0xFE);
        buf.append(0x01);
        buf.append(0xFE);
        buf.append(0x01);

        buf.append(0x0C);
        buf.append(0x0C);
        buf.append(0x0C);
        buf.append(0x0C);
        serial->write(buf);
    }
    else if((ui->checkBox_1->isChecked())&&(ui->checkBox_2->isChecked()))
    {
        buf.append(0xFE);
        buf.append(0x01);
        buf.append(0xFE);
        buf.append(0x01);

        buf.append(0x11);
        buf.append(0x11);
        buf.append(0x11);
        buf.append(0x11);
        serial->write(buf);
    }

}
void MainWindow::backButtonClick()
{
    QByteArray buf;
    if((ui->checkBox_1->isChecked())&&!(ui->checkBox_2->isChecked()))
    {
        buf.append(0xFE);
        buf.append(0x01);
        buf.append(0xFE);
        buf.append(0x01);

        buf.append(0x02);
        buf.append(0x02);
        buf.append(0x02);
        buf.append(0x02);
        serial->write(buf);
    }
    else if(!(ui->checkBox_1->isChecked())&&(ui->checkBox_2->isChecked()))
    {
        buf.append(0xFE);
        buf.append(0x01);
        buf.append(0xFE);
        buf.append(0x01);

        buf.append(0x0A);
        buf.append(0x0A);
        buf.append(0x0A);
        buf.append(0x0A);
        serial->write(buf);
    }
    else if((ui->checkBox_1->isChecked())&&(ui->checkBox_2->isChecked()))
    {
        buf.append(0xFE);
        buf.append(0x01);
        buf.append(0xFE);
        buf.append(0x01);

        buf.append(0x0F);
        buf.append(0x0F);
        buf.append(0x0F);
        buf.append(0x0F);
        serial->write(buf);
    }

}

void MainWindow::stopButtonClick()
{
    QByteArray buf;
    if((ui->checkBox_1->isChecked())&&!(ui->checkBox_2->isChecked()))
    {
        buf.append(0xFE);
        buf.append(0x01);
        buf.append(0xFE);
        buf.append(0x01);

        buf.append(0x05);
        buf.append(0x05);
        buf.append(0x05);
        buf.append(0x05);
        serial->write(buf);
    }
    else if(!(ui->checkBox_1->isChecked())&&(ui->checkBox_2->isChecked()))
    {
        buf.append(0xFE);
        buf.append(0x01);
        buf.append(0xFE);
        buf.append(0x01);

        buf.append(0x0D);
        buf.append(0x0D);
        buf.append(0x0D);
        buf.append(0x0D);
        serial->write(buf);
    }
    else if((ui->checkBox_1->isChecked())&&(ui->checkBox_2->isChecked()))
    {
        buf.append(0xFE);
        buf.append(0x01);
        buf.append(0xFE);
        buf.append(0x01);

        buf.append(0x12);
        buf.append(0x12);
        buf.append(0x12);
        buf.append(0x12);
        serial->write(buf);
    }

}

void MainWindow::selectButtonClick()
{
    QByteArray buf;
    if((ui->checkBox_1->isChecked())&&(ui->checkBox_2->isChecked()))
    {
        buf.append(0xFE);
        buf.append(0x01);
        buf.append(0xFE);
        buf.append(0x01);

        buf.append(0x09);
        buf.append(0x09);
        buf.append(0x09);
        buf.append(0x09);

        serial->write(buf);
    }
    else if((ui->checkBox_1->isChecked())&&!(ui->checkBox_2->isChecked()))
    {
        buf.append(0xFE);
        buf.append(0x01);
        buf.append(0xFE);
        buf.append(0x01);

        buf.append(0x06);
        buf.append(0x06);
        buf.append(0x06);
        buf.append(0x06);

        serial->write(buf);
    }
    else if(!(ui->checkBox_1->isChecked())&&(ui->checkBox_2->isChecked()))
    {
        buf.append(0xFE);
        buf.append(0x01);
        buf.append(0xFE);
        buf.append(0x01);

        buf.append(0x07);
        buf.append(0x07);
        buf.append(0x07);
        buf.append(0x07);

        serial->write(buf);
    }

    ui->checkBox_1->setText("本地设备");
    ui->checkBox_2->setText("远程设备");
}
void MainWindow::exitButtonClick()
{
    QByteArray buf;
    buf.append(0xFE);
    buf.append(0x01);
    buf.append(0xFE);
    buf.append(0x01);

    buf.append(0x08);
    buf.append(0x08);
    buf.append(0x08);
    buf.append(0x08);

    ui->checkBox_1->setText("nrf发送模式");
    ui->checkBox_2->setText("nrf接收模式");
    serial->write(buf);
}
