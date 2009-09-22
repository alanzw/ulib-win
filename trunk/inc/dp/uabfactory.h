#ifndef U_DP_ABSTRACT_FACTORY_H
#define U_DP_ABSTRACT_FACTORY_H

namespace huys
{

namespace dp
{

namespace creational
{

//
class AbstractProduct;

//
class AbstractFactory
{
public:
    virtual AbstractProduct *createProduct()
    {std::cout << "Abstract Creating..." << "\n";};
};

//
class AbstractProduct
{
public:
    virtual void create()
    {std::cout << "Abstract Product" << "\n";};
};

//
class ConcreteProduct : public AbstractProduct
{
public:
    void create()
    {
        std::cout << "Concrete Product" << "\n";
    }
};

//
class ConcreteFactory : public AbstractFactory
{
public:
    ConcreteProduct *createProduct()
    {
        std::cout << "Concrete Creating..." << "\n";
        return new ConcreteProduct;
    }
};


}; // namespace creational


}; // namespace dp


}; // namespace huys

#endif // U_DP_ABSTRACT_FACTORY_H

