#include <vector>
#include <cstdlib>
#include <iostream>

typedef std::vector<int> DataVector;

void set_data(const int max_size, DataVector* data)
{
    data->clear();
    
    for (size_t i = 0; i < max_size; ++i)
    {
        std::cout << "--";
    }
    std::cout << std::endl;

    for (size_t i = 0; i < max_size; ++i)
    {
        const int val = std::rand() % 10;
        std::cout << val << " ";
        data->push_back(val);
    }
    std::cout << "random data" << std::endl;
};

void print(const DataVector data, const std::string desc)
{
    for (const auto& val : data)
    {
        std::cout << val << " ";
    }
    std::cout << desc << std::endl;
};

void merge(const DataVector left, const DataVector right, DataVector* data)
{
    data->clear();
    int li = 0;
    int ri = 0;
    while (li < left.size() && ri < right.size())
    {
        if (left[li] <= right[ri])
        {
            data->push_back(left[li]);
            li++;
        }
        else
        {
            data->push_back(right[ri]);
            ri++;
        }
    }

    while (li < left.size())
    {
        data->push_back(left[li]);
        li++;
    }

    while (ri < right.size())
    {
        data->push_back(right[ri]);
        ri++;
    }
};

void merge_sort(DataVector* data)
{
    const int data_size = data->size();
    if (data_size > 1)
    {
        DataVector left, right;
        for (size_t i = 0; i < data_size / 2; ++i)
        {
            left.push_back((*data)[i]);
        }
        for (size_t i = data_size / 2; i < data_size; ++i)
        {
            right.push_back((*data)[i]);
        }
        merge_sort(&left);
        merge_sort(&right);
        merge(left, right, data);
    }
};

void bubble_sort(DataVector* data)
{
    bool data_swapped = true;
    while (data_swapped)
    {
        data_swapped = false;
        for (int i = 1; i < data->size(); ++i)
        {
            const int prev_val = (*data)[i - 1];
            const int curr_val = (*data)[i];
            if (curr_val < prev_val)
            {
                (*data)[i - 1] = curr_val;
                (*data)[i] = prev_val;
                data_swapped = true;
                break;
            }
        }
    }
};

void insertion_sort(DataVector* data)
{
    class Node
    {
    public:
        Node()
          : Node(-1) {};
        Node(const int data)
          : data_(data)
          , next_(nullptr)
          , prev_(nullptr) {};
        int data_;
        Node* next_;
        Node* prev_;
    };

    if (data->empty()) { return; }

    Node* end_node = new Node((*data)[data->size() - 1]);
    Node* start_node = new Node();
    start_node->next_ = end_node;
    end_node->prev_ = start_node;
    data->pop_back();

    while (!data->empty())
    {
        Node* new_node = new Node((*data)[data->size() - 1]);
        data->pop_back();

        if (new_node->data_ > end_node->data_)
        {
            end_node->next_ = new_node;
            new_node->prev_ = end_node;
            end_node = new_node;
        }
        else
        {
            Node* curr_node = end_node;
            while (curr_node->prev_ != nullptr && curr_node->prev_->data_ != -1 &&
                   curr_node->prev_->data_ > new_node->data_)
            {
                curr_node = curr_node->prev_;
            }

            Node* prev_node = curr_node->prev_;
            prev_node->next_ = new_node;
            curr_node->prev_ = new_node;
            new_node->prev_ = prev_node;
            new_node->next_ = curr_node;
        }
    }

    Node* curr_node = start_node->next_;
    while (curr_node != nullptr)
    {
        data->push_back(curr_node->data_);
        curr_node = curr_node->next_;
    }
};

void quick_sort(DataVector* data)
{

};

int main()
{
    const int max_size = 20;
    std::srand(std::time(nullptr));
    std::vector<int> data;

    set_data(max_size, &data);
    merge_sort(&data);
    print(data, "merge sort");

    set_data(max_size, &data);
    bubble_sort(&data);
    print(data, "bubble sort");

    set_data(max_size, &data);
    insertion_sort(&data);
    print(data, "insertion sort");

    set_data(max_size, &data);
    quick_sort(&data);
    print(data, "quick sort");
};