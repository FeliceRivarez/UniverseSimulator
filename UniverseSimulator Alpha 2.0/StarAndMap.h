#pragma once
#ifndef STARANDMAP_H
#define STARANDMAP_H

#include <iostream>
#include <cmath>
#include <QPushButton>
#include <QIcon>
#include <QLabel>
#include "Log.h"
class debugwindow;
#include "debugwindow.h"
class civ;
class Map;
class battle;
using namespace std;


class battle
{
public:
    civ *nations=NULL;
    star*location=NULL;
    double atk_power=0;
    void fight();
};

class civ;
class star: public QPushButton
{
public:
    double garrison=0;
    int selected=0;
    int x;//x坐标
    int y;//y坐标
    int n;//星系序号
    int res, poplim;//自然属性（资源基础数量，人口基础上限）
    int pr;//实际生产资源
    double def=5;//星系军事实力
    void change();
    QIcon button_icon;//添加图标
    QIcon attacked;
    star();
    int pop=0;
    int natural_value;//殖民优先程度
    int owned=0;//是否被一个文明占领
    int colonizing=0;//正在被殖民
    civ *owner=NULL;//星系的占领者
    debugwindow *debugwindow;//数据显示窗口
    double growth=0;
    void select();

    int AtWar=0;
    int value=0;
    battle ongoing;

};

class civ
{
public:
    int type=0;//文明类型：1为友好型，2为保守型，3为侵略型，4为灭绝者
    int num;//文明编号
    int destroyed = 0;//是否已经灭亡
    civ(int t);
    star** own;//拥有的星系（指针
    star** observed;//可观测星系（指针
    civ();
    int owned=0;//拥有星系数
    double income=0;//资源结余
    int resource=0;//总资源数
    int total_pop=0;//总人口
    int total_capacity=0;//总人口上限
    int observe=0;//总可观测星系
    int contacts=0;//有外交联系的国家数目
    double science=1;//科技指数
    double science_add=0;//科技分段增长
    double research=100;//科研预算
    double milit=1;//军事预算
    int n_candid=0;//候选可殖民星系数
    double pop_upkeep=0.5;//基础人口维护费
    double defcon=1;//战备等级，数值越高战备程度越高

    civ **list;//建立联系的文明列表
    int *list_rela;//与列表中各文明的关系
    int friends=0;
    int enemies=0;
    int opponents=0;

    double power=0;//机动军事力量
    int *truce;//与之停战的文明
    civ **warWith;//正在交战的文明
    int num_wars=0;
    void attack();//进攻敌对星系
    Map*upper;

    QIcon *I;
    void icon_set();
    void nextTurn();//回合操作
    void balance();//获取结余资源
    void repaint();//给星系换颜色
    void expand();//扩张
    void population();//人口核算与增长
    void allocate();//预算分配
    void diplo_rela();//diplomatic relations，外交关系
    void sci_dev();//科技发展
    void defense();//地区防御
    void milit_gain();//机动军力
    void defconSet();//战备等级
    void militAllocate();//防卫部队分配
    void reinforce();//增援前线部队
    ~civ()
    {
        delete *own;
        delete *observed;
        delete I;
    }

    int picture=0;
};

class War//以链表形式存储
{
public:
    Map*a;//指向上级
    civ*side01;//第一方
    civ*side02;//第二方
    int length=0;//战争已经进行的回合数
    int limit=0;//战争结束的回合数
    War *previous;//指向前一个节点
    War *next=NULL;//指向后一个节点
    int IsRoot=0;//是否是根节点
    int IsLast=0;//是否是末尾
    int overtime=0;//是否即将结束
    War(civ* side1, civ* side2);
    void terminate();//删除自身
    void goThrough();//遍历所有节点并加时
};

class Map
{
public:
    //QString path = createMultipleFolders("C:\\UniverseSimulatorLog");
    //QString pathFile = createFile(path,".txt");
    int height=660, length=920;
    int n_stars;//星系数量
    int n_civ;//初始文明数
    int agressive=1;
    star* s;//星系数组
    Map(int a,int b);
    void mapgen(int a);//生成随机星图
    civ *c;//文明数组
    void civdis(int a,int b);//文明随机分布
    void observe();//重新分配视野
    void diplomatic();//外交关系分配

    void conflict();//战争判定
    War root;
    War* last=&root;
    int n_wars=0;
    int surviving=0;
    int total_pop=0;
    int historical=0;
    void termin();//遍历战争链表并剔除超时战争
    void warlist();//刷新每个文明的战争列表
    void truce();//刷新每个文明的停战回合数
    void star_value();//刷新每个星系的战略价值
    void assign(int x);
    void totalpop();
    void survived();

    void writeLog();
    MainWindow *father;
    civ* strongest();
};

#endif // STARANDMAP_H
