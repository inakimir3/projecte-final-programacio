#include <iostream>
using namespace std;

struct Car {
    string driverName;
    string team;
    enum Status { BOX, RACING, FINISHED, RETIRED } status;
};

class List {
private:
    struct Node {
        Car car;
        Node* next;
        Node* prev;
        Node(const Car& c) : car(c), next(nullptr), prev(nullptr) {}
    };

    Node* first = nullptr;
    Node* last = nullptr;
    unsigned int numElements = 0;

public:
    ~List() {
        while (first != nullptr) {
            Node* temp = first;
            first = first->next;
            delete temp;
        }
    }

    unsigned int getNumElements() const {
        return numElements;
    }

    void pushBack(const Car& car) {
        Node* newNode = new Node(car);

        if (!last) {
            first = last = newNode;
        } else {
            last->next = newNode;
            newNode->prev = last;
            last = newNode;
        }
        numElements++;
    }

    bool pilotExists(const string& driverName) const {
        Node* current = first;
        while (current) {
            if (current->car.driverName == driverName)
                return true;
            current = current->next;
        }
        return false;
    }

    bool removeByDriver(const string& driverName) {
        Node* current = first;
        while (current) {
            if (current->car.driverName == driverName) {

                if (current->prev)
                    current->prev->next = current->next;
                else
                    first = current->next;

                if (current->next)
                    current->next->prev = current->prev;
                else
                    last = current->prev;

                delete current;
                numElements--;
                return true;
            }
            current = current->next;
        }
        return false;
    }

    bool overtakePosition(const string& driverName) {
        Node* current = first;

        while (current) {
            if (current->car.driverName == driverName) {

                if (!current->prev)
                    return false;

                Node* previous = current->prev;
                Node* beforePrevious = previous->prev;
                Node* afterCurrent = current->next;

                current->prev = beforePrevious;
                current->next = previous;

                previous->prev = current;
                previous->next = afterCurrent;

                if (afterCurrent)
                    afterCurrent->prev = previous;

                if (beforePrevious)
                    beforePrevious->next = current;
                else
                    first = current;

                if (previous == last)
                    last = previous;

                return true;
            }
            current = current->next;
        }
        return false;
    }

    bool changeCarStatus(const string& driverName, Car::Status newStatus) {
        Node* current = first;
        while (current) {
            if (current->car.driverName == driverName) {
                current->car.status = newStatus;
                return true;
            }
            current = current->next;
        }
        return false;
    }

    string statusToString(Car::Status status) const {
        switch (status) {
            case Car::BOX: return "BOX";
            case Car::RACING: return "EN CURSA";
            case Car::FINISHED: return "FINALITZAT";
            default: return "ABANDONAT";
        }
    }

    void print() const {
        Node* current = first;
        int position = 1;

        while (current) {
            cout << position << "- "
                 << current->car.driverName
                 << " | " << current->car.team
                 << " (" << statusToString(current->car.status) << ")"
                 << endl;
            current = current->next;
            position++;
        }
    }

    void printReverse() const {
        Node* current = last;
        int position = numElements;

        while (current) {
            cout << position << "- "
                 << current->car.driverName
                 << " | " << current->car.team
                 << " (" << statusToString(current->car.status) << ")"
                 << endl;
            current = current->prev;
            position--;
        }
    }
};

class Race {
private:
    string circuitName;
    List classification;

public:
    void addCar(const Car& car) {
        if (classification.getNumElements() >= 20) {
            cout << "No hi caben més cotxes a la cursa" << endl;
            return;
        }

        if (classification.pilotExists(car.driverName)) {
            cout << "Aquest pilot ja existeix" << endl;
            return;
        }

        classification.pushBack(car);
    }

    void removeCar(const string& driverName) {
        classification.removeByDriver(driverName);
    }

    void changeStatus(const string& driverName, Car::Status status) {
        if (status == Car::RETIRED)
            removeCar(driverName);
        else
            classification.changeCarStatus(driverName, status);
    }

    void overtake(const string& driverName) {
        classification.overtakePosition(driverName);
    }

    void printClassification() const {
        classification.print();
    }

    void printReverseClassification() const {
        classification.printReverse();
    }
};

int main() {
    Race race;

    Car c1 = {"Charles Leclerc", "Ferrari", Car::RACING};
    Car c2 = {"Max Verstappen", "RedBull", Car::RACING};
    Car c3 = {"George Russell", "Mercedes", Car::RACING};
    Car c4 = {"Lando Norris", "McLaren", Car::RACING};
    Car c5 = {"Fernando Alonso", "Aston Martin", Car::RACING};
    Car c6 = {"Carlos Sainz", "Williams", Car::RACING};
    Car c7 = {"Pierre Gasly", "Alpine", Car::RACING};

    race.addCar(c1);
    race.addCar(c2);
    race.addCar(c3);
    race.addCar(c4);
    race.addCar(c5);
    race.addCar(c6);
    race.addCar(c7);

    cout << "Classificacio inicial:" << endl;
    race.printClassification();
    cout << endl;

    cout << "Classificacio inversa:" << endl;
    race.printReverseClassification();
    cout << endl;

    cout << "--- AVANÇAR POSICIONS ---" << endl;
    cout << "Carlos Sainz avança una posicio" << endl;
    race.overtake("Carlos Sainz");
    race.printClassification();
    cout << endl;

    cout << "Carlos Sainz torna a avançar" << endl;
    race.overtake("Carlos Sainz");
    race.printClassification();
    cout << endl;

    cout << "--- CANVIS D'ESTAT ---" << endl;
    cout << "Fernando Alonso entra a BOX" << endl;
    race.changeStatus("Fernando Alonso", Car::BOX);

    cout << "Charles Leclerc finalitza la cursa" << endl;
    race.changeStatus("Charles Leclerc", Car::FINISHED);
    cout << endl;

    cout << "Classificacio despres dels canvis d'estat:" << endl;
    race.printClassification();
    cout << endl;

    cout << "--- ABANDONAMENT ---" << endl;
    cout << "Pierre Gasly abandona" << endl;
    race.changeStatus("Pierre Gasly", Car::RETIRED);
    cout << endl;

    cout << "--- CLASSIFICACIO FINAL ---" << endl;
    race.printClassification();
}
