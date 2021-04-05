#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTime>
#include <QDebug>

#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , devType(USBCAN2)
    , devIndex(0)
    , canIndex(0)
{
    ui->setupUi(this);

    connect(&timer, &QTimer::timeout, this, &MainWindow::slotTimeout);

    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("bms.db");
    if(!db.open()){
        QMessageBox::critical(0, "error", "can not open bms.db", QMessageBox::Cancel);
        return;
    }
    QSqlQuery query1(db);
    bool ret = query1.exec("create table bms_data (id int primary key, "
                //0x351
                "battery_charge_voltage int, charge_current_limit int , discharge_current_limit int, "
                //0x355
                "soc int, soh int, "
                //0x356
                "battery_voltage int, battery_current int, battery_temp int, "
                //0x359
                "over_current_discharge boolean, under_temp boolean, over_temp boolean, under_voltage boolean, "
                "bms_internal1 boolean, over_current_charge boolean, "
                "high_current_discharge boolean, low_temp boolean, high_temp boolean, low_voltage boolean, high_voltage boolean, "
                "bms_internal2 boolean, high_current_charge boolean, "
                "pack_no int, manufacturer_name1 varchar(1), manufacturer_name2 varchar(1), "
                //0x35C
                "charge_enable boolean, discharge_enable boolean, charge_immediately boolean, "
                //0x40X
                "max_cell_voltage int, max_cell_voltage_no int, min_cell_voltage int, min_cell_voltage_no int, "
                //0x41X
                "max_temp int, max_temp_no int, min_temp int, min_temp_no int, "
                //0x42X
                "socx int, sohx int, total_pressure int, total_current int, "
                //0x43X
                "charging_under_temp boolean, charging_over_temp boolean, discharge_over_current boolean, total_pressure_under_voltage boolean, "
                //0x44X
                "cell_voltage1 int, cell_voltage2 int, cell_voltage3 int, cell_voltage4 int, "
                //0x45X
                "cell_voltage5 int, cell_voltage6 int, cell_voltage7 int, cell_voltage8 int, "
                //0x46X
                "cell_voltage9 int, cell_voltage10 int, cell_voltage11 int, cell_voltage12 int, "
                //0x47X
                "cell_voltage13 int, cell_voltage14 int, cell_voltage15 int, cell_voltage16 int, "
                //0x48X
                "temp1 int, temp2 int, temp3 int, temp4 int);");
    if(!ret){
        qDebug() << "create database failed";
        return;
    }
}

MainWindow::~MainWindow()
{
    delete ui;

    CloseDevice(devType, devIndex);
}

void MainWindow::slotTimeout()
{
    qDebug()<<"timeout, " << QTime::currentTime().toString();
}


void MainWindow::on_pushButton_OnOff_clicked()
{
    //打开设备
    if(OpenDevice(devType, devIndex, 0) != STATUS_OK){
        qDebug()<<"open can failed";
        return;
    }
    //初始化某一路CAN
    INIT_CONFIG initConfig;
    initConfig.AccCode = 0;
    initConfig.AccMask = 0xFFFFFF;
    initConfig.Filter = 0;
    //500k
    initConfig.Timing0 = 0;
    initConfig.Timing1 = 0x1C;
    initConfig.Mode = 0;
    if(InitCAN(devType, devIndex, canIndex, &initConfig) == STATUS_ERR){
        qDebug()<<"init can failed";
        CloseDevice(devType, devIndex);
        return;
    }
    //启动某一路CAN
    if(StartCAN(devType, devIndex, canIndex) == STATUS_ERR){
        qDebug()<<"start can failed";
        CloseDevice(devType, devIndex);
        return;
    }

    timer.start(1000);
}
