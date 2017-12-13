#include <SDL2/SDL.h>
#include <string.h>
#include "SDL2/SDL_image.h"
#include <iostream>
#include <cstdlib>
#include <fstream>
#define NCOL 20


using namespace std;

typedef struct Valores{
    int nFilas, nColumnas;
}Orden;

class Player{
    private:
        string name;
        string maze;
        int tiempo;
        int coordx;
        int coordy;
    public:
        Player();
        void InicializarJugador(string, string, int, int, int);
        void AlmacenarPlayer(Player);
        string GetName();
        string GetMaze();
        int GetTiempo();
        int GetCoordx();
        int GetCoordy();
        
        
};

typedef struct nodo{
    Player dato;
    nodo *sig;
}lista;

class TopTen{
    private:
        lista *pInicio;
        
    public:
        TopTen();
        int Size();
        int NumPlayer();
        void Mostrar();
        void OrdenarNombres(string [], int);
        void OrdenarPasos(int[], int);
        void InsertarPlayer(Player);
        void GuardarTopTen();
        void LeerTopTenNombres();
        void LeerTopTenPasos();
        void BorrarListaaux();
        void BorrarLista(nodo *p);
};

class VentanaPrincipal{
    private:
        /*Variable donde se alamcenara nuestra ventana*/
        SDL_Window *mainWindow; 
        /*Variable donde guardara el canvas*/
        SDL_Surface *windowSurface;
        
    public:
        VentanaPrincipal(); // constructor para incializar las variables para la interfaz
        void IniciarVentana(string, string); // aca iniciarmeos nuestra ventana
        void ColocarImagen(int , int , SDL_Surface *); // fucnion que nos servira para colocar una imagen
        void VentanaAyuda();

    };

class Archivo{
        public:

    
        static Orden ObtenerOrden(string);
        static int BuscarSalida(char [][NCOL], int, int, int);
        static void LlenarMatriz(char [][NCOL], int, string);
        static void MostrarMatriz(char[][NCOL], int, int);
            
};




