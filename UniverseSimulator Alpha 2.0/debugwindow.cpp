#include <QObject>
#include <QPushButton>
#include <QTimer>
#include "mainwindow.h"
#include "debugwindow.h"
#include <QIcon>
#include <QPainter>
#include <QFile>

debugwindow::debugwindow(QWidget *parent):garrison(":/new/images/images/garrison.jpg")
{
    test.setParent(this);
    test.setText("星系视图");
    //test.setMaximumSize(50,50);
    //test.setMinimumSize(50,50);
    test.show();
    test.move(0,625);
    connect(&test,&QPushButton::pressed,this,&debugwindow::viewChange0);

    civ_view.setParent(this);
    civ_view.setText("文明视图");
    //test.setMaximumSize(50,50);
    //test.setMinimumSize(50,50);
    civ_view.show();
    civ_view.move(100,625);
    connect(&civ_view,&QPushButton::pressed,this,&debugwindow::viewChange1);

    uni_view.setParent(this);
    uni_view.setText("宇宙视图");
    //test.setMaximumSize(50,50);
    //test.setMinimumSize(50,50);
    uni_view.show();
    uni_view.move(200,625);
    connect(&uni_view,&QPushButton::pressed,this,&debugwindow::viewChange2);
    //a.setParent(this);
    //a.move(50,50);
    //a.show();

    ft.setPointSize(11);

    icon_init(&a,20,30);

    icon_init(&star_num,20,340);
    icon_init(&star_owner,200,340);
    icon_init(&star_res,20,370);
    icon_init(&star_poplim,200,370);
    icon_init(&star_pop,20,400);
    icon_init(&star_def,20,430);

    icon_init(&civ_num,20,340);
    icon_init(&civ_owned,200,340);

    icon_init(&civ_res,20,370);
    icon_init(&civ_income,200,370);

    icon_init(&civ_total_pop,20,400);
    icon_init(&civ_pop_upkeep,200,400);

    icon_init(&civ_contacts,20,430);
    icon_init(&civ_type,200,430);
    icon_init(&civ_friends,20,460);
    icon_init(&civ_opponents,200,460);
    icon_init(&civ_enemies,20,490);

    icon_init(&civ_research,20,520);
    icon_init(&civ_milit,200,520);
    icon_init(&civ_science,20,550);
    icon_init(&civ_type,200,550);

    icon_init(&civ_power,20,580);
    icon_init(&civ_defcon,200,580);
    icon_init(&civ_num_wars,20,610);

    icon_init(&star_garrison,10,450);
    star_garrison.setStyleSheet("color:blue");
    icon_init(&star_AtWar,20,420);
    icon_init(&star_atk,200,450);
    star_atk.setStyleSheet("color:red");
    icon_init(&nations,200,480);
    icon_init(&location,10,480);

    icon_init(&total_wars,20,340);
    icon_init(&total_pop,200,340);
    icon_init(&historical,20,370);
    icon_init(&surviving,200,370);
    icon_init(&Default,20,450);
    icon_init(&Warning,20,400);
    char buffer[64];
    sprintf(buffer, "点击左侧的一个星系以在此显示数据");
    Default.setText(buffer);
    Default.adjustSize();
    move(950,0);
    setFixedSize(400,700);

    QTimer *timer = new QTimer(this);   //初始化一个定时器。
    connect(timer, &QTimer::timeout, this, &debugwindow::update);   //设置信号与槽 ，槽函数update()需要再类中声明一下。
    timer->start(10 / 1);    //设置定时器刷新频率，即变量刷新频率。


    //setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);


    garri.setParent(this);
    garri.setStyleSheet(":/new/images/images/garrison.jpg");
    //garri.adjustSize();
    garri.move(0,400);
    garri.raise();
    garri.setFlat(true);
    //garri.show();

    show();
    total_wars.hide();
    total_pop.hide();
    surviving.hide();
    historical.hide();
    Warning.hide();
}

void debugwindow::icon_init(QLabel* n, int x,int y)
{
    n->setParent(this);
    n->move(x,y);
    n->show();
    n->setFont(ft);
}

