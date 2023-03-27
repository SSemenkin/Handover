#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>
#include "handovermaker.h"
#include "network/telnet.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    HandoverType selectedHandoverType() const;
    VendorLTE selectedLTEVendor() const;

    static QMap<QString, QString> cellIdToLAC();

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void processTelnetOutput(const QString &responce);
    void processTelnetErrors(const QString &errorMessage);
private:
    Ui::MainWindow *ui;
    QSettings *m_settings;
    QString    m_lastDirectory;
    QScopedPointer<Telnet> m_telnetMSS02;
    QScopedPointer<Telnet> m_telnetMSS03;
    QScopedPointer<Telnet> m_telnetMSS04;

    static QMap<QString, QString> m_cellIdToLAC;
};
#endif // MAINWINDOW_H
