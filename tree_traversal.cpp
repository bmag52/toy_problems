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
    
    void balance_lite(int idx)
    {
        while (idx != 0 && heap_[parent_idx(idx)] > heap_[idx])
        {
            swap(&heap_[idx], &heap_[parent_idx(idx)]);
            idx = parent_idx(idx);
        }
    };

    void balance_heavy(int idx) 
    { 
        int idx_left = left_idx(idx); 
        int idx_right = right_idx(idx); 
        int smallest_idx = idx; 
        if (idx_left < heap_.size() && heap_[idx_left] < heap_[idx])
        {
            smallest_idx = idx_left; 
        }
        if (idx_right < heap_.size() && heap_[idx_right] < heap_[smallest_idx])
        {
            smallest_idx = idx_right; 
        }
        if (smallest_idx != idx) 
        { 
            swap(&heap_[idx], &heap_[smallest_idx]); 
            balance_heavy(smallest_idx); 
        } 
    };

    void insert_key(const int key)
    {
        heap_.push_back(key);
        balance_lite(heap_.size() - 1);
    };

    bool delete_key(const int key)
    {
        auto it = std::find_if(heap_.begin(), heap_.end(),
            [key](const int val) { return val == key; });
        if (it != heap_.end())
        {
            const int idx = std::distance(heap_.begin(), it);
            heap_[idx] = std::numeric_limits<int>::min();
            balance_lite(idx);
            delete_min_key();
            return true;
        }
        return false;
    };

    void delete_min_key()
    {       
        heap_.erase(heap_.begin());
        balance_heavy(0);
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

int main()
{
    int num_test_keys = 50;
    std::srand(std::time(nullptr));

    BinaryMinHeap bmh(num_test_keys);

    // insert keys
    int count = 0;
    std::cout << "--------" << std::endl;
    std::cout << "inserting: ";
    while (count < num_test_keys)
    {
        const int key = std::rand() % num_test_keys;
        std::cout << key << " ";
        bmh.insert_key(key);
        count++;
    }
    std::cout << std::endl;
    bmh.print();

    // delete keys
    count = 0;
    std::cout << "--------" << std::endl;
    std::cout << "deleting: ";
    while (count < num_test_keys)
    {
        const int key = std::rand() % num_test_keys;
        if (bmh.delete_key(key))
        {
            std::cout << key << " ";
        }
        count++;
    }
    std::cout << std::endl;
    bmh.print();
}