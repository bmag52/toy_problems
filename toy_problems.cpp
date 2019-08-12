#include <iostream>
#include <limits>
#include <vector>
#include <string>
#include <map>

// https://www.interviewcake.com/question/python/stock-price
typedef std::vector<float> PriceVector;
PriceVector generate_stock_prices()
{
    const float max_stock_price_change = 2;
    
    PriceVector stock_prices;
    stock_prices.push_back(500); // initial stock price
    for (size_t i = 1; i < 60 * 8; ++i)
    {
        const float prev_stock_price = stock_prices[i - 1];
        const float stock_price_change = 
                static_cast<float>(std::rand() % 100) / 100 * 
                    max_stock_price_change;
        const float curr_stock_price = prev_stock_price + 
                (std::rand() % 2 == 0 
                    ? stock_price_change
                    : -1 * stock_price_change);
        stock_prices.push_back(curr_stock_price);
    }
    return stock_prices;
};

void get_max_profit(const PriceVector stock_prices)
{
    if (stock_prices.size() < 2)
    {
        return;
    }
    float min_price = stock_prices[0];
    float max_profit = stock_prices[1] - stock_prices[0];
    for (const auto& curr_price : stock_prices)
    {
        const float curr_profit = curr_price - min_price;
        if (curr_profit > max_profit)
        {
            max_profit = curr_profit;
        }
        if (curr_price < min_price)
        {
            min_price = curr_price;
        }
    }
    std::cout << "max profit: " << max_profit << std::endl;
}

void string_manipulation()
{
    std::string test = "test";
    for (size_t i = 0; i < test.length(); i++)
    {
        char letter = test[i];
        std::cout << letter << std::endl;
    }
};

void sort_orders(std::vector<std::string> orders)
{
    // 1234 5678 9123 4567
    //  ID     METADATA
    // prime orders: alpha numeric ids
    // non prime orders: non alpha numeric ids
    // prime orders are sorted and returned first
    //      ties are sorted by metadata
    // non prime orders are returned in intial order last 

    // bin order numbers into following containers
    std::vector<std::string> prime_ids;
    std::vector<std::string> prime_metadatas;
    std::vector<std::string> non_prime_orders;
    for (const auto order : orders)
    {
        std::string id;
        std::string metadata;
        int id_digit_count = 0;
        int space_count = 0;
        for (const auto letter : order)
        {
            space_count += letter == ' ' ? 1 : 0;
            
            if (space_count == 0)
            {
                id += letter;
                id_digit_count += std::isdigit(letter) ? 1 : 0;
            }
            else
            {
                metadata += letter;
            }

            // early exit for non prime orders
            if (id_digit_count == 4)
            {
                non_prime_orders.push_back(order);
                break;    
            }
        }

        if (id_digit_count < 4)
        {
            prime_ids.push_back(id);
            prime_metadatas.push_back(metadata);
        }
    }

    // sort prime orders
    auto compare_prev = [](std::vector<std::string> strings, size_t idx)
    {
        return std::strcmp(strings[idx].c_str(), strings[idx - 1].c_str());
    };

    auto swap_prev = [](std::vector<std::string>& strings, size_t idx)
    {
        std::string tmp = strings[idx];
        strings[idx] = strings[idx - 1];
        strings[idx - 1] = tmp;
    };

    bool swapped = true;
    while (swapped)
    {
        swapped = false;
        for (size_t i = 1; i < prime_ids.size() && i < prime_metadatas.size(); ++i)
        {
            int id_compare_result = compare_prev(prime_ids, i);
            if (id_compare_result == 0)
            {
                int metadata_compare_result = compare_prev(prime_metadatas, i);
                if (metadata_compare_result < 0)
                {
                    swapped = true;
                }
            }
            else if (id_compare_result < 0)
            {
                swapped = true;
            }

            if (swapped)
            {
                swap_prev(prime_ids, i);
                swap_prev(prime_metadatas, i);
                break;
            }
        }
    }

    // print sorted orders
    std::cout << "-- prime orders --" << std::endl;
    for (size_t i = 0; i < prime_ids.size() && i < prime_metadatas.size(); ++i)
    {
        std::cout << prime_ids[i] << prime_metadatas[i] << std::endl;
    }
    std::cout << "-- non prime orders --" << std::endl;
    for (const auto non_prime_order : non_prime_orders)
    {
        std::cout << non_prime_order << std::endl;
    }
}

void k_largest(std::vector<int> vals, const int k)
{
    std::map<size_t, int> k_largest_vals;
    for (size_t i = 1; i <= k; i++)
    {
        int max_val = std::numeric_limits<int>::min();
        size_t max_idx = 0;
        for (size_t j = 0; j < vals.size(); j++)   
        {
            if (vals[j] > max_val && k_largest_vals.find(j) == k_largest_vals.end())
            {
                max_val = vals[j];
                max_idx = j;
            }
        }
        k_largest_vals[max_idx] = max_val;
    }

    for (const auto entry : k_largest_vals)
    {
        std::cout << entry.second << std::endl;
    }
}

void pythagorean_triplet(std::vector<int> vals)
{
    struct PythagoreanTriplet
    {
        int a_;
        int b_;
        int c_;

        PythagoreanTriplet(int a, int b, int c)
          : a_(a)
          , b_(b)
          , c_(c)
        {    
        };

        bool operator==(const PythagoreanTriplet other) const
        {
            return a_ == other.a_ && b_ == other.b_ && c_ == other.c_;
        };

        void print_data() const
        {
            std::cout << a_ << ", " << b_ << ", " << c_ << std::endl;
        };
    };

    std::vector<PythagoreanTriplet> triplets;
    for (const auto a : vals)
    {
        for (const auto b : vals)
        {
            for (const auto c : vals)
            {
                if (a * a + b * b == c * c)
                {
                    PythagoreanTriplet triplet(a, b, c);
                    if (std::find_if(triplets.begin(), triplets.end(),
                        [triplet](const PythagoreanTriplet other)
                        { return other == triplet; }) == triplets.end())
                    {
                        triplets.push_back(triplet);
                    }
                }
            }
        }
    }

    for (const auto triplet : triplets)
    {
        triplet.print_data();
    }
}

int main()
{
    std::srand(std::time(nullptr));
    get_max_profit(generate_stock_prices());
    string_manipulation();  

    std::vector<std::string> orders;
    orders.push_back("zbc1 123s 69s2 019c");
    orders.push_back("mbc1 123s 69s2 019c");
    orders.push_back("abc1 123s 69s2 019c");
    orders.push_back("l32i 123s 69s2 019c");
    orders.push_back("abc1 023s 69s2 019c");
    orders.push_back("1234 123s 69s2 019c");
    orders.push_back("7890 123s 69s2 019c");
    sort_orders(orders);

    std::vector<int> vals1 = {1, 7, 3, 0, 20, 10, 2, 6, 50};
    k_largest(vals1, 4);

    std::vector<int> vals2 = {3, 4, 5, 6, 2, 4, 8, 10, 10, 15};
    pythagorean_triplet(vals2);
}