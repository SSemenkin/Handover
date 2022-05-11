#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_settings(new QSettings("s.ini", QSettings::IniFormat))
    , m_lastDirectory(m_settings->value("lastDir", QDir::homePath()).toString())
{

    ui->setupUi(this);

    const QStringList items {"2G", "3G", "4G Huawei", "4G Ericsson"};
    ui->comboBox->addItems(items);
    ui->comboBox_2->addItems(items);

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this, "Выбрать файл", m_lastDirectory);
    if (filename.isEmpty()) {
        return;
    }

    m_lastDirectory = filename;

    QString result = HandoverMaker::makeHandovers(selectedHandoverType(), filename, selectedLTEVendor());
    ui->textBrowser->setText(result);
}

HandoverType MainWindow::selectedHandoverType() const
{

}

VendorLTE MainWindow::selectedLTEVendor() const
{

}

