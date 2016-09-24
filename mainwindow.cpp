#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtGui>
#include <QtCore>

#include "globals.h"
#include "scard.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)

{
    ui->setupUi(this);

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(1000);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::update()
{
    scard sc;
    /*
     * 1. Estalish a context
     */
    sc.establishContext();

    /*
     * 2. Let the user choose a reader.
     */
    /*
     *  2.a. List all available readers
     */

    sc.cardListReaders();

    sc.cardConnect();

    sc.cardGetAttribATR(SCARD_ATTR_ATR_STRING);

    sc.cardGetAttribSerialNumber(SCARD_ATTR_VENDOR_IFD_SERIAL_NO);

//    sc.cardGetAttribDeviceSystemName(SCARD_ATTR_POWER_MGMT_SUPPORT);
    sc.cardGetProp();

    sc.cardReadData();

    ui->lineEdit_serialNumber->setText(sc.get_serialNumber());
    ui->lineEdit_ATR->setText(sc.getATR());
    ui->lineEdit_UID->setText(sc.get_UID());
}

void MainWindow::on_actionTest_triggered()
{

}
