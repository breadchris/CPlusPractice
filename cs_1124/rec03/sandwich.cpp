#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Sandwich {
public:
    Sandwich (int tomato = 2, int cheese = 1, float mustard = .05,
                float mayonnaise = 1, int bread = 2):
                tomato(tomato), cheese(cheese), mustard(mustard),
                mayonnaise(mayonnaise), bread(bread) {}

    void display(ostream& os = cout) {
        os << "This sandwich contains: " << bread << " slices of bread, " <<
            cheese << " slices of cheese, " << mayonnaise << " oz of mayonnaise, " << 
            tomato << " slices of tomato, " << mustard << " oz of mustard, " << endl;

    }

    int getBread() const {
        return bread;
    }

    int getCheese() const {
        return cheese;
    }
    
    float getMayonnaise() const {
        return mayonnaise;
    }

    int getTomato() const {
        return tomato;
    }

    float getMustard() const {
        return mustard;
    }

    void setCheese(int newCheese) {
        cheese = newCheese;
    }
    
    void setMayonnaise(float newMayonnaise) {
        mayonnaise = newMayonnaise;
    }

    void setTomato(int newTomato) {
        tomato = newTomato;
    }

    void setMustard(float newMustard) {
        mustard = newMustard;
    }
private:
    int bread;
    int cheese;
    float mayonnaise;
    int tomato;
    float mustard;
};

class SandwichTruck {
public:
    SandwichTruck (vector<Sandwich>& sandwiches): shipment(sandwiches), inventoryChecked(false) {}

    void display (ostream& os = cout) {
        os << "This truck has: " << endl;
        for (Sandwich& s : shipment) {
            os << "\t ";
            s.display(os);
            os << "\n";
        }
    }

    void loadTruck (vector<Sandwich>& sandwiches) {
        shipment = sandwiches;
        inventoryChecked = false;
    }

    void checkInventory(ostream& os = cout) {
        display(os);
        inventoryChecked = true;
    }

private:
    vector<Sandwich> shipment;
    bool inventoryChecked;
};

int main() {
    
    vector<Sandwich> orders;
    SandwichTruck truck(orders);
    
    // Customer 1
    Sandwich order1;
    orders.push_back(order1);
    // Customer 2
    Sandwich order2;
    order2.setMustard(0);
    orders.push_back(order2);
    // Customer 3
    Sandwich order3(2, 2);
    orders.push_back(order3);
    // Customer 4
    Sandwich order4;
    orders.push_back(order4);
    // Delivery Truck Checked
    truck.loadTruck(orders);
    truck.display();
    // Customer 2's order is checked
    cout << "Customer 2's mustard is " << order2.getMustard() << " oz. " << endl;
}