void debugwindow::update()
{
    if(view==3)
    {
        showing=NULL;
        char buffer[64];
        sprintf(buffer, "为保证稳定性，数据窗口已锁定在宇宙视图 \n请选择高速及以下速度来解除锁定");
        Warning.setText(buffer);
        Warning.adjustSize();
        Warning.show();
        star_AtWar.hide();
        star_atk.hide();
        star_res.hide();
        star_pop.hide();
        star_poplim.hide();
        star_num.hide();
        star_def.hide();
        star_garrison.hide();
        star_owner.hide();
        location.hide();
        nations.hide();

        civ_num.hide();
        civ_res.hide();
        civ_owned.hide();
        star_owner.hide();
        civ_income.hide();
        civ_total_pop.hide();
        civ_contacts.hide();
        civ_pop_upkeep.hide();
        civ_research.hide();
        civ_milit.hide();
        civ_science.hide();
        civ_num_wars.hide();
        civ_power.hide();
        civ_defcon.hide();
        civ_enemies.hide();
        civ_opponents.hide();
        civ_friends.hide();
        civ_type.hide();

        total_wars.show();
        total_pop.show();
        surviving.show();
        historical.show();

    }
    else
    {
        Warning.hide();
    }
    if(showing!=NULL)
    {
        Default.hide();
        repaint();
        char buffer[256];
        if(view==0)
        {
            star_AtWar.show();
            star_atk.show();
            star_res.show();
            star_pop.show();
            star_poplim.show();
            star_num.show();
            star_def.show();
            star_garrison.show();
            location.show();


            sprintf(buffer, "资源:%d",showing->res);
            star_res.setText(buffer);
            star_res.setText(buffer);
            star_res.adjustSize();

            sprintf(buffer, "人口上限:%d",showing->poplim);
            star_poplim.setText(buffer);
            star_poplim.setText(buffer);
            star_poplim.adjustSize();

            sprintf(buffer, "人口数量:%d",showing->pop);
            star_pop.setText(buffer);
            star_pop.setText(buffer);
            star_pop.adjustSize();

            sprintf(buffer, "星系序号:%d",showing->n);
            star_num.setText(buffer);
            star_num.setText(buffer);
            star_num.adjustSize();

            sprintf(buffer, "星系防御力:%f",showing->def);
            star_def.setText(buffer);
            star_def.setText(buffer);
            star_def.adjustSize();

            sprintf(buffer, "星系守备舰队:%f",showing->garrison);
            star_garrison.setText(buffer);
            star_garrison.adjustSize();




            if(showing->AtWar==1)
            {
                star_AtWar.hide();
                //star_AtWar.show();
                star_atk.show();
                nations.show();
                sprintf(buffer, "星系是否正在战斗中:%d",showing->AtWar);
                star_AtWar.setText(buffer);
                star_AtWar.adjustSize();

                sprintf(buffer, "星系敌方战斗力:%f",showing->ongoing.atk_power);
                star_atk.setText(buffer);
                star_atk.adjustSize();


                sprintf(buffer, "进攻方:%d号文明",showing->ongoing.nations->num);
                nations.setText(buffer);
                nations.adjustSize();

            }
            else
            {
                star_AtWar.hide();
                star_atk.hide();
                nations.hide();
            }
            if(showing->owner!=NULL)
            {
                sprintf(buffer, "星系所有者:%d号文明",showing->owner->num);
                star_owner.setText(buffer);
                star_owner.setText(buffer);
                star_owner.adjustSize();
                star_owner.show();
                sprintf(buffer, "防守方:%d号文明",showing->owner->num);
                location.setText(buffer);
                location.adjustSize();
                location.show();
            }
        }
        else
        {
            star_AtWar.hide();
            star_atk.hide();
            star_res.hide();
            star_pop.hide();
            star_poplim.hide();
            star_num.hide();
            star_def.hide();
            star_garrison.hide();
            star_owner.hide();
            location.hide();
            nations.hide();
        }

        if(view==1)
        {
            if(showing->owner!=NULL)
            {
                sprintf(buffer, "文明存储资源:%d",int(showing->owner->resource));
                civ_res.setText(buffer);
                civ_res.setText(buffer);
                civ_res.adjustSize();
                civ_res.show();

                sprintf(buffer, "文明占有星系数:%d",showing->owner->owned);
                civ_owned.setText(buffer);
                civ_owned.setText(buffer);
                civ_owned.adjustSize();
                civ_owned.show();

                sprintf(buffer, "文明收入:%f",showing->owner->income);
                civ_income.setText(buffer);
                civ_income.setText(buffer);
                civ_income.adjustSize();
                civ_income.show();

                sprintf(buffer, "文明总人口:%d",showing->owner->total_pop);
                civ_total_pop.setText(buffer);
                civ_total_pop.setText(buffer);
                civ_total_pop.adjustSize();
                civ_total_pop.show();

                sprintf(buffer, "文明已经建立的外交关系:%d",showing->owner->contacts);
                civ_contacts.setText(buffer);
                civ_contacts.setText(buffer);
                civ_contacts.adjustSize();
                civ_contacts.show();

                sprintf(buffer, "文明的人口维护费:%f",showing->owner->pop_upkeep);
                civ_pop_upkeep.setText(buffer);
                civ_pop_upkeep.setText(buffer);
                civ_pop_upkeep.adjustSize();
                civ_pop_upkeep.show();

                sprintf(buffer, "文明的科研投入:%d",int(showing->owner->milit));
                civ_research.setText(buffer);
                civ_research.setText(buffer);
                civ_research.adjustSize();
                civ_research.show();

                sprintf(buffer, "文明的军事投入:%d",int(showing->owner->science_add));
                civ_milit.setText(buffer);
                civ_milit.setText(buffer);
                civ_milit.adjustSize();
                civ_milit.show();

                sprintf(buffer, "文明的科技水平:%f",showing->owner->science);
                civ_science.setText(buffer);
                civ_science.setText(buffer);
                civ_science.adjustSize();
                civ_science.show();

                if(showing->owner->type==1)
                {
                    civ_type.setStyleSheet("color:blue");
                    sprintf(buffer, "文明类型：友好型");
                    civ_type.setText(buffer);
                    civ_type.setText(buffer);
                    civ_type.adjustSize();
                    civ_type.show();
                }
                if(showing->owner->type==2)
                {
                    civ_type.setStyleSheet("color:orange");
                    sprintf(buffer, "文明类型：保守型");
                    civ_type.setText(buffer);
                    civ_type.setText(buffer);
                    civ_type.adjustSize();
                    civ_type.show();
                }
                if(showing->owner->type==3)
                {
                    civ_type.setStyleSheet("color:red");
                    sprintf(buffer, "文明类型：侵略型");
                    civ_type.setText(buffer);
                    civ_type.setText(buffer);
                    civ_type.adjustSize();
                    civ_type.show();
                }
                if(showing->owner->type==4)
                {
                    civ_type.setStyleSheet("color:purple");
                    sprintf(buffer, "文明类型：灭绝政体");
                    civ_type.setText(buffer);
                    civ_type.setText(buffer);
                    civ_type.adjustSize();
                    civ_type.show();
                }


                sprintf(buffer, "友好关系数:%d",showing->owner->friends);
                civ_friends.setText(buffer);
                civ_friends.setText(buffer);
                civ_friends.adjustSize();
                civ_friends.show();

                sprintf(buffer, "对手关系数:%d",showing->owner->opponents);
                civ_opponents.setText(buffer);
                civ_opponents.setText(buffer);
                civ_opponents.adjustSize();
                civ_opponents.show();

                sprintf(buffer, "敌对关系数:%d",showing->owner->enemies);
                civ_enemies.setText(buffer);
                civ_enemies.setText(buffer);
                civ_enemies.adjustSize();
                civ_enemies.show();

                sprintf(buffer, "正在进行的战争:%d",showing->owner->num_wars);
                civ_num_wars.setText(buffer);
                civ_num_wars.setText(buffer);
                civ_num_wars.adjustSize();
                civ_num_wars.show();

                sprintf(buffer, "文明军事力量:%d",int(showing->owner->power));
                civ_power.setText(buffer);
                civ_power.setText(buffer);
                civ_power.adjustSize();
                civ_power.show();

                sprintf(buffer, "文明战备等级:%d",int(showing->owner->defcon));
                civ_defcon.setText(buffer);
                civ_defcon.setText(buffer);
                civ_defcon.adjustSize();
                civ_defcon.show();

                sprintf(buffer, "文明序号:%d",showing->owner->num);
                civ_num.setText(buffer);
                civ_num.setText(buffer);
                civ_num.adjustSize();
                civ_num.show();
            }
            if(showing->owner==NULL)
            {
                civ_num.hide();
                civ_res.hide();
                civ_owned.hide();
                star_owner.hide();
                civ_income.hide();
                civ_total_pop.hide();
                civ_contacts.hide();
                civ_pop_upkeep.hide();
                civ_research.hide();
                civ_milit.hide();
                civ_science.hide();
                civ_num_wars.hide();
                civ_power.hide();
                civ_defcon.hide();
                civ_enemies.hide();
                civ_opponents.hide();
                civ_friends.hide();
                civ_type.hide();
            }
        }
        else
        {
            civ_res.hide();
            civ_owned.hide();
            star_owner.hide();
            civ_income.hide();
            civ_total_pop.hide();
            civ_contacts.hide();
            civ_pop_upkeep.hide();
            civ_research.hide();
            civ_milit.hide();
            civ_science.hide();
            civ_num_wars.hide();
            civ_power.hide();
            civ_defcon.hide();
            civ_enemies.hide();
            civ_opponents.hide();
            civ_friends.hide();
            civ_type.hide();
            civ_num.hide();
        }
        if(view==2)
        {
            total_wars.show();
            total_pop.show();
            surviving.show();
            historical.show();
        }
        else
        {
            total_wars.hide();
            total_pop.hide();
            surviving.hide();
            historical.hide();
        }
    }
}

