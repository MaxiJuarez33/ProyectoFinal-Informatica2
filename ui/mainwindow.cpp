#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "serialReader.h"
#include <QMessageBox>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_sendButton_clicked()
{
    QString portName = ui->portIn->text();
    ui->portIn->clear();
    SerialReader serialReader(portName.toStdString().c_str());
    if (serialReader.openPort())
    {
        QMessageBox::information(this, "Port Opened", "Port opened successfully");
        ui->showPort->setText(portName);
    }
    else
    {
        QMessageBox::warning(this, "Port Error", "Port could not be opened");
    }
}