#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>

QMap<QString, QString> MainWindow::m_cellIdToLAC {};

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_settings(new QSettings("s.ini", QSettings::IniFormat))
    , m_lastDirectory(m_settings->value("lastDir", QDir::homePath()).toString())
    , m_telnetMSS02(new Telnet("", m_settings->value("hostname_mss02", "10.56.135.16").toString(),
                                   m_settings->value("username_mss02", "Administrator").toString(),
                                   m_settings->value("password_mss02", "Admin023").toString()))
    , m_telnetMSS03 (new Telnet("", m_settings->value("hostname_mss03", "10.104.133.3").toString(),
                                   m_settings->value("username_mss03", "administrator").toString(),
                                   m_settings->value("passsword_mss03", "Administrator1@").toString()))
    , m_telnetMSS04(new Telnet("", m_settings->value("hostname_mss04", "10.20.112.160").toString(),
                                   m_settings->value("username_mss04", "ts_user").toString(),
                                   m_settings->value("password_mss04", "apg43l2@").toString()))
{
    ui->setupUi(this);
    const QStringList items {"2G", "3G", "4G Huawei", "4G Ericsson"};
    ui->comboBox->addItems(items);
    ui->comboBox_2->addItems(items);

    QObject::connect(m_telnetMSS02.data(), &Telnet::commandExecuted, this, &MainWindow::processTelnetOutput);
    QObject::connect(m_telnetMSS02.data(), &Telnet::errorOccured, this, &MainWindow::processTelnetErrors);

    QObject::connect(m_telnetMSS03.data(), &Telnet::commandExecuted, this, &MainWindow::processTelnetOutput);
    QObject::connect(m_telnetMSS03.data(), &Telnet::errorOccured, this, &MainWindow::processTelnetErrors);

    QObject::connect(m_telnetMSS04.data(), &Telnet::commandExecuted, this, &MainWindow::processTelnetOutput);
    QObject::connect(m_telnetMSS04.data(), &Telnet::errorOccured, this, &MainWindow::processTelnetErrors);

    m_telnetMSS02->connectToNode();
    m_telnetMSS02->executeCommand("MGAAP:AREA=all;");

    m_telnetMSS03->connectToNode();
    m_telnetMSS03->executeCommand("MGAAP:AREA=all;");

    m_telnetMSS04->connectToNode();
    m_telnetMSS04->executeCommand("MGAAP:AREA=all;");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this, "Выбрать файл", m_lastDirectory, "CSV File (*.csv)");
    if (filename.isEmpty()) {
        return;
    }

    m_lastDirectory = filename;
    m_settings->setValue("lastDir", m_lastDirectory);
}

HandoverType MainWindow::selectedHandoverType() const
{
    HandoverType result {HandoverType::INVALID};
    int from = ui->comboBox->currentIndex();
    int to   = ui->comboBox_2->currentIndex();

    if (from == 0 && to == 0) {
        result = HandoverType::GSM_TO_GSM;
    } else if (from == 0 && to == 1) {
        result = HandoverType::GSM_TO_UMTS;
    } else if (from == 0 && to > 1) {
        result = HandoverType::GSM_TO_LTE;
    } else if (from == 1 && to == 0) {
        result = HandoverType::UMTS_TO_GSM;
    } else if (from == 1 && to == 1) {
        result = HandoverType::UMTS_TO_UMTS;
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
    int to   = ui->comboBox_2->currentIndex();

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

QMap<QString, QString> MainWindow::cellIdToLAC()
{
    return m_cellIdToLAC;
}

void MainWindow::on_pushButton_2_clicked()
{
    qDebug() << static_cast<int>(selectedLTEVendor());
    QString result = HandoverMaker::makeHandovers(selectedHandoverType(), m_lastDirectory, selectedLTEVendor());
    ui->textBrowser->setHtml(result.replace("\n", "<br>"));
}

void MainWindow::processTelnetOutput(const QString &responce)
{
    const QStringList rows = responce.split("\n", Qt::SkipEmptyParts);
    for (const QString &row : rows) {

        if(row.contains("255-99"))  {
            QStringList elements = row.trimmed().split('-', Qt::SkipEmptyParts);
            if (elements.size() == 4) {
                QString cellId = elements.last();
                int spaceIndex = cellId.indexOf(' ');
                if (spaceIndex != -1) {
                    cellId = cellId.left(spaceIndex);
                }
                m_cellIdToLAC[cellId] = elements.at(2);
            } else {
                qDebug() << "Undefined behavior with row " << elements;
            }
        }
    }

    Telnet *telnetSender = qobject_cast<Telnet*>(sender());

    disconnect(telnetSender, &Telnet::errorOccured, this, &MainWindow::processTelnetErrors);
}

void MainWindow::processTelnetErrors(const QString &errorMessage)
{
    QMessageBox::critical(nullptr, "Error", "Error occurred when we try get RNC LACs.\n" + errorMessage);
}

