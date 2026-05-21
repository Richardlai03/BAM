#include<queue>
#include<iostream>
#include<vector>
#include<ctime>
#include<string>
#include<functional>
#include<algorithm>

using namespace std;

class priceTimePriorityMatching {
public:
    struct Order {
        int orderID;
        long long timeCreated;
        string side;
        double price;
        int quantity;
    };

    struct Execution {
        int executionID;
        double price;
        int quantity;
        int sellOrderID;
        int buyOrderID;
    };

    struct CompareBuyOrder {
        bool operator()(const Order& a, const Order& b) {
            if (a.price != b.price) {
                return a.price < b.price;
            }
            return a.timeCreated > b.timeCreated;
        }
    };

    struct CompareSellOrder {
        bool operator()(const Order& a, const Order& b) {
            if (a.price != b.price) {
                return a.price > b.price;
            }
            return a.timeCreated > b.timeCreated;
        }
    };

    priority_queue<Order, vector<Order>, CompareBuyOrder> buyBook;
    priority_queue<Order, vector<Order>, CompareSellOrder> sellBook;
    int execID = 1;
    vector<Execution> executions;
    void processing (Order a) {
        if (a.side == "buy") {
            while (!sellBook.empty() && a.price >= sellBook.top().price) {
                Order b = sellBook.top();
                sellBook.pop();
                int execQuantity = min(a.quantity, b.quantity);
                a.quantity -= execQuantity;
                b.quantity -= execQuantity;
                executions.push_back({execID++, b.price, execQuantity, b.orderID, a.orderID});
                if (b.quantity > 0) {
                    sellBook.push(b);
                }
            }
            if (a.quantity > 0) {
                buyBook.push(a);
        }
        else if (a.side == "sell") {
            while (!buyBook.empty() && a.quantity > 0 && a.price <= buyBook.top().price) {
                Order b = buyBook.top();
                buyBook.pop();
                int execQuantity = min(a.quantity, b.quantity);
                a.quantity -= execQuantity;
                b.quantity -= execQuantity;
                executions.push_back({execID++, b.price, execQuantity, a.orderID, b.orderID});
                if(b.quantity > 0) {
                    buyBook.push(b);
                }
            }
            if(b.quantity > 0) {
                buyBook.push(b);
            }        
        }
    }
};


