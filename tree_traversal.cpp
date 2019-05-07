#include <cmath>
#include <vector>
#include <iostream>
#include <cstdlib>
#include <ctime>

class BinaryMinHeap
{
public:
    BinaryMinHeap(const size_t max_nodes)
      : max_nodes_(max_nodes)
    {
    };

    int parent_idx(const int idx) { return (idx - 1) / 2; };
    int left_idx(const int idx) { return (2 * idx) + 1; };
    int right_idx(const int idx) { return (2 * idx) + 2; };
    int idx_level(const int idx)
    { 
        int level = 0;
        int num_indicies_on_and_above_level = 0;
        while (idx >= num_indicies_on_and_above_level)
        {
            num_indicies_on_and_above_level += std::pow(2, level);
            level++;
        }
        return level;
    }
    
    void swap(int* x, int* y)
    {
        int temp = *x;
        *x = *y;
        *y = temp;
    };

    void heapify(int idx) 
    { 
        int idx_left = left_idx(idx); 
        int idx_right = right_idx(idx); 
        int smallest_idx = idx; 
        if (idx_right >= heap_.size())
        {
            if (idx_left >= heap_.size())
            {
                return;
            }
            else
            {
                smallest_idx = idx_left;
            }
        }
        else
        {
            if (heap_[idx_left] <= heap_[idx_right])
            {
                smallest_idx = idx_left;
            }
            else
            {
                smallest_idx = idx_right;
            }
        }
        if (heap_[idx] > heap_[smallest_idx])
        {
            swap(&heap_[smallest_idx], &heap_[idx]);
            heapify(smallest_idx);
        }
    };

    void insert_key(const int key)
    {
        heap_.push_back(key);
        int idx = heap_.size() - 1;
        while (idx != 0 && heap_[parent_idx(idx)] > heap_[idx])
        {
            swap(&heap_[idx], &heap_[parent_idx(idx)]);
            idx = parent_idx(idx);
        }
    };

    void delete_key(const int key)
    {
        auto it = std::find_if(heap_.begin(), heap_.end(),
            [key](const int val) { return val == key; });
        if (it != heap_.end())
        {
            const int idx = std::distance(heap_.begin(), it);
            heap_[idx] = heap_.back();
            if (heap_.size() > 1)
            {
                heap_.pop_back();
                heapify(idx);
            }
        }
    };

    void delete_min_key()
    {       
        if (!heap_.empty())
        {
            heap_[0] = heap_.back();
            if (heap_.size() > 1)
            {
                heap_.pop_back();
                heapify(0);
            }
        }
    };

    void print()
    {
        if (heap_.empty())
        {
            return;
        }

        int prev_level = 1;
        std::cout << heap_[0];
        for (size_t i = 1; i < heap_.size(); i++)
        {
            int curr_level = idx_level(i);
            if (curr_level != prev_level)
            {
                std::cout << std::endl;
            }
            std::cout << heap_[i] << " ";
            prev_level = curr_level;
        }
        std::cout << std::endl;
    };

    std::vector<int> heap_;
    size_t max_nodes_;
};

class Node
{
public:
    Node()
      : Node(-1) {};
    Node(const int data)
      : data_(data)
      , left_(nullptr)
      , right_(nullptr) {};
    void print()
    {
        std::cout << data_ << " " <<
            (left_ != nullptr ? left_->data_ : -1) << " " <<
            (right_ != nullptr ? right_->data_ : -1) << " " <<
            std::endl;
    };
    int data_;
    Node* left_;
    Node* right_;
};

class BinarySearchTree
{
public:

    BinarySearchTree()
      : root_(nullptr) {};

    void insert_key(const int data)
    {
        insert_key(data, root_);
    };

    void insert_key(const int data, Node* &root)
    {
        if (root == nullptr)
        {
            root = new Node(data);
        }
        else if (data < root->data_)
        {
            insert_key(data, root->left_);
        }
        else if (data >= root->data_)
        {
            insert_key(data, root->right_);
        }
    };

    void delete_key(const int data)
    {
        delete_key(data, root_, root_);
    };

    void find_min(Node* &root, Node* &root_parent)
    {
        while (root->left_ != nullptr)
        {
            root_parent = root;
            root = root->left_;
        }
    };

    void replace_root(Node* root_replacement, Node* &parent, Node* &root)
    {
        if (parent == root)
        {
            // root is root
            parent = root_replacement;
        }
        else if (parent->left_ && parent->left_->data_ == root->data_)
        {
            parent->left_ = root_replacement;
        }
        else if (parent->right_ && parent->right_->data_ == root->data_)
        {
            parent->right_ = root_replacement;
        }
        else
        {
            std::cout << "parent is not connected to root" << std::endl;
            return;
        }
    }

    void delete_key(const int data, Node* &parent, Node* &root)
    {
        if (root == nullptr)
        {
            return;
        }
        else if (data < root->data_)
        {
            delete_key(data, root, root->left_);
        }
        else if (data > root->data_)
        {
            delete_key(data, root, root->right_);
        }

        // found the node, so delete it

        else if (root->left_ != nullptr && root->right_ != nullptr)
        {
            Node* successor = root->right_;
            Node* successor_parent = root;
            find_min(successor, successor_parent);

            if (successor_parent != root)
            {
                // fill the gap caused by moving the successor
                successor_parent->left_ = successor->right_;
            }

            // give the successor the root leaves w/o cyclic pointers
            successor->left_ = root->left_ == successor ? nullptr : root->left_;
            successor->right_ = root->right_ == successor ? nullptr : root->right_;

            // replace the root
            replace_root(successor, parent, root);
            
        }
        else if (root->left_ == nullptr && root->right_ == nullptr)
        {
            replace_root(nullptr, parent, root);
        }
        else if (root->left_ == nullptr)
        {
            replace_root(root->right_, parent, root);
        }
        else if (root->right_ == nullptr)
        {
            replace_root(root->left_, parent, root);
        }
    };

    void print_in_order()
    {
        std::cout << "tree contents: ";
        print_in_order(root_);
        std::cout << std::endl;
    }

    void print_in_order(Node* root)
    {
        if (root != nullptr)
        {
            print_in_order(root->left_);
            std::cout << root->data_ << " ";
            print_in_order(root->right_);
        }
    };

    Node* root_;
};

enum class KeyManipulation
{
    INSERT,
    DELETE,
    DELETE_MIN,
};

void manipulate_keys(const int num_test_keys, const KeyManipulation manipulation, BinaryMinHeap* bmh)
{
    int count = 0;
    while (count < num_test_keys)
    {
        const int key = std::rand() % num_test_keys;
        switch (manipulation)
        {
            case KeyManipulation::INSERT:
                std::cout << key << " ";
                bmh->insert_key(key);
                break;
            case KeyManipulation::DELETE:
                std::cout << key << " ";
                bmh->delete_key(key);
                break;
            case KeyManipulation::DELETE_MIN:
                if (key % 10 == 0)
                {
                    std::cout << ".";
                    bmh->delete_min_key();
                    break;    
                }
        }
        count++;
    }
    std::cout << std::endl;
    bmh->print();
};

int main()
{
    int num_test_keys = 50;
    std::srand(std::time(nullptr));

    BinaryMinHeap bmh(num_test_keys);
    BinarySearchTree bst;

    bst.insert_key(100);
    bst.insert_key(50);
    bst.insert_key(75);
    bst.insert_key(25);
    bst.insert_key(67);
    bst.insert_key(89);
    bst.insert_key(69);
    bst.insert_key(68);
    bst.insert_key(72);
    bst.print_in_order();

    bst.delete_key(50);
    bst.print_in_order();

    bst.delete_key(100);
    bst.print_in_order();

    bst.delete_key(75);
    bst.print_in_order();

    // insert keys
    std::cout << "*** inserting: ";
    manipulate_keys(num_test_keys, KeyManipulation::INSERT, &bmh);

    // delete keys
    std::cout << "*** deleting: ";
    manipulate_keys(num_test_keys, KeyManipulation::DELETE, &bmh);

    // delete min key
    std::cout << "*** deleting min: ";
    manipulate_keys(num_test_keys, KeyManipulation::DELETE_MIN, &bmh);
}