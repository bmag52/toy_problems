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

class BinarySearchTree()
{
public:
    class Node ()
    {
    public:
        Node()
          : Node(-1) {};
        Node(const int data)
          : data_(data)
          , left_(nullptr)
          , right_(nullptr) {};
        int data_;
        Node* left_;
        Node* right_;
    };

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
        else if (data > root->data_)
        {
            insert_key(data, root->right_);
        }
    };

    void delete_key(const int data)
    {
        delete_key(const int data, nullptr, root_);
    }

    void find_min(Node* &parent, Node* &root)
    {
        while (root->left_ != nullptr)
        {
            parent = root;
            root = root->left_;
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
        else if (root->left_ != nullptr && root->right_ != nullptr)
        {
            find_min(parent, root->right_);
            delete_key(data, parent, root);
        }
        else if (root->left_ != nullptr)
        {

        }
    }

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