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
        tm timeCreated;
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

    void processing (Order a) {
        if (a.side == "buy") {
            while (a.price >= sellBook.top().price) {
                Order b = sellBook.top();
                int execQuantity = a.quantity;
                if (a.quantity > b.quantity) {
                    execQuantity = b.quantity;
                }
                Execution exec {1, b.price, execQuantity, b.orderID, a.orderID};
                b.quantity -= execQuantity;
                if (b.quantity == 0) {
                    sellBook.pop();
                }
            }
            buyBook.push(a);
        }
        else if (a.side == "sell") {
            while (a.price <= buyBook.top().price) {
                Order b = buyBook.top();
                int execQuantity = a.quantity;
                if (a.quantity > b.quantity) {
                    execQuantity = b.quantity;
                }
                Execution exec {1, b.price, execQuantity, a.orderID, b.orderID};
                b.quantity -= execQuantity;
                if(b.quantity == 0) {
                    buyBook.pop();
                }
            }
            sellBook.push(a);
        }
    }


};


