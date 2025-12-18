#include <iostream>
using namespace std;


struct Cotxe {
    string nomPilot;
    string escuderia;
    int voltes;
    enum Estat {BOX, EN_CURSA, FINALITZAT, ABANDONAR};
};

class Cursa {
private:
    string nomCircuit;
    List classificacio;
public:
    void afegirCotxe(); 
    void treureCotxe(); // abandonar, eliminar de la llista
    void canviEstat(Estat estat) { 
        if (estat == ABANDONAR){ 
            treureCotxe();
        }
        
    };
    void display();
    // display al reves
    void adelantar();
};

int main(){}