void debugwindow::paintEvent(QPaintEvent *)
{
    QPixmap image2;
    QPixmap image3;
    if(view==0)
    {
        if(showing!=NULL)
        {
            if(showing->AtWar==0)
            {
                image2.load(":/new/images/images/ui/garrison.jpg");
                QPainter painter(this);
                painter.drawPixmap(0,90,400,250,image2);
            }
            else
            {
                image2.load(":/new/images/images/ui/fleetbattle.jpg");
                QPainter painter(this);
                painter.drawPixmap(0,90,400,250,image2);
            }
        }


    }
    if(view==1)
    {
        if(showing!=NULL)
        {
            if(showing->owner!=NULL)
            {
                if(showing->owner->picture==0)
                {
                    image2.load(":/new/images/images/ui/city1.jpg");
                }
                if(showing->owner->picture==1)
                {
                    image2.load(":/new/images/images/ui/city2.jpg");
                }
                if(showing->owner->picture==2)
                {
                    image2.load(":/new/images/images/ui/city4.jpg");
                }
                if(showing->owner->picture==3)
                {
                    image2.load(":/new/images/images/ui/city5.jpg");
                }
                QPainter painter(this);
                painter.drawPixmap(0,90,400,250,image2);
            }
        }

    }
    if(view==2||view==3)
    {
        image2.load(":/new/images/images/ui/galaxy.jpg");
        QPainter painter(this);
        painter.drawPixmap(0,90,400,250,image2);
    }
    QPixmap image1;
    int i=rand()%2;
    if(i==0)
    {
        image1.load(":/new/images/images/EmptyStar.png");
        QPainter painter2(this);
        painter2.drawPixmap(0,0,20,20,image1);
    }
    if(i==1)
    {
        image1.load(":/new/images/images/underAttack.png");
        QPainter painter2(this);
        painter2.drawPixmap(0,0,20,20,image1);
    }
}

void debugwindow::viewChange0()
{
    if(view!=3)
    view=0;
}

void debugwindow::viewChange1()
{
    if(view!=3)
    view=1;
}

void debugwindow::viewChange2()
{
    if(view!=3)
    view=2;
}


//QLabel civ_pop_upkeep;
//QLabel civ_research;
//QLabel civ_milit;
//QLabel civ_science;

