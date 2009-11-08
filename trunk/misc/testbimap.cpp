/* Test suite for codeproject::bimap.
 *
 * MSVC++ users: set compiler options /GX /EHa
 *
 * (C) 2002-2006 Joaquín M López Muñoz (joaquin@tid.es). All rights reserved.
 *
 * Last modified: January 10th, 2006
 */

#ifdef _MSC_VER
#pragma warning(disable:4503)
#endif

#include "bimap.h"
#include <algorithm>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <string.h>
#include <vector>

#ifdef _MSC_VER
#include <crtdbg.h>
#include <eh.h>
#include <windows.h>
#endif

using codeproject::bimap;

#ifdef _MSC_VER
class SEH_exception:public std::runtime_error
{
public:
  SEH_exception(unsigned int code,const char* msg):std::runtime_error(msg),code_(code){}
  unsigned int code()const{return code_;}
private:
  unsigned int code_;
};

static void SEH_translator(unsigned int code,EXCEPTION_POINTERS*)
{
  std::string msg;
  switch(code){
    case EXCEPTION_ACCESS_VIOLATION: msg="access violation";break;
    case EXCEPTION_ARRAY_BOUNDS_EXCEEDED: msg="array bounds exceeded";break;
    case EXCEPTION_BREAKPOINT: msg="breakpoint exception";break;
    case EXCEPTION_DATATYPE_MISALIGNMENT: msg="datatype misalignment";break;
    case EXCEPTION_FLT_DENORMAL_OPERAND: msg="flt denormal operand";break;
    case EXCEPTION_FLT_DIVIDE_BY_ZERO: msg="flt divide by zero";break;
    case EXCEPTION_FLT_INEXACT_RESULT: msg="flt inexact result";break;
    case EXCEPTION_FLT_INVALID_OPERATION: msg="flt invalid operation";break;
    case EXCEPTION_FLT_OVERFLOW: msg="flt overflow";break;
    case EXCEPTION_FLT_STACK_CHECK: msg="flt stack check";break;
    case EXCEPTION_FLT_UNDERFLOW: msg="flt underflow";break;
    case EXCEPTION_ILLEGAL_INSTRUCTION: msg="illegal instruction";
    case EXCEPTION_IN_PAGE_ERROR: msg="in page error";break;
    case EXCEPTION_INT_DIVIDE_BY_ZERO: msg="divide by zero";break;
    case EXCEPTION_INT_OVERFLOW: msg="overflow"; break;
    case EXCEPTION_INVALID_DISPOSITION: msg="invalid disposition";break;
    case EXCEPTION_NONCONTINUABLE_EXCEPTION: msg="noncontinuable exception";break;
    case EXCEPTION_PRIV_INSTRUCTION: msg="priv instruction";break;
    case EXCEPTION_SINGLE_STEP: msg="single step trap";break;
    case EXCEPTION_STACK_OVERFLOW: msg="stack overflow";break;
    default:{
      char buf[1024];
      sprintf(buf,"SEH exception (%x)",code);
      msg=buf;
    }
    break;
  }
  throw SEH_exception(code,msg.c_str());
}
#endif /* _MSC_VER */

static void check(bool b,int line,const char * msg)
{
  if(!b){
    /* outputting to cout fools the ctrdbg library into thinking
     * there're memory leaks. We use old printf instead.
     */
    printf("line %d check failed: %s\n",line,msg);
  }
}

#define CHECK(c) check(c,__LINE__,#c)
#define CHECK2(c,msg) check(c,__LINE__,msg)

struct c_str_compare
{
  bool operator()(const char* s1,const char* s2)const{return strcmp(s1,s2)<0;}
};

int randn(int n){
  int r;
  while((r=rand())==RAND_MAX){}
  return (int)(((double)r/RAND_MAX)*n);
} 

