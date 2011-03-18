//中国电子科技集团公司

//第一研究室

//呼文韬

//hu_hu605@163.com

//随机初始种群
//编码方式为格雷码
//选择方法为随机遍历
//采用了精英保存策略
//采用了自适应的交叉率和变异率
//采用了与模拟退火算法相结合的尺度变换
//采用了均匀交叉法

#include <stdlib.h> 
#include <stdio.h> 
#include <math.h> 
#include <iostream.h>
#include <iomanip.h>
#include <time.h>
#include <windows.h>
#define IM1 2147483563
#define IM2 2147483399
#define AM (1.0/IM1)
#define IMM1 (IM1-1)
#define IA1 40014
#define IA2 40692
#define IQ1 53668
#define IQ2 52774
#define IR1 12211
#define IR2 3791
#define NTAB 32
#define NDIV (1+IMM1/NTAB)
#define EPS 1.2e-7
#define RNMX (1.0-EPS)
#define zhizhenjuli 0.005
#define PI 3.14159265358 
#define T0 100000//温度要取得很高才行。
#define zhongqunshu1 200
#define zuobianjie -2000
#define youbianjie 2000
unsigned int seed=0; //seed 为种子，要设为全局变量 
void mysrand(long int i) //初始化种子 
{ 
    seed = -i; 
} 
long a[1];
//double hundun;
//double c=4;
//设置全局变量
struct individual
{
    unsigned *chrom;  //染色体；
    double geti;//变量值
    double shiyingdu; //目标函数的值；
    double fitness;   //变换后的适应度值；
};
individual *zuiyougeti;//精英保存策略
int zhongqunshu;   //种群大小
individual *nowpop;//当前代
individual *newpop;//新一代
double sumfitness;//当代的总适应度fitness
double sumshiyingdu;//当代的总适应度shiyingdu
double maxfitness;//最大适应度
double avefitness;//平均适应度
double maxshiyingdu;//最大适应度
double avgshiyingdu;//平均适应度
float pc;//交叉概率
float pm;//变异概率
int lchrom;//染色体长度
int maxgen;//最大遗传代数 
int gen;//遗传代数
//函数
int flipc(double ,double );//判断是否交叉
int flipm(double );//判断是否变异
int rnd(int low,int high);//产生low与high之间的任意数
void initialize();//遗传算法初始化
void preselectfitness(); //计算sumfiness,avefitness,maxfitness
void generation();
double suijibianli();//产生随机遍历指针
int fuzhi(float );//选择要复制的个体
void crossover(individual ,individual ,individual &,individual &);//交叉
void bianyi(individual &);//变异
void mubiaohanshu(individual &);//计算适应度
void chidubianhuan(individual &);//对shiyingdu进行尺度变换赋给fitness
double ran1(long *);//随机数初始
void bianma(double bianliang,unsigned *p);//编码
double yima(unsigned *p);
void guanjiancanshujisuan();//计算shiyingdu,根据shiyingdu计算sumshiyingdu,对shiyingdu进行尺度变换变成fitness,根据fitness计算sumfitness，avefitness，maxfitness
void jingyingbaoliu();
void glp(int n,int s,int *,int (*)[1],float (*)[1]);//glp生成函数
BOOL Exist(int Val, int Num, int *Array);//判断一个数在前面是否出现过
int cmpfitness(const void *p1,const void *p2)
{
    float i=((individual *)p1)->shiyingdu;//现在是按照"适应度"排序，改成"个体"的话就是按照"个体"排序
    float j=((individual *)p2)->shiyingdu;
    return i<j ? -1:(i==j ? 0:1);//现在是按升序牌排列，将1和-1互换后就是按降序排列
}
void main()
{
    initialize();
    cout<<zuiyougeti->geti<<" "<<zuiyougeti->shiyingdu<<endl;/////////////
    for(gen=1;gen<maxgen;gen++)
    { generation();
    }
    jingyingbaoliu();
    cout<<setiosflags(ios::fixed)<<setprecision(6)<<zuiyougeti->geti<<" "<<setiosflags(ios::fixed)<<setprecision(6)<<(zuiyougeti->shiyingdu)<<endl;////////////////
    delete [] newpop;
    delete [] nowpop;
    delete [] zuiyougeti;
    system("pause");
}
void initialize()
{
    int q[zhongqunshu1][1],s=1;
    float xx[zhongqunshu1][1];//生成的glp用x储存
    int h[1]={1};//生成向量
    zuiyougeti=new individual;//最优个体的生成
    zhongqunshu=200;//种群数量
    nowpop=new individual[zhongqunshu1];//当代
    newpop=new individual[zhongqunshu1];//新一代
    maxgen=150;//最大代数
    gen=0;//起始代
    lchrom=22;//基因数量的初始化
    mysrand(time(0));//随机数种子
    a[0]=seed;//随机数种子
    //对最优个体的初始化
    zuiyougeti->geti=0;
    zuiyougeti->fitness=0;
    zuiyougeti->shiyingdu=0;
    //
    glp(zhongqunshu,s,h,q,xx);
    //for(int i=0;i<zhongqunshu1;i++)//产生初始种群
    //{
    //  for(int j=0;j<s;j++)
    // {
    //  nowpop[i].geti=zuobianjie+(youbianjie-zuobianjie)*xx[i][j];
    // }
    //}
    for(int i=0;i<zhongqunshu1;i++)//产生初始种群
    {
        nowpop[i].geti=zuobianjie+(youbianjie-(zuobianjie))*ran1(a);
    }
    //nowpop[0].geti=999;//////////////////////////
    guanjiancanshujisuan();
    jingyingbaoliu(); //精英保留的实现
    guanjiancanshujisuan();//计算shiyingdu,根据shiyingdu计算sumshiyingdu,对shiyingdu进行尺度变换变成fitness,根据fitness计算sumfitness，avefitness，maxfitness
}
void jingyingbaoliu() //精英保留的实现
{
    individual *zuiyougetiguodu;
    zuiyougetiguodu=new individual[zhongqunshu1];//建立一个过渡数组
    for(int i=0;i<zhongqunshu;i++)//将当代个体复制到过渡数组中
    zuiyougetiguodu[i]=nowpop[i];
    qsort(zuiyougetiguodu,zhongqunshu1,sizeof(individual),&cmpfitness);//按fitness升序排序
    // cout<<"zuiyougetiguodu适应度:"<<zuiyougetiguodu[zhongqunshu1-1].shiyingdu<<endl;///////////
    // cout<<"zuiyougeti适应度:"<<zuiyougeti->shiyingdu<<endl;///////////////////
    //system("pause");
    if(zuiyougetiguodu[zhongqunshu-1].shiyingdu>zuiyougeti->shiyingdu)
    {
        *zuiyougeti=zuiyougetiguodu[zhongqunshu1-1];//如果最优个体的fitness比当代最大的fitness小则用当代的代替之
        //cout<<"zuiyougetiguodu个体:"<<zuiyougetiguodu[zhongqunshu1-1].geti<<endl;/////////////
        //cout<<"zuiyougeti个体:"<<zuiyougeti->geti<<endl;/////////////
    }
    else
    nowpop[rnd(0,(zhongqunshu1-1))]=*zuiyougeti;//否则的话从当代中随机挑选一个用最优个体代替之
    delete [] zuiyougetiguodu;//释放过渡数组
}
void guanjiancanshujisuan()//计算shiyingdu,根据shiyingdu计算sumshiyingdu,对shiyingdu进行尺度变换变成fitness,根据fitness计算sumfitness，avefitness，maxfitness
{
    for(int i=0;i<zhongqunshu;i++)//计算shiyingdu
    mubiaohanshu(nowpop[i]);
    for(i=0;i<zhongqunshu;i++)//对shiyingdu进行尺度变换变成fitness
    chidubianhuan(nowpop[i]);
    preselectfitness();//根据fitness计算sumfitness，avefitness，maxfitness
}
void mubiaohanshu(individual &bianliang)//计算shiyingdu
{
    bianliang.shiyingdu=(bianliang.geti*cos(bianliang.geti)+2.0);//目标函数
}
void chidubianhuan(individual &bianliang)//对shiyingdu进行尺度变换变成fitness
{
    double T;//退火温度
    T=T0*(pow(0.99,(gen+1-1)));
    double sum=0;
    for(int j=0;j<zhongqunshu;j++)
    sum+=exp(nowpop[j].shiyingdu/T);
    bianliang.fitness=exp(bianliang.shiyingdu/T)/sum;//算出fitness
}
void preselectfitness()//根据fitness计算sumfitness，avefitness，maxfitness
{
    int j;
    sumfitness=0;
    for(j=0;j<zhongqunshu;j++)
    sumfitness+=nowpop[j].fitness;
    individual *guodu;
    guodu=new individual[zhongqunshu1];
    for(j=0;j<zhongqunshu;j++)
    guodu[j]=nowpop[j];
    qsort(guodu,zhongqunshu1,sizeof(individual),&cmpfitness);
    maxfitness=guodu[zhongqunshu1-1].fitness;
    avefitness=sumfitness/zhongqunshu1;
    delete [] guodu;
}
void generation()
{
    individual fuqin1,fuqin2,*pipeiguodu,*pipeichi;
    int *peiduishuzu;//用来存放产生的随机配对
    pipeiguodu=new individual[zhongqunshu1];
    pipeichi=new individual[zhongqunshu1];
    peiduishuzu=new int[zhongqunshu1];
    int member1,member2,j=0,fuzhijishu=0,i=0,temp=0,tt=0;
    float zhizhen;
    //随机遍历的实现
    for(zhizhen=suijibianli();zhizhen<1;(zhizhen=zhizhen+zhizhenjuli))//设定指针1/66
    {
        pipeichi[fuzhijishu]=nowpop[fuzhi(zhizhen)];
        fuzhijishu++;
    }

    //交叉与变异的实现
    //交叉
    for(i=0;i<zhongqunshu1;i++)
    {
        peiduishuzu[i]=-1;
    }
    for (i=0; i<zhongqunshu1; i++)
    { 
        temp =rnd(0,zhongqunshu1-1); //产生值在0-zhongqunshu1-1的随机数 
        while(Exist(temp, i, peiduishuzu))//判断产生的随机数是否已经产生过，如果是，则再产生一个随机数 
        {
            temp =rnd(0,zhongqunshu1-1);
        }
        //如果没有的话，则把产生的随机数放在peiduishuzu中 
        *(peiduishuzu+i) = temp; 
    }
    for(i=0;i<zhongqunshu1-1;i=i+2)
    {
        fuqin1=pipeichi[peiduishuzu[i]];
        fuqin2=pipeichi[peiduishuzu[i+1]];
        crossover(fuqin1,fuqin2,newpop[i],newpop[i+1]);
    }
    for(j=0;j<zhongqunshu1;j++)
    {
        //if(newpop[j].geti<-1000)
        //cout<<"个体数值小于下界了";
        nowpop[j].geti=newpop[j].geti;
    }
    //
    guanjiancanshujisuan();
    //变异的实现
    for(j=0;j<zhongqunshu;j++)
    {
        bianyi(nowpop[j]);
    } 
    //
    guanjiancanshujisuan();
    //精英保留的实现
    jingyingbaoliu();
    //
    guanjiancanshujisuan();
    delete [] peiduishuzu;
    delete [] pipeichi;
    delete [] pipeiguodu;
}
void crossover(individual parent1,individual parent2,individual &child1,individual &child2)//交叉
{
    int j;
    unsigned *panduan;
    panduan=new unsigned[lchrom];
    parent1.chrom=new unsigned[lchrom];
    parent2.chrom=new unsigned[lchrom];
    child1.chrom=new unsigned[lchrom];
    child2.chrom=new unsigned[lchrom];
    //cout<<"jiaocha"<<endl;///////////////////////
    bianma(parent1.geti,parent1.chrom);
    bianma(parent2.geti,parent2.chrom);
    if(flipc(parent1.fitness,parent2.fitness))
    {
        for(j=0;j<lchrom;j++)
        panduan[j]=rnd(0,1);
        //for(j=0;j<lchrom;j++)////////////////
        // {
        //  cout<<panduan[j];/////////////
        // }
        // cout<<endl;////////////////
        // system("pause");////////////////
        for(j=0;j<lchrom;j++)
        {
            if(panduan[j]==1)
            child1.chrom[j]=parent1.chrom[j];
            else
            child1.chrom[j]=parent2.chrom[j];
        }
        for(j=0;j<lchrom;j++)
        {
            if(panduan[j]==0)
            child2.chrom[j]=parent1.chrom[j];
            else
            child2.chrom[j]=parent2.chrom[j];
        }
        //for(j=0;j<lchrom;j++)////////////////
        //{
        //  cout<<child1.chrom[j];/////////////
        // }
        //cout<<endl;////////////////
        // system("pause");////////////////
        child1.geti=yima(child1.chrom);
        child2.geti=yima(child2.chrom);
        delete [] child2.chrom;
        delete [] child1.chrom;
        delete [] parent2.chrom;
        delete [] parent1.chrom;
        delete [] panduan;
    }
    else
    {
        for(j=0;j<lchrom;j++)
        {
            child1.chrom[j]=parent1.chrom[j];
            child2.chrom[j]=parent2.chrom[j];
        }
        child1.geti=yima(child1.chrom);
        child2.geti=yima(child2.chrom);
        delete [] child2.chrom;
        delete [] child1.chrom;
        delete [] parent2.chrom;
        delete [] parent1.chrom;
        delete [] panduan;
    }
}
void bianyi(individual &child)//变异
{
    child.chrom=new unsigned[lchrom];
    //cout<<"变异"<<endl;
    bianma(child.geti,child.chrom);
    for(int i=0;i<lchrom;i++)
    if(flipm(child.fitness))
    {
        if(child.chrom[i]=0)
        child.chrom[i]=1;
        else
        child.chrom[i]=0;
    }
    child.geti=yima(child.chrom);
    delete [] child.chrom;
}
void bianma(double bianliang,unsigned *p)//编码
{
    unsigned *q;
    unsigned *gray;
    q=new unsigned[lchrom];
    gray=new unsigned[lchrom];
    int x=0;
    int i=0,j=0;
    if(bianliang<zuobianjie)///////////////////
    {
        cout<<"bianliang:"<<bianliang<<endl;/////////
        system("pause");
    }
    //cout<<youbianjie-(zuobianjie)<<endl;
    //system("pause");
    x=(bianliang-(zuobianjie))*((pow(2,lchrom)-1)/(youbianjie-(zuobianjie)));
    //cout<<x<<endl;///////////
    if(x<0)
    system("pause");///////////
    for(i=0;i<lchrom;i++)
    {
        q[i]=0;
        p[i]=0;
    }
    i=0;
    while (x!=0&&(i!=lchrom)) 
    { 
        q[i]=(unsigned)(x%2); 
        x=x/2; 
        i++;
    } 
    // for(i=0;i<lchrom;i++)//////////////////
    //  cout<<q[i];///////////////
    // cout<<endl;///////////
    int w=lchrom-1;
    if(q[w]!=0&&q[w]!=1)
    system("pause");
    for(j=0;j<lchrom&&w>0;j++)
    {
        p[j]=q[w];
        w--;
    }
    //cout<<"yuanma"<<endl;
    //for(j=0;j<lchrom;j++)///////////
    // cout<<p[j];////////
    //cout<<endl;////////////////////
    gray[0]=p[0];
    for(j=1;j<lchrom;j++)
    {
        if(p[j-1]==p[j])
        gray[j]=0;
        else if(p[j-1]!=p[j])
        gray[j]=1;
    }
    for(j=0;j<lchrom;j++)
    p[j]=gray[j];
    //cout<<"geleima"<<endl;
    //for(j=0;j<lchrom;j++)///////////
    // cout<<p[j];////////
    //cout<<endl;////////////////////
    //system("pause");///////////

    delete [] gray;
    delete [] q;
}
double yima(unsigned *p)  //译码
{

    int i=0;
    // for(i=0;i<lchrom;i++)/////////
    // {
    //  cout<<p[i];//////
    // }
    // cout<<endl;/////////
    // system("pause");//////////
    int x=0;
    unsigned *q;
    q=new unsigned[lchrom];
    q[0]=p[0];
    // cout<<q[0]<<endl;//////////////////
    // system("pause");//////////

    for(int j=1;j<lchrom;j++)
    {
        if(q[j-1]==p[j])
        q[j]=0;
        else if(q[j-1]!=p[j])
        q[j]=1;
    }
    // for(i=0;i<lchrom;i++)//////
    // {
    // cout<<q[i];//////////
    // if(q[i]!=0&&q[i]!=1)
    // {
    //  cout<<q[i];
    //  system("pause");
    // }
    // }
    // cout<<endl;////////
    // system("pause");///////////////////
    for(i=0;i<lchrom;i++)
    x=x+q[i]*pow(2,(lchrom-i-1));
    if(x<0)
    {
        cout<<"译码出错1"<<endl;
        system("pause");
    }
    //cout<<"x:"<<x<<endl;
    double bianliang;
    //cout<<pow(2,22)<<endl;
    //cout<<2000*x<<endl;
    //cout<<(x*(2000/(pow(2,22)-1)))<<endl;
    bianliang=(x*((youbianjie-(zuobianjie))/(pow(2,lchrom)-1)))+zuobianjie;
    if(bianliang<zuobianjie)
    {
        cout<<"译码出错2"<<endl;
        system("pause");
    }
    delete [] q;
    return bianliang;
}
double ran1(long *idum)
{
    int j;
    long k;
    static long idum2=123456789;
    static long iy=0;
    static long iv[NTAB];
    float temp;
    if (*idum <= 0) 
    { 
        if (-(*idum) < 1) *idum=1;
        else *idum = -(*idum);
        idum2=(*idum);
        for (j=NTAB+7;j>=0;j--) 
        { 
            k=(*idum)/IQ1;
            *idum=IA1*(*idum-k*IQ1)-k*IR1;
            if (*idum < 0) *idum += IM1;
            if (j < NTAB) iv[j] = *idum;
        }
        iy=iv[0];
    }
    k=(*idum)/IQ1; 
    *idum=IA1*(*idum-k*IQ1)-k*IR1; 
    if (*idum < 0) *idum += IM1; 
    k=idum2/IQ2;
    idum2=IA2*(idum2-k*IQ2)-k*IR2; 
    if (idum2 < 0) idum2 += IM2;
    j=iy/NDIV;
    iy=iv[j]-idum2;
    iv[j] = *idum; 
    if (iy < 1) iy += IMM1;
    if ((temp=AM*iy) > RNMX) return RNMX; 
    else return temp;
}
double suijibianli()//随机遍历
{
    double i=ran1(a);
    while(i>zhizhenjuli)
    {
        i=ran1(a);
    }
    //cout<<i<<endl;//////////////
    return i;
}
int fuzhi(float p)//复制
{
    int i;
    double sum=0;
    if(sumfitness!=0)
    {
        for(i=0;(sum<p)&&(i<zhongqunshu);i++)
        sum+=nowpop[i].fitness/sumfitness;
    }
    else
    i=rnd(1,zhongqunshu1);
    return(i-1);
}

