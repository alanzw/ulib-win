#include <iostream>
using std::cout;
using std::endl;
using std::cin;

namespace Tower_Of_Hanoi
{

void move(int count, int start, int finish, int temp)
{
    if (count > 0)
    {
        move(count-1, start, temp, finish);
        cout << "Move disk " << count << " from " << start
             << " to " << finish << "." << endl;
        move(count-1, temp, finish, start);
    }
}

void move_v2(int count, int start, int finish, int temp)
{
    int swap;
    while (count > 0)
    {
        move_v2(count-1, start, temp, finish);
        cout << "Move disk " << count << " from " << start
            << " to " << finish << "." << endl;
        --count;
        swap = start;
        start = temp;
        temp = swap;
    }
}

void test()
{
    move(3, 1, 3, 2);

    cout << "-------------------------------" << endl;

    move_v2(3, 1, 3, 2);
}

}; // namespace Tower_Of_Hanoi

namespace Eight_Queens
{

const int max_board = 4;

class Queens
{
public:
    Queens(int size)
    : board_size(size),
      count(0)
    {
        for (int i=0; i<board_size; ++i)
        {
            for (int j=0; j<board_size; ++j)
            {
                queen_square[i][j] = false;
            }
        }
    }

    bool is_solved() const
    {

    }

    void print() const
    {
        for (int j=0; j<board_size; ++j)
        {
            for (int i=0; i<board_size; ++i)
            {
                cout << queen_square[i][j] << " ";
            }
            cout << endl;
        }
    }

    bool unguarded(int col) const
    {
        int i;
        bool ok = true;
        for (i=0; ok && i<count; ++i)
        {
            ok = !queen_square[i][col];
        }
        for (i=1; ok && count-1>=0 && col-i>=0; ++i)
        {
            ok = !queen_square[count-i][col-i];
        }
        for (i=1; ok && count-1>=0 && col+i<board_size; ++i)
        {
            ok = !queen_square[count-i][col+i];
        }
        return ok;
    }

    bool insert(int col)
    {
        queen_square[count++][col] = true;
    }

    bool remove(int col)
    {

    }
private:
    int count;
    int board_size;
    bool queen_square[max_board][max_board];
};

void solve_from(Queens &config)
{
    if (config.is_solved())
    {
        config.print();
    }
    else
    {
        for (int col=0; col<config.board_size; ++col)
        {
            config.insert(col);
            solve_from(config);
            config.remove(col);
        }
    }
}

void test()
{
    Queens config(4);
    solve_from(config);
}

}; // namespace Four_Queen


int main()
{
    Tower_Of_Hanoi::test();

    cin.ignore();

    return 0;
}
