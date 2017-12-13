#include "interfaz.h"
#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_mixer.h"
#include <stdlib.h>


using namespace std;
        
        char CAMINO = 'C';
        char OBSTACULO = 'P';
        char SALIDA = 'S';
        char RASTRO = 'X';
        //Direcciones
        const int ARRIBA = 1;
        const int ABAJO = 2;
        const int DERECHA = 3;
        const int IZQUIERDA = 4;
        TopTen p1,p3;
        Player p2;

        //Inicialmente el rato vera hacia ABAJO 
        int direccion = ARRIBA;
        int filaActual, colActual;
        int filaAnt, colAnt;
        
        int pasos = 0;
        
        string nombre, archivo;
        int cant, opc, i, flag = 0;
        

//CLASE ARCHIVO
Orden Archivo::ObtenerOrden(string ruta){
    Orden matriz;
    matriz.nColumnas = 0; matriz.nFilas = 0; 
    string line; bool flag = true;
 
    ifstream archivo(ruta.c_str());
    while(getline(archivo, line)){
        matriz.nFilas++;
        if(flag){
            matriz.nColumnas = line.length();
            flag = false;
        }
    }    
    archivo.close();
    return matriz;
}

void Archivo::LlenarMatriz(char m[][NCOL], int col, string ruta){
    ifstream archivo(ruta.c_str());
    string linea;
    int fila = 0;
    while(getline(archivo, linea)){
        for(int columna = 0; columna < col; columna++){
            m[fila][columna]= linea.at(columna);
        }
        fila++;
    }
    archivo.close();
}

void Archivo::MostrarMatriz(char m[][NCOL], int fil, int col){
    for(int fila = 0; fila< fil; fila++){
        for (int columna = 0; columna < col; columna++){
            cout<< m[fila][columna]<< "   ";
        }
        cout << endl;  
    }
    
}

int Archivo::BuscarSalida(char laberinto[][NCOL], int direccion, int fil, int col){

    if(laberinto[fil][col] == OBSTACULO || laberinto[fil][col] == RASTRO){
        return 0;
    }
    else{
        if(laberinto[fil][col] == SALIDA){
            //laberinto[fil][col] = 'x';
            cout << "El raton encontro la salida!" << endl;
            return 1;
        }
        else{
            laberinto[fil][col] = RASTRO;
            switch (direccion){
            case ARRIBA:
                if(BuscarSalida(laberinto, IZQUIERDA, fil, col-1)){
                    return 1;
                }
                else if(BuscarSalida(laberinto, ARRIBA, fil-1, col)){
                    return 1;
                }
                else if(BuscarSalida(laberinto, DERECHA, fil, col+1)){
                    return 1;
                }
                
                else{
                    laberinto[fil][col] = CAMINO;
                    return 0;
                }
                break;
                
            case IZQUIERDA:
                if(BuscarSalida(laberinto, ABAJO, fil+1, col)){
                    return 1;
                }
                
                else if(BuscarSalida(laberinto, IZQUIERDA, fil, col-1)){
                    return 1;
                }
                else if(BuscarSalida(laberinto, ARRIBA, fil-1, col)){
                    return 1;
                }
                else{
                    laberinto[fil][col] = CAMINO;
                    return 0;
                }
                break;
                
            case ABAJO:
                if(BuscarSalida(laberinto, ABAJO, fil+1, col)){
                    return 1;
                }
                else if(BuscarSalida(laberinto, DERECHA, fil, col+1)){
                    return 1;
                }
                else if(BuscarSalida(laberinto, IZQUIERDA, fil, col-1)){
                    return 1;
                }
                else{
                    laberinto[fil][col] = CAMINO;
                    return 0;
                }
                break;
           
            case DERECHA:
                if(BuscarSalida(laberinto, ABAJO, fil+1, col)){
                    return 1;
                }
                else if(BuscarSalida(laberinto, ARRIBA, fil-1, col)){
                    return 1;
                }
                else if(BuscarSalida(laberinto, DERECHA, fil, col+1)){
                    return 1;
                }
                else{
                    laberinto[fil][col] = CAMINO;
                    return 0;
                }
                break;
            }
        }
    }
}

//CLASE VENTANA PRINCIPAL
VentanaPrincipal::VentanaPrincipal()
{
    this->mainWindow=NULL; // colocamos nuestro puneteros a null;
    this->windowSurface=NULL; // colocamos // nuestro canas a null
    
    SDL_Init(SDL_INIT_VIDEO); // inciamos la interfaz grafica
    
}


