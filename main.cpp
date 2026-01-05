#include <iostream>
using namespace std;

struct Cotxe {
    string nomPilot;
    string escuderia;
    int voltes;
    enum Estat {BOX, EN_CURSA, FINALITZAT, ABANDONAR} estat;
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

    void push_back(const Cotxe& c) {
        Node* newNode = new Node(c);
        if (last == nullptr) {
            first = last = newNode;
        }
        else {
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
            }
            else {
                newNode->next = first;
                first->prev = newNode;
                first = newNode;
            }
            num_elems++;
        }
        else if (position >= num_elems) {
            push_back(c);
        }
        else {
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
    void print() const {
        Node* current = first;
        while (current != nullptr) {
            std::cout << current->cotxe.nomPilot << " ";
            current = current->next;
        }
        std::cout << std::endl;
    };


};





class Cursa {
private:
    string nomCircuit;
    List classificacio;
public:
    void afegirCotxe(); // 
    void treureCotxe(); // abandonar, eliminar de la llista
    void canviEstat(Cotxe::Estat estat) {
        if (estat == Cotxe::ABANDONAR){
            treureCotxe();
        }
        
    };
    void display();
    // display al reves
    void adelantar();
};

int main(){}

