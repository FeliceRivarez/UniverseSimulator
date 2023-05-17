#include <QIcon>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPalette>
#include <QLabel>
#include <cmath>
#include <QTimer>
#include <QPushButton>
#include <QPixmap>
#include <QPainter>
#include "Log.h"
#include <QFile>

MainWindow::MainWindow(QWidget *parent,int c_n,int s_n,int ag)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow), a(s_n,c_n),sub(this)

{


    a.father=this;
    a.agressive=ag;
    n_civ=c_n;
    n_stars=s_n;
    changeSpeed.setText("极高速");
    changeSpeed.adjustSize();
    changeSpeed2.setText("高速");
    changeSpeed2.adjustSize();
    changeSpeed3.setText("慢速");
    changeSpeed3.adjustSize();
    changeSpeed4.setText("最慢");
    changeSpeed4.adjustSize();
    clear.setText("清空数据窗口");
    clear.adjustSize();
    connect(&changeSpeed,&QPushButton::pressed,this,&MainWindow::speed);
    connect(&changeSpeed2,&QPushButton::pressed,this,&MainWindow::speed2);
    connect(&changeSpeed3,&QPushButton::pressed,this,&MainWindow::speed3);
    connect(&changeSpeed4,&QPushButton::pressed,this,&MainWindow::speed4);
    connect(&clear,&QPushButton::pressed,this,&MainWindow::cleared);
    changeSpeed.setParent(this);
    changeSpeed.move(950,30);
    changeSpeed.show();
    changeSpeed2.setParent(this);
    changeSpeed2.move(1050,30);
    changeSpeed2.show();
    changeSpeed3.setParent(this);
    changeSpeed3.move(1150,30);
    changeSpeed3.show();
    changeSpeed4.setParent(this);
    changeSpeed4.move(1250,30);
    changeSpeed4.show();
    clear.setParent(this);
    clear.move(1250,80);
    //clear.show();
    a.mapgen(n_stars);
    a.civdis(n_stars,n_civ);
    sub.setParent(this);
    for(int i=0;i<n_stars;i+=1)
    {
        a.s[i].debugwindow=&sub;
        a.s[i].setText("");
        a.s[i].setParent(this);
        a.s[i].move(a.s[i].x,a.s[i].y);
        a.s[i].setMaximumSize(20,20);
        a.s[i].setMinimumSize(20,20);
        a.s[i].setIcon( a.s[i].button_icon);//加上图标
        a.s[i].setFlat(true);
        a.s[i].show();
        connect(&a.s[i],&QPushButton::pressed,&a.s[i],&star::select);
    }
    pause.setParent(this);
    pause.setText("暂停");
    pause.adjustSize();
    pause.move(950,80);
    pause.show();
    connect(&pause,&QPushButton::pressed,this,&MainWindow::pausing);
    setFixedSize(1350,700);
    //sub.show();

    sub.pal.setBrush(QPalette::Window,QBrush(QPixmap(":/new/images/images/PureBlack.jpg")));
    sub.setPalette(sub.pal);

    QTimer *timer = new QTimer(this);   //初始化一个定时器。
    connect(timer, &QTimer::timeout, this, &MainWindow::update);   //设置信号与槽 ，槽函数update()需要再类中声明一下。
    timer->start(10 / 1);    //设置定时器刷新频率，即变量刷新频率。
    connect(turning, &QTimer::timeout, this, &MainWindow::turn);   //设置信号与槽 ，槽函数update()需要再类中声明一下。
    speed();
    changeSpeed.raise();
    sub.garri.raise();

    changeSpeed2.raise();
    changeSpeed3.raise();
    changeSpeed4.raise();
    clear.raise();
}

void MainWindow::speed()
{
    turning->stop();
    turning->start(10 / 1);
    sub.view=3;
}
void MainWindow::speed2()
{
    turning->stop();
    turning->start(200 / 1);
    if(sub.view==3)
    {
        sub.view=1;
        sub.showing=&a.s[0];
    }
}
void MainWindow::speed3()
{
    turning->stop();
    turning->start(500 / 1);
    if(sub.view==3)
    {
        sub.view=1;
        sub.showing=&a.s[0];
    }
}
void MainWindow::speed4()
{
    turning->stop();
    turning->start(1000 / 1);
    if(sub.view==3)
    {
        sub.view=1;
        sub.showing=&a.s[0];
    }
}
MainWindow::~MainWindow()
{
    sub.close();
    delete ui;
}

void MainWindow::update()
{
    char buffer[256];
    sprintf(buffer, "回合:%d",t);
    sub.a.setText(buffer);
    //sub.test.setText(buffer);
    sub.a.adjustSize();

    sprintf(buffer, "目前全宇宙战争总数:%d",a.n_wars);
    sub.total_wars.setText(buffer);
    sub.total_wars.adjustSize();

    sprintf(buffer, "发生过的战争总数:%d",a.historical);
    sub.historical.setText(buffer);
    sub.historical.adjustSize();

    sprintf(buffer, "目前全宇宙人口总和:%d",a.total_pop);
    sub.total_pop.setText(buffer);
    sub.total_pop.adjustSize();

    sprintf(buffer, "目前仍未灭亡的文明:%d",a.surviving);
    sub.surviving.setText(buffer);
    sub.surviving.adjustSize();
}

void MainWindow::turn()
{
    if(paused==1)
    {
        return;
    }
    t+=1;
    a.star_value();
    a.observe();
    a.diplomatic();
    a.truce();
    a.root.goThrough();
    a.termin();
    a.warlist();
    a.conflict();
    a.warlist();

    for(int i=0;i<a.n_civ;i+=1)
    {
        if(a.c[i].destroyed==0)
        {
            a.c[i].nextTurn();
        }
    }

    int temp=0;
    for(int i=0;i<a.n_stars;i+=1)
    {
        a.s[i].ongoing.fight();
        if(a.s[i].AtWar==1)
        {
            temp+=1;
        }
    }
    a.totalpop();
    a.survived();
    if(t%100==0)
    {
        //a.writeLog();
    }
}

void MainWindow::paintEvent(QPaintEvent *)
{
    QPixmap image2;
    image2.load(":/new/images/images/background.jpg");
    QPainter painter(this);
    painter.drawPixmap(0,0,950,700,image2);

    QPixmap image1;
    image1.load(":/new/images/images/debugwindow.png");
    QPainter painter2(this);
    painter2.drawPixmap(950,0,1350,700,image1);
}

void MainWindow::pausing()
{
    if(paused==0)
    {
        paused=1;
        if(sub.view==3)
        {
            speed2();
            sub.showing=&a.s[0];
        }
    }
    else
    {
        paused=0;
        if(sub.view==3)
        {
            speed2();
            sub.showing=&a.s[0];
        }
    }
}

void MainWindow::cleared()
{
    sub.view=3;
}