void VentanaPrincipal::IniciarVentana(string nombre, string archivo)
{
    
    bool inicio = true;
    const int filas = Archivo::ObtenerOrden(archivo).nFilas;
    const int columnas = Archivo::ObtenerOrden(archivo).nColumnas;
    char laberinto[filas][NCOL];
    Archivo::LlenarMatriz(laberinto, columnas,archivo);

    int antx=0, anty=0,f=1, j =2, opc ; 
    SDL_Surface *imageWall=NULL, *guardar , *imageStreet=NULL , *imageExit=NULL, *imageMouse=NULL, *imageFondo = NULL;
    SDL_Surface *buttonSearch = NULL, *buttonNew = NULL, *buttonTopTen = NULL, *buttonExit =NULL ,*logo = NULL, *ayuda = NULL;
   
    
    bool isRunning =true; // con esta bandera indicaremos que el programa esta correindo
    SDL_Event ev; //variable que tendra los eventos
    /*Creando la ventana*/
    this->mainWindow=SDL_CreateWindow("Proyecto X",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,1000,600,SDL_WINDOW_SHOWN);
    this->windowSurface = SDL_GetWindowSurface(this->mainWindow);
    /*Cragando las imagenes*/
    imageWall=IMG_Load("pared.png");
    imageStreet=IMG_Load("pasillo.png");
    imageExit=IMG_Load("salida.png");
    imageMouse=IMG_Load("raton.png");
    imageFondo = IMG_Load("fondo2.jpg");
    logo = IMG_Load("logo.png");
    guardar = IMG_Load("guardar.jpg");
 
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048)< 0 ){
        cout << "ERROR" << Mix_GetError();
    }
	
    Mix_Music *bgm = Mix_LoadMUS("heaven.mp3");
    
    buttonSearch = IMG_Load("BuscarS.png");
    buttonNew = IMG_Load("nuevo.png");
    buttonTopTen = IMG_Load("top10.png");
    buttonExit = IMG_Load("cancel.png");
    ayuda = IMG_Load("ayuda.png");

    
    /*Luego para que este activa crearemos un evento para configurar
     el boton salir*/
   
    while(isRunning) // si esta en true continuara el juego
    {

        
        while(SDL_PollEvent(&ev) != 0) //escuchamos los diferentes eventos que se producen
        {
            if(ev.type == SDL_QUIT) // si el usuario da clic en boton salir de la venana
            {
                isRunning=false; // colocamos false para salir del while y terminar el juego
            }
            
            else if(ev.type == SDL_MOUSEBUTTONDOWN)
            {
                if(ev.button.button == SDL_BUTTON_LEFT)
                {
                 //posicionar el raton
                    
                    //cout << "x: " << ev.button.x << endl;
                    //cout << "y: " << ev.button.y << endl;
                   
                    if( (ev.button.x >= 0 && ev.button.x <= (filas*30)) && (ev.button.y >= 0 && ev.button.y <= (columnas*30)))
                    {
                        if((laberinto[ev.button.y/30][ev.button.x/30] == CAMINO) and inicio)
                        {
        
                            filaActual = (ev.button.y/30)*1;
                            colActual = (ev.button.x/30)*1;
                            VentanaPrincipal::ColocarImagen((ev.button.y/30)*30,(ev.button.x/30)*30,imageMouse);
                            inicio = false;
                        }
                    }
                    if ( (ev.button.y >= 20 && ev.button.y <= 55 ) && (ev.button.x >= 820 && ev.button.x <= (939)) && (j==2)){
                        VentanaAyuda();

                    }
                    if ( (ev.button.y >= 525 && ev.button.y <= 572 ) && (ev.button.x >= 820 && ev.button.x <= (979))){
                        isRunning = false;
                        
                      

                    }
                    if ( (ev.button.y >= 15 && ev.button.y <= 78 ) && (ev.button.x >= 620 && ev.button.x <= (695))){
                        p2.InicializarJugador(nombre,archivo,pasos,filaAnt*30,colAnt*30);
                        p2.AlmacenarPlayer(p2);
                        
                      

                    }
                    if ( (ev.button.y >= 525 && ev.button.y <= 572 ) && (ev.button.x >= 620 && ev.button.x <= (779))){
                            SDL_MinimizeWindow(this->mainWindow);
                            cout<<"**********Top 10**********"<<endl;
                            cout<<"1.Orden Alfabetico"<<endl;
                            cout<<"2.Orden por Puntaje"<<endl;
                            cout<<"Elija una opcion: ";
                            cin>>opc;
                            switch(opc){
                                case 1:
                                    p1.LeerTopTenNombres();
                                    p1.Mostrar();
                                    //p1.GuardarTopTen();
                                    p1.BorrarListaaux();
                                    break;
                                case 2:
                                    p1.LeerTopTenPasos();
                                    p1.Mostrar();
                                    //p1.GuardarTopTen();
                                    p1.BorrarListaaux();
                                    break;
                            }
                            
                                    //Top10();

                    }
                    if ( (ev.button.y >= 450 && ev.button.y <= 497 ) && (ev.button.x >= 820 && ev.button.x <= (979))){
                        SDL_DestroyWindow(this->mainWindow);
                        VentanaPrincipal::IniciarVentana(nombre, archivo);

                        

                    }
                    if ( (ev.button.y >= 450 && ev.button.y <= 497) && (ev.button.x >= 620 && ev.button.x <= (779))){
                        colAnt = colActual;
                        filaAnt = filaActual;
                        Archivo::BuscarSalida(laberinto,direccion,filaActual,colActual);
                            for(int i=0;i<filas;i++){
                                for(int j=0;j<columnas;j++){
                                    if((laberinto[i][j] == RASTRO)){
                                       VentanaPrincipal::ColocarImagen(i*30,j*30,imageMouse);
                                       pasos= pasos+1;
                                    }
                                }
                            }
                            
                         }
                           
                            
                    }
                    
                    
                    
                }
            
                else if (ev.type == SDL_KEYDOWN){
                            switch (ev.key.keysym.sym){
                                case SDLK_p:
                                    if (!Mix_PlayingMusic())
                                        Mix_PlayMusic(bgm, -1);
                                    else if(Mix_PausedMusic())
                                        Mix_ResumeMusic();
                                    else 
                                        Mix_PauseMusic();
                                    break;
                                    
                                case SDLK_s: 
                                    Mix_HaltMusic();
                                    break;
                            } 
                    

                }
            
        }
            
            
            
            if(f==1){
                SDL_BlitSurface(imageFondo,NULL,windowSurface,NULL);

                    //Separador del laberinto
                for(int i = 0; i<20;i++){
                    VentanaPrincipal::ColocarImagen(i*30,570,imageWall);
                }
                    // recorremos la matriz para llenarla
                for(int i=0;i<filas;i++){
                     for(int j=0;j<columnas;j++){
                         if((laberinto[i][j] == OBSTACULO)){
                             VentanaPrincipal::ColocarImagen(i*30,j*30,imageWall);
                         }
                         else if((laberinto[i][j] == CAMINO)){
                             VentanaPrincipal::ColocarImagen(i*30,j*30,imageStreet);
                         }
                         else if((laberinto[i][j] == SALIDA)){
                              VentanaPrincipal::ColocarImagen(i*30,j*30,imageExit);
                         }

                     }
                }
                VentanaPrincipal::ColocarImagen(450,620,buttonSearch);
                VentanaPrincipal::ColocarImagen(525,620,buttonTopTen);
                VentanaPrincipal::ColocarImagen(525,820,buttonExit);
                VentanaPrincipal::ColocarImagen(20, 870, ayuda);
                VentanaPrincipal::ColocarImagen(450, 820, buttonNew);
                VentanaPrincipal::ColocarImagen(0, 700, logo);
                VentanaPrincipal::ColocarImagen(15, 620, guardar);

                        
                f=0;
            
            }
        SDL_UpdateWindowSurface(this->mainWindow);
        // refresacmos la ventana
        
    }
        
        
   
   SDL_FreeSurface(imageWall);
   SDL_FreeSurface(imageStreet); 
   Mix_FreeMusic(bgm);
   bgm = NULL;
   
   SDL_DestroyWindow(this->mainWindow); // destruimos la ventana
   imageWall = imageStreet = NULL;
   this->mainWindow=NULL;
   Mix_Quit();
   SDL_Quit(); // salimos de sdl

    }
 

