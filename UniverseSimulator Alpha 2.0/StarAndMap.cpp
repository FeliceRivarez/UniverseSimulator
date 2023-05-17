#include <iostream>
#include <cmath>
#include "StarAndMap.h"
#include "mainwindow.h"
#include <QFile>
#include "Log.h"
using namespace std;

double distance(double x1, double y1, double x2, double y2)
{
    double a = x1 - x2;
    double b = y1 - y2;
    return sqrt(a * a + b * b);
}

Map::Map(int a,int b):root(NULL,NULL)//星系数，文明数
{
    root.IsRoot=1;
    root.IsLast=1;
    root.a=this;
    this->n_stars = a;
    s = new star[a];
    this->n_civ = b;
    c = new civ[b];
    for (int i = 0; i < b; i += 1)
    {
        c[i].own = new star * [a];
        c[i].observed = new star * [a];
        c[i].num=i;
        c[i].icon_set();
    }
}

void Map::mapgen(int a)
{
    srand(time(NULL));
    for (int i = 0; i < a; i += 1)//要求生成a个星系
    {
        if (i == 0)//第一个星系不会出现和别的星系重合的情况
        {
            s[i].x = abs(rand() % length);
            s[i].y = abs(rand() % height);
            //初始化
            s[i].n = i;
            s[i].res = rand() % 5 + 1;
            s[i].poplim = rand() % 90 + 10;
            s[i].owned=0;
            s[i].ongoing.location=&s[i];
        }
        else
        {
            s[i].n = i;
            s[i].res = rand() % 5 + 1;
            s[i].poplim = rand() % 90 + 10;
            s[i].owned=0;
            s[i].ongoing.location=&s[i];
            int token = 0;
            while (token == 0)
            {
                int temp_x = abs(rand() % length);
                int temp_y = abs(rand() % height);
                int token1 = 0;
                for (int i1 = 0; i1 < i && token1 == 0; i1 += 1)
                {
                    if (distance(temp_x, temp_y, s[i1].x, s[i1].y) < 20)//检验是否重合
                    {
                        token1 = 1;
                    }
                }
                if (token1 == 1)
                {
                    continue;//如果与别的星系重合了，就重新随机生成一个坐标
                }
                if (token1 == 0)
                {
                    s[i].x = temp_x;
                    s[i].y = temp_y;
                    token = 1;
                }
            }
        }
    }
}

void Map::civdis(int a, int b)//星系数，文明数，同时是重要的文明初始化函数
{
    srand(time(NULL));
    int* assigned = new int[a];
    for (int i = 0; i < b; )
    {
        int temp = rand() % a;
        int token;
        token = 0;
        for (int i1 = 0; i1 < i&&token==0; i1 += 1)
        {
            if (assigned[i1] == temp)
            {
                token = 1;
            }
        }
        if (token == 0)
        {
            c[i].own[0] = &s[temp];
            c[i].owned+=1;
            c[i].upper=this;
            c[i].picture=rand()%4;
            int chance=rand()%1000;
            if(300>=chance)
            {
                c[i].type=1;
            }
            if(300<chance&&chance<=600)
            {
                c[i].type=2;
            }
            if(600<chance&&chance<=900)
            {
                c[i].type=3;
            }
            if(900<chance)
            {
                c[i].type=4;
            }
            c[i].list=new civ*[b];
            c[i].list_rela=new int[b];
            for(int i1=0;i1<b;i1+=1)
            {
                c[i].list_rela[i1]=0;
            }
            c[i].warWith=new civ*[b];
            c[i].truce=new int[b];
            for(int i1=0;i1<2;i1+=1)
            {
                c[i].truce[i1]=0;
            }
            assigned[i] = temp;
            s[temp].pop=s[temp].poplim/2;
            s[temp].owned=1;
            s[temp].owner=&c[i];
            i += 1;
        }
        token = 0;
    }
    delete[] assigned;
}



star::star():button_icon(":/new/images/images/EmptyStar.png"),attacked(":/new/images/images/underAttack.png")
{
    natural_value=poplim/20+res;
}
void star::change()//按钮测试函数
{
    if(owned==0)
    {
        if(selected==0)
        {
            QIcon *newIcon=new QIcon (":/new/images/images/icon_1.png");
            this->setIcon(*newIcon);
            delete newIcon;
            selected=1;
        }
        else
        {
            QIcon *newIcon=new QIcon (":/new/images/images/HugeStar.png");
            this->setIcon(*newIcon);
            delete newIcon;
            selected=0;
        }
    }
}

void star::select()
{
    debugwindow->showing=this;
}

void civ::nextTurn()
{
    diplo_rela();
    defconSet();
    population();
    balance();
    defense();
    milit_gain();
    expand();
    repaint();
    allocate();
    militAllocate();
    attack();
    reinforce();
}

void Map::observe()
{
    for(int i=0;i<n_civ;i+=1)//遍历所有文明
    {
        int temp=0;//文明可观测星系总数
        for(int i1=0;i1<c[i].owned;i1+=1)//遍历单个文明的所有星系
        {
            for(int i2=0;i2<n_stars;i2+=1)//遍历全图星系
            {
                //if(s[i2].owned==1)
                //{
                    //continue;
                //}
                int token=0;
                for(int i3=0;i3<c[i].owned&&token==0;i3+=1)//遍历单个文明所有星系，确认待测星系不属于此文明
                {
                    if(c[i].own[i3]->n==s[i2].n)
                    {
                        token=1;
                    }
                }
                for(int i3=0;i3<temp&&token==0;i3+=1)//遍历单个文明所有已观测星系，确认待测星系未被观测
                {
                    if(c[i].observed[i3]->n==s[i2].n)
                    {
                        token=1;
                    }
                }
                if(token==0)
                {
                    if(distance(c[i].own[i1]->x,c[i].own[i1]->y,s[i2].x,s[i2].y)<70)
                    {
                        c[i].observed[temp]=&s[i2];
                        temp+=1;
                    }
                }
                token=0;
            }
        }
        for(int i1=temp;i1<n_stars;i1+=1)//清理残余指针
        {
            c[i].observed[i1]=NULL;
        }
        c[i].observe=temp;
        temp=0;
    }
}

void civ::repaint()//给星系换颜色
{
    for(int i1=0;i1<owned;i1+=1)
    {
        own[i1]->setIcon(*I);
        if(own[i1]->AtWar==1)
        {
            own[i1]->setIcon(own[i1]->attacked);
        }
    }
};

void civ::expand()//扩张
{
    srand(time(NULL));
    if(observe==0)//如果没有能观测的非己方星系，直接退出
    {
        return;
    }
    int **candidate=new int*[observe];
    for(int i=0;i<observe;i+=1)
    {
        candidate[i]=new int[2];
    }
    int n_candidate=0;
    for(int i=0;i<observe;i+=1)//对所有已观测星系遍历，无主则将序号和权重放入二维数组
    {
        if(observed[i]->owned==0)
        {
            candidate[n_candidate][0]=i;
            candidate[n_candidate][1]=(observed[i]->poplim / 20 + observed[i]->res) / 2 + (rand() % (observed[i]->poplim / 20 + observed[i]->res)) / 2;
            n_candidate+=1;
        }
    }
    for(int i=0;i<n_candidate;i+=1)//对候选二维数组进行排序
    {
        for(int i1=0;i1<n_candidate-1;i1+=1)
        {
            if(candidate[i1][1]<candidate[i1+1][1])
            {
                int temp1=candidate[i1][0];
                int temp2=candidate[i1][1];
                candidate[i1][0]=candidate[i1+1][0];
                candidate[i1][1]=candidate[i1+1][1];
                candidate[i1+1][0]=temp1;
                candidate[i1+1][1]=temp2;
            }
        }
    }
    if(resource>=1000*owned/5&&n_candidate>0)//必须强调候选数量大于零，不然会出现错误导致程序退出
    {
        resource-=1000*owned/5;
        own[owned]=observed[candidate[0][0]];
        observed[candidate[0][0]]->owned=1;
        if(observed[candidate[0][0]]->poplim>=5)
        {
            observed[candidate[0][0]]->pop=5;
        }
        else
        {
            observed[candidate[0][0]]->pop=observed[candidate[0][0]]->poplim;
        }
        observed[candidate[0][0]]->owner=this;
        owned+=1;
    }
    n_candid=n_candidate;
    for(int i=0;i<observe;i+=1)
    {
        delete[] candidate[i];
    }
    delete[] candidate;
};

