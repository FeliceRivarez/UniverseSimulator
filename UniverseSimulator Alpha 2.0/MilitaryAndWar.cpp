#include <iostream>
#include <cmath>
#include "StarAndMap.h"
using namespace std;

void civ::milit_gain()
{
    power+=milit;
}

void civ::defense()
{
    for(int i=0;i<owned;i+=1)
    {
        if(own[i]->AtWar==0&&own[i]->def<int(own[i]->pop/5))
        {
            own[i]->def+=0.1;
        }
    }
}

War::War(civ* side1, civ* side2)//节点的初始化
{
    side01=side1;//指向战争双方的指针
    side02=side2;
}

void War::terminate()
{
    side01->num_wars-=1;//战争数量减一
    side02->num_wars-=1;
    side01->truce[side02->num]=50;//停战期
    side02->truce[side01->num]=50;
    if(IsRoot==1)
    {
        return;//若是根节点，不执行delete操作
    }
    //链表的节点删除
    if(next!=NULL)
    {
        previous->next=next;
        next->previous=previous;
    }
    if(IsLast==1&&IsRoot==0)
    {
        a->last=previous;
        previous->next=NULL;
        previous->IsLast=1;
    }
    delete this;
}

void War::goThrough()//遍历战争链表，每回合执行一次
{
    if(IsRoot==1)
    {
        if(next!=NULL)
        {
            next->goThrough();
        }
        return;
    }
    length+=1;
    if(length>=limit)//达到时间限制之后，对这个节点进行标记
    {
        for(int i=0;i<side01->contacts;i+=1)
        {
            if(side01->list[i]==side02)
            {
                side01->list_rela[i]=0;
            }
        }
        for(int i=0;i<side02->contacts;i+=1)
        {
            if(side02->list[i]==side01)
            {
                side02->list_rela[i]=0;
            }
        }
        overtime=1;
    }
    if(IsLast==1)
    {
        return;
    }
    next->goThrough();//递归
    return;
}

void Map::termin()//遍历整个链表并且对被标记为已经结束的战争进行删除
{
    War*temp;
    temp=&root;
    if(temp->next==NULL)
    {
        return;
    }
    temp=root.next;
    while(temp->IsLast==0)
    {
        if(temp->overtime==1)
        {
            if(temp->next!=NULL)
            {
                temp=temp->next;
                temp->terminate();
            }
        }
        else
        {
            temp=temp->next;
        }
    }
    if(temp->overtime==1)
    {
        temp->terminate();
    }
    return;
}