void VentanaPrincipal::ColocarImagen(int x, int y, SDL_Surface *image)
{
    SDL_Rect destination;
    destination.x=y;
    destination.y=x;
    SDL_BlitSurface(image,NULL,this->windowSurface,&destination);
    //SDL_Flip(this->mainWindowSurface);
    //SDL_UpdateWindowSurface(this->mainWindow);
}

void VentanaPrincipal::VentanaAyuda(){
    
    SDL_Window *window1 = NULL;  
    SDL_Surface *windowSurface = NULL;

    SDL_Init(SDL_INIT_VIDEO);              
    
    SDL_Surface *fondo = NULL;
    
    fondo = IMG_Load("VentanaAyuda.png");
    
    window1 = SDL_CreateWindow("AYUDA",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,640,425,SDL_WINDOW_SHOWN);
    windowSurface = SDL_GetWindowSurface(window1);
    
    if (window1 == NULL) {
           printf("Could not create window: %s\n", SDL_GetError());

    }
    SDL_Event ev;
    bool running = true;
    while(running){
    while(SDL_PollEvent(&ev) !=0)
		{
			if (ev.type == SDL_WINDOWEVENT)
			{
                            switch (ev.window.event){
                                case SDL_WINDOWEVENT_CLOSE:
                                   
                                    running = false;
                            }
			
			}
                        if (ev.type == SDL_WINDOWEVENT){
                            switch (ev.window.event){
                                case SDL_WINDOWEVENT_MOVED:
                                    break;
                            }
                        }
                        //if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT)){
                            //cout << "Hola";
                        //}
	
		}
                SDL_BlitSurface(fondo,NULL,windowSurface,NULL);
                SDL_UpdateWindowSurface(window1);

	}
    SDL_FreeSurface(windowSurface);
    SDL_DestroyWindow(window1);
    
    
    }

