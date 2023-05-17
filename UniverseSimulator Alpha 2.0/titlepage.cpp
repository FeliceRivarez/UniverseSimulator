#include <QObject>
#include <QPushButton>
#include <QTimer>
#include "mainwindow.h"
#include "debugwindow.h"
#include "titlepage.h"
#include <QIcon>
#include <QSlider>
#include <QPainter>

titlepage::titlepage(QWidget *parent)
{
    QFont ft;
    ft.setPointSize(20);
    start.setParent(this);
    start.setText("Start The Simulation");
    start.setFont(ft);
    start.setFlat(true);
    start.move(450,600);
    start.adjustSize();
    start.show();
    connect(&start,&QPushButton::pressed,this,&titlepage::beginSim);
    setFixedSize(1200,700);
    show();

    n_civ = new QSlider(Qt::Horizontal, this);
    n_civ->setParent(this);
    initSlide(n_civ,50,550,300,20,2,15);
    civ_num.setParent(this);
    civ_num.move(50,540);

    n_star = new QSlider(Qt::Horizontal, this);
    n_star->setParent(this);
    initSlide(n_star,450,550,300,20,100,350);
    star_num.setParent(this);
    star_num.move(450,540);

    agr = new QSlider(Qt::Horizontal, this);
    agr->setParent(this);
    initSlide(agr,850,550,300,20,1,5);
    agr_num.setParent(this);
    agr_num.move(850,540);

    change(1);
}

void titlepage::beginSim()
{
    MainWindow *w=new MainWindow(NULL,n_civ->value(),n_star->value());
    w->show();
    //hide();
}

void titlepage::initSlide(QSlider* slider,int x,int y,int len,int height,int min,int max)
{
    slider->setGeometry(x,y,len,height);
    slider->setMinimum(min);
    slider->setMaximum(max);
    slider->setValue(int(max));
    slider->setSingleStep(1);
    slider->show();
    connect(slider,&QSlider::valueChanged,this,&titlepage::change);
}

void titlepage::change(int)
{
    char buffer[256];
    int value=n_civ->value();
    sprintf(buffer, "文明数量:%d",value);
    civ_num.setText(buffer);
    civ_num.adjustSize();
    civ_num.show();

    value=n_star->value();
    sprintf(buffer, "星系数量:%d",value);
    star_num.setText(buffer);
    star_num.adjustSize();
    star_num.show();

    value=agr->value();
    sprintf(buffer, "战争频率:%d",value);
    agr_num.setText(buffer);
    agr_num.adjustSize();
    agr_num.show();
}

void titlepage::paintEvent(QPaintEvent *)
{
    QPixmap image2;
    image2.load(":/new/images/images/ui/titlepage.jpg");
    QPainter painter(this);
    painter.drawPixmap(0,0,1200,550,image2);
}
