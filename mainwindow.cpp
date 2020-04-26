#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <dialog.h>
#include <dialog.cpp>

using namespace std ;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Window|Qt::WindowMinimizeButtonHint|Qt::WindowCloseButtonHint);//al parecer aquí hay algún error en el qt que confunde los botones

    connect(ui->pushButton,SIGNAL(clicked(bool)), this, SLOT(ConnectionToDevice()));
    connect(ui->pushButton_2,SIGNAL(clicked(bool)),this, SLOT(DisconnectDevice()));
    connect(&client, SIGNAL(stateChanged(QModbusDevice::State)), this, SLOT(stateChanged(QModbusDevice::State)));
    connect(ui->comboBox, SIGNAL(currentIndexChanged(int)), this,SLOT(indexChanged(int)) );
    indexChanged(0);
    connect(ui->pushButton_3,SIGNAL(clicked(bool)),this, SLOT(setCoilsStatus()));
    connect(ui->pushButton_4,SIGNAL(clicked(bool)),this,SLOT(setHoldingRegisters()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

QString MainWindow::ipaddress() const
{
return m_ipaddress;
}

void MainWindow::set_ipaddress(const QString &ipaddress)
{
    m_ipaddress = ipaddress;
}

qint64 MainWindow::port() const
{
 return m_port;
}

void MainWindow::set_port(const qint64 port)
{
    m_port = port;
}

qint64 MainWindow::server() const
{
 return m_server;
}

void MainWindow::set_server(const qint64 server)
{
    m_server = server;
}

qint64 MainWindow::startAddress() const
{
    return m_startAddress;

}

void MainWindow::set_startAddress(const qint64 startAddress)
{
    m_startAddress = startAddress;
}

qint64 MainWindow::values() const
{
    return m_values;
}

void MainWindow::set_values(const qint64 values)
{
    m_values = values;
}

void MainWindow::ConnectionToDevice()
{
    m_ipaddress = ui->lineEdit->text();
    m_port = ui->lineEdit_2->text().toInt();


    client.setConnectionParameter(QModbusDevice::NetworkPortParameter, m_port);
    client.setConnectionParameter(QModbusDevice::NetworkAddressParameter, m_ipaddress);

    client.connectDevice();

}

void MainWindow::DisconnectDevice()
{
    client.disconnectDevice();
}


void MainWindow::getValues()
{

   m_server = ui->spinBox->value();
   m_startAddress = ui->spinBox_2->value();
   m_values = ui->spinBox_3->value();

   if (rt == 0) {
   ui->spinBox_3->setMaximum(20);
   QModbusDataUnit dataUnit(QModbusDataUnit::Coils, m_startAddress, m_values);
   auto reply = client.sendReadRequest(dataUnit, m_server);
   connect(reply, SIGNAL(finished()), this, SLOT(showCoils()));
 }

   if (rt == 1)
   {
    QModbusDataUnit dataUnit(QModbusDataUnit::HoldingRegisters, m_startAddress, m_values);
  //  dataUnit.setStartAddress(m_startAddress);
  //  dataUnit.setValueCount(m_values);
    auto reply = client.sendReadRequest(dataUnit, m_server);
    connect(reply, SIGNAL(finished()), this, SLOT(showHoldingRegisters()));
   }

    if (rt == 2)
    {
     QModbusDataUnit dataUnit(QModbusDataUnit::DiscreteInputs, m_startAddress, m_values);
   //  dataUnit.setStartAddress(m_startAddress);
   //  dataUnit.setValueCount(m_values);
     auto reply = client.sendReadRequest(dataUnit, m_server);
     connect(reply, SIGNAL(finished()), this, SLOT(showInputStatus()));
    }
     if (rt == 3){
      QModbusDataUnit dataUnit(QModbusDataUnit::InputRegisters, m_startAddress, m_values);
    //  dataUnit.setStartAddress(m_startAddress);
    //  dataUnit.setValueCount(m_values);
      auto reply = client.sendReadRequest(dataUnit, m_server);
      connect(reply, SIGNAL(finished()), this, SLOT(showInputRegisters()));
   }


}

void MainWindow::stateChanged(QModbusDevice::State state)
{
    QString s;
    switch(state)
    {
        case  QModbusDevice::UnconnectedState:
            s = "Dispositivo desconectado";
             ui->pushButton->setEnabled(1);
        break;
        case QModbusDevice::ConnectingState:
            s = "Conectandose";
        break;
        case QModbusDevice::ConnectedState:
            s = "Dispositivo conectado";
            startRequest();
            ui->pushButton->setEnabled(0);
        break;
        case QModbusDevice::ClosingState:
            s = "Cerrando";
        break;
    }
    ui->statusBar->showMessage(s);


}



void MainWindow::showCoils()
{
    QModbusReply* reply = qobject_cast<QModbusReply*>(sender());
    if (!reply) return;
    QModbusDataUnit data = reply->result();

    ui->groupBox_4->setVisible(true);
    ui->listWidget->clear();

        ui->checkBox->setChecked(data.value(0));
        ui->checkBox_2->setChecked(data.value(1));
        ui->checkBox_3->setChecked(data.value(2));
        ui->checkBox_4->setChecked(data.value(3));
        ui->checkBox_5->setChecked(data.value(4));
        ui->checkBox_6->setChecked(data.value(5));
        ui->checkBox_7->setChecked(data.value(6));
        ui->checkBox_8->setChecked(data.value(7));
        ui->checkBox_9->setChecked(data.value(8));

        ui->checkBox_10->setChecked(data.value(9));
        ui->checkBox_11->setChecked(data.value(10));
        ui->checkBox_12->setChecked(data.value(11));
        ui->checkBox_13->setChecked(data.value(12));
        ui->checkBox_14->setChecked(data.value(13));
        ui->checkBox_15->setChecked(data.value(14));
        ui->checkBox_16->setChecked(data.value(15));
        ui->checkBox_13->setChecked(data.value(16));
        ui->checkBox_14->setChecked(data.value(17));
        ui->checkBox_15->setChecked(data.value(18));
        ui->checkBox_16->setChecked(data.value(19));


}

void MainWindow::showHoldingRegisters()
{
    QModbusReply* reply = qobject_cast<QModbusReply*>(sender());
    if (!reply) return;
    QModbusDataUnit data1 = reply->result();

          ui->listWidget->clear();
          ui->groupBox_4->setVisible(false);
          for (qint16 i = 0 ; i < m_values ; i++){
          ui->listWidget->addItem(QString::number(data1.value(i)));
           }
   reply->deleteLater();

}

void MainWindow::setCoilsStatus()
{
 Dialog d;
    qint64 valor;
    qint64 pos;

    if (d.exec())
    {

       //Ui_Dialog.lineEdit->setVisible(false);
     //   pos = Ui_Dialog.spinBox->value();
     //   valor = Ui_Dialog.spinBox_2->value();


    }
    QModbusDataUnit dataUnit;
    dataUnit.setRegisterType(QModbusDataUnit::Coils) ;
    dataUnit.setStartAddress(pos);
    //dataUnit.setValueCount(1);
    QVector<quint16> vec;
    vec << valor;
    dataUnit.setValues(vec);
    client.sendWriteRequest(dataUnit, m_server);

     auto reply = client.sendReadRequest(dataUnit, m_server);
   if (reply->isFinished())
    {
        ui->statusBar->showMessage("Success Write");
    }


}

void MainWindow::setHoldingRegisters()
{
 Dialog e;
    qint64 valor;
    qint64 pos;

    if (e.exec())
    {

    //    Ui_Dialog.spinBox_2->setVisible(false);
    //    pos = Ui_Dialog.spinBox->value();
    //    valor = Ui_Dialog.lineEdit->value();
    }



    QModbusDataUnit dataUnit;
    dataUnit.setRegisterType(QModbusDataUnit::Coils) ;
    dataUnit.setStartAddress(pos);
    //dataUnit.setValueCount(1);
    QVector<quint16> vec;
    vec << valor;
    dataUnit.setValues(vec);
    client.sendWriteRequest(dataUnit, m_server);

    auto reply = client.sendReadRequest(dataUnit, m_server);
   if (reply->isFinished())
    {
        ui->statusBar->showMessage("Success Write");
    }

}


void MainWindow::indexChanged(int val)
{

if (val == 0)  {  // Coils
    rt = 0;
}
else if (val == 1){ //Holding Registers
    rt = 1;
}
else if (val == 2){ //Input States
    rt = 2;
}
else if (val == 3){ //Input Registers
    rt = 3;
}

}


void MainWindow::startRequest()
{
    connect(&requestTimer, SIGNAL(timeout()), this, SLOT(getValues()));
    requestTimer.start(100);
}

void MainWindow::showInputStatus()
{
    QModbusReply* reply = qobject_cast<QModbusReply*>(sender());
    if (!reply) return;
    QModbusDataUnit data = reply->result();

    ui->groupBox_4->setVisible(true);
    ui->listWidget->clear();

        ui->checkBox->setChecked(data.value(0));
        ui->checkBox_2->setChecked(data.value(1));
        ui->checkBox_3->setChecked(data.value(2));
        ui->checkBox_4->setChecked(data.value(3));
        ui->checkBox_5->setChecked(data.value(4));
        ui->checkBox_6->setChecked(data.value(5));
        ui->checkBox_7->setChecked(data.value(6));
        ui->checkBox_8->setChecked(data.value(7));
        ui->checkBox_9->setChecked(data.value(8));

        ui->checkBox_10->setChecked(data.value(9));
        ui->checkBox_11->setChecked(data.value(10));
        ui->checkBox_12->setChecked(data.value(11));
        ui->checkBox_13->setChecked(data.value(12));
        ui->checkBox_14->setChecked(data.value(13));
        ui->checkBox_15->setChecked(data.value(14));
        ui->checkBox_16->setChecked(data.value(15));
        ui->checkBox_13->setChecked(data.value(16));
        ui->checkBox_14->setChecked(data.value(17));
        ui->checkBox_15->setChecked(data.value(18));
        ui->checkBox_16->setChecked(data.value(19));
}

void MainWindow::showInputRegisters()
{
    QModbusReply* reply = qobject_cast<QModbusReply*>(sender());
    if (!reply) return;
    QModbusDataUnit data1 = reply->result();

          ui->listWidget->clear();
          ui->groupBox_4->setVisible(false);
          for (qint16 i = 0 ; i < m_values ; i++){
          ui->listWidget->addItem(QString::number(data1.value(i)));
           }
   reply->deleteLater();
}

