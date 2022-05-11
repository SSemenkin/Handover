#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>
#include "handovermaker.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();
    HandoverType selectedHandoverType() const;
    VendorLTE selectedLTEVendor() const;

private:
    Ui::MainWindow *ui;
    QSettings *m_settings;
    QString m_lastDirectory;
};
#endif // MAINWINDOW_H
