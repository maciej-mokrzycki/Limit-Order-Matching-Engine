#include <cstdint>
#include <deque>
#include <iostream>
#include <iterator>
#include <map>
#include <set>
#include <string>

using namespace std;

enum class Side{
    Buy,
    Sell
};

struct Order{
    int64_t id;
    Side side;
    int64_t price;
    int64_t remaining;
};

class OrderBook{
private:
    map<int64_t, deque<Order>> bids;
    map<int64_t, deque<Order>> asks;

    // No two orders can have the same ID.
    set<int64_t> used_ids;

public:
    void add_order(Order incoming){
        // Check input.
        if (incoming.id <= 0 ||
            incoming.price <= 0 ||
            incoming.remaining <= 0){
            cout << "ERROR: ID, price, quantity must be positive\n";
            return;
        }

        if(used_ids.contains(incoming.id)){
            cout << "ERROR: this ID has been already used\n";
            return;
        }

        used_ids.insert(incoming.id);

        const bool buying = incoming.side == Side::Buy;

        auto& opposite = buying ? asks : bids;

        while(incoming.remaining > 0 && !opposite.empty()){
            // choose the lowest ask or the highest bid.
            auto level = buying
                ? opposite.begin()
                : prev(opposite.end());

            const int64_t resting_price = level->first;

            const bool prices_match = buying
                ? incoming.price >= resting_price
                : incoming.price <= resting_price;

            if(!prices_match){
                break;
            }

            // Oldest order at this price.
            Order& resting = level->second.front();

            const int64_t traded_quantity =
                incoming.remaining < resting.remaining
                    ? incoming.remaining
                    : resting.remaining;

            const int64_t buy_id = buying
                ? incoming.id
                : resting.id;

            const int64_t sell_id = buying
                ? resting.id
                : incoming.id;


            // Execute at the resting order's price.
            cout << "TRADE"
                 << " buy=" << buy_id
                 << " sell=" << sell_id
                 << " price=" << resting_price
                 << " quantity=" << traded_quantity
                 << '\n';

            incoming.remaining -= traded_quantity;
            resting.remaining -= traded_quantity;

            if(resting.remaining == 0){
                level->second.pop_front();
            }
            // Do not access resting after removing its order.


            if(level->second.empty()){
                opposite.erase(level);
            }
            // after erase() the memory is freed
        }

        // adding the resting part to the order book.
        if(incoming.remaining > 0){
            auto& own_side = buying ? bids : asks;
            own_side[incoming.price].push_back(incoming);

            cout << "RESTING"
                 << " id=" << incoming.id
                 << " quantity=" << incoming.remaining
                 << '\n';
        }
    }

    void print_book() const{
        cout << "BIDS\n";  // Highest price first.

        if(bids.empty())
            cout << "empty\n";

        for(auto level = bids.rbegin(); level != bids.rend(); ++level){
            for(const Order& order : level->second){
                cout << "  price=" << order.price
                     << " quantity=" << order.remaining
                     << " id=" << order.id
                     << '\n';
            }
        }

        cout << "ASKS\n";  // Lowest price first.

        if(asks.empty())
            cout << "empty\n";

        for(const auto& [price, orders] : asks){
            for(const Order& order : orders){
                cout << "  price=" << price
                     << " quantity=" << order.remaining
                     << " id=" << order.id
                     << '\n';
            }
        }
    }
};

int main(){
    OrderBook Book;

    // Available commands.
    cout << "Commands:\n"
         << "  BUY id price quantity\n"
         << "  SELL id price quantity\n"
         << "  BOOK\n"
         << "  QUIT\n";

    string command;

    while(cin >> command){
        if(command == "QUIT")
            break;

        if(command == "BOOK"){
            Book.print_book();
        }
        else if(command == "BUY" || command == "SELL"){
            int64_t id, price, quantity;

            if(!(cin >> id >> price >> quantity)){
                cout << "ERROR: expected three integers\n";
                break;
            }

            Order order;
            order.id = id;
            order.price = price;
            order.remaining = quantity;

            if(command == "BUY"){
                order.side = Side::Buy;
            } else{
                order.side = Side::Sell;
            }

            Book.add_order(order);
        }
        else{
            cout << "ERROR: unknown command\n";
        }
    }

    return 0;
}
