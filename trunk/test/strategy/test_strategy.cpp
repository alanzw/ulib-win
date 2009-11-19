/*
 * =====================================================================================
 *
 *       Filename:  test_strategy.cpp
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  09/27/2009 08:40:33 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  huys (hys), huys03@gmail.com
 *        Company:  hu
 *
 * =====================================================================================
 */

#include <iostream>
#include <fstream>
#include <cstring>

#include <exception>

using namespace std;

class Strategy;

class TestBed
{
public:
    enum StrategyType
    {
        Dummy, Left, Right, Center
    };
    TestBed()
    {
        strategy_ = NULL;
    }
    void setStrategy(int type, int width);
    void doIt();
private:
    Strategy *strategy_;
};

class Strategy
{
public:
    Strategy(int width): width_(width){}
    void format()
    {
        char line[80], word[30];
        ifstream inFile("quote.txt", ios::in);
        line[0] = '\0';

        inFile >> word;
        strcat(line, word);
        while (inFile >> word)
        {
            if (strlen(line) + strlen(word) + 1 > width_)
                justify(line);
            else
                strcat(line, " ");
            strcat(line, word);
        }
        justify(line);
    }
protected:
    int width_;
private:
    virtual void justify(char *line) = 0;
};

class LeftStrategy: public Strategy
{
public:
    LeftStrategy(int width): Strategy(width){}
private:
    /* virtual */void justify(char *line)
    {
        cout << line << endl;
        line[0] = '\0';
    }
};

class RightStrategy: public Strategy
{
public:
    RightStrategy(int width): Strategy(width){}
private:
    /* virtual */void justify(char *line)
    {
        char buf[80];
        int offset = width_ - strlen(line);
        memset(buf, ' ', 80);
        strcpy(&(buf[offset]), line);
        cout << buf << endl;
        line[0] = '\0';
    }
};

class CenterStrategy: public Strategy
{
public:
    CenterStrategy(int width): Strategy(width){}
private:
    /* virtual */void justify(char *line)
    {
        char buf[81];
        int offset = (width_ - strlen(line)) / 2;
        memset(buf, ' ', 80);
        strcpy(&(buf[offset]), line);
        cout << buf << endl;
        line[0] = '\0';
    }
};

void TestBed::setStrategy(int type, int width)
{
    delete strategy_;
    if (type == Left)
        strategy_ = new LeftStrategy(width);
    else if (type == Right)
        strategy_ = new RightStrategy(width);
    else if (type == Center)
        strategy_ = new CenterStrategy(width);
}

void TestBed::doIt()
{
    strategy_->format();
}

int main()
{

try {
    TestBed test;
    int answer, width;
    cout << "Exit(0) Left(1) Right(2) Center(3): ";
    cin >> answer;
    //while (answer)
    while (answer && cin.good())
    {
        cout << "Width: ";
        cin >> width;

        if (80 < width)
        {
            cerr << "width should be less than 80" << endl;
        }
        else
        {
            test.setStrategy(answer, width);
            test.doIt();
        }
        cout << "Exit(0) Left(1) Right(2) Center(3): ";
        cin >> answer;
        //while (answer != 0 && answer != 1  && answer != 2 && answer != 3)
        //{
        //    cout << "Exit(0) Left(1) Right(2) Center(3): ";
        //    cin >> answer;
        //}
    }

} catch (exception &e)
{

}
    return 0;
}

