#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QModbusTcpClient>
#include <QTimer>
#include <QModbusDataUnit>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QString ipaddress() const;
    void set_ipaddress(const QString &ipaddress);
    qint64 port() const;
    void set_port(const qint64 port);
    qint64 server() const ;
    void set_server(const qint64 server);
    qint64 startAddress() const;
    void set_startAddress(const qint64 startAddress);
    qint64 values() const;
    void set_values(const qint64 values);

public slots:
    void ConnectionToDevice();
    void DisconnectDevice();
    void stateChanged(QModbusDevice::State state);
    void getValues();
    void showCoils();
    void showHoldingRegisters();
    void setCoilsStatus();
    void setHoldingRegisters();
    void showInputStatus();
    void showInputRegisters();
    void indexChanged(int val);


private:
    void startRequest();
    Ui::MainWindow *ui;
    QString m_ipaddress;
    qint64 m_port;
    qint64 m_server;
    qint64 m_startAddress;
    qint64 m_values;
    QModbusTcpClient client;
    QTimer requestTimer;
    QModbusDataUnit dataUnit;
    qint16 rt ;

};

#endif // MAINWINDOW_H