void Map::warlist()//在每次有战争结束时，对链表进行重新遍历，初始化每一个文明的战争数量与作战对象
{
    n_wars=0;
    War*temp;
    temp=&root;
    for(int i=0;i<n_civ;i+=1)
    {
        c[i].num_wars=0;
    }
    if(temp->next!=NULL)
    {
        temp=temp->next;
    }
    while(temp->next!=NULL)
    {
        temp->side01->warWith[temp->side01->num_wars]=temp->side02;
        temp->side02->warWith[temp->side02->num_wars]=temp->side01;
        temp->side01->num_wars+=1;
        temp->side02->num_wars+=1;
        temp=temp->next;
        n_wars+=1;
    }
    if(temp->IsLast==1&&temp->IsRoot==0)
    {
        temp->side01->warWith[temp->side01->num_wars]=temp->side02;
        temp->side02->warWith[temp->side02->num_wars]=temp->side01;
        temp->side01->num_wars+=1;
        temp->side02->num_wars+=1;
        n_wars+=1;
    }
    return;
}
void Map::conflict()//战争创建机制
{
    srand(time(NULL));
    for(int i=0;i<n_civ;i+=1)//对所有文明遍历
    {
        if(c[i].destroyed==1)//被摧毁的文明不能宣战
        {
            continue;
        }
        for(int i1=0;i1<c[i].contacts;i1+=1)//对所有联系的文明遍历
        {
            int token=0;
            for(int i2=0;i2<c[i].num_wars&&token==0;i2+=1)//遍历所有与该文明交战的文明，确保目前没有交战
            {
                if(c[i].warWith[i2]==c[i].list[i1])
                {
                    token=1;
                }
            }
            if(c[i].truce[c[i].list[i1]->num]>0)//确保没有停战
            {
                token=1;
            }
            if(c[i].list[i1]->destroyed==1)//不能对已经灭亡的文明宣战
            {
                token=1;
            }
            if(token==1)//如果已经交战或者仍然处在停战状态，跳过与该文明的战争判定
            {
                token=0;
                continue;
            }
            int chance=rand()%1000;
            if(c[i].list_rela[i]>=100&&c[i].list_rela[i]<500)//小型冲突
            {
                if(chance<=1*agressive)
                {
                    War *n=new War(&c[i],c[i].list[i1]);
                    n->limit=5+rand()%10;
                    n->a=this;
                    n->IsLast=1;
                    n->previous=last;
                    last->next=n;
                    last->IsLast=0;
                    last=n;
                    historical+=1;
                }
                continue;
            }
            if(c[i].list_rela[i]>=0&&c[i].list_rela[i]<100)//中型冲突
            {
                if(chance<=5*agressive)
                {
                    War *n=new War(&c[i],c[i].list[i1]);
                    n->limit=15+rand()%30;
                    n->a=this;
                    n->IsLast=1;
                    n->previous=last;
                    last->next=n;
                    last->IsLast=0;
                    last=n;
                    historical+=1;
                }
                continue;
            }
            if(c[i].list_rela[i]>=-100&&c[i].list_rela[i]<0)//大型冲突
            {
                if(chance<=10*agressive)
                {
                    War *n=new War(&c[i],c[i].list[i1]);
                    n->limit=rand()%50+25;
                    n->a=this;
                    n->IsLast=1;
                    n->previous=last;
                    last->next=n;
                    last->IsLast=0;
                    last=n;
                    historical+=1;
                }
                continue;
            }
            if(c[i].list_rela[i]>=-200&&c[i].list_rela[i]<-100)//小规模战争
            {
                if(chance<=20*agressive)
                {
                    War *n=new War(&c[i],c[i].list[i1]);
                    n->limit=40+rand()%80;
                    n->a=this;
                    n->IsLast=1;
                    n->previous=last;
                    last->next=n;
                    last->IsLast=0;
                    last=n;
                    historical+=1;
                }
                continue;
            }
            if(c[i].list_rela[i]>=-600&&c[i].list_rela[i]<-200)//中规模战争
            {
                if(chance<=20*agressive)
                {
                    War *n=new War(&c[i],c[i].list[i1]);
                    n->limit=60+rand()%120;
                    n->a=this;
                    n->IsLast=1;
                    n->previous=last;
                    last->next=n;
                    last->IsLast=0;
                    last=n;
                    historical+=1;
                }
                continue;
            }
            if(c[i].list_rela[i]<-600)//全面战争
            {
                if(chance<=30*agressive)
                {
                    War *n=new War(&c[i],c[i].list[i1]);
                    n->limit=100+rand()%200;
                    n->a=this;
                    n->IsLast=1;
                    n->previous=last;
                    last->next=n;
                    last->IsLast=0;
                    last=n;
                    historical+=1;
                }
                continue;
            }
        }
    }
}

void Map::truce()
{
    for(int i=0;i<n_civ;i+=1)
    {
        for(int i1=0;i1<n_civ;i1+=1)
        {
            if(c[i].truce[i1]>0)
            {
                c[i].truce[i1]-=1;
            }
        }
    }
}
//以上为战争判定与创建机制
//--------------------分界线----------------------
//以下为战斗创建与判定机制

void Map::star_value()
{
    for(int i=0;i<n_stars;i+=1)
    {
        s[i].value=s[i].pop+s[i].poplim+10*s[i].res;
    }
}