Player::Player(){}

void Player::InicializarJugador(string name,string maze,int tiempo,int coordx,int coordy){
    this->name = name;
    this->maze = maze;
    this->tiempo = tiempo;
    this->coordx = coordx;
    this->coordx = coordy;
}

string Player::GetName(){
    return this->name;
}

int Player::GetTiempo(){
    return this->tiempo;
}

string Player::GetMaze(){
    return this->maze;
}

int Player::GetCoordx(){
    return this->coordx;
}

int Player::GetCoordy(){
    return this->coordy;
}

void Player::AlmacenarPlayer(Player p1){
        
    ofstream archivo("Usuarios.txt", ios::app);
    
    archivo<<p1.GetName()<<';';
    archivo<<p1.GetMaze()<<':';
    archivo<<p1.GetTiempo()<<',';
    archivo<<p1.GetCoordx()<<'*';
    archivo<<p1.GetCoordy()<<endl;
    
    archivo.close();
}

//CLASE TOPTEN
TopTen::TopTen(){
    this->pInicio = NULL;
}

int TopTen::Size(){
    int longitud=0;
    lista *salto = this->pInicio;
    while(salto!=NULL){
        longitud++;
        salto = salto->sig;
    }
    return longitud;
}

void TopTen::Mostrar(){
    lista *aux = pInicio;
    while(aux!=NULL){
        cout<<endl<<"****Informacion del Jugador****"<<endl;
        cout<<"Nombre: "<<aux->dato.GetName()<<endl;
        cout<<"Laberinto: "<<aux->dato.GetMaze()<<endl;
        cout<<"Pasos: "<<aux->dato.GetTiempo()<<endl;
        //cout<<"Coordenada x: "<<aux->dato.GetCoordx()<<endl;
        //cout<<"Coordenada y: "<<aux->dato.GetCoordy()<<endl;
        aux = aux->sig;
    }
}

void TopTen::InsertarPlayer(Player p1){
    lista *nuevo = new lista;
    lista *salto =  new lista;
    nuevo->dato = p1;
    
    if(pInicio==NULL){
        nuevo->sig=NULL;
        pInicio = nuevo;
    }
    else{
        salto = pInicio;
        while(salto->sig)
            salto = salto->sig;
        salto->sig = nuevo;
        nuevo->sig = NULL;
    }
}

void TopTen::GuardarTopTen(){
    lista *aux = pInicio;
    
    ofstream archivo("Usuarios.txt", ios::trunc);
    while(aux!=NULL){
        
        archivo<<aux->dato.GetName()<<';';
        archivo<<aux->dato.GetMaze()<<':';
        archivo<<aux->dato.GetTiempo()<<',';
        archivo<<aux->dato.GetCoordx()<<'*';
        archivo<<aux->dato.GetCoordy()<<endl;
        aux = aux->sig;
    }
    archivo.close();
}

int TopTen::NumPlayer(){
    ifstream archivo;
    string temp;
    int cont=0;
    
    archivo.open("Usuarios.txt",ios::in);
    while(getline(archivo,temp)){
        cont= cont+1;
    }
    archivo.close();
    
    return cont;
}

void TopTen::OrdenarPasos(int num[], int n){
    int i,j,aux;
    //int num[];
    
    for(i=0;i<n;i++){
        for(j=0;j<(n-1);j++){
            if(num[j] > num[j+1]){
                aux = num[j];
                num[j] = num[j+1];
                num[j+1] = aux;
            }
        }
    }      
}

