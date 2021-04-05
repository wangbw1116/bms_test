#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include "ECanVci.h"
#include <QSqlDatabase>
#include <QSqlQuery>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    DWORD devType;
    DWORD devIndex;
    DWORD canIndex;
    QTimer timer;
    QSqlDatabase db;

private slots:
    void slotTimeout();
    void on_pushButton_OnOff_clicked();
};
#endif // MAINWINDOW_H