/*void civ::attack()//进攻
{
    if(num_wars==0)
    {
        return;
    }
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
    for(int i=0;i<observe;i+=1)//对所有已观测星系遍历，若主人为敌对星系，则将序号和权重放入二维数组
    {
        int token=0;
        for(int i1=0;i1<num_wars&&token==0;i1+=1)
        {
            if(observed[i]->owner==warWith[i1])
            {
                token=1;
            }
        }
        if(token==1)
        {
            candidate[n_candidate][0]=i;
            candidate[n_candidate][1]=observed[i]->value/2+(observed[i]->value*rand()%10)/2;
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
};*/

void civ::attack()//进攻
{
    if(num_wars==0)
    {
        return;
    }
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
    for(int i=0;i<observe;i+=1)//对所有已观测星系遍历，若主人为敌对星系，则将序号和权重放入二维数组
    {
        int token=0;
        for(int i1=0;i1<num_wars&&token==0;i1+=1)
        {
            if(observed[i]->owner==warWith[i1])
            {
                token=1;
            }
        }
        if(token==1)
        {
            candidate[n_candidate][0]=i;
            candidate[n_candidate][1]=observed[i]->value/2+(observed[i]->value*rand()%10)/2;
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

    if(n_candidate>0)//必须强调候选数量大于零，不然会出现错误导致程序退出
    {
        int chance=rand()%3;
        double attack_power=(1-chance/10)*power;
        double spendings=0;
        int token=0;
        for(int i=0;i<n_candidate&&token==0;i+=1)//遍历所有目标星系
        {
            int i1=candidate[i][0];
            if(attack_power>=2*observed[i1]->def&&observed[i1]->AtWar==0)
            {
                spendings+=2*observed[i1]->def;
                attack_power-=2*observed[i1]->def;
                observed[i1]->ongoing.atk_power=2*observed[i1]->def;
                observed[i1]->ongoing.nations=this;
                observed[i1]->AtWar=1;
            }
            else
            {
                token=1;
            }
        }
        power-=spendings;
    }
    for(int i=0;i<observe;i+=1)
    {
        delete[] candidate[i];
    }
    delete[] candidate;
};


void battle::fight()
{
    if(location->AtWar==0)
    {
        return;
    }
    srand(time(NULL));
    double atk_lost=location->garrison*(0.5+(rand()%10)/10);
    double garrison_lost=atk_power*(0.5+(rand()%10)/10);
    atk_power-=atk_lost;
    if(atk_power<=0)
    {
        atk_power=0;
        nations=NULL;
        location->AtWar=0;
        return;
    }
    location->garrison-=garrison_lost;
    int token=0;
    if(location->garrison<=0)
    {
        location->garrison=0;
        token=1;
    }
    if(token==0)
    {
        return;
    }
    location->def-=1;
    if(nations->type==1)
    {
        location->def-=2;
    }
    if(nations->type==4)
    {
        if(location->pop>=5)
        {
            location->pop-=rand()%10;
        }
        if(location->pop<5)
        {
            location->pop=5;
        }
    }
    if(nations->type==3)
    {
        if(location->pop>=5)
        {
            location->pop-=rand()%2;
        }
        if(location->pop<5)
        {
            location->pop=5;
        }
    }

    if(location->def<=0)
    {
        civ*temp=location->owner;
        int temp1=location->owner->num;
        location->def=5;
        nations->power+=atk_power;
        atk_power=0;
        nations->own[nations->owned]=location;
        nations->owned+=1;
        location->owner=nations;
        temp->upper->assign(temp1);
        location->AtWar=0;
    }
}

void civ::militAllocate()//防守
{
    for(int i=0;i<owned;i+=1)//对所有已拥有星系遍历，将序号和权重放入二维数组
    {
        if(own[i]->AtWar==0)
        {
            power+=own[i]->garrison;
            own[i]->garrison=0;
        }
    }
    srand(time(NULL));
    int **candidate=new int*[owned];
    for(int i=0;i<owned;i+=1)
    {
        candidate[i]=new int[2];
    }
    int n_candidate=0;
    int totalValue=0;
    for(int i=0;i<owned;i+=1)//对所有已拥有星系遍历，将序号和权重放入二维数组
    {
        candidate[n_candidate][0]=i;
        int t=(own[i]->value/2+(own[i]->value*rand()%10)/2)*((own[i]->AtWar*4)+1);
        totalValue+=t;
        candidate[n_candidate][1]=t;
        n_candidate+=1;
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

    if(n_candidate>0)//必须强调候选数量大于零，不然会出现错误导致程序退出
    {
        int chance=rand()%5;
        double defend_power=(1-chance/10)*power;
        double spendings=0;
        int token=0;
        double basic_defence=defend_power*0.2/owned;
        for(int i=0;i<owned;i+=1)//给所有星系配备基本防御部队
        {
            own[i]->garrison+=basic_defence;
            defend_power-=basic_defence;
            spendings+=basic_defence;
        }
        for(int i=0;i<n_candidate&&token==0;i+=1)//遍历所有目标星系
        {
            if(own[candidate[i][0]]->AtWar==1)//被攻击的星系直接满足需求
            {
                if(defend_power>=own[candidate[i][0]]->ongoing.atk_power)
                {
                    own[candidate[i][0]]->garrison+=1.5*own[candidate[i][0]]->ongoing.atk_power;
                    spendings+=1.5*own[candidate[i][0]]->ongoing.atk_power;
                    defend_power-=1.5*own[candidate[i][0]]->ongoing.atk_power;
                }
                else
                {
                    own[candidate[i][0]]->garrison+=defend_power;
                    spendings+=defend_power;
                    defend_power=0;
                }
            }
            else
            {
                double t_def=candidate[i][1]/totalValue;
                if(defend_power>=t_def)
                {
                    own[candidate[i][0]]->garrison+=t_def;
                    spendings+=t_def;
                    defend_power-=t_def;
                }
                else
                {
                    own[candidate[i][0]]->garrison+=defend_power;
                    spendings+=defend_power;
                    defend_power=0;
                }
            }
        }
        power-=spendings;
    }
    delete[] candidate;
}

void civ::reinforce()
{
    if(num_wars==0)
    {
        return;
    }
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
    for(int i=0;i<observe;i+=1)//对所有已观测星系遍历，若主人为敌对星系，则将序号和权重放入二维数组
    {
        int token=0;
        for(int i1=0;i1<num_wars&&token==0;i1+=1)
        {
            if(observed[i]->owner==warWith[i1])
            {
                token=1;
            }
        }
        if(token==1)
        {
            candidate[n_candidate][0]=i;
            candidate[n_candidate][1]=observed[i]->value/2+(observed[i]->value*rand()%10)/2;
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

    if(n_candidate>0)//必须强调候选数量大于零，不然会出现错误导致程序退出
    {
        int chance=rand()%3;
        double attack_power=power;
        double spendings=0;
        int token=0;
        for(int i=0;i<n_candidate&&token==0;i+=1)//遍历所有目标星系
        {
            int i1=candidate[i][0];
            if(observed[i1]->ongoing.atk_power>observed[i1]->garrison)
            {
                continue;
            }
            if(observed[i1]->ongoing.atk_power<observed[i1]->garrison&&observed[i1]->AtWar==1&&observed[i1]->ongoing.nations==this)
            {
                if(attack_power>=1.2*(observed[i1]->garrison-observed[i1]->ongoing.atk_power))
                {
                    observed[i1]->ongoing.atk_power+=1.2*(observed[i1]->garrison-observed[i1]->ongoing.atk_power);
                    spendings+=1.2*(observed[i1]->garrison-observed[i1]->ongoing.atk_power);
                    attack_power-=1.2*(observed[i1]->garrison-observed[i1]->ongoing.atk_power);
                }
                else
                {
                    token=1;
                }
            }
        }
        power-=spendings;
    }
    for(int i=0;i<observe;i+=1)
    {
        delete[] candidate[i];
    }
    delete[] candidate;
}
