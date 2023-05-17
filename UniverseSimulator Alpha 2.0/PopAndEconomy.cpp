#include <iostream>
#include <cmath>
#include "StarAndMap.h"
using namespace std;


void civ::balance()
{
    int temp=0;
    for(int i1=0;i1<owned;i1+=1)
    {
        temp+=(own[i1]->pop)*(own[i1]->res)*science;
    }
    temp-=research+milit+total_pop*pop_upkeep;
    income=temp;
    resource+=int(income);
    sci_dev();
};//获取结余资源

void civ::population()
{
    for(int i1=0;i1<owned;i1+=1)//人口增长
    {
        if(own[i1]->pop<=own[i1]->poplim/2)//达到2K之前
        {
            own[i1]->growth+=own[i1]->pop*(pop_upkeep-0.2);
        }
        else//达到2k之后
        {
            own[i1]->growth+=(own[i1]->poplim/2)*(pop_upkeep-0.2)-0.3*((own[i1]->pop)-(own[i1]->poplim/2));
        }
        if(own[i1]->pop<=own[i1]->poplim)
        {
            if(own[i1]->growth>100)//按照百分点增长每个pop
            {
                own[i1]->pop+=1;
                own[i1]->growth=0;
            }
        }
        else
        {
            continue;
        }
    }
    int temp=0;//人口核算
    for(int i1=0;i1<owned;i1+=1)
    {
        temp+=own[i1]->pop;
    }
    total_pop=temp;
    temp=0;
    for(int i=0;i<owned;i+=1)
    {
        temp+=own[i]->poplim;
    }
    total_capacity=temp;
}

void civ::allocate()
{
    double savings=0;//存储权重
    double sci=0;//科研权重
    double pop_gr=0;//人口权重
    double max_pop=0;//可接受的最高人口维护费
    max_pop=income*0.5/total_pop;
    double min_pop=0.3;//可接受的最低人口维护费
    pop_gr=total_capacity/total_pop;
    if(n_candid>=1)
    {
        savings=1;
    }
    if(defcon<=1)
    {
        if(pop_gr<=0.5)
        {
            pop_upkeep=max_pop;
        }
        if(pop_gr>0.5)
        {
            pop_upkeep=(max_pop+min_pop)/2;
        }
        double leftover=income-pop_upkeep*total_pop;
        if(savings==1)
        {
           research = leftover*0.2;
           milit=leftover*0.1;
        }
        else
        {
           research = leftover*0.8;
           milit=leftover*0.1;
        }

    }
    if(defcon<=2&&defcon>1)
    {
        if(pop_gr<=0.5)
        {
           pop_upkeep=max_pop*0.7+min_pop*0.3;
        }
        if(pop_gr>0.5)
        {
           pop_upkeep=(max_pop+min_pop)/2;
        }
        double leftover=income-pop_upkeep*total_pop;
        if(savings==1)
        {
           research = leftover*0.2;
           milit=leftover*0.2;
        }
        else
        {
           research = leftover*0.75;
           milit=leftover*0.15;
        }
    }
    if(defcon<=3&&defcon>2)
    {
        if(pop_gr<=0.5)
        {
           pop_upkeep=max_pop*0.6+min_pop*0.4;
        }
        if(pop_gr>0.5)
        {
           pop_upkeep=max_pop*0.3+min_pop*0.7;
        }
        double leftover=income-pop_upkeep*total_pop;
        if(savings==1)
        {
           research = leftover*0.2;
           milit=leftover*0.2;
        }
        else
        {
           research = leftover*0.6;
           milit=leftover*0.3;
        }
    }
    if(defcon<=4&&defcon>3)
    {
        if(pop_gr<=0.5)
        {
           pop_upkeep=max_pop*0.5+min_pop*0.5;
        }
        if(pop_gr>0.5)
        {
           pop_upkeep=max_pop*0.2+min_pop*0.8;
        }
        double leftover=income-pop_upkeep*total_pop;
        if(savings==1)
        {
           research = leftover*0.1;
           milit=leftover*0.4;
        }
        else
        {
           research = leftover*0.4;
           milit=leftover*0.5;
        }
    }
    if(defcon>4)
    {
        pop_upkeep=min_pop;
        double leftover=income-pop_upkeep*total_pop;
        if(savings==1)
        {
           research = leftover*0.1;
           milit=leftover*0.5;
        }
        else
        {
           research = leftover*0.2;
           milit=leftover*0.7;
        }
        if(resource>100)
        {
           int use=resource*0.1;
           resource-=use;
           power+=0.8*use;
        }
    }
}

void civ::sci_dev()
{
    srand(time(NULL));
    int chance=rand()%1000;
    if(chance<=700)
    {
        science_add+=((rand()%5)/10+0.7)*research;
    }
    if(700<chance&&chance<=900)
    {
        science_add+=((rand()%5)/10+1)*research;
    }
    if(900<chance&&chance<=950)
    {
        science_add+=((rand()%5)/10)*research;
    }
    if(950<chance&&chance<=1000)
    {
        science_add+=10*research;
    }
    while(science_add>=2000*pow(3,science))
    {
        science_add-=2000*pow(3,science);
        science+=0.1;
    }
}

void civ::defconSet()
{
    double opposing_power=0;
    for(int i=0;i<contacts;i+=1)
    {
        if(list_rela[i]<-100&&list_rela[i]>=-500)
        {
           opposing_power+=list[i]->power/2;
        }
        if(list_rela[i]<-500)
        {
           opposing_power+=list[i]->power;
        }
    }
    if(power>0)
    {
        defcon=1+opposing_power/power;
    }
    if(num_wars>=1)
    {
        defcon+=1;
    }
    if(defcon>5)
    {
        defcon=5;
    }
}
