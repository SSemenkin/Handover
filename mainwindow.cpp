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
    HandoverType result {HandoverType::INVALID};
    int from = ui->comboBox->currentIndex();
    int to   = ui->comboBox->currentIndex();

    if (from == 0 && to == 0) {
        result = HandoverType::GSM_TO_GSM;
    } else if (from == 0 && to == 1) {
        result = HandoverType::GSM_TO_UMTS;
    } else if (from == 0 && to > 1) {
        result = HandoverType::GSM_TO_LTE;
    } else if (from == 1 && to == 0) {
        result = HandoverType::UMTS_TO_GSM;
    } else if (from == 1 && to == 1) {
        result = HandoverType::LTE_TO_UMTS;
    } else if (from == 1 && to > 1) {
        result = HandoverType::UMTS_TO_LTE;
    } else if (from > 1 && to == 0) {
        result = HandoverType::LTE_TO_GSM;
    } else if (from > 1 && to == 1) {
        result = HandoverType::LTE_TO_UMTS;
    }

    return result;
}

VendorLTE MainWindow::selectedLTEVendor() const
{
    int from = ui->comboBox->currentIndex();
    int to   = ui->comboBox->currentIndex();

    if (from == to) {
        return VendorLTE::Invalid;
    } else if (from == 2 || to == 2) {
        return VendorLTE::Huawei;
    } else if (from == 3 || to == 3) {
        return VendorLTE::Ericsson;
    } else {
        return VendorLTE::Invalid;
    }
}

