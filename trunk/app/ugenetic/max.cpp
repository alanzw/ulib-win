//�й����ӿƼ����Ź�˾

//��һ�о���

//�����

//hu_hu605@163.com

//�����ʼ��Ⱥ
//���뷽ʽΪ������
//ѡ�񷽷�Ϊ�������
//�����˾�Ӣ�������
//����������Ӧ�Ľ����ʺͱ�����
//��������ģ���˻��㷨���ϵĳ߶ȱ任
//�����˾��Ƚ��淨

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
#define T0 100000//�¶�Ҫȡ�úܸ߲��С�
#define zhongqunshu1 200
#define zuobianjie -2000
#define youbianjie 2000
unsigned int seed=0; //seed Ϊ���ӣ�Ҫ��Ϊȫ�ֱ��� 
void mysrand(long int i) //��ʼ������ 
{ 
    seed = -i; 
} 
long a[1];
//double hundun;
//double c=4;
//����ȫ�ֱ���
struct individual
{
    unsigned *chrom;  //Ⱦɫ�壻
    double geti;//����ֵ
    double shiyingdu; //Ŀ�꺯����ֵ��
    double fitness;   //�任�����Ӧ��ֵ��
};
individual *zuiyougeti;//��Ӣ�������
int zhongqunshu;   //��Ⱥ��С
individual *nowpop;//��ǰ��
individual *newpop;//��һ��
double sumfitness;//����������Ӧ��fitness
double sumshiyingdu;//����������Ӧ��shiyingdu
double maxfitness;//�����Ӧ��
double avefitness;//ƽ����Ӧ��
double maxshiyingdu;//�����Ӧ��
double avgshiyingdu;//ƽ����Ӧ��
float pc;//�������
float pm;//�������
int lchrom;//Ⱦɫ�峤��
int maxgen;//����Ŵ����� 
int gen;//�Ŵ�����
//����
int flipc(double ,double );//�ж��Ƿ񽻲�
int flipm(double );//�ж��Ƿ����
int rnd(int low,int high);//����low��high֮���������
void initialize();//�Ŵ��㷨��ʼ��
void preselectfitness(); //����sumfiness,avefitness,maxfitness
void generation();
double suijibianli();//�����������ָ��
int fuzhi(float );//ѡ��Ҫ���Ƶĸ���
void crossover(individual ,individual ,individual &,individual &);//����
void bianyi(individual &);//����
void mubiaohanshu(individual &);//������Ӧ��
void chidubianhuan(individual &);//��shiyingdu���г߶ȱ任����fitness
double ran1(long *);//�������ʼ
void bianma(double bianliang,unsigned *p);//����
double yima(unsigned *p);
void guanjiancanshujisuan();//����shiyingdu,����shiyingdu����sumshiyingdu,��shiyingdu���г߶ȱ任���fitness,����fitness����sumfitness��avefitness��maxfitness
void jingyingbaoliu();
void glp(int n,int s,int *,int (*)[1],float (*)[1]);//glp���ɺ���
BOOL Exist(int Val, int Num, int *Array);//�ж�һ������ǰ���Ƿ���ֹ�
int cmpfitness(const void *p1,const void *p2)
{
    float i=((individual *)p1)->shiyingdu;//�����ǰ���"��Ӧ��"���򣬸ĳ�"����"�Ļ����ǰ���"����"����
    float j=((individual *)p2)->shiyingdu;
    return i<j ? -1:(i==j ? 0:1);//�����ǰ����������У���1��-1��������ǰ���������
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
    float xx[zhongqunshu1][1];//���ɵ�glp��x����
    int h[1]={1};//��������
    zuiyougeti=new individual;//���Ÿ��������
    zhongqunshu=200;//��Ⱥ����
    nowpop=new individual[zhongqunshu1];//����
    newpop=new individual[zhongqunshu1];//��һ��
    maxgen=150;//������
    gen=0;//��ʼ��
    lchrom=22;//���������ĳ�ʼ��
    mysrand(time(0));//���������
    a[0]=seed;//���������
    //�����Ÿ���ĳ�ʼ��
    zuiyougeti->geti=0;
    zuiyougeti->fitness=0;
    zuiyougeti->shiyingdu=0;
    //
    glp(zhongqunshu,s,h,q,xx);
    //for(int i=0;i<zhongqunshu1;i++)//������ʼ��Ⱥ
    //{
    //  for(int j=0;j<s;j++)
    // {
    //  nowpop[i].geti=zuobianjie+(youbianjie-zuobianjie)*xx[i][j];
    // }
    //}
    for(int i=0;i<zhongqunshu1;i++)//������ʼ��Ⱥ
    {
        nowpop[i].geti=zuobianjie+(youbianjie-(zuobianjie))*ran1(a);
    }
    //nowpop[0].geti=999;//////////////////////////
    guanjiancanshujisuan();
    jingyingbaoliu(); //��Ӣ������ʵ��
    guanjiancanshujisuan();//����shiyingdu,����shiyingdu����sumshiyingdu,��shiyingdu���г߶ȱ任���fitness,����fitness����sumfitness��avefitness��maxfitness
}
void jingyingbaoliu() //��Ӣ������ʵ��
{
    individual *zuiyougetiguodu;
    zuiyougetiguodu=new individual[zhongqunshu1];//����һ����������
    for(int i=0;i<zhongqunshu;i++)//���������帴�Ƶ�����������
    zuiyougetiguodu[i]=nowpop[i];
    qsort(zuiyougetiguodu,zhongqunshu1,sizeof(individual),&cmpfitness);//��fitness��������
    // cout<<"zuiyougetiguodu��Ӧ��:"<<zuiyougetiguodu[zhongqunshu1-1].shiyingdu<<endl;///////////
    // cout<<"zuiyougeti��Ӧ��:"<<zuiyougeti->shiyingdu<<endl;///////////////////
    //system("pause");
    if(zuiyougetiguodu[zhongqunshu-1].shiyingdu>zuiyougeti->shiyingdu)
    {
        *zuiyougeti=zuiyougetiguodu[zhongqunshu1-1];//������Ÿ����fitness�ȵ�������fitnessС���õ����Ĵ���֮
        //cout<<"zuiyougetiguodu����:"<<zuiyougetiguodu[zhongqunshu1-1].geti<<endl;/////////////
        //cout<<"zuiyougeti����:"<<zuiyougeti->geti<<endl;/////////////
    }
    else
    nowpop[rnd(0,(zhongqunshu1-1))]=*zuiyougeti;//����Ļ��ӵ����������ѡһ�������Ÿ������֮
    delete [] zuiyougetiguodu;//�ͷŹ�������
}
void guanjiancanshujisuan()//����shiyingdu,����shiyingdu����sumshiyingdu,��shiyingdu���г߶ȱ任���fitness,����fitness����sumfitness��avefitness��maxfitness
{
    for(int i=0;i<zhongqunshu;i++)//����shiyingdu
    mubiaohanshu(nowpop[i]);
    for(i=0;i<zhongqunshu;i++)//��shiyingdu���г߶ȱ任���fitness
    chidubianhuan(nowpop[i]);
    preselectfitness();//����fitness����sumfitness��avefitness��maxfitness
}
void mubiaohanshu(individual &bianliang)//����shiyingdu
{
    bianliang.shiyingdu=(bianliang.geti*cos(bianliang.geti)+2.0);//Ŀ�꺯��
}
void chidubianhuan(individual &bianliang)//��shiyingdu���г߶ȱ任���fitness
{
    double T;//�˻��¶�
    T=T0*(pow(0.99,(gen+1-1)));
    double sum=0;
    for(int j=0;j<zhongqunshu;j++)
    sum+=exp(nowpop[j].shiyingdu/T);
    bianliang.fitness=exp(bianliang.shiyingdu/T)/sum;//���fitness
}
void preselectfitness()//����fitness����sumfitness��avefitness��maxfitness
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
    int *peiduishuzu;//������Ų�����������
    pipeiguodu=new individual[zhongqunshu1];
    pipeichi=new individual[zhongqunshu1];
    peiduishuzu=new int[zhongqunshu1];
    int member1,member2,j=0,fuzhijishu=0,i=0,temp=0,tt=0;
    float zhizhen;
    //���������ʵ��
    for(zhizhen=suijibianli();zhizhen<1;(zhizhen=zhizhen+zhizhenjuli))//�趨ָ��1/66
    {
        pipeichi[fuzhijishu]=nowpop[fuzhi(zhizhen)];
        fuzhijishu++;
    }

    //����������ʵ��
    //����
    for(i=0;i<zhongqunshu1;i++)
    {
        peiduishuzu[i]=-1;
    }
    for (i=0; i<zhongqunshu1; i++)
    { 
        temp =rnd(0,zhongqunshu1-1); //����ֵ��0-zhongqunshu1-1������� 
        while(Exist(temp, i, peiduishuzu))//�жϲ�����������Ƿ��Ѿ�������������ǣ����ٲ���һ������� 
        {
            temp =rnd(0,zhongqunshu1-1);
        }
        //���û�еĻ�����Ѳ��������������peiduishuzu�� 
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
        //cout<<"������ֵС���½���";
        nowpop[j].geti=newpop[j].geti;
    }
    //
    guanjiancanshujisuan();
    //�����ʵ��
    for(j=0;j<zhongqunshu;j++)
    {
        bianyi(nowpop[j]);
    } 
    //
    guanjiancanshujisuan();
    //��Ӣ������ʵ��
    jingyingbaoliu();
    //
    guanjiancanshujisuan();
    delete [] peiduishuzu;
    delete [] pipeichi;
    delete [] pipeiguodu;
}
void crossover(individual parent1,individual parent2,individual &child1,individual &child2)//����
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
void bianyi(individual &child)//����
{
    child.chrom=new unsigned[lchrom];
    //cout<<"����"<<endl;
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
void bianma(double bianliang,unsigned *p)//����
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
double yima(unsigned *p)  //����
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
        cout<<"�������1"<<endl;
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
        cout<<"�������2"<<endl;
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
double suijibianli()//�������
{
    double i=ran1(a);
    while(i>zhizhenjuli)
    {
        i=ran1(a);
    }
    //cout<<i<<endl;//////////////
    return i;
}
int fuzhi(float p)//����
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

int rnd(int low, int high)           /*������low��high֮�����һ���������*/
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
int flipc(double p,double q)//�ж��Ƿ񽻲�
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
int flipm(double p)//�ж��Ƿ����
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
    //���q
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
    //���x
    for(i=0;i<n;i++)
    {
        for(j=0;j<s;j++)
        {
            *(*(xx+i)+j)=(float)(2*(*(*(q+i)+j))-1)/(2*n);
        }
    }
}
BOOL Exist(int Val, int Num, int *Array)//�ж�һ�����Ƿ���һ�������ǰNum������ 
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