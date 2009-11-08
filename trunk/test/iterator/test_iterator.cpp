/*
 * =====================================================================================
 *
 *       Filename:  test_iterator.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  09/27/2009 08:33:21 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  huys (hys), huys03@gmail.com
 *        Company:  hu
 *
 * =====================================================================================
 */

/* Intent
 *  -- Provide a way to access the elements of an aggregate object 
 *     sequentially without exposing its underlying representation.
 *  -- The C++ and Java standard library abstraction that makes it
 *     possible to decouple collection classes and algorithms.
 *  -- Promote to ¡°full object status¡± the traversal of a collection.
 *  -- Polymorphic traversal
 */

/* Includes:
 *	-- Iterator
 *    -- Concrete Iterator
 *    -- Container
 *    -- Concrete Container
 */

#include <iostream>
#include <vector>
#include <set>
#include <iterator>

using namespace std;


namespace Use_Method
{

class Stack
{
    int items[10];
    int sp;
public:
    friend class StackIter;
    Stack()
    {
        sp =  - 1;
    }
    void push(int in)
    {
        items[++sp] = in;
    }
    int pop()
    {
        return items[sp--];
    }
    bool isEmpty()
    {
        return (sp ==  - 1);
    }
    class StackIter *createIterator()const; // 2. Add a createIterator() member
};

class StackIter
{
    // 1. Design an "iterator" class
    const Stack *stk;
    int index;
public:
    StackIter(const Stack *s)
    {
        stk = s;
    }
    void first()
    {
        index = 0;
    }
    void next()
    {
        index++;
    }
    bool isDone()
    {
        return index == stk->sp + 1;
    }
    int currentItem()
    {
        return stk->items[index];
    }
};

StackIter *Stack::createIterator()const
{
    return new StackIter(this);
}

bool operator == (const Stack &l, const Stack &r)
{
    // 3. Clients ask the container object to create an iterator object
    StackIter *itl = l.createIterator();
    StackIter *itr = r.createIterator();
    // 4. Clients use the first(), isDone(), next(), and currentItem() protocol
    for (itl->first(), itr->first(); !itl->isDone(); itl->next(), itr->next())
    {
    	if (itl->currentItem() != itr->currentItem()) break;
    }
    bool ans = itl->isDone() && itr->isDone();
    delete itl;
    delete itr;
    return ans;
}

void test()
{
    Stack s1;
    for (int i=1; i<5; i++)
    {
    	s1.push(i);
    }
    Stack s2(s1), s3(s1), s4(s1), s5(s1);
    s3.pop();
    s5.pop();
    s4.push(2);
    s5.push(9);
    cout << "1 == 2 is " << (s1 == s2) << endl;
    cout << "1 == 3 is " << (s1 == s3) << endl;
    cout << "1 == 4 is " << (s1 == s4) << endl;
    cout << "1 == 5 is " << (s1 == s5) << endl;

}

};

namespace Use_Operator
{

class Stack
{
    int items[10];
    int sp;
  public:
    friend class StackIter;
    Stack()
    {
        sp =  - 1;
    }
    void push(int in)
    {
        items[++sp] = in;
    }
    int pop()
    {
        return items[sp--];
    }
    bool isEmpty()
    {
        return (sp ==  - 1);
    }
};

class StackIter
{
    const Stack &stk;
    int index;
  public:
    StackIter(const Stack &s): stk(s)
    {
        index = 0;
    }
    void operator++()
    {
        index++;
    }
    bool operator()()
    {
        return index != stk.sp + 1;
    }
    int operator *()
    {
        return stk.items[index];
    }
};

bool operator == (const Stack &l, const Stack &r)
{
  StackIter itl(l), itr(r);
  for (; itl(); ++itl, ++itr)
    if (*itl !=  *itr)
      break;
  return !itl() && !itr();
}

void test()
{
  Stack s1;
  int i;
  for (i = 1; i < 5; i++)
    s1.push(i);
  Stack s2(s1), s3(s1), s4(s1), s5(s1);
  s3.pop();
  s5.pop();
  s4.push(2);
  s5.push(9);
  cout << "1 == 2 is " << (s1 == s2) << endl;
  cout << "1 == 3 is " << (s1 == s3) << endl;
  cout << "1 == 4 is " << (s1 == s4) << endl;
  cout << "1 == 5 is " << (s1 == s5) << endl;
}

};

namespace Test
{

//////////////////////////////////////////////////////////////////////////
template <typename T, class U>
class Iterator
{
public:
	typedef typename std::vector<T>::iterator iter_type;
	Iterator(U*  pData):m_pData(pData){
		m_it = m_pData->m_data.begin();
	}
 
	void first()
	{
		m_it = m_pData->m_data.begin();
	}
 
	void next()
	{
		m_it++;
	}
 
	bool isDone()
	{
		return (m_it == m_pData->m_data.end());
	}
 
	iter_type current()
	{
		return m_it;
	}
private:
	U* m_pData;
	iter_type m_it;
};
 
template<class T,class U,class A>
class setIterator
{
public:
	typedef typename std::set<T,U>::iterator iter_type;
 
	setIterator(A* pData):m_pData(pData)
	{
		m_it = m_pData->m_data.begin();
	}
 
	void first()
	{
		m_it = m_pData->m_data.begin();
	}
 
	void next()
	{
		m_it++;
	}
 
	bool isDone()
	{
		return (m_it == m_pData->m_data.end());
	}
 
	iter_type current()
	{
		return m_it;
	}
 
private:
	A*				m_pData;		
	iter_type		m_it;
};

template <class T>
class aggregate
{
	friend class Iterator<T, aggregate>;
public:
	void add(T a)
	{
		m_data.push_back(a);
	}
 
	Iterator<T,aggregate>* create_iterator()
	{
		return new Iterator<T,aggregate>(this);
	}
 
 
private:
	std::vector<T> m_data;
};
template <class T,class U>
class aggregateSet
{
	friend class setIterator<T,U,aggregateSet>;
public:
	void add(T a)
	{
		m_data.insert(a);
	}
 
	setIterator<T,U,aggregateSet>* create_iterator()
	{
		return new setIterator<T,U,aggregateSet>(this);
	}
 
	void Print()
	{
		copy(m_data.begin(),m_data.end(), ostream_iterator<T>(cout,"\r\n"));
	}
 
protected:
private:
	std::set<T,U> m_data;
};

class Money
{
public:
	Money(int a=0):m_data(a){}
 
	void SetMoney(int a){
		m_data = a;
	}
 
	int GetMoney()
	{
		return m_data;
	}
 
protected:
private:
	int m_data;
};
 
class Name
{
public:
	Name(string name):m_name(name){}
 
	const string& GetName() const{
		return m_name;
	}
 
	friend ostream& operator<<(ostream& cout,Name name)
	{
		cout<<name.GetName();
		return cout;
	}
 
private:
	string m_name;
};
 
struct NameLess
{
	bool operator() ( const Name& lhs, const Name& rhs) const
	{
		return lhs.GetName() < rhs.GetName();
	}
};
 
void test()
{
		//sample 1
	cout<<"________________Iterator with int______________________________________"<<endl;
	aggregate<int> agg;
 
	for(int i=0;i<10;i++)
	{
		agg.add(i);
	}
 
 
	Iterator< int,aggregate<int> > *it = agg.create_iterator();
	for(it->first();!it->isDone();it->next())
	{
		cout<<*it->current()<<endl;
	}
 
 
	//sample 2
	aggregate<Money> agg2;
	Money a(100),b(1000),c(10000);
	agg2.add(a);
	agg2.add(b);
	agg2.add(c);
 
	cout<<"________________Iterator with Class Money______________________________"<<endl;
	Iterator< Money,aggregate<Money> > *it2 = agg2.create_iterator();
	it2->first();
	while (!it2->isDone()) {
		cout<<((Money*)(it2->current()->GetMoney()))<<endl;
		it2->next();
	}
 
	//sample 3
	cout<<"________________Set Iterator with Class Name______________________________"<<endl;
 
	aggregateSet<Name,NameLess> aset;
	aset.add(Name("Qmt"));
	aset.add(Name("Bmt"));
	aset.add(Name("Cmt"));
	aset.add(Name("Amt"));
 
 
	setIterator<Name,NameLess,aggregateSet<Name,NameLess>  > * it3 = aset.create_iterator();
	for(it3->first();!it3->isDone();it3->next())
	{
		cout<<(*it3->current())<<endl;
	}
}

};

int main()
{
	Use_Method::test();

	Use_Operator::test();

    Test::test();
    
    return 0;
}