int main(void)
{
#ifdef _MSC_VER
#ifndef NDEBUG
  _CrtSetReportMode(_CRT_WARN,_CRTDBG_MODE_FILE);
  _CrtSetReportFile(_CRT_WARN,_CRTDBG_FILE_STDOUT);
  _CrtSetReportMode(_CRT_ERROR,_CRTDBG_MODE_FILE);
  _CrtSetReportFile(_CRT_ERROR,_CRTDBG_FILE_STDOUT);
  _CrtSetReportMode(_CRT_ASSERT,_CRTDBG_MODE_FILE);
  _CrtSetReportFile(_CRT_ASSERT,_CRTDBG_FILE_STDOUT);

  _CrtMemState memstate_before;
  _CrtMemCheckpoint(&memstate_before);
#endif

  _set_se_translator(SEH_translator);
#endif

  try{

    /* basic usage */
    {
      bimap<int,std::string> bm;

      bm[1]="Monday";
      bm[2]="Tuesday";
      bm[3]="Wednesday";
      bm[4]="Thursday";
      bm[5]="Friday";
      bm[6]="Saturday";
      bm[7]="Sunday";

      CHECK(bm[1].get()=="Monday");
      CHECK(bm[2].get()=="Tuesday");
      CHECK(bm[3].get()=="Wednesday");
      CHECK(bm[4].get()=="Thursday");
      CHECK(bm[5].get()=="Friday");
      CHECK(bm[6].get()=="Saturday");
      CHECK(bm[7].get()=="Sunday");
      CHECK(bm["Monday"]==   1);
      CHECK(bm["Tuesday"]==  2);
      CHECK(bm["Wednesday"]==3);
      CHECK(bm["Thursday"]== 4);
      CHECK(bm["Friday"]==   5);
      CHECK(bm["Saturday"]== 6);
      CHECK(bm["Sunday"]==   7);
    }

    /* copy, assignment and equality */
    {
      bimap <int,double> bm;
      bm[1]=1.0;
      bm[2]=2.0;
      bm[3]=3.0;

      bimap <int,double> bm2(bm);
      CHECK(bm==bm2);CHECK(bm.from==bm2.from);CHECK(bm.to==bm2.to);
      CHECK(bm2==bm);CHECK(bm2.from==bm.from);CHECK(bm2.to==bm.to);
      CHECK(bm2<=bm);CHECK(bm2.from<=bm.from);CHECK(bm2.to<=bm.to);
      CHECK(bm2>=bm);CHECK(bm2.from>=bm.from);CHECK(bm2.to>=bm.to);

      bm2.erase(3.0);
      CHECK(bm!=bm2);CHECK(bm.from!=bm2.from);CHECK(bm.to!=bm2.to);
      CHECK(bm2!=bm);CHECK(bm2.from!=bm.from);CHECK(bm2.to!=bm.to);
      CHECK(bm2<bm);CHECK(bm2.from<bm.from);CHECK(bm2.to<bm.to);
      CHECK(bm>bm2);CHECK(bm.from>bm2.from);CHECK(bm.to>bm2.to);

      bimap <int,double> bm3;
      bm3=bm;
      CHECK(bm==bm3);CHECK(bm.from==bm3.from);CHECK(bm.to==bm3.to);
      CHECK(bm3==bm);CHECK(bm3.from==bm.from);CHECK(bm3.to==bm.to);

      bimap<int,double> bm4;
      bm4.from=bm3.from;
      CHECK(bm3==bm4);CHECK(bm3.from==bm4.from);CHECK(bm3.to==bm4.to);

      bimap<int,double> bm5;
      bm5.to=bm4.to;
      CHECK(bm4==bm5);CHECK(bm4.from==bm4.from);CHECK(bm4.to==bm5.to);
    }

    /* inverse copy constructor */
    {
      bimap<int,double> bm;
      bm[1]=1.0;
      bm[2]=4.0;
      bm[3]=9.0;

      bimap<double,int> bm2(bm);
      CHECK(std::equal(bm.to.begin(),bm.to.end(),bm2.begin()));
      CHECK(std::equal(bm2.to.begin(),bm2.to.end(),bm.begin()));
    }

    /* unicity of from and to values */
    {
      bimap<int,int> bm;
      bm[1]=1;
      bm[2]=2;
      bm[3]=3;

      bm[3]=3; // same assignment should work OK
      bm[3]=4; // reassignment

      try{
        bm[3]=1;
        CHECK2(false,"previous assignment is not permitted");
      }catch(codeproject::bimap_base::duplicate_value&){/*OK*/}

      try{
        bm[5]=1;
        CHECK2(false,"previous assignment is not permitted");
      }catch(codeproject::bimap_base::duplicate_value&){/*OK*/}

      try{
        bm.to[3]=1;
        CHECK2(false,"previous assignment is not permitted");
      }catch(codeproject::bimap_base::duplicate_value&){/*OK*/}

      try{
        bm.to[5]=1;
        CHECK2(false,"previous assignment is not permitted");
      }catch(codeproject::bimap_base::duplicate_value&){/*OK*/}

      bm.from.erase(1);
      bm[3]=1;
      CHECK(bm[3]==1);
    }

    /* missing value */
    {
      bimap<int,int> bm;

      try{
        int i;
        i=bm[1];
        CHECK2(false,"previous assignment is not permitted");
      }catch(codeproject::bimap_base::value_not_found&){/*OK*/}

      try{
        int i;
        i=bm.to[1];
        CHECK2(false,"previous assignment is not permitted");
      }catch(codeproject::bimap_base::value_not_found&){/*OK*/}
    }

    /* iterator retrieval */
    {
      bimap<int,double> bm;
 
      for(int i=1;i<=100;++i)bm[i]=double(2*i);

      {
        int n=0;
        for(codeproject::bimap<int,double>::const_iterator it=bm.begin();
            it!=bm.end();++it){
          n+=it->first;
        }
        for(codeproject::bimap<int,double>::to::const_reverse_iterator rit=bm.to.rbegin();
            rit!=bm.to.rend();++rit){
          n+=rit->second;
        }
        CHECK(n==(1+100)*100);
      }

      {
        double d=0;
        for(codeproject::bimap<int,double>::to::const_iterator it=bm.to.begin();
            it!=bm.to.end();++it){
          d+=it->first;
        }
        for(codeproject::bimap<int,double>::const_reverse_iterator rit=bm.rbegin();
            rit!=bm.rend();++rit){
          d+=rit->second;
        }
        CHECK(d==(1.0+100.0)*100.0*2.0);
      }
    }

    /* utility standard methods */
    {
      bimap<const char*,const char*> bm;
      bm["one"]=  "uno";
      bm["two"]=  "dos";
      bm["three"]="tres";
      bm["four"]= "cuatro";
      bm["five"]= "cinco";
      bm["six"]=  "seis";
      bm["seven"]="siete";
      bm["eight"]="ocho";
      bm["nine"]= "nueve";
      bm["ten"]=  "diez";

      CHECK(bm.size()==10);
      CHECK(!bm.empty());
    }

    /* insertion and removal */
    {
      bimap<char,int> bm;

      bm.insert(std::make_pair('a',97));
      bm.insert(codeproject::make_inv_pair(101,'e'));
      bm.insert(bm.from.end(),std::make_pair('i',105));
      bm.insert(bm.to.end(),codeproject::make_inv_pair(111,'o'));
      bm.insert(std::make_pair('u',117));
      CHECK(bm['a']==int('a'));
      CHECK(bm['e']==int('e'));
      CHECK(bm['i']==int('i'));
      CHECK(bm['o']==int('o'));
      CHECK(bm['u']==int('u'));

      CHECK(!(bm.insert(std::make_pair(97,'a')).second));

      std::vector<std::pair<char,int> > v;
      std::copy(bm.begin(),bm.end(),std::back_inserter(v));
      bimap<char,int> bm2;
      bm2.insert(v.begin(),v.end());
      CHECK(bm2==bm);

      bm.erase(bm.begin());
      CHECK(bm.begin()->second==int('e'));
      bm.to.erase(int('e'));
      CHECK(bm.to.begin()->second=='i');
      bm.erase(bm.begin(),bm.end());
      CHECK(bm.empty());

      bm['a']=97;
      bm.clear();
      CHECK(bm.empty());

      bm['a']=97;
      bm.to.clear();
      CHECK(bm.empty());

      bm['a']=97;
      bm['e']=101;
      bm['i']=105;
      bm['o']=111;
      bm['u']=117;
      bimap<char,int> bm_copy(bm);
      bimap<char,int> bm3;
      bm3['A']=65;
      bm3['E']=69;
      bm3['I']=73;
      bm3['O']=79;
      bm3['U']=85;
      bimap<char,int> bm3_copy(bm3);

      bm.swap(bm3);
      CHECK(bm==bm3_copy&&bm3==bm_copy);

      bm.from.swap(bm3.from);
      CHECK(bm==bm_copy&&bm3==bm3_copy);


#if defined(_MSC_VER)&&_MSC_VER<=1300
      /* VC++ 6.0/7.0 forces to specify the namespace now, should have been
       * deduced by Koening lookup.
       */
      codeproject::swap(bm3.to,bm.to);
#else
      swap(bm3.to,bm.to);
#endif

      CHECK(bm==bm3_copy&&bm3==bm_copy);

#if defined(_MSC_VER)&&_MSC_VER<=1300
      codeproject::swap(bm,bm3); 
#else
      swap(bm,bm3); 
#endif

      CHECK(bm==bm_copy&&bm3==bm3_copy);
    }

    /* lookup */

    {
      typedef bimap<const char*,const char*,c_str_compare,c_str_compare> bimap_c_str;
      bimap_c_str bm;
      bm["Bill"] ="William";
      bm["Tom"]  ="Thomas";
      bm["Chris"]="Christian";
      bm["Norm"] ="Norman";
      bm["Bob"]  ="Robert";

      CHECK(strcmp(bm.find("Bill")->second,"William")==0);
      CHECK(strcmp(bm.to.find("Thomas")->second,"Tom")==0);
      CHECK(strcmp(bm.find("Chris")->second,"Christian")==0);
      CHECK(strcmp(bm.to.find("Norman")->second,"Norm")==0);
      CHECK(strcmp(bm.find("Bob")->second,"Robert")==0);
      CHECK(bm.find("Joe")==bm.end());
      CHECK(bm.to.find("Reginald")==bm.to.end());

      CHECK(bm.count("Vill")==0);
      CHECK(bm.to.count("Robert")==1);

      CHECK(strcmp(bm.lower_bound("Bill")->first,"Bill")==0);
      CHECK(strcmp(bm.to.lower_bound("Christiansen")->first,"Christiansen")>0);
      CHECK(strcmp(bm.upper_bound("Jim")->first,"Jim")>0);
      CHECK(strcmp(bm.to.upper_bound("Robert")->first,"Robert")>0);

      std::pair<bimap_c_str::iterator,bimap_c_str::iterator> p=
        bm.equal_range("Norm");
      CHECK(p.first==bm.lower_bound("Norm")&&p.second==bm.upper_bound("Norm"));

      std::pair<bimap_c_str::to::iterator,bimap_c_str::to::iterator> p2=
        bm.to.equal_range("Zenon");
      CHECK(p2.first==bm.to.lower_bound("Zenon")&&p2.second==bm.to.upper_bound("Zenon"));
    }

    /* data thrashing to check for memory leaks */
    {
      bimap<int,int> bm;

      srand(1024); // we make it deterministic
      for(int i=0;i<10000;++i){
        try{
          bm[randn(100)]=randn(100);
        }catch(codeproject::bimap_base::duplicate_value&){}
        try{
          bm.to[randn(000)]=randn(100);
        }catch(codeproject::bimap_base::duplicate_value&){}
        bm.erase(randn(100));
        bm.to.erase(randn(100));
      }
    }
  }
  catch(std::exception& e){
    CHECK2(false,e.what());
  }
  catch(...){
    CHECK2(false,"unexpected exception");
  }

#ifdef _MSC_VER
#ifndef NDEBUG
  _CrtMemState memstate_after;
  _CrtMemCheckpoint(&memstate_after);
  _CrtMemState memstate_diff;
  if(_CrtMemDifference(&memstate_diff,&memstate_before,&memstate_after)){
    printf("Memory leaks detected\n");
    _CrtMemDumpStatistics(&memstate_diff);
  }
#endif
#endif
  
  return 0;
}
