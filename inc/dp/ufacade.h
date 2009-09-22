/*
 * =====================================================================================
 *
 *       Filename:  ufacade.h
 *
 *    Description:  dp.structural.facade
 *                    The facade pattern can make the task of accessing
 *                    a large number of modules much simpler by providing
 *                    an additional interface layer.
 *
 *        Version:  1.0
 *        Created:  2009-7-22 20:00:52
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  huys (hys), huys03@gmail.com
 *        Company:  hu
 *
 * =====================================================================================
 */

#ifndef U_FACADE_H
#define U_FACADE_H

namespace huys
{

namespace dp
{

namespace structural
{

namespace UFacade
{

class CPU
{
public:
    void execute() {}
};

class Memory
{
public:
    void load() {}
};

class HardDrive
{
public:
    void byte() {}
};

/// Facade
class Computer
{
public:
    void start()
    {
        _men.load();
        _hd.byte();
        _cpu.execute();
    }
private:
    CPU _cpu;
    Memory _men;
    HardDrive _hd;
}

/// Usage:
///   Computer a_computer;
///   a_computer.start();
///

}; // namespace UFacade


}; // namespace structural
}; // namespace dp
}; // namespace huys

#endif // U_FACADE_H

