#include <iostream>
using namespace std;

struct Cotxe {
    string nomPilot;
    string escuderia;
    enum Estat {BOX, EN_CURSA, FINALITZAT, ABANDONAT} estat;
};

class List {
private:
    struct Node {
        Cotxe cotxe;
        Node* next;
        Node* prev;
        Node(const Cotxe& c) : cotxe(c), next(nullptr), prev(nullptr) {}
    };

    Node* first = nullptr;
    Node* last = nullptr;
    unsigned int num_elems = 0;

public:
    List() {
        first = nullptr;
        last = nullptr;
        num_elems = 0;
    }

    ~List() {
        while (first != nullptr) {
            Node* temp = first;
            first = first->next;
            delete temp;
        }
    }

    int getNumElems() {
        return num_elems;
    }

    Node* getFirst() {
        return first;
    }

    void push_back(const Cotxe& c) {
        Node* newNode = new Node(c);

        if (last == nullptr) {
            first = last = newNode;
        } else {
            last->next = newNode;
            newNode->prev = last;
            last = newNode;
        }
        num_elems++;
    }

    void insert(unsigned int position, const Cotxe& c) {
        if (position == 0) {
            Node* newNode = new Node(c);

            if (first == nullptr) {
                first = last = newNode;
            } else {
                newNode->next = first;
                first->prev = newNode;
                first = newNode;
            }
            num_elems++;
        } else if (position >= num_elems) {
            push_back(c);
        } else {
            Node* current = first;
            unsigned int index = 0;
            while (index < position) {
                current = current->next;
                index++;
            }

            Node* newNode = new Node(c);
            newNode->next = current;
            newNode->prev = current->prev;

            if (current->prev != nullptr) {
                current->prev->next = newNode;
            }
            current->prev = newNode;
            num_elems++;
        }
    }

    bool removeByPilot(const string& nomPilot) {
        Node* actual = first;
        while (actual != nullptr) {
            if (actual->cotxe.nomPilot == nomPilot) {

                if (actual->prev != nullptr) {
                    actual->prev->next = actual->next;
                } else {
                    first = actual->next;
                }

                if (actual->next != nullptr) {
                    actual->next->prev = actual->prev;
                } else {
                    last = actual->prev;
                }

                delete actual;
                num_elems--;
                return true;
            }
            actual = actual->next;
        }
        return false;
    }

    bool passPosition(const string& nomPilot) {
        Node* actual = first;

        while (actual != nullptr) {
            if (actual->cotxe.nomPilot == nomPilot) {

                if (actual->prev == nullptr) {
                    return false;
                }

                Node* anterior = actual->prev;
                Node* abans = anterior->prev;
                Node* despres = actual->next;

                actual->prev = abans;
                actual->next = anterior;

                anterior->prev = actual;
                anterior->next = despres;

                if (despres != nullptr) {
                    despres->prev = anterior;
                }

                if (abans != nullptr) {
                    abans->next = actual;
                } else {
                    first = actual;
                }

                if (anterior == last) {
                    last = anterior;
                }

                return true;
            }
            actual = actual->next;
        }
        return false;
    }

    bool changeStatus(const string& nomPilot, Cotxe::Estat nouEstat) {
        Node* actual = first;

        while (actual != nullptr) {
            if (actual->cotxe.nomPilot == nomPilot) {
                actual->cotxe.estat = nouEstat;
                return true;
            }
            actual = actual->next;
        }
        return false;
    }

    string estatToString(Cotxe::Estat estat) const {
        switch (estat) {
            case Cotxe::BOX: return "BOX";
            case Cotxe::EN_CURSA: return "EN CURSA";
            case Cotxe::FINALITZAT: return "FINALITZAT";
            default: return "ABANDONAT";
        }
    }

    void print() const {
        Node* current = first;
        int pos = 1;

        while (current != nullptr) {
            cout << pos << "- "
                 << current->cotxe.nomPilot
                 << " | " << current->cotxe.escuderia
                 << " (" << estatToString(current->cotxe.estat) << ")"
                 << endl;
            current = current->next;
            pos++;
        }
    }