int rnd(int low, int high)           /*在整数low和high之间产生一个随机整数*/
{
    int i;
    if(low >= high)
    i = low;
    else
    {
        i =(int)((ran1(a) * (high - low + 1)) + low);
        if(i > high) i = high;
    }
    return(i);
}
int flipc(double p,double q)//判断是否交叉
{
    double pc1=0.9,pc2=0.6;
    if((p-q)>0)
    {
        if(p>=avefitness)
        {
            pc=pc1-(pc1-pc2)*(p-avefitness)/(maxfitness-avefitness);
        }
        else
        pc=pc1;
    }
    else
    {
        if(q>=avefitness)
        {
            pc=pc1-(pc1-pc2)*(q-avefitness)/(maxfitness-avefitness);
        }
        else
        pc=pc1;
    }
    if(ran1(a)<=pc)
    return(1);
    else
    return(0);
}
int flipm(double p)//判断是否变异
{
    double pm1=0.001,pm2=0.0001;
    if(p>=avefitness)
    {
        pm=(pm1-(pm1-pm2)*(maxfitness-p)/(maxfitness-avefitness));
    }
    else
    pm=pm1;
    if(ran1(a)<=pm)
    return(1);
    else
    return(0);
}
void glp(int n,int s,int *h,int (*q)[1],float (*xx)[1])//glp
{
    int i=0,j=0;
    //求解q
    for(i=0;i<n;i++)
    {
        for(j=0;j<s;j++)
        {
            *(*(q+i)+j)=((i+1)*(*(h+j)))%n;
        }
    }
    i=n-1;
    for(j=0;j<s;j++)
    {
        *(*(q+i)+j)=n;
    }
    //求解x
    for(i=0;i<n;i++)
    {
        for(j=0;j<s;j++)
        {
            *(*(xx+i)+j)=(float)(2*(*(*(q+i)+j))-1)/(2*n);
        }
    }
}
BOOL Exist(int Val, int Num, int *Array)//判断一个数是否在一个数组的前Num个数中 
{ 
    BOOL FLAG = FALSE; 
    int i; 
    for (i=0; i<Num; i++) 
    if (Val == *(Array + i))
    { 
        FLAG = TRUE; 
        break; 
    } 
    return FLAG; 
}