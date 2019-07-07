#include <iostream>
#include <limits>
#include <vector>
#include <string>

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

int main()
{
    std::srand(std::time(nullptr));
    get_max_profit(generate_stock_prices());
    string_manipulation();  
}