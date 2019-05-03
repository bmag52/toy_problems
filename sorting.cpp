#include <vector>
#include <cstdlib>
#include <iostream>

typedef std::vector<int> DataVector;

void set_data(const int max_size, DataVector* data)
{
    data->clear();
    std::cout << "adding data: ";
    for (size_t i = 0; i < max_size; ++i)
    {
        const int val = std::rand() % 10;
        std::cout << val << " ";
        data->push_back(val);
    }
    std::cout << std::endl;
};

void print(const DataVector data, const std::string desc)
{
    std::cout << desc << ": ";
    for (const auto& val : data)
    {
        std::cout << val << " ";
    }
    std::cout << std::endl;
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

int main()
{
    const int max_size = 20;
    std::srand(std::time(nullptr));
    std::vector<int> data;

    set_data(max_size, &data);
    merge_sort(&data);
    print(data, "merge sort");
};