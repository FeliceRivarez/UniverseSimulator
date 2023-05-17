#include "StarAndMap.h"
#include <QIcon>

civ::civ(int t)
{
    own = new star * [t];
    observed = new star * [t];
    for (int i = 0; i < t; i += 1)
    {
        own[i] = NULL;
        observed[i] = NULL;
    }    
}

civ::civ()
{

}

void civ::icon_set()
{
    if(num==0)
    {
        I=new QIcon(":/new/images/images/stars/star1.png");
    }
    if(num==1)
    {
        I=new QIcon(":/new/images/images/stars/star2.png");
    }
    if(num==2)
    {
        I=new QIcon(":/new/images/images/stars/star3.png");
    }
    if(num==3)
    {
        I=new QIcon(":/new/images/images/stars/Star4.png");
    }
    if(num==4)
    {
        I=new QIcon(":/new/images/images/stars/Star5.png");
    }
    if(num==5)
    {
        I=new QIcon(":/new/images/images/stars/Star6.png");
    }
    if(num==6)
    {
        I=new QIcon(":/new/images/images/stars/Star7.png");
    }
    if(num==7)
    {
        I=new QIcon(":/new/images/images/stars/Star8.png");
    }
    if(num==8)
    {
        I=new QIcon(":/new/images/images/stars/Star9.png");
    }
    if(num==9)
    {
        I=new QIcon(":/new/images/images/stars/Star10.png");
    }
    if(num==10)
    {
        I=new QIcon(":/new/images/images/stars/Star11.png");
    }
    if(num==11)
    {
        I=new QIcon(":/new/images/images/stars/Star12.png");
    }
    if(num==12)
    {
        I=new QIcon(":/new/images/images/stars/Star13.png");
    }
    if(num==13)
    {
        I=new QIcon(":/new/images/images/stars/Star14.png");
    }
    if(num==14)
    {
        I=new QIcon(":/new/images/images/stars/Star15.png");
    }
    if(num==15)
    {
        I=new QIcon(":/new/images/images/stars/Star16.png");
    }
    if(num==16)
    {
        I=new QIcon(":/new/images/images/stars/Star17.png");
    }
    if(num==17)
    {
        I=new QIcon(":/new/images/images/stars/Star18.png");
    }
    if(num==18)
    {
        I=new QIcon(":/new/images/images/stars/Star19.png");
    }
}
