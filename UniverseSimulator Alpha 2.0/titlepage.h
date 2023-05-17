#pragma once
#ifndef TITLEPAGE_H
#define TITLEPAGE_H
#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QPalette>
#include "StarAndMap.h"
#include <QSlider>


class titlepage: public QWidget
{
    Q_OBJECT;
public:
    titlepage(QWidget *parent= nullptr);
    QPushButton start;
    void beginSim();
    void initSlide(QSlider* slider,int x,int y,int len,int height,int min,int max);

    QSlider *n_civ;
    QLabel civ_num;
    QSlider *n_star;
    QLabel star_num;

    QSlider *agr;
    QLabel agr_num;
    void change(int);

protected:
    void paintEvent(QPaintEvent *);
};

#endif // TITLEPAGE_H
