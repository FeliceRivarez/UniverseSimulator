#pragma once
#ifndef DEBUGWINDOW_H
#define DEBUGWINDOW_H
#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QPalette>
class star;
class MainWindow;
#include "StarAndMap.h"
#include <QFont>
#include "Log.h"

class debugwindow: public QWidget
{
    Q_OBJECT;
public:
    explicit debugwindow(QWidget *parent=0);
    QFont ft;
    QPushButton test;
    QPushButton civ_view;
    QPushButton uni_view;
    QLabel a;
    QLabel star_res;//选中星系资源
    QLabel star_poplim;//选中星系人口上限
    QLabel star_pop;//选中星系总人口
    QLabel star_owner;//选中星系所属文明
    QLabel star_num;//选中星系序号
    QLabel civ_num;//选中文明序号
    QLabel civ_res;//选中文明资源存储
    QLabel civ_owned;//选中文明拥有星系数量
    QLabel civ_income;//文明资源收入
    QLabel civ_total_pop;//文明总人口
    QLabel civ_contacts;//文明总外交关系数
    QLabel civ_pop_upkeep;//文明人口维护费
    QLabel civ_research;//文明科研经费
    QLabel civ_milit;//文明军事预算
    QLabel civ_science;//文明科技发展程度
    QLabel civ_type;//文明种类
    QLabel civ_friends;//友好文明
    QLabel civ_opponents;//对手文明
    QLabel civ_enemies;//敌对文明
    QLabel civ_num_wars;//战争数量
    QLabel total_wars;//世界总战争数
    QLabel civ_power;
    QLabel star_def;
    QLabel civ_defcon;
    QLabel star_garrison;
    QLabel star_AtWar;
    QLabel star_atk;
    QLabel surviving;
    QLabel total_pop;
    QLabel historical;
    QLabel nations;
    QLabel location;
    QLabel Default;
    QLabel Warning;
    star *showing=NULL;
    void update();
    QPalette pal =this->palette();
    void icon_init(QLabel *n, int x,int y);//图标初始化
    int view=0;//0为星系视图，1为文明视图，2为宇宙视图，3为清空视图

    QPushButton garri;
    QIcon garrison;
    void paintEvent(QPaintEvent *);
    void viewChange0();
    void viewChange1();
    void viewChange2();


};

#endif // DEBUGWINDOW_H
