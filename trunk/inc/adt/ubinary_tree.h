#ifndef U_BINARY_TREE_H
#define U_BINARY_TREE_H

namespace huys
{

namespace ADT
{

template <class Entry>
struct Binary_node
{
    //
    Entry data;
    Binary_node<Entry> *left;
    Binary_node<Entry> *right;

    //
    Binary_node()
    : data(), left(0), right(0)
    {}

    Binary_node(const Entry &other)
    : data(other), left(0), right(0)
    {}
};

enum Error_code {
    success,
    fail,
    range_error,
    underflow,
    overflow,
    fatal,
    not_present,
    duplicate_error,
    entry_insert,
    entry_found,
    internal_error
};

template <class Entry, class V = void (*)(Entry &)>
class Binary_tree
{
public:
    typedef V Visit;
public:
    //
    Binary_tree()
    : _root(0)
    {}

    //
    bool empty() const
    {
        return (0==_root);
    }


    void inorder(Visit visit)
    {
        recursive_inorder(_root, visit);
    }

    void preorder(Visit visit)
    {
        recursive_preorder(_root, visit);
    }

    void postorder(Visit visit)
    {
        recursive_postorder(_root, visit);
    }

    int size() const
    {
        return recursive_size(_root);
    }

    void clear()
    {
        destroy_tree(_root);
    }

    int height() const
    {
        return height_node(_root);
    }

    void insert(const Entry &data)
    {
        if(_root != 0)
        {
            insert_node(data, _root);
        }
        else
        {
            _root=new Node;
            _root->data = data;
            _root->left = 0;
            _root->right = 0;
        }
    }

    void mirror()
    {
        mirror_node(_root);
    }

    Binary_tree(const Binary_tree<Entry> &original)
    {
        
    }

    Binary_tree & operator = (const Binary_tree<Entry> &original)
    {

    }

    ~Binary_tree()
    {
        clear();
    }
protected:
    //
    typedef Binary_node<Entry> Node;
    void recursive_inorder(Node *sub_root, Visit visit)
    {
        if ( 0 != sub_root )
        {
            recursive_inorder(sub_root->left, visit);
            (*visit)(sub_root->data);
            recursive_inorder(sub_root->right, visit);
        }
    }

    //
    void recursive_preorder(Node *sub_root, Visit visit)
    {
        if ( 0 != sub_root )
        {
            (*visit)(sub_root->data);
            recursive_preorder(sub_root->left, visit);
            recursive_preorder(sub_root->right, visit);
        }
    }

    //
    void recursive_postorder(Node *sub_root, Visit visit)
    {
        if ( 0 != sub_root )
        {
            recursive_postorder(sub_root->left, visit);
            recursive_postorder(sub_root->right, visit);
            (*visit)(sub_root->data);
        }
    }

    int recursive_size(Node *node) const
    {
        if ( 0 == node )
        {
            return 0;
        }
        else
        {
            return ( recursive_size(node->left) + 1 + recursive_size(node->right) );
        }
    }

    void destroy_tree(Node *node)
    {
        if (0 != node)
        {
            destroy_tree(node->left);
            destroy_tree(node->right);
            delete node;
        }
    }

    void insert_node(const Entry &data, Node *node)
    {
        if( data < node->data)
        {
            if(node->left != 0)
            {
                insert_node(data, node->left);
            }
            else
            {
                node->left=new Node;
                node->left->data=data;
                node->left->left=0;    //Sets the left child of the child node to null
                node->left->right=0;   //Sets the right child of the child node to null
            }
        }
        else if(data >= node->data)
        {
            if(node->right != 0)
            {
                insert_node(data, node->right);
            }
            else
            {
                node->right = new Node;
                node->right->data = data;
                node->right->left = 0;  //Sets the left child of the child node to null
                node->right->right = 0; //Sets the right child of the child node to null
            }
        }
    }

    int height_node(Node *node) const
    {
        if (node == 0)
        {
            return 0;
        }
        else
        {
            // compute the depth of each subtree
            int lDepth = height_node(node->left);
            int rDepth = height_node(node->right);

            // use the larger one
            if (lDepth > rDepth)
            {
                return (lDepth+1);
            }
            else
            {
                return (rDepth+1);
            }
        }
    }

    void mirror_node(Node *node)
    {
        if (node == 0)
        {
            return;
        }
        else
        {
            Node *temp = node->left;
            node->left = node->right;
            node->right = temp;
        }
    }
protected:
    Node *_root;
private:
};

/* 
 * Binary Search Tree
 * --------
 * 1. The key of the root (if it exists) is greater than the key in any node in the
 *    left subtree of the root.
 * 2. The key of the root (if it exists) is less than the the key in any node in the
 *    right subtree of the root.
 * 3. The left and right subtrees of the root are again binary search tree.
 */

template <class Record>
class Search_tree : public Binary_tree<Record>
{
public:
    Error_code insert(const Record &new_data)
    {
        return search_and_insert(this->_root, new_data);
    }

    Error_code remove(const Record &target)
    {
        return search_and_destroy(this->_root, target);
    }

    Error_code tree_search(Record &target) const
    {
        Error_code result = success;

        Binary_node<Record> * found = search_for_node(this->_root, target);

        if (NULL = found)
        {
            result = not_present;
        }
        else
        {
            target = found->data;
        }
        return result;
    }
protected:
    Binary_node<Record> * search_for_node(Binary_node<Record> * sub_root, const Record & target)
    {
        if (NULL == sub_root || sub_root->data == target)
        {
            return sub_root;
        }
        else if (sub_root->data < target)
        {
            return search_for_node(sub_root->right, target);
        }
        else
        {
            return search_for_node(sub_root->left, target);
        }
    }

    Binary_node<Record> * search_for_node_loop(Binary_node<Record> * sub_root, const Record & target)
    {
        while (sub_root == NULL && sub_root->data != target)
        {
            if (sub_root->data < target)
            {
                sub_root = sub_root->right;
            }
            else
            {
                sub_root = sub_root->left;
            }
        }
        return sub_root;
    }

    Error_code search_and_insert(Binary_node<Record> *&sub_root, const Record &new_data)
    {
        if (NULL == sub_root)
        {
            sub_root = new Binary_node<Record>(new_data);
            return success;
        }
        else if (new_data < sub_root->data)
        {
            return search_and_insert(sub_root->left, new_data);
        }
        else if (new_data > sub_root->data)
        {
            return search_and_insert(sub_root->right, new_data);
        }
        else
        {
            return duplicate_error;
        }
    }

    Error_code remove_root(Binary_node<Record> *&sub_root)
    {
        if (NULL == sub_root)
        {
            return not_present;
        }
        Binary_node<Record> * to_delete = sub_root;
        if (NULL == sub_root->right)
        {
            sub_root = sub_root->left;
        }
        else if (NULL == sub_root->left)
        {
            sub_root = sub_root->right;
        }
        else
        {
            to_delete = sub_root->left;
            Binary_node<Record> *parent = sub_root;
            while (NULL != to_delete->right)
            {
                parent = to_delete;
                to_delete = to_delete->right;
            }
            sub_root->data = to_delete->data;
            if (parent == sub_root)
            {
                sub_root->left = to_delete->left;
            }
            else
            {
                parent->right = to_delete->left;
            }
        }
        delete to_delete;
        return success;
    }

    Error_code search_and_destroy(Binary_node<Record> * &sub_root, const Record &target)
    {
        if (NULL == sub_root || sub_root->data == target)
        {
            return remove_root(sub_root);
        }
        else if (target < sub_root->data)
        {
            return search_and_destroy(sub_root->left, target);
        }
        else
        {
            return search_and_destroy(sub_root->right, target);
        }
    }
private:
};

template <class Record>
class Buildable_tree : public Search_tree<Record>
{
public:
    Error_code build_tree(const Record *supply, int n)
    {
        Error_code ordered_data = success;
        int count = 0;
        Record x;
        Record last_x;

        Binary_node<Record> * none = NULL;

        return success;
    }
private:

};

}; // namespace ADT

}; // namespace huys

#endif // U_BINARY_TREE_H
