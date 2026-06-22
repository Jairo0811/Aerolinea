#include <iostream>
#include <string>
#include <stdlib.h>
#include <conio.h>
#include <stdio.h>
using namespace std;

struct Nodo{
    string nombre;
    Nodo *siguiente;
};



class Paises {
private:
    Nodo *cabeza, *cola;

public:
    Paises(){
    cabeza = NULL;
    cola = NULL;
    }


////////////////////////////////////////////////////////////////////////////////

    void introducirDestino(string pais){
    Nodo *temp = new Nodo;
    temp-> nombre = pais;
    temp-> siguiente = NULL;
        if(cabeza == NULL){
            cabeza = temp;
            cola = temp;
            temp = NULL;
        } else {
        cola -> siguiente = temp;
        cola = temp;
        }
    }

//////////////////////////////////////////////////////////////////
    bool tiene_origen_o_destino(string origen ,string destino){
        Nodo * temp = new Nodo;
        temp = cabeza;
        bool esVerdad = false;
        int aiuda = 0;
        while(temp != NULL){
            if(temp-> nombre ==  origen){
                aiuda++;
            }

            if(temp -> nombre == destino){
                aiuda++;
            }

            if(aiuda == 2){
                esVerdad = true;
                break;
            }
            temp = temp->siguiente;
        }
        return esVerdad;
    }

//////////////////////////////////////////////////////////////////

    int cuenta (string origen, string destino){

    int cont = 0;
    if(origen == destino){
    return 0;
    }
    Nodo *temp = new Nodo;
    temp = cabeza;
    Nodo* principio = new Nodo;
    principio = cabeza;
        while(temp != NULL){

                if(temp -> nombre == origen) {
                    cont++;
                    temp = temp->siguiente;
                    continue;
                }


                if(temp-> nombre == destino && cont > 0){
                    cont++;
                    break;
                }

                if(cont > 0 ){
                    cont++;
                    temp = temp->siguiente;
                    continue;
                }

                temp = temp -> siguiente;

                if(temp -> siguiente == NULL){
                    temp->siguiente = principio;
                    temp = temp->siguiente;
                }

        }
    return cont -= 2;
    }

////////////////////////////////////////////////////////////////////////

    string buscarRuta(string origen, string destino){

    string ruta = "";
    int coun = 0;

    if(!tiene_origen_o_destino(origen,destino)){
        ruta = "El origen o el destino que ingresaste no existe";
        return ruta;
    }

    if(origen == destino){
    ruta = "Estas en ese lugar";
    return ruta;
//    cout<<ruta<<endl;
    }

    Nodo *temp = new Nodo;
    temp = cabeza;
    Nodo* principio = new Nodo;
    principio = cabeza;
        while(temp != NULL){

                if(temp -> nombre == origen) {
                    coun++;
                    ruta += temp->nombre;
                    temp = temp->siguiente;
                    ruta += " Hacia ";
                    ruta += temp->nombre;
                    continue;
                }

                if(temp-> nombre == destino){

                    break;
                }

                if(coun > 0 ){
                    coun++;
                    ruta += "\n";
                    ruta += temp->nombre;
                    ruta += " Hacia ";
                    temp = temp->siguiente;
                    ruta += temp->nombre;
                    continue;
                }



                temp = temp -> siguiente;
                if(temp -> siguiente == NULL){
                    temp->siguiente = principio;
                    temp = temp->siguiente;
                }

        }
      return ruta;
    }

};

void ingresaEnter(){
while(true){
         cout << "Presiona Enter para ir al menu principal" << endl;
         if(getch() == 13){
            break;
         }
}
}

string preguntador(){
string hihi;
getline(cin,hihi);
return hihi;
}

Paises paises;



int main()
{


    paises.introducirDestino("miami");
    paises.introducirDestino("orlando");
    paises.introducirDestino("santo domingo");
    paises.introducirDestino("san francisco");

    string origen, destino;
    string s,j;
    int finaI;
 //   cout << paises.tiene_origen_o_destino("miami","santo domingo");
 //   cout << paises.tiene_origen_o_destino("sad","santo domingo") << endl;
        while(true){
    string respuesta;
    cout << "Aerolinea" << endl;
    cout << "1-Introducir destino" << endl;
    cout << "2-Introducir ruta" << endl;
    cout << "3-Buscar Ruta" << endl ;
    cout << "Introduzca la opcion que desee: ";
    getline(cin,respuesta);

    if(respuesta == "1"){

                system("cls");
                cout<<"Ingrese el pais nuevo"<<endl;
                cin>>destino;
                cout<<"Agregado correctamente\n\n\n";
                paises.introducirDestino(destino);
                system("cls");

    } else if(respuesta == "2"){

                system("cls");
                cout<<"Ingrese el pais origen"<<endl;
                cin>>origen;
                cout<<"Ingrese el pais destino"<<endl;
                cin>>destino;
                finaI = paises.cuenta(origen,destino);
                cout<< "Esta a " << finaI << " vuelos de llegar a su destino "<<endl <<endl;

    }else if(respuesta == "3"){

                system("cls");

                cout<<"Ingrese el pais origen"<<endl;
                getline(cin,s);

                cout<<" Ingrese el pais destino"<<endl;
                getline(cin,j);

                cout<< paises.buscarRuta(s,j) << endl <<endl;


    }


        }

}