    void printReverse() const {
        Node* current = last;
        int pos = num_elems;

        while (current != nullptr) {
            cout << pos << "- "
                 << current->cotxe.nomPilot
                 << " | " << current->cotxe.escuderia
                 << " (" << estatToString(current->cotxe.estat) << ")"
                 << endl;
            current = current->prev;
            pos--;
        }
    }

    bool existeixPilot(const string& nomPilot) const {
        Node* current = first;
        while (current != nullptr) {
            if (current->cotxe.nomPilot == nomPilot)
                return true;
            current = current->next;
        }
        return false;
    }
};

class Cursa {
private:
    string nomCircuit;
    List classificacio;
public:
    void afegirCotxe(const Cotxe& car) {
        if (classificacio.getNumElems() < 20) {
            if (classificacio.existeixPilot(car.nomPilot)) {
                cout << "Aquest pilot ja existeix" << endl;
                return;
            }

            classificacio.push_back(car);
        } else {
            cout << "No hi caben més cotxes a la cursa" << endl;
        }
    }

    void treureCotxe(const string& nomPilot) {
        classificacio.removeByPilot(nomPilot);
    }

    void canviEstat(const string& nomPilot, Cotxe::Estat nouEstat) {
        if (nouEstat == Cotxe::ABANDONAT) {
            treureCotxe(nomPilot);
        } else {
            classificacio.changeStatus(nomPilot, nouEstat);
        }
    }

    void display() {
        classificacio.print();
    }

    void displayReverse() {
        classificacio.printReverse();
    }

    void adelantar(const string& nomPilot) {
        classificacio.passPosition(nomPilot);
    }

};

int main() {
    Cursa cursa;

    Cotxe c1 = {"Charles Leclerc", "Ferrari", Cotxe::EN_CURSA};
    Cotxe c2 = {"Max Verstappen", "RedBull", Cotxe::EN_CURSA};
    Cotxe c3 = {"George Russell", "Mercedes", Cotxe::EN_CURSA};
    Cotxe c4 = {"Lando Norris", "McLaren", Cotxe::EN_CURSA};
    Cotxe c5 = {"Fernando Alonso", "Aston Martin", Cotxe::EN_CURSA};
    Cotxe c6 = {"Carlos Sainz", "Williams", Cotxe::EN_CURSA};
    Cotxe c7 = {"Pierre Gasly", "Alpine", Cotxe::EN_CURSA};


    cursa.afegirCotxe(c1);
    cursa.afegirCotxe(c2);
    cursa.afegirCotxe(c3);
    cursa.afegirCotxe(c4);
    cursa.afegirCotxe(c5);
    cursa.afegirCotxe(c6);
    cursa.afegirCotxe(c7);


    cout << "Classificacio inicial:" << endl;
    cursa.display();
    cout << endl;

    cout << "Classificacio inversa:" << endl;
    cursa.displayReverse();
    cout << endl;

    cout << "--- AVANÇAR POSICIONS ---" << endl;
    cout << "Carlos Sainz avança una posicio" << endl;
    cursa.adelantar("Carlos Sainz");
    cursa.display();
    cout << endl;


    cout << "Carlos Sainz torna a avançar" << endl;
    cursa.adelantar("Carlos Sainz");
    cursa.display();
    cout << endl;

    cout << "--- CANVIS D'ESTAT ---" << endl;
    cout << "Fernando Alonso entra a BOX" << endl;
    cursa.canviEstat("Fernando Alonso", Cotxe::BOX);

    cout << "Charles Leclerc finalitza la cursa" << endl;
    cursa.canviEstat("Charles Leclerc", Cotxe::FINALITZAT);
    cout << endl;

    cout << "Classificacio despres dels canvis d'estat:" << endl;
    cursa.display();
    cout << endl;

    cout << "--- ABANDONAMENT ---" << endl;
    cout << "Pierre Gasly abandona" << endl;
    cursa.canviEstat("Pierre Gasly", Cotxe::ABANDONAT);
    cout << endl;

    cout << "--- CLASSIFICACIO FINAL ---" << endl;
    cursa.display();

}