void TopTen::OrdenarNombres(string nom[], int n){
    int i,j;
    string aux;
    
    for(i=0;i<n;i++){
        for(j=0;j<(n-1);j++){
            if(nom[j] > nom[j+1]){
                aux = nom[j];
                nom[j] = nom[j+1];
                nom[j+1] = aux;
            }
        }
    }         
}

void TopTen::LeerTopTenNombres(){
    ifstream archivo;
    string linea;
    Player p;
    archivo.open("Usuarios.txt",ios::in);
    
   
    //declaramos arreglos
    int cont = this->NumPlayer();
    string nombres[cont];
    
    int i=0;
    while(getline(archivo, linea)){
        int pos[3] = {linea.find(';'), linea.find(':'), linea.find(',')};
        
        int size[3] = {pos[0], (pos[2]-pos[1]-1)};
        string cadena[2] = {linea.substr(0,size[0]),//Nombre
                            linea.substr((pos[0]+1), size[1])}; //Pasos
        
        nombres[i] = cadena[0];
        i=i+1;
    }
    

    this->OrdenarNombres(nombres,cont);
    
    archivo.clear();
    archivo.seekg(0,archivo.beg);
    
    for(int i = 0; i<cont; i++){
        while(getline(archivo, linea)){
            int pos[5] = {linea.find(';'), linea.find(':'), linea.find(','), linea.find('*'), linea.length()};
            int size[5] = {pos[0], (pos[1]-pos[0]-1), (pos[2]-pos[1]-1), (pos[3]-pos[2]-1),(pos[4]-pos[3]-1)};
            string cadena[5] = {linea.substr(0,size[0]),
                                   linea.substr((pos[0]+1), size[1]),
                                   linea.substr((pos[1]+1), size[2]),
                                   linea.substr((pos[2]+1), size[3]),
                                   linea.substr((pos[3]+1), size[4])};
            if(nombres[i]== cadena[0]){
                p.InicializarJugador(cadena[0], cadena[1], atoi(cadena[2].c_str()), atoi(cadena[3].c_str()), atoi(cadena[4].c_str()));
                //cout << p.GetName();
                //cout << ".I.";
                this->InsertarPlayer(p);
                archivo.clear();
                archivo.seekg(0,archivo.beg);
                break;
            }
        }
    }
    archivo.close();
}

void TopTen::LeerTopTenPasos(){
    ifstream archivo;
    string linea;
    Player p;
    archivo.open("Usuarios.txt",ios::in);
    
   
    //declaramos arreglos
    int cont = this->NumPlayer();
    int pasos[cont];
    
    int i=0;
    while(getline(archivo, linea)){
        int pos[3] = {linea.find(';'), linea.find(':'), linea.find(',')};
        
        int size[3] = {pos[0], (pos[2]-pos[1]-1)};
        string cadena[2] = {linea.substr(0,size[0]),//Nombre
                            linea.substr((pos[1]+1), size[1])}; //Pasos
        cout<<cadena[1];
        pasos[i] = atoi(cadena[1].c_str());
        i=i+1;
    }
    cout<<pasos[cont];

    this->OrdenarPasos(pasos,cont);
    
    archivo.clear();
    archivo.seekg(0,archivo.beg);
    
    for(int i = 0; i<cont; i++){
        //cout<<"hola";
        while(getline(archivo, linea)){
            
            int pos[5] = {linea.find(';'), linea.find(':'), linea.find(','), linea.find('*'), linea.length()};
            int size[5] = {pos[0], (pos[1]-pos[0]-1), (pos[2]-pos[1]-1), (pos[3]-pos[2]-1),(pos[4]-pos[3]-1)};
            string cadena[5] = {linea.substr(0,size[0]),
                                   linea.substr((pos[0]+1), size[1]),
                                   linea.substr((pos[1]+1), size[2]),
                                   linea.substr((pos[2]+1), size[3]),
                                   linea.substr((pos[3]+1), size[4])};
            cout<<pasos[5];
            if(pasos[i] == atoi(cadena[2].c_str())){
                //cout<<"hola2";
                p.InicializarJugador(cadena[0], cadena[1], atoi(cadena[2].c_str()), atoi(cadena[3].c_str()), atoi(cadena[4].c_str()));
                this->InsertarPlayer(p);
                archivo.clear();
                archivo.seekg(0,archivo.beg);
                break;
            }
        }
    }
    archivo.close();
}

void TopTen::BorrarListaaux(){
    if(pInicio){
        BorrarLista(pInicio);
        pInicio = NULL;
    }
}

void TopTen::BorrarLista(nodo *p){
    if(p->sig){
        BorrarLista(p->sig);
    delete(p);
    }
}