void Map::assign(int x)
{
    int temp=0;
    for(int i1=0;i1<n_stars;i1+=1)
    {
        if(s[i1].owner==&c[x])
        {
            c[x].own[temp]=&s[i1];
            temp+=1;
        }
    }
    c[x].owned=temp;
    if(temp==0)
    {
        c[x].destroyed=1;
    }
}

void Map::totalpop()
{
    int temp=0;
    for(int i=0;i<n_stars;i+=1)
    {
        temp+=s[i].pop;
    }
    total_pop=temp;
}

void Map::survived()
{
    int temp=0;
    for(int i=0;i<n_civ;i+=1)
    {
        if(c[i].destroyed==0)
        {
            temp+=1;
        }
    }
    surviving=temp;
}

civ* Map::strongest()
{
    int **civs=new int*[n_civ];
    for(int i=0;i<n_civ;i+=1)
    {
        civs[i]=new int[2];
    }

    for(int i=0;i<n_civ;i+=1)//对所有已观测星系遍历，无主则将序号和权重放入二维数组
    {
        civs[i][1]=c[i].owned*50+c[i].total_pop+c[i].income/2000+c[i].science*1000;//第一列：文明序号
        civs[i][0]=i;
    }
    for(int i=0;i<n_civ;i+=1)//对候选二维数组进行排序
    {
        for(int i1=0;i1<n_civ-1;i1+=1)
        {
            if(civs[i1][1]<civs[i1+1][1])
            {
                int temp=civs[i1][0];
                int temp1=civs[i1][1];
                civs[i1][0]=civs[i1+1][0];
                civs[i1][1]=civs[i1+1][1];
                civs[i1+1][0]=temp;
                civs[i1+1][1]=temp1;
            }
        }
    }
    int temp=civs[0][0];
    delete[] civs;
    return &c[temp];
}

void Map::writeLog()
{
    /*char buffer[128];
    QFile file(pathFile);//上面操作已经创建出来的文件名
    file.open(QIODevice::WriteOnly|QIODevice::Append|QIODevice::Text);
    sprintf(buffer, "第%d回合：",father->t);
    QTextStream txtOutput(&file);
    //civ* temp=strongest();
    txtOutput <<'\n'<< buffer<<'\n';
    sprintf(buffer, "剩余文明:%d",surviving);
    txtOutput << buffer<<'\n';

    sprintf(buffer, "正在进行的战争:%d",n_wars);
    txtOutput << buffer<<'\n';

    sprintf(buffer, "曾经发生过的战争:%d",historical);
    txtOutput << buffer<<'\n';

    sprintf(buffer, "现存总人口:%d",total_pop);
    txtOutput << buffer<<'\n';

    /*sprintf(buffer, "目前最强大的文明:%d",temp->num);
    txtOutput << buffer<<'\n';

    sprintf(buffer, "   该文明占有星系数:%d",temp->owned);
    txtOutput << buffer<<'\n';

    sprintf(buffer, "   该文明人口总数:%d",temp->total_pop);
    txtOutput << buffer<<'\n';

    sprintf(buffer, "   该文明军事力量:%f",temp->power);
    txtOutput << buffer<<'\n';

    sprintf(buffer, "   该文明科技水平:%f",temp->science);
    txtOutput << buffer<<'\n';

    sprintf(buffer, "   该文明总收入:%d",temp->income);
    txtOutput << buffer<<'\n';

    if(temp->type==1)
    {
        sprintf(buffer, "   文明类型：友好型");
        txtOutput << buffer<<'\n';
    }
    if(temp->type==2)
    {
        sprintf(buffer, "   文明类型：保守型");
        txtOutput << buffer<<'\n';;
    }
    if(temp->type==3)
    {
        sprintf(buffer, "   文明类型：侵略型");
        txtOutput << buffer<<'\n';
    }
    if(temp->type==4)
    {
        sprintf(buffer, "   文明类型：灭绝政体");
        txtOutput << buffer<<'\n';
    }*/
}
