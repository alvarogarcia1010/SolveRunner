#include "interfaz.h"
#include <stdlib.h>

using namespace std;

int CantFilas();
void LeerListaLaberintos(string []);

int main(int argc, char** argv) {
    string nombre, archivo;
    int cant, opc, i, flag = 0;
    
    
    //Busco todos los txt y los guardo en un archivo 
    system("find -name \"*.txt\" > lista.txt");
    
    cant = CantFilas();
    
    string maze[cant];
    
    LeerListaLaberintos(maze);
    

    //Pido Nombre
    cout << "SOLVERRUNNER 2000" << endl;
    cout << "Nickname: "; getline(cin, nombre);
    
    for( i = 0; i< cant; i++){
        cout << i+1 << ". "<<  maze[i] << endl;
    }
    
    cout << "DE TODAS LAS OPCIONES, NO ELEGIR Usuarios.txt y lista.txt"<<endl;
    cout <<"Ingrese una opcion: "; 
    cin>>opc;
    
    while((opc >= 1 or opc<=cant or opc > cant ) and ( flag == 0 )){
        
        if(opc >= 1 and opc<=cant){
            archivo = maze[opc-1];
            flag = 1;
        }
        else if (opc > cant) {
            cout<<"Opcion no valida" << endl;
            cout <<"Ingrese una opcion: "; cin>>opc;
            flag = 0;
        }
    }
    
    
    cout<< archivo << endl;
    VentanaPrincipal v;
    v.IniciarVentana(nombre, archivo);

    
    return 0;
}

int CantFilas(){
    ifstream archivo;
    string temp;
    int cont=0;
    
    archivo.open("lista.txt");
    
    while(getline(archivo,temp)){
        cont= cont+1;
    }
    archivo.close();
    
    return cont;
}

void LeerListaLaberintos(string m[]){
    ifstream archivo("lista.txt");
    string linea, temp;
    int i = 0;

    while(getline(archivo, linea)){
        temp = linea.substr(2);
        m[i] = temp;
        i++;
    }
}