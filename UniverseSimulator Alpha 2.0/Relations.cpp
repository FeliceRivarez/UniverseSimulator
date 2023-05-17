#include <iostream>
#include <cmath>
#include "StarAndMap.h"
using namespace std;

void civ::diplo_rela()
{
    for(int i=0;i<observe;i+=1)//遍历所有可观测星系
    {
        int token;
        token=0;
        if(observed[i]->owner==NULL)//观测到的星系无主，则遍历下一个
        {
            continue;
        }
        for(int i1=0;i1<contacts&&token==0;i1+=1)
        {
            if(list[i1]==observed[i]->owner)
            {
                token=1;
            }
        }
        if(token==0)
        {
            list[contacts]=observed[i]->owner;
            contacts+=1;
        }
        token=0;
    }
}

void Map::diplomatic()
{
    srand(time(NULL));
    for(int i=0;i<n_civ;i+=1)//遍历所有文明
    {
        int chance=rand()%1000;
        for(int i1=0;i1<c[i].contacts;i1+=1)
        {
            int other;
            int token=0;
            for(int i2=0;i2<c[i].list[i1]->contacts&&token==0;i2+=1)
            {
                if(c[i].list[i1]->list[i2]==&c[i])
                {
                    other=i2;
                    token=1;
                }
            }
            token=0;
            if(c[i].type==1&&c[i].list[i1]->type==1)//友好vs友好
            {

                if(chance<800)
                {
                c[i].list_rela[i1]+=rand()%10;
                }
                else
                {
                    c[i].list_rela[i1]-=rand()%10;
                }
                c[i].list[i1]->list_rela[other]=c[i].list_rela[i1];
            }
            if((c[i].list[i1]->type==1&&c[i].type==2)||(c[i].list[i1]->type==2&&c[i].type==1))//友好vs保守
            {
                if(chance<500)
                {
                    c[i].list_rela[i1]+=rand()%10;
                }
                else
                {
                    c[i].list_rela[i1]-=rand()%10;
                }
                c[i].list[i1]->list_rela[other]=c[i].list_rela[i1];
            }
            if((c[i].list[i1]->type==1&&c[i].type==3)||(c[i].list[i1]->type==3&&c[i].type==1))//友好vs侵略
            {
                if(chance<200)
                {
                    c[i].list_rela[i1]+=rand()%10;
                }
                else
                {
                    c[i].list_rela[i1]-=rand()%10;
                }
                c[i].list[i1]->list_rela[other]=c[i].list_rela[i1];
            }
            if(c[i].list[i1]->type==2&&c[i].type==2)//保守vs保守
            {
                if(chance<600)
                {
                    c[i].list_rela[i1]+=rand()%5;
                }
                else
                {
                    c[i].list_rela[i1]-=rand()%5;
                }
                c[i].list[i1]->list_rela[other]=c[i].list_rela[i1];
            }
            if((c[i].list[i1]->type==2&&c[i].type==3)||(c[i].list[i1]->type==3&&c[i].type==2))//保守vs侵略
            {
                if(chance<100)
                {
                    c[i].list_rela[i1]+=rand()%10;
                }
                else
                {
                    c[i].list_rela[i1]-=rand()%5;
                }
                c[i].list[i1]->list_rela[other]=c[i].list_rela[i1];
            }
            if(c[i].list[i1]->type==2&&c[i].type==2)//侵略vs侵略
            {
                if(chance<100)
                {
                    c[i].list_rela[i1]+=rand()%5;
                }
                else
                {
                    c[i].list_rela[i1]-=rand()%10;
                }
                c[i].list[i1]->list_rela[other]=c[i].list_rela[i1];
            }
            if((c[i].list[i1]->type==4)||(c[i].type==4))//保守vs侵略
            {
                c[i].list_rela[i1]=-1000;
                c[i].list[i1]->list_rela[other]=c[i].list_rela[i1];
            }
            if(c[i].list[i1]->destroyed==1)
            {
                c[i].list_rela[i1]=0;
            }
        }
        int temp_f=0;
        int temp_o=0;
        int temp_e=0;
        for(int i1=0;i1<c[i].contacts;i1+=1)
        {
            if(c[i].list_rela[i1]>100)
            {
                temp_f+=1;
            }
            if(c[i].list_rela[i1]<-100&&c[i].list_rela[i1]>=-500)
            {
                temp_o+=1;
            }
            if(c[i].list_rela[i1]<-500)
            {
                temp_e+=1;
            }
        }
        c[i].friends=temp_f;
        c[i].opponents=temp_o;
        c[i].enemies=temp_e;
    }
}

