#include <iostream>

#include "adt/ubinary_tree.h"

void printTree(int &data)
{
    std::cout << "*" << data << "*";
}

int main()
{
    using std::cout;
    using std::endl;
    using std::cin;

    huys::ADT::Binary_tree<int> b_tree;

    cout << "Size of tree : "  << b_tree.size() << endl;
    cout << "Height of tree : "  << b_tree.height() << endl;

    b_tree.insert(11);

    cout << "-----------------------------------------------------" << endl;
    cout << "After insert:" << endl;
    cout << "Size of tree : "  << b_tree.size() << endl;
    cout << "Height of tree : "  << b_tree.height() << endl;

    b_tree.insert(10);
    b_tree.insert(12);

    cout << "-----------------------------------------------------" << endl;
    cout << "After insert:" << endl;
    cout << "Size of tree : "  << b_tree.size() << endl;
    cout << "Height of tree : "  << b_tree.height() << endl;

    cout << "Inorder : ";
    b_tree.inorder(printTree);
    cout << endl;

    cout << "Preorder : ";
    b_tree.preorder(printTree);
    cout << endl;

    cout << "Postorder : ";
    b_tree.postorder(printTree);
    cout << endl;

    cout << "Doubleorder : ";
    b_tree.doubleorder(printTree);
    cout << endl;

    cout << "levelbylevel : ";
    b_tree.levelbylevel(printTree);
    cout << endl;

    b_tree.mirror();

    cout << "-----------------------------------------------------" << endl;
    cout << "After mirror:" << endl;

    cout << "Inorder : ";
    b_tree.inorder(printTree);
    cout << endl;
    cout << "Preorder : ";
    b_tree.preorder(printTree);
    cout << endl;
    cout << "Postorder : ";
    b_tree.postorder(printTree);
    cout << endl;

    //class TTClass;
    //typedef char type_must_be_complete[sizeof(TTClass)?1:-1];
    //(void)sizeof(type_must_be_complete);

    huys::ADT::Binary_tree<int> new_tree(b_tree);
    cout << "-----------------------------------------------------" << endl;
    cout << "Copy tree:" << endl;

    cout << "Inorder : ";
    new_tree.inorder(printTree);
    cout << endl;
    cout << "Preorder : ";
    new_tree.preorder(printTree);
    cout << endl;
    cout << "Postorder : ";
    new_tree.postorder(printTree);
    cout << endl;



    cout << "-----------------------------------------------------" << endl;

    huys::ADT::Search_tree<int> stree;

    stree.insert(3);
    stree.insert(2);
    stree.insert(4);
    stree.insert(1);
    stree.insert(5);

    cout << "Size of tree : "  << stree.size() << endl;
    cout << "Height of tree : "  << stree.height() << endl;

    cout << "Inorder : ";
    stree.inorder(printTree);
    cout << endl;

    stree.remove(3);

    cout << "-----------------------------------------------------" << endl;
    cout << "After remove:" << endl;
    cout << "Inorder : ";
    stree.inorder(printTree);
    cout << endl;

    cin.ignore();

    return 0;
}
