
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QIcon>
#include <QTimer>
#include <QMainWindow>
#include <QPushButton>
#include "StarAndMap.h"
#include <QLabel>
#include "ui_mainwindow.h"
#include <QPalette>
#include "debugwindow.h"
#include "Log.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow

{
public:
    MainWindow(QWidget *parent = nullptr,int c_n=2,int s_n=100,int ag=1);
    QPushButton b;
    QPushButton changeSpeed;
    QPushButton changeSpeed2;
    QPushButton changeSpeed3;
    QPushButton changeSpeed4;
    QPushButton pause;
    QPushButton clear;
    int paused=0;
    void pausing();
    Map a;
    debugwindow sub;
    ~MainWindow();
    //QLabel label;
    void update();
    int t=0;
    void turn();
    star *selected=NULL;

    void init();
    int n_civ=2;
    int n_stars=100;
    int agressive=1;
    void speed();
    void speed2();
    void speed3();
    void speed4();
    void cleared();
    int pace=0;
    QTimer *turning = new QTimer(this);   //初始化一个定时器。


protected:
    void paintEvent(QPaintEvent *);
private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
