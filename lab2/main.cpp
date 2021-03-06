/*** LABARTORIO 2 EDA, Integrantes: Julio Arrieta, Mariano Castro y Guillermo Moreira **/
#include <iostream>
#include <stdio.h>
#include <cstring>
#include <string>

using namespace std;

enum _retorno{OK, ERROR, NO_IMPLEMENTADA};
typedef _retorno TipoRet;

struct nodoListaArg{
    int pos;
    string info;
    nodoListaArg * ant;
    nodoListaArg * sig;
};
typedef nodoListaArg * ListaArg;

struct nodoListaCelda{
    int nroCelda;
    string info;
    nodoListaCelda * ant;
    nodoListaCelda * sig;
};
typedef nodoListaCelda * ListaCelda;

struct nodoListaColum{
    string nombre;
    bool PK;
    int nroColum;
    nodoListaColum * ant;
    nodoListaColum * sig;
};
typedef nodoListaColum * ListaColum;

struct  nodoListaTupla{
    int indice;
    ListaCelda celda;
    nodoListaTupla * ant;
    nodoListaTupla * sig;
};
typedef nodoListaTupla * ListaTupla;

struct nodoABBTabla{
    string nombre;
    int cantColumnas;
    ListaColum columna;
    ListaTupla tupla;
    nodoABBTabla * padre;
    nodoABBTabla * izq;
    nodoABBTabla * der;
};
typedef nodoABBTabla * ABBTabla;

//* Funciones para tablas (ABBTabla) **//
ABBTabla crearNodoABBTabla(string nombre); //Crea una nueva tabla con el nombre recibido
ABBTabla obtenerPosicion(ABBTabla &t, string nombre);//Recorre en orden y devuelve el puntero a un elemento dado
void imprimirArbol( ABBTabla t); //Recorre en orden y muestra los elementos en pantalla
bool miembro(string x, ABBTabla t); //Chequea si un elemento pertenece al ABBTabla
void insertDer(ABBTabla &t, string nombre);//Inserta un nodo a la derecha
void insertIzq(ABBTabla &t, string nombre);//Inserta un nodo a la izquierda
void insertNodoTabla(ABBTabla &t, string nombre);
void borrarNodoTabla(ABBTabla nodoEliminar); //Eliminar el nodo (tabla) recibido del arbol
ABBTabla obtenerMinimo(ABBTabla t);//retorna el nodo mas a la izquierda
void reemplazar(ABBTabla nodo1, ABBTabla nodo2);//Funcion para reemplazar el nodo1 por el nodo2 **/
ABBTabla traerNodoTabla(string nombre, ABBTabla &t);//Busca el nodo por el nombre y lo retorna lo retorna NULL si no lo encuentra

//**** GLOBALES *****//
ABBTabla t = NULL;

//** OPERACIONES ***/
TipoRet createTable(string nombreTabla);
TipoRet dropTable(string nombreTabla);
TipoRet addCol(string nombreTabla, string nombreCol);
TipoRet dropCol(string nombreTabla, string nombreCol);
TipoRet insertInto(string nombreTabla, string valoresTupla);
TipoRet deleteFrom(string nombreTabla, string condicionEliminar);
TipoRet update(string nombreTabla, string condicionModificar, string columnaModificar, string valorModificar );
TipoRet printDataTable(string nombreTabla);
TipoRet selectWhere(string nombreTabla2, string condicion, string nombreTabla1);
TipoRet select(string nombreTabla2, string valoresColumnas, string nombreTabla1);
TipoRet join(string tabla1, string tabla2, string tabla3 );//Junta la tabla 1 con la 2 y forman la 3
TipoRet printTables();

/** FUNCIONES Y PROCEDIMIENTOS AUXILIARES */
void leerComando(ABBTabla t, string comando ); //Interpreta el comando de entrada
void mostrarAyuda(); // Imprime la Ayuda con los comandos validos
string traerParametro(ListaArg L, int n);  //Trae del a posicion n un valor tipo string de la lista de parametros
ListaArg crearListaArg();               //Crea una lista para guardar parametros
void imprimirArg(ListaArg L);           //Imprime los argumentos de la lista
void borrarListaArg(ListaArg &L);
void agregarArg(ListaArg L, string arg); //Agrega un nuevo parametro al final de la lista de parametros
void cargarListaArg(ListaArg L, string allArg, char separador);
int buscarColumna(ListaColum L, string nombreColum); //Busca una columna por su nombre y retorna su posicion
bool compararCelda(ListaCelda L, int nroCelda, char operador, string valor); //compara celdas y retorna un boolean
bool buscarTuplaPorPK(ListaTupla &sonda, string pk);//Recibe la pk y busca la tupla que lo tenga, si no esta devuelve NULL
bool agregarTuplaOrdenada(ListaTupla &auxTupla, string pk, ListaArg listaValores);//agrega una nueva tupla de forma ordenada por su pk
void agregarTuplaFinal(ListaTupla &auxTupla, ListaArg listaValores);    //agrega una nueva tupla al final de la lista
void agregarTuplaSiguiente(ListaTupla &auxTupla, ListaArg listaValores);//agrega la siguiente tupla a la posicion actual
void ordenarIndiceTupla(ListaTupla &auxTupla);     //Recorre todas las tuplas ordenando los indices cuando una es eliminada
void ordenarNroColum(ListaColum &auxColum);     //Recorre todas las columnas ordenando los nroColum cuando una columna es eliminada
void ordenarNroCelda(ListaCelda &auxCelda);    //Recorre todas las celadas de una tupla y ordenando los indices cuando una es eliminada
void cargarTupla(ListaTupla &auxTupla, ListaArg listaArg); //Llena un registro con la lista de argumentos recibidos en forma de lista
int lengthArg(ListaArg L);  //Obtiene el largo de una lista de parametros
char traerOperador( string condicion); //Recibe una condicion en forma de string y recupera el operador utilizado retornandolo como char
ListaTupla buscaTuplaValor(ListaTupla L, int cantColumnas, char operador, string valor); //retorna un puntero a la tupla buscada
void borrarTupla(ListaTupla &auxTupla); //Borra la tupla actual
void borrarColumna(ListaColum &auxColum, int nroColum); //Recibe el numero de la columna y la borra de lista de columnas(Nombres de campos)
void borrarCelda(ListaCelda LCelda, int nroCelda);
void borrarColumnasTabla(ListaColum &auxColum); //Borra todas las columnas de una tabla
void borrarCeldasTupla(ListaCelda &auxCelda); //borra todas las celdas
void borrarTuplasTabla(ListaTupla &auxTupla); //Borra todas las tuplas de una tabla
bool comienzaCon(string valor, string datoCelda); //Comprueba si el dato de una celda comienda con un determinado valor
void modificarCelda(ListaCelda &auxCelda, int nroCelda, string nuevoValor);
void copiarColumnas(ListaColum colum1, ListaColum colum2);//Copia los nombres de la colum1 en la colum2
void agregarColumna(ListaColum L, string nombre); //Agrega una columna al final de la lista, sin mensajes ni validaciones
void insertarReg(string nombreTabla, string valoresTupla);//agrega un nuevo registro
//Recibe una lista de celdas llena, un string vacio y la posicion desde la cual se van a copiar los datos. Retorna un string en este formato "valor1:valor2:valor3"
string traerDatosCeldas(ListaCelda L, string values, int pos);
bool columnasRepetidas(ListaColum L1, ListaColum L2);//Verifica si hay columnas repetidas entre las columnas de 2 tablas, sin contar las PK
void unirColumnas(ListaColum L1, ListaColum L2, int pos);//Une las columnas de la tabla2 con las columnas de la tabla1 (Los agrega al final de la tabla1 )
int cantColumnas(ListaColum L);//Cuenta la cantidad de columnas que tiene una tabla
string traerDato(ListaCelda LC, int pos);//Recibe la posicion y obtiene el dato de la celda indicada
void imprimirReg(ListaCelda LC);//imprime la lista de celdas que recibe

int main(){
    extern ABBTabla t;
    string comando;

    while(comando!="exit"){ //mantiene la terminal esperando ordenes
        getline(cin, comando);
        if( comando.empty()!=true )
            leerComando(t, comando);
        else
            cout << "> No ha ingresado ningun comando" <<endl;
    }
    cout<<endl<< "> Bye..." <<endl;

    return 0;
}

TipoRet createTable(string nombreTabla){
    TipoRet res = OK;
    extern ABBTabla t;               //ABBTabla Global
    ABBTabla auxTabla = t;
    if( miembro(nombreTabla,t) ){ //Si la tabla existe retorna error
        cout<<"> Ya existe una tabla de nombre "<<nombreTabla<<endl;
        res = ERROR;
        return res;
    }
    else{   //Si no existe crea el nuevo nodo tabla
        //obtenerPosicion(auxTabla, nombreTabla );//Obtiene la ubicacion para la nueva tabla segun su nombre
        insertNodoTabla(t, nombreTabla);//Recibe el nombre de una tabla y la crea en la posicion correcta
        return res;
    }
}

TipoRet dropTable(string nombreTabla){
    TipoRet res = OK;
    extern ABBTabla t;               //Variable tipo listaTabla Global
    ABBTabla auxTabla = t;
    ListaColum auxColum, borrarColum;
    ListaTupla auxTupla, borrarTupla;
    ListaCelda auxCelda, borrarCelda;
    if( traerNodoTabla(nombreTabla, auxTabla)!=NULL ){ //Si la tabla existe la trae y si no retorna NULL

            auxColum = auxTabla->columna;     //Apunta a las columnas a eliminar
            borrarColumnasTabla(auxColum); //Borra la lista de columnas de la tabla
            auxTupla = auxTabla->tupla; //Apunta al comienzo de las tuplas a borrar
            while( auxTupla != NULL ){//Recorro todas las tuplas y borro sus celdas
                auxCelda = auxTupla->celda; //Apunta a la lista de celdas de la tupla actual
                borrarCeldasTupla(auxCelda);//Elimina la lista de celdas de la tupla actual
                auxTupla = auxTupla->sig;   //Avanza a la siguiente tupla
            }
            auxTupla = auxTabla->tupla; //Apunta al comienzo de las tuplas a borrar
            borrarTuplasTabla(auxTupla);  //Borra todas la tuplas de una tabla
            //Por ultimo borra la tabla
            borrarNodoTabla(auxTabla);
    }
    res = ERROR;
    return res;
}

TipoRet addCol(string nombreTabla, string nombreCol){
    TipoRet res = NO_IMPLEMENTADA;
    extern ABBTabla t;               //ListaTabla Global
    ABBTabla auxTabla;
    ListaColum auxColum = NULL;
    ListaColum nuevaColum = NULL;
    auxTabla = traerNodoTabla(nombreTabla,t); //Si existe la tabla, se para apuntando sobre ella
    if( auxTabla!=NULL ){
        if( auxTabla->tupla->sig == NULL ){ // Chequea que la tabla no tenga ningun registro cargado **/
            auxColum = auxTabla->columna; //Puntero auxiliar para recorrer las columnas
            while( auxColum->sig != NULL ){//Recorre la lista de columnas y cheque que no exista una columna con el mismo nombre
                auxColum = auxColum->sig;
                if( auxColum->nombre == nombreCol ){
                    cout<<"> ¡Operacion no valida!. Ya existe una columna llamada \""<<nombreCol<<"\""<<endl;
                    res = ERROR;
                    return res;
                }
            }
            //Si no se encontro ninguna columna con el mismo nombre:
            auxColum = auxTabla->columna;//Resetea el puntero a la lista columnas
            agregarColumna(auxColum, nombreCol);
            auxTabla->cantColumnas = cantColumnas(auxTabla->columna);
            res = OK;
            return res;
        }else{
            cout<<"> ¡Operacion no valida!. La tabla ya tiene registros cargados"<<endl;
            res = ERROR;
            return res;
        }

    }
    cout<<"> La tabla \""<<nombreTabla<<"\" no existe."<<endl;
    res = ERROR;
    return res;
}

TipoRet dropCol(string nombreTabla, string nombreCol){
    TipoRet res = NO_IMPLEMENTADA;
    int nroColBuscada;
    extern ABBTabla t;               //ListaTabla Global
    ABBTabla auxTabla;           //puntero auxiliar tipo ListaTabla
    ListaColum auxColum;
    ListaColum borraColum;
    ListaTupla auxTupla;
    ListaCelda auxCelda;
    ListaCelda borraCelda;
    /** Busca si existe la tabla **/
    auxTabla = traerNodoTabla(nombreTabla, t); //si la tabla existe devuelve el puntero a ella, si no el puntero es NULL
    if( auxTabla!=NULL ){
            auxColum = auxTabla->columna;
            if( buscarColumna(auxColum, nombreCol) == 1000 ){
                cout<<"> La columna \""<<nombreCol<<"\" no existe"<<endl;
                res = ERROR;
                return res;
            }
            while( auxColum->sig != NULL ){  //Recorre la lista de columnas y chequea que no exista una columna con el mismo nombre
                if( auxColum->sig->nombre == nombreCol ){
                    nroColBuscada = buscarColumna(auxColum, nombreCol); //Obtiene el numero de la columna, si no lo encuentra retorna 1000
                    if( auxColum->sig->PK==true && auxTabla->cantColumnas == 1 ){
                        auxTupla = auxTabla->tupla;
                        while( auxTupla->sig != NULL ){
                            ListaTupla borrar = auxTupla->sig;
                            auxTupla->sig = borrar->sig;
                            borrar->ant = NULL;
                            delete borrar;
                        }
                        ordenarIndiceTupla(auxTabla->tupla);
                    }
                    if( auxColum->sig->PK==true && auxTabla->cantColumnas>1 ){//si hay mas de una columna no se pude borrar la pk
                        cout<<"> La columna \""<<nombreCol<<"\" es Clave Primaria y hay otras columnas que se identifican por ella."<<endl;
                        res = ERROR;
                        return res;
                    }
                    else{
                        borrarColumna( auxColum, nroColBuscada); //Borra la columna
                        ordenarNroColum(auxTabla->columna);    //Ordena los numeros de las columnas
                        auxTabla->cantColumnas = cantColumnas(auxTabla->columna);
                        //auxTabla->cantColumnas--;         //Decrementa en uno la cantidad de columnas que tiene la tabla
                        auxTupla = auxTabla->tupla->sig;//arranca en la tupla 1
                        while( auxTupla != NULL ){ //Busca en todas las tuplas
                            auxCelda = auxTupla->celda;
                            borrarCelda(auxCelda, nroColBuscada);//borra la celda
                            ordenarNroCelda( auxTupla->celda ); //ordena el indice de la celda
                            auxTupla = auxTupla->sig;       //Avanza a la  siguiente tupla
                        }
                        cout<<"> Columna \""<<nombreCol<<"\" eliminada"<<endl;
                        res = OK;
                        return res;
                    }
                }
                auxColum = auxColum->sig;
            }
            cout<<"> La Columna \""<<nombreCol<<"\" no existe."<<endl;
            res = ERROR;
            return res;
    }else{  //Si la tabla no existe
        cout<<"> La tabla \""<<nombreTabla<<"\" no existe."<<endl;
        res = ERROR;
        return res;
    }
}

TipoRet insertInto(string nombreTabla, string valoresTupla){
    TipoRet res = OK;
    extern ABBTabla t;                           //ListaTabla Global
    ABBTabla auxTabla;
    ListaTupla auxTupla = NULL;
    ListaArg listaValores = crearListaArg();            //crea una lista de valores para recibir los argumentos
    cargarListaArg(listaValores, valoresTupla, ':');    //carga los valores en una lista
    auxTabla = traerNodoTabla(nombreTabla, t); //si la tabla existe devuelve el puntero a ella, si no el puntero es NULL
    if( auxTabla != NULL){
        if( auxTabla->cantColumnas== 0 ){
            cout<<"> La tabla no tiene columnas creadas"<<endl;
            res = ERROR;
            return res;
        }
        if( auxTabla->cantColumnas == lengthArg(listaValores) ){//Chequea si la cantidad de valores pasados es igual a los campos que tiene a tabla
            auxTupla = auxTabla->tupla;
            string pk = traerParametro(listaValores, 1); //obtiene la pk cursada
            if(agregarTuplaOrdenada(auxTupla, pk, listaValores)){  //Devuelve true si pudo insertar la tupla de forma ordenada
                cout<<"> Nuevo registro agregado con exito"<<endl;
                borrarListaArg(listaValores);
                return res;
            }else{
                cout<<"> No se pudo insertar el registro"<<endl;
                res = ERROR;
                borrarListaArg(listaValores);
                return res;
            }
        }
    }
    else{
        cout<<"> La tabla no existe"<<endl;
        res = ERROR;
        return res;
    }
}

TipoRet deleteFrom(string nombreTabla, string condicion){
    TipoRet res = OK;
    extern ABBTabla t;               //ListaTabla Global
    ABBTabla auxTabla;                //Tabla auxiliar
    ListaTupla auxTupla = NULL;
    ListaCelda auxCelda = NULL;
    string msjRespuesta;                    //Respuesta a retornar
    int regAfectados = 0;                //Cuenta la cantidad de elementos encontrados
    char operador = traerOperador(condicion); //obtiene el operador de la condicion a buscar
    ListaArg listaCondicion = crearListaArg(); //crea una lista con las condiciones
    cargarListaArg(listaCondicion, condicion, operador); // separa ambas partes de la condicion
    string nombreColumna    = traerParametro(listaCondicion,1);
    string valorFiltro   = traerParametro(listaCondicion,2);
    auxTabla = traerNodoTabla(nombreTabla, t); //si la tabla existe devuelve el puntero a ella, si no el puntero es NULL
    if( auxTabla != NULL){
        int nroColumna = buscarColumna(auxTabla->columna, nombreColumna); //si el nombre de la columna existe retorna su posicion, si no retorna 1000
        if( nroColumna != 1000 ){ //Chequea que exista la columna
            auxTupla = auxTabla->tupla->sig; //Puntero auxiliar para recorrer las tuplas
            bool encontrado = false;
            while( auxTupla!= NULL ){
                auxCelda = auxTupla->celda;
                if( operador!='-' ){
                    encontrado = compararCelda(auxCelda, nroColumna, operador, valorFiltro);
                    if( encontrado == true ){
                        cout<<"> Registro eliminado-> ";
                        imprimirReg(auxCelda);
                        borrarCeldasTupla(auxCelda);
                        borrarTupla(auxTupla);
                        regAfectados++; //Cuenata los registros encontrados
                    }
                }
                if( operador=='-' ){
                	cout<<"> Registro eliminado-> ";
                    imprimirReg(auxCelda);
                    borrarCeldasTupla(auxCelda);
                    borrarTupla(auxTupla);
                    regAfectados++;
                }
                auxTupla = auxTupla->sig;
            }
            if( auxTupla != NULL ){ //Orneda los indices de las tuplas (registros)
                ordenarIndiceTupla(auxTupla);
            }
            borrarListaArg(listaCondicion);
            if( regAfectados == 0 ) //Si no ecuentra el valor
                cout<<"> No existe ninguna celda con el valor \""<<valorFiltro;
            else
                cout<<"> Registros afectados "<<regAfectados;//Imprime la cantidad de registros afectados si encuentra alguno
            cout<<msjRespuesta<<endl; //Imprime en pantalla la respuesta
            return res;
        }
        else{
            cout<<"> No hay campo con ese nombre";
            borrarListaArg(listaCondicion);
            res = ERROR;
            return res;
        }

    }
}

TipoRet update(string nombreTabla, string condicionModificar, string columnaModificar, string valorModificar ){
    TipoRet res = OK;
    extern ABBTabla t;               //ListaTabla Global
    ABBTabla auxTabla;             //Tabla auxiliar
    ListaTupla auxTupla = NULL;
    ListaCelda auxCelda = NULL;
    int regAfectados   = 0;                //Cuenta los registros afectados
    char operador = traerOperador(condicionModificar); //obtiene el operador de la condicion a buscar
    ListaArg listaCondicion = crearListaArg();           //Crea una lista con las condiciones
    cargarListaArg(listaCondicion, condicionModificar, operador); // Separa ambas partes de la condicion
    string columnaFiltro = traerParametro(listaCondicion,1);// Nombre de la columna por la cual filtrar
    string valorFiltro   = traerParametro(listaCondicion,2); //Valor que debe cumplir el filtro
    auxTabla = traerNodoTabla(nombreTabla, t); //si la tabla existe devuelve el puntero a ella, si no el puntero es NULL
    if( auxTabla != NULL){
        int nroColumnaMod = buscarColumna(auxTabla->columna, columnaModificar); //si el nombre de la columna existe retorna su posicion, si no retorna 1000
        if( nroColumnaMod == 1 ){
            cout<<"> No se puede modificar la clave primaria"<<endl;
            return res = ERROR;
        }
        int nroColumFiltro = buscarColumna(auxTabla->columna, columnaFiltro);
        if( nroColumFiltro != 1000  && nroColumnaMod != 1000 ){ //Chequea que existan ambas columnas en la tabla
            auxTupla = auxTabla->tupla->sig; //Puntero auxiliar para recorrer las tuplas
            while( auxTupla!= NULL ){
                auxCelda = auxTupla->celda;
                if( operador != '-' ){
                    if( compararCelda( auxCelda, nroColumFiltro, operador, valorFiltro) ){
                        cout<<"> Registro modifcado-> "<< auxCelda->sig->info<<":"<< auxCelda->sig->sig->info<<":"<<auxCelda->sig->sig->sig->info<<endl;
                        modificarCelda( auxCelda, nroColumnaMod, valorModificar);
                        regAfectados++;
                    }
                }
                if( operador == '-' ){//Si no se escribe ninguna condicion se aplica para todos registros la acctualizacion
                    modificarCelda( auxCelda, nroColumnaMod, valorModificar);
                    regAfectados++;
                }
                auxTupla = auxTupla->sig;
            }
            borrarListaArg(listaCondicion);
            if( operador == '-')
                cout<<"> Actualizando todos los registros"<<endl;
            cout<<"> Registros afectados "<<regAfectados<<endl;
            return res;
        }
        else{
            cout<<"> No hay campo con ese nombre"<<endl;
            res = ERROR;
            borrarListaArg(listaCondicion);
            return res;
        }
    }
}

TipoRet printDataTable(string nombreTabla){
    TipoRet res = OK;
    extern ABBTabla t;               //ListaTabla Global
    ABBTabla auxTabla = traerNodoTabla(nombreTabla, t);
    if( auxTabla!=NULL ){
        cout<<"  Tabla "<<auxTabla->nombre<<endl;
        ListaColum auxColum = auxTabla->columna; //Puntero auxiliar para recorrer las columnas
        /** Recorre la lista campos */
        while( auxColum->sig != NULL ){ //Recorre la lista de columnas y chequea que no exista una columna con el mismo nombre
            auxColum = auxColum->sig;
            if( auxColum->ant->ant == NULL )
                cout<<"  "<< auxColum->nombre;
            if( auxColum->nroColum > 1 && auxColum->sig != NULL )
                cout<<":"<< auxColum->nombre;
            if( auxColum->sig == NULL && auxColum->nroColum > 1){
                cout<<":"<< auxColum->nombre <<endl;
                continue;
                    }
            if( auxColum->sig == NULL )
                        cout<<endl;
        }
        /** Recorre la lista tuplas (resgistros)*/
        ListaTupla auxTupla = auxTabla->tupla;   // puntero que apunta a la celda dummy de la lista tuplas
        ListaCelda auxCelda;
        auxCelda = NULL;
        while( auxTupla->sig != NULL ){         //Recorre la lista de tuplas
            auxTupla = auxTupla->sig;
            auxCelda = auxTupla->celda;  //Puntero que apunta a ListaCelda dummy de la tupla actual
            /** Recorre las celdas dentro de una tupla */
            while( auxCelda->sig != NULL ){
                auxCelda = auxCelda->sig;
                if( auxCelda->ant->ant == NULL )
                    cout<<"  "<< auxCelda->info;
                if( auxCelda->nroCelda > 1 && auxCelda->sig != NULL )
                    cout<<":"<< auxCelda->info;
                if( auxCelda->sig == NULL && auxCelda->nroCelda > 1){
                    cout<<":"<< auxCelda->info <<endl;
                    continue;
                }
                if( auxCelda->sig == NULL )
                    cout<<endl;

            }
        }
        return res;

    }
    cout<<"> La tabla \""<<nombreTabla<<"\" no existe."<<endl;
    res = ERROR;
    return res;
}

void mostrarAyuda(){
    cout <<endl<< "\tAYUDA DE COMANDOS: "<<endl;
    cout << "Nota: todos los comandos son 'case sensitive' "<<endl;
    cout << "* help: Imprimir ayuda" <<endl<<endl;
    cout << "* Operadores validos para condicines en consultas:"<<endl;
    cout << " (=) igual a ..."<<endl;
    cout << " (!) diferente a ..."<<endl;
    cout << " (*) comienza con ..."<<endl;
    cout << " (-) todo ..."<<endl<<endl;
    cout << "* createTable(nombreTabla): Crear tabla"<<endl;
    cout << " Ejem: crear la tabla Empleados"<<endl;
    cout << " Orden:  createTable(Empleados)"<<endl<<endl;
    cout << "* dropTable(nombreTabla): Eliminar tabla"<<endl;
    cout << " Ejem: eliminar la tabla Productos"<<endl;
    cout << " Orden: dropTable(Productos)"<<endl<<endl;
    cout << "* addCol(nombreTabla, nombreCol): Agregar columna"<<endl;
    cout << " Ejem: agregar columna Apellido en la tabla Empleados"<<endl;
    cout << " Orden: addCol(Empleados, Apellido)"<<endl<<endl;
    cout << "* dropCol(nombreTabla, nombreCol): Eliminar columna"<<endl;
    cout << " Ejem: elimina la columna idProyecto de la tabla Proyectos"<<endl;
    cout << " Orden: dropCol(Proyectos, idProyecto)"<<endl<<endl;
    cout << "* insertInto(nombreTabla, valoresTupla): Insertar registro"<<endl;
    cout << " Ejem: agregar a Telma Perez en la tabla Personas "<<endl;
    cout << " Orden: insertInto(Personas, 3333111: Telma: Perez)"<<endl<<endl;
    cout << "* deleteFrom(nombreTabla, condicion): Eliminar registro"<<endl;
    cout << " Ejem: elimina todos los registros de apellido Perez, de Personas"<<endl;
    cout << " Orden: deleteFrom(Personas, apellido=Perez)"<<endl<<endl;
    cout << "* update(nombreTabla, condicion, columna, valor): Atualizar tabla" <<endl;
    cout << " Ejem: modifica el apellido a Ramos a todos los de nombre Pepe"<<endl;
    cout << " Orden: update(Personas,Nombre=Pepe,apellido,Ramos)"<<endl<<endl;
    cout << "* printDataTable(Clientes): Muestrar contenido de la tabla"<<endl;
    cout << " Ejem: muestra el contenido de la tabla Clientes"<<endl;
    cout << " Orden: printDataTable(Clientes)"<<endl<<endl;
    cout << "* selectWhere(nombreTabla2,Condicion,nombreTable1): Copia tabla1 en tabla2"<<endl;
    cout << " Ejem: copia los registros con apellido Perez de Clientes a Clientes2"<<endl;
    cout << " Orden: selectWhere(Clientes2,Apellido=Perez,Clientes)"<<endl<<endl;
    cout << "* select(nombreTabla2,Columnas,nombreTable1): Copia columnas de tabla1 a tabla2"<<endl;
    cout << " Ejem: copia la PK con los campos apellido y direccion de Clientes a Clientes2"<<endl;
    cout << " Orden: select(Clientes2,ci:Apellido,Direccion,Clientes)"<<endl<<endl;
    cout << "* join(nombreTabla1,nombreTabla2,nombreTable3): junta tabla1 y tabla2"<<endl;
    cout << " Ejem: junta la tabla Empleados y Clientes en una nueva llamada EC"<<endl;
    cout << " Orden: join(Empleados,Clientes,EC)"<<endl<<endl;
    cout << "* printTables(): mostrar todas las tablas"<<endl;
    cout << " Ejem: muestra en pantalla todas las tablas "<<endl;
    cout << " Orden: printTables()"<<endl<<endl;
}

TipoRet selectWhere(string nombreTabla2, string condicion, string nombreTabla1){
    TipoRet res = NO_IMPLEMENTADA;
    extern ABBTabla t;
    ListaTupla auxTupla1 = NULL;
    ListaColum auxColum1 = NULL;
    ListaCelda auxCelda1 = NULL;
    char operador = traerOperador(condicion);           //obtiene el operador de la condicion a buscar
    ListaArg listaCondicion = crearListaArg();           //Crea una lista con las condiciones
    cargarListaArg(listaCondicion, condicion, operador); // Separa ambas partes de la condicion
    string columnaFiltro = traerParametro(listaCondicion,1);// Nombre de la columna por la cual filtrar
    string valorFiltro   = traerParametro(listaCondicion,2); //Valor que debe cumplir el filtro
    int regCopiados = 0;
    if( nombreTabla1.empty() ){  // Valida que el nombre de la tabla1 no este vacio
        cout<<"> Falta el nombre de la tabla a copiar"<<endl;
        res = ERROR;
        return res;
    }
    if( nombreTabla2.empty() ){ // Valida que el nombre de la tabla2 no este vacio
        cout<<"> Falta especificar el nombre de la nueva tabla"<<endl;
        res = ERROR;
        return res;
    }
    else{
        if( miembro( nombreTabla2,t ) ){ //Si el nombre de la tabl2 ya existe retorna error
            cout<<"> El nombre de la nueva tabla ya existe!"<<endl;
            res = ERROR;
            return res;
        }
    }
    ABBTabla auxTabla1 = traerNodoTabla(nombreTabla1,t); //Trae el nodo de la tabla1
    if( auxTabla1 != NULL ){ //Valida si la tabla1 existe
        auxColum1 = auxTabla1->columna;
        int nroColumna = buscarColumna(auxColum1, columnaFiltro); //Si la columna para hacer el filtro existe retorna su nro. y si no existe retorna 1000
        if( nroColumna != 1000 ){               //Chequea que exista la columna
            auxTupla1 = auxTabla1->tupla->sig;   //Puntero auxiliar para recorrer las tuplas
            bool encontrado = false;            //Esta variable pasa a true en dada tupla si encuentra un elemento que coincide
            bool tabla2Existe = false;          //Al encontrar el primer elemento crea la tabla2 y esta variable pasa a true
            while( auxTupla1 != NULL ){           //Busca en tupla por tupla
                encontrado = false;
                auxCelda1 = auxTupla1->celda;
                if( operador!='-' ) /*********************Copia solo los encontrados por el filtro *******************/
                    encontrado = compararCelda(auxCelda1, nroColumna, operador, valorFiltro);//Compara el contenido de la celda nroColumncon con el valorFiltro y retorna un bool
                if( operador=='-' ) /********************************** Copia toda la tabla *************************/
                    encontrado = true;
                /****************************************************************************************************/
                if( encontrado == true ){//Si encuentra en valor chequea si la tabla existe de lo contrario la crea
                    if( tabla2Existe == false ){
                        createTable(nombreTabla2);//Crea la tabla2
                        tabla2Existe = true;      //Se asegura que no se cree otra vez
                        ABBTabla auxTabla2 = traerNodoTabla(nombreTabla2, t);//trae la tabla2 recien creada
                        auxTabla2->cantColumnas = auxTabla1->cantColumnas;   //setea la misca cantidad de columnas que la tabla1
                        ListaColum auxColum2 = auxTabla2->columna;
                        copiarColumnas(auxColum1, auxColum2);              //Copia los nombres de las columnas de la tabla1 a la tabla2
                        string valores = traerDatosCeldas(auxCelda1,"", 1);
                        insertarReg(nombreTabla2,valores);
                        regCopiados++;
                    }
                    else{
                        string valores = traerDatosCeldas(auxCelda1,"", 1);
                        insertarReg(nombreTabla2,valores);
                        regCopiados++; //Cuenata los registros encontrados
                    }
                }
                auxTupla1 = auxTupla1->sig;  // Avanza a la siguiente tupla (registros)
            }
            borrarListaArg(listaCondicion);
            if( regCopiados == 0 ) //Si no ecuentra el valor
                cout<<"> No existe ninguna celda con el valor \""<<valorFiltro;
            else
                cout<<"> Registros copiados "<<regCopiados<<endl;//Imprime la cantidad de registros copiados si encuentra alguno
            res = OK;
            return res;
        }
        else{
            cout<<"> No hay campo con ese nombre";
            borrarListaArg(listaCondicion);
            res = ERROR;
            return res;
        }

    }
     //Si la tabla1 no existe retorna error
    cout<<"La tabla \""<<nombreTabla1<<"\" no existe"<<endl;
    res = ERROR;
    return res;
}


TipoRet select(string nombreTabla2, string valoresColumnas, string nombreTabla1){
    TipoRet res = OK;
    extern ABBTabla t;
    int pos = 0;
    int regCopiados = 0;
    string values;
    if( miembro( nombreTabla2,t ) ){ //Si el nombre de la tabl2 ya existe retorna error
        cout<<"> El nombre de la nueva tabla ya existe!" <<endl;
        res = ERROR;
        return res;
    }
    ABBTabla auxTabla1 = traerNodoTabla(nombreTabla1, t);
    ListaColum auxColum1 = auxTabla1->columna;
    ListaArg listaColumnas = crearListaArg();
    cargarListaArg(listaColumnas, valoresColumnas,':');//Guarda los nombres de las columnas en la lista
    int cantCampos = lengthArg(listaColumnas);
    for(int i=1; i<=cantCampos; i++){ //Se chequean que todos los nombres de las columnas recibidos, existen en la tabla1
        if( buscarColumna(auxColum1, traerParametro(listaColumnas, i)) == 1000 ){ //Si retorno mil significa que no lo encontro
            cout<<"> La columna \""<<traerParametro(listaColumnas,i)<<"\" no existe en la tabla \""<<nombreTabla1<<"\""<<endl;
            res = ERROR;
            return res;
        }
    }
    size_t posPK = valoresColumnas.find(auxColum1->sig->nombre);
    if ( posPK == std::string::npos ){//Verifica si el usuario ingreso la columna PK, si no lo hizo la agrega al inicio
        valoresColumnas = auxColum1->sig->nombre+":"+valoresColumnas;
        borrarListaArg(listaColumnas);
        cargarListaArg(listaColumnas, valoresColumnas,':');
    }
    //Verifica si el fomato de las columnas es el correcto, con la pk como primer columna
    if( traerParametro(listaColumnas,1) != auxColum1->sig->nombre ){
        res = ERROR;
        cout <<"  El orden de las columnas no es valido \"" << valoresColumnas << "\""<<endl;
        return res;
    }
    //Se crea la tabla2
    createTable( nombreTabla2);
    ABBTabla auxTabla2 = traerNodoTabla(nombreTabla2, t);//trae el puntero a la tabla2
    ListaTupla auxTupla2 = auxTabla2->tupla;
    ListaColum auxColum2 = auxTabla2->columna;
    cantCampos = lengthArg(listaColumnas);//Obtiene nuevamente la cantidad de capos, por si se le agrego la pk
    for(int i=1; i<=cantCampos; i++ ){ //Crea todos los campos en la tabla 2
        agregarColumna(auxColum2, traerParametro(listaColumnas,i));
    }
    auxTabla2->cantColumnas = cantColumnas(auxTabla2->columna); //Cuenta las columnas creadas y actuliza el valor en la tabla2
    ListaCelda auxCelda2 = NULL;
    ListaTupla auxTupla1 = auxTabla1->tupla->sig;
    ListaCelda auxCelda1 = NULL;
    //***Recorre todos los registros de la tabla 1, para llenar la nueva tabla con las columnas seleccionadas **********
    while( auxTupla1 != NULL ){
        auxCelda1 = auxTupla1->celda->sig; //Apunta a la primer celda del registro actual

        for(int i=1; i <= lengthArg(listaColumnas) ; i++ ){
            pos = buscarColumna(auxColum1 ,traerParametro(listaColumnas, i));
            if( i == 1 ){
                values = traerDato(auxCelda1, pos);
                continue;
            }
            values = values+":"+traerDato(auxCelda1, pos);
        }
        insertarReg(nombreTabla2, values); //Inserta un nuevo registro en la tabla 2
        regCopiados++;
        auxTupla1 = auxTupla1->sig; //Avanza de registro en la tabla 1
    }
    borrarListaArg(listaColumnas);//Borra toda la lista;
    cout<<"> Registros copiados "<<regCopiados<<endl;//Imprime la cantidad de registros copiados si encuentra alguno
    res = OK;
    return res;
}


TipoRet join(string tabla1, string tabla2, string tabla3 ){//Junta la tabla 1 con la 2 y forman la 3
    TipoRet res = NO_IMPLEMENTADA;
    extern ABBTabla t;
    ListaTupla auxTupla1 = NULL;
    ListaColum auxColum1 = NULL;
    ListaCelda auxCelda1 = NULL;
    ListaTupla auxTupla2 = NULL;
    ListaColum auxColum2 = NULL;
    ListaCelda auxCelda2 = NULL;
    int regCopiados = 0;
    string valorPK;
    string datos;
    if( miembro( tabla3, t)){ //Si la tabla tres ya existe retorna error
        cout<<"> La tabla "<<tabla3<<" ya existe"<<endl;
        return ( res = ERROR );
    }
    ABBTabla auxTabla1 = traerNodoTabla(tabla1,t); //Trae el nodo de la tabla1
    ABBTabla auxTabla2 = traerNodoTabla(tabla2,t); //Trae el nodo de la tabla2
    if( auxTabla1==NULL ){
        cout<<"> La tabla "<<tabla1<<" no existe"<<endl;
        return ( res = ERROR );
    }
    if( auxTabla2==NULL ){
        cout<<"> La tabla "<<tabla2<<" no existe"<<endl;
        return ( res = ERROR );
    }
    if( auxTabla1->cantColumnas < 1 || auxTabla2->cantColumnas < 1 ){//Valida que la tabla1 y 2 tengan almenos un campo
        cout<<"> No hay columnas suficientes"<<endl;
        return ( res = ERROR );
    }
    if( auxTabla1->columna->sig->nombre.compare(auxTabla2->columna->sig->nombre) != 0 ){//Valida que la tabla1 y 2 tengan almenos un campo
        cout<<"> Las claves primarias no concuerdan"<<endl;
        return ( res = ERROR );
    }
    auxColum1 = auxTabla1->columna;
    auxColum2 = auxTabla2->columna;
    if( columnasRepetidas(auxColum1, auxColum2) ){ //Verifica si hay columnas repetidas en las tablas, sin contar las pk
        cout<<"> Existen columnas repetidas en las tablas"<<endl;
        return ( res = ERROR );
    }
    createTable( tabla3 );                          //Crea la nueva tabla;
    ABBTabla auxTabla3 = traerNodoTabla(tabla3 , t);  //Trae la tabla3
    ListaColum auxColum3 = auxTabla3->columna;
    unirColumnas(auxColum3, auxColum1, 1);         //Une todos los campos de la tabla1 en la nueva tabla (tabla3)
    unirColumnas(auxColum3, auxColum2, 2);         //Une los campos desde la columna 2 de la tabla2 en la nueva tabla (tabla3)
    auxTabla3->cantColumnas = cantColumnas(auxColum3); //Obtiene la cantidad de columnas
    /********** Recorre la tabla1 ********/
    auxTupla1 = auxTabla1->tupla->sig;    //Reinicia el puntero
    while( auxTupla1 != NULL ){
        auxCelda1 = auxTupla1->celda;
        auxTupla2 = auxTabla2->tupla->sig; //Reinicia el puntero
        valorPK =  auxCelda1->sig->info;
        /********** Recorre la tabla2 ********/
        bool encontrado = false;            //Esta variable pasa a true en dada tupla si encuentra un elemento que coincide
        while( auxTupla2 != NULL ){         //Busca en tupla por tupla en la tabla2 los registros en los que la pk concuerde con la tabla1
            encontrado = false;
            auxCelda2 = auxTupla2->celda;
            /*********************Copia solo los encontrados en la tabla2 *******************/
            encontrado = compararCelda(auxCelda2, 1, '=', valorPK);//Compara el contenido de la celda nroColumncon con el valorFiltro y retorna un bool
            if( encontrado == true ){//Si encuentra en valor chequea si la tabla existe de lo contrario la crea
                datos = traerDatosCeldas(auxCelda1,"", 1);
                datos += ":"+traerDatosCeldas(auxCelda2,"", 2);
                insertarReg(tabla3 ,datos);
                regCopiados++; //Cuenata los registros encontrados
            }
            auxTupla2 = auxTupla2->sig;  // Avanza a la siguiente tupla de la tabla2 (registros)
        }
        auxTupla1 = auxTupla1->sig;  // Avanza a la siguiente tupla de la tabla1 (registros)
    }
    cout<<"> Registros copiados "<<regCopiados<<endl;//Imprime la cantidad de registros copiados si encuentra alguno
    res = OK;
    return res;
}


TipoRet printTables(){
    extern ABBTabla t;
    if( t == NULL )
        cout<<" No hay datos"<<endl<<endl;
    else
        imprimirArbol(t);
}

/****************   LEE EL INGRESO DE LOS COMANDOS   ***********************/
void leerComando(ABBTabla t, string comando){
    TipoRet res;
    string caracter, nombreTabla, columnas, tabla1, tabla2, tabla3, condicion;
    int nroComas =0;
    int nroArg=0;
    for (int i=0; i<comando.length(); ++i){ // Borra todos los espacios en blanco, cuenta las comas y los dos puntos
        caracter = comando[i];
        if( caracter == " " ){
            comando.erase(i, 1);
            i--;
        }
        if( caracter =="," ){
            nroComas++;
        }
    }
    size_t posApertura = comando.find("(");      // posicion del parentesis de apertura
    string sentencia = comando.substr(0,posApertura); //setencia ingresada con espacios
    size_t posCierre = comando.find(")");        // posicion de parentesis de cierre
    string allArg = comando.substr(posApertura+1 ,(posCierre - posApertura -1)); // obtiene el contenido de los argumentos

    //Se cargan los argumentos recibidos en una lista
    ListaArg listaArg = crearListaArg();
    cargarListaArg(listaArg, allArg, ',');

    if( sentencia=="selectWhere" ){
        tabla1 = traerParametro(listaArg, 3);
        tabla2 = traerParametro(listaArg,1);
        condicion = traerParametro(listaArg, 2);
        if( tabla1.empty() ){  // Valida que el nombre de la tabla1 no este vacio
        	cout<<"> Falta el nombre de la tabla a copiar"<<endl;
        	res = ERROR;
    	}
    	if( tabla2.empty() ){ // Valida que el nombre de la tabla2 no este vacio
        	cout<<"> Falta especificar el nombre de la nueva tabla"<<endl;
        	res = ERROR;
    	}
    	if( condicion.empty() ){ // Valida que la condicion no este vacio
        	cout<<"> Falta especificar la condicion"<<endl;
        	res = ERROR;
    	}
    	if( !tabla2.empty() && !tabla2.empty() && !condicion.empty() ){
        	res = selectWhere(tabla2, condicion, tabla1);
    	}
        if( res == 0 )
            cout<< "> Query OK: Se copio la tabla \""<<tabla1<<"\" en la tabla \""<<tabla2<<"\""<<endl<<endl;
        if( res == 1)
            cout<< "> Query ERROR: Al copiar la tabla"<<endl<<endl;
        return;
    }

    if( sentencia=="select" ){ //createTable( nombreTabla )
        tabla1 = traerParametro(listaArg,3);
        tabla2 = traerParametro(listaArg,1);
        columnas = traerParametro(listaArg,2);
        if( columnas.empty() ){  // Valida qque no este vacio los valores de las columnas
            cout<<"> No se indico ninguna columna"<<endl;
            res = ERROR;
        }
        if( tabla1.empty() ){  // Valida que el nombre de la tabla1 no este vacio
            cout<<"> Falta el nombre de la tabla a copiar"<<endl;
            res = ERROR;
        }
        if( tabla2.empty() ){ // Valida que el nombre de la tabla2 no este vacio
            cout<<"> Falta especificar el nombre de la nueva tabla"<<endl;
            res = ERROR;
        }
        if( !tabla1.empty() && !tabla2.empty() && !columnas.empty()  ){
            res = select(tabla2, columnas, tabla1);
        }
        if( res == 0 )
            cout<< "> Query OK: Se copio la tabla \""<<tabla1<<"\" en la tabla \""<<tabla2<<"\""<<endl<<endl;
        if( res == 1)
            cout<< "> Query ERROR: Al copiar la tabla \""<<tabla1<<"\""<<endl<<endl;
        return;
    }

    if( sentencia=="join" ){
        tabla1 = traerParametro(listaArg,1);
        tabla2 = traerParametro(listaArg,2);
        tabla3 = traerParametro(listaArg,3);
        if( tabla1.empty() ){  // Valida que el nombre de la tabla1 no este vacio
        	cout<<"> Falta el nombre de la primer tabla a juntar"<<endl;
        	res = ERROR;
    	}
    	if( tabla2.empty() ){ // Valida que el nombre de la tabla2 no este vacio
        	cout<<"> Falta el nombre de la segunda tabla a juntar"<<endl;
        	res = ERROR;
    	}
    	if( tabla3.empty() ){ // Valida que la condicion no este vacio
        	cout<<"> Falta el nombre de la nueva tabla"<<endl;
        	res = ERROR;
    	}
    	if( !tabla1.empty() && !tabla2.empty() && !tabla3.empty() ){
        	res = join(tabla1, tabla2, tabla3);
    	}
        if( res == 0 )
            cout<< "> Query OK: Tabla \""<<tabla1<<"\" se junto con \""<<tabla2<<"\""<<endl<<endl;
        if( res == 1)
            cout<< "> Query ERROR: Al crear juntar las tablas \""<<tabla1<<"\" y \""<<tabla2<<"\""<<endl<<endl;
        return;
    }

    if( sentencia=="createTable" ){
        nombreTabla = traerParametro(listaArg,1);
        if( nombreTabla.empty() ){  // Valida que el nombre de la tabla no este vacio
            cout<<"> Falta el nombre de la tabla a crear"<<endl;
            res = ERROR;
        }
        if( !nombreTabla.empty() ){
            res = createTable(nombreTabla);
        }
        if( res == 0 )
            cout<< "> Query OK: Se creo la tabla \""<<nombreTabla<<"\""<<endl<<endl;
        if( res == 1)
            cout<< "> Query ERROR: No se creo la tabla"<<endl<<endl;
        return;
    }

    if( sentencia=="printTables" ){
        res = printTables();
        if( res == 0 )
            cout<< "> Query OK"<<endl<<endl;
    }

    if( sentencia=="dropTable" ){ // dropTable( nombreTabla )
        nombreTabla = traerParametro(listaArg,1);
        if( nombreTabla.empty() ){  // Valida que el nombre de la tabla no este vacio
            cout<<"> Falta el nombre de la tabla"<<endl;
            res = ERROR;
        }
        if( !nombreTabla.empty() ){  // Valida que el nombre de la columna no este vacio
            res = dropTable(nombreTabla);
        }
        if( res == 0 )
            cout<< "> Query OK: La Tabla \""<<nombreTabla<<"\" fue eliminada"<<endl<<endl;
        if( res == 1 )
            cout<< "> Query ERROR: La tabla \""<<nombreTabla<<"\" no existe"<<endl<<endl;
        return;
    }

    if( sentencia == "addCol" ) {//addCol( nombreTabla, nombreCol )
        nombreTabla = traerParametro(listaArg,1);
        string nombreColumna = traerParametro(listaArg, 2);//obtiene e nombre de la columna ha agregar
        if( nombreTabla.empty() ){  // Valida que el nombre de la tabla no este vacio
            cout<<"> Falta el nombre de la tabla"<<endl;
            res = ERROR;
        }
        if( nombreColumna.empty() ){  // Valida que el nombre de la columna no este vacio
            cout<<"> Falta el nombre de la columna"<<endl;
            res = ERROR;
        }
        if( !nombreTabla.empty() && !nombreColumna.empty() ){  // Valida que el nombre de la columna no este vacio
            res = addCol(nombreTabla, nombreColumna);
        }
        if(  res == 0 )
            cout<< "> Query OK: La columna \""<<nombreColumna<<"\" ha sido creada"<<endl<<endl;
        if( res == 1)
            cout<< "> Query ERROR: No se pudo crear la columna"<<endl<<endl;
        if( res == 2 )
            cout<< "> Query ERROR: No se realizo la operacion"<<endl<<endl;
        return;
    }

    if( sentencia == "dropCol" ){ //   dropCol( nombreTabla, nombreCol)
        nombreTabla = traerParametro(listaArg,1);
        string nombreColumna = traerParametro(listaArg, 2);
        if( nombreTabla.empty() ){  // Valida que el nombre de la tabla no este vacio
            cout<<"> Falta el nombre de la tabla a crear"<<endl;
            res = ERROR;
        }
        if( nombreColumna.empty() ){  // Valida que el nombre de la tabla no este vacio
            cout<<"> Falta el nombre de la columna a eliminar"<<endl;
            res = ERROR;
        }
        if( !nombreTabla.empty() && !nombreColumna.empty() ){
            res = dropCol(nombreTabla, nombreColumna);
        }
        if(  res == 0 )
            cout<< "> Query OK: La columna \""<<nombreColumna<<"\" ha sido eliminada"<<endl<<endl;
        if( res == 1)
            cout<< "> Query ERROR: No se pudo eliminar la columna"<<endl<<endl;
        if( res == 2 )
            cout<< "> Query ERROR: No se realizo la operacion"<<endl<<endl;
        return;
    }

    if( sentencia == "insertInto" ){// insertInto( nombreTabla,valoresTupla")
        nombreTabla = traerParametro(listaArg,1);
        string valoresTupla = traerParametro(listaArg, 2);
        if( nombreTabla.empty() ){  // Valida que el nombre de la tabla no este vacio
            cout<<"> Falta el nombre de la tabla a crear"<<endl;
            res = ERROR;
        }
        if( valoresTupla.empty() ){  // Valida que el nombre de la tabla no este vacio
            cout<<"> No agrego ningun valor"<<endl;
            res = ERROR;
        }
        if( !nombreTabla.empty() && !valoresTupla.empty() ){
            res = insertInto(nombreTabla, valoresTupla);
        }
        if(  res == 0 )
            cout<< "> Query OK: Nuevo resitro en la tabla \""<<nombreTabla<<"\""<<endl<<endl;
        if( res == 1)
            cout<< "> Query ERROR: No se puedo agregar el registro en la tabla \""<<nombreTabla<<"\""<<endl<<endl;
        if( res == 2 )
            cout<< "> Query ERROR: No se realizo la operacion"<<endl<<endl;
        return;

    }

    if( sentencia == "deleteFrom" ){ //deleteFrom( nombreTabla, condicionEliminar )
        nombreTabla = traerParametro(listaArg,1);
        string condicionEliminar = traerParametro(listaArg, 2);
        if( nombreTabla.empty() ){  // Valida que el nombre de la tabla no este vacio
            cout<<"> Falta el nombre de la tabla"<<endl;
            res = ERROR;
        }
        if( condicionEliminar.empty() ){  // Valida que el nombre de la tabla no este vacio
            cout<<"> No agrego ninguna condicion valida"<<endl;
            cout<<"> Puede utilizar la ayuda escrbiendo la palabra 'help'"<<endl;
            res = ERROR;
        }
        if( !nombreTabla.empty() && !condicionEliminar.empty() ){
            res = deleteFrom(nombreTabla, condicionEliminar);
        }
        if(  res == 0 )
            cout<< "> Query OK "<<endl<<endl;
        if( res == 1)
            cout<< "> Query ERROR: Se produjo un error al intentar eliminar registros"<<endl<<endl;
        if( res == 2 )
            cout<< "> Query ERROR: No se realizo la operacion"<<endl<<endl;
        return;

    }
    if( sentencia == "update" ){ // update( nombreTabla, condicionModificar, columnaModificar, valorModificar)
        nombreTabla = traerParametro(listaArg,1);
        string condModificar = traerParametro(listaArg, 2);
        string columna = traerParametro(listaArg, 3);
        string valoModificar = traerParametro(listaArg, 4);
        if( nombreTabla.empty() ){  // Valida que el nombre de la tabla no este vacio
            cout<<"> Falta el nombre de la tabla"<<endl;
            res = ERROR;
        }
        if( condModificar.empty() ){  // Valida que el nombre de la tabla no este vacio
            cout<<"> No agrego ninguna condicion valida"<<endl;
            cout<<"> Puede utilizar la ayuda escrbiendo la palabra 'help'"<<endl;
            res = ERROR;
        }
        if( valoModificar.empty() ){  // Valida que el nombre de la tabla no este vacio
            cout<<"> No especifico el nuevo valor"<<endl;
            res = ERROR;
        }
        if( columna.empty() ){  // Valida que el nombre de la tabla no este vacio
            cout<<"> No especifico la columna a actulizar"<<endl;
            res = ERROR;
        }
        if( !nombreTabla.empty() && !condModificar.empty() && !columna.empty() && !valoModificar.empty() ){
            res = update(nombreTabla, condModificar, columna, valoModificar);
        }
        if(  res == 0 )
            cout<< "> Query OK"<<endl<<endl;
        if( res == 1)
            cout<< "> Query ERROR: Error al actulizar el registro"<<endl<<endl;
        if( res == 2 )
            cout<< "> Query ERROR: No se realizo la operacion"<<endl<<endl;
        return;
    }

    if( sentencia == "printDataTable" ){  // printDataTable( nombreTabla );
        nombreTabla = traerParametro(listaArg,1);
        res = printDataTable(nombreTabla);
        if( res == 0 )
            cout<< "> Query OK"<<endl<<endl;
        if( res == 1 )
            cout<< "> Query ERROR: No se pudo mostrar la tabla"<<endl<<endl;
        return;
    }
    if( sentencia == "help"  )//  mostrarAyuda()
        mostrarAyuda();
    if( sentencia!="join" && sentencia!="printTables" && sentencia!="select" && sentencia!="selectWhere" && sentencia!="createTable" && sentencia!="dropTable" && sentencia!="addCol" && sentencia!="dropCol" && sentencia!="insertInto" && sentencia!="deleteFrom" && sentencia!="update" && sentencia!="printDataTable" && sentencia!="help" && sentencia!="exit" )
        cout << "> ¡EL comando '" << comando <<"' no es valido!"<<endl<<endl;

}

//Obtiene el parametro en la posicion n de la lista. Nota: debe recibir un n valido
string traerParametro(ListaArg L, int n){
    string res="";
    if( L == NULL )
        return res;
    if( L!=NULL && L->pos!=n )
        return traerParametro(L->sig, n);
    if( L!=NULL && L->pos == n )
        return L->info;
}

void borrarTupla(ListaTupla &auxTupla){
    if( auxTupla == NULL )
        return;
    if( auxTupla->ant != NULL ){
        ListaTupla borrar = auxTupla;
        auxTupla = auxTupla->ant;
        auxTupla->sig = borrar->sig;
        if( borrar->sig!= NULL){
            borrar->sig->ant = borrar->ant;
        }
        delete borrar;
    }
}

void borrarColumna(ListaColum &auxColum, int nroColum){
    if( auxColum->sig != NULL && auxColum->sig->nroColum != nroColum)
        borrarColumna(auxColum->sig, nroColum);
    if( auxColum->sig != NULL && auxColum->sig->nroColum == nroColum){
        ListaColum borra = auxColum->sig;
        auxColum->sig = borra->sig;
        if( borra->sig != NULL )
            borra->sig->ant = auxColum;
        delete borra;
    }
}

void borrarCelda(ListaCelda LCelda, int nroCelda){
    if( LCelda->sig != NULL && LCelda->sig->nroCelda != nroCelda)
        borrarCelda(LCelda->sig, nroCelda);
    if( LCelda->sig != NULL && LCelda->sig->nroCelda == nroCelda){
        ListaCelda borra = LCelda->sig;
        LCelda->sig = borra->sig;
        if( borra->sig != NULL )
            borra->sig->ant = LCelda;
        delete borra;
    }
}

void borrarCeldasTupla(ListaCelda &auxCelda){//borra todas las celdas
    if( auxCelda==NULL)
        return;
    if( auxCelda->sig==NULL ){
        ListaCelda borrar = auxCelda;
        auxCelda = NULL;
        delete borrar;
        return;
    }else{
        ListaCelda borrar = auxCelda->sig;
        auxCelda->sig = borrar->sig;
        delete borrar;
        borrarCeldasTupla(auxCelda->sig);
    }
}

/******/
ListaTupla buscaTuplaValor(ListaTupla L, int nroColumna, char operador, string valor){
    if( L != NULL ){
        if( compararCelda(L->celda, nroColumna, operador, valor) ){
            return L;
        }else{
            return buscaTuplaValor(L->sig, nroColumna, operador, valor);
        }
    }
}
/*** Va hasta el nroCelda indicado, realiza la comparacion y retorna un bool **/
bool compararCelda(ListaCelda L, int nroCelda, char operador, string valor){
    while( L->sig!=NULL && L->nroCelda!= nroCelda ){
        L = L->sig;
    }
        if( operador == '=' ){
            if( valor.compare(L->info) == 0 ){
                return true;
            }
            else{
                return false;
            }
        }
        if( operador == '>' ){
            if( valor.compare(L->info) < 0 )
                return true;
            else
                return false;
        }

        if( operador == '<' ){
            if( valor.compare(L->info) > 0 )
                return true;
            else
                return false;
        }

        if( operador == '!' ){
            if( valor.compare(L->info) != 0 )
                return true;
            else
                return false;
        }

        if( operador == '*' ){
            if( comienzaCon(valor, L->info) )
                return true;
            else
                return false;
        }
        if( operador == '+' ) //Si no hay operador de comparacion se recibe + para representar todos los registros
            return true;

}

//Fucion para comparar el incio de un string con otro
bool comienzaCon(string valor, string datoCelda){
    int largoValor = valor.length();
    int largoDato  = datoCelda.length();
    if( largoDato >= largoValor ){
        size_t posFin = largoValor;
        string datoCorto = datoCelda.substr(0, posFin);
        if( valor.compare(datoCorto) == 0 )//Si el valor es igual al inicio del valor de la celda retorna true
            return true;
        else
            return false;
    }else
        return false;

}

ListaArg crearListaArg(){
    ListaArg listaArg = new nodoListaArg;
    listaArg->pos = 0;
    listaArg->ant = NULL;
    listaArg->sig = NULL;
    return listaArg;
}

void agregarArg(ListaArg L, string arg){
    ListaArg nuevo = new nodoListaArg;
    ListaArg aux = L;
    nuevo->info = arg;
    nuevo->sig = NULL;
    while( aux->sig!=NULL )
        aux = aux->sig;
    aux->sig = nuevo;
    nuevo->ant = aux;
    nuevo->pos = aux->pos+1;
}

int lengthArg(ListaArg L){ //obtiene el largo de la lista de argumentos
    if( L == NULL )
        return 0;
    if( L->pos == 0 )
        return lengthArg(L->sig);
    else
        return ( 1 + (lengthArg(L->sig)));
}

void imprimirArg(ListaArg L){//esta funcion es solo para modo testing
    if( L != NULL ){
        cout<< "|"<<L->pos<< " = "<<L->info<<"|"<<endl;
        imprimirArg( L->sig );
    }
}

void cargarListaArg(ListaArg L, string allArg, char separador){//Carga una lista de argumentos
    int largo = allArg.length();
    char parametros[largo];
    int inicio=0;
    string dato;
    strcpy(parametros,allArg.c_str());
    for(int i=0; i< largo; i++){
        if( parametros[i]!= separador){
            dato += parametros[i];
        }else{
            agregarArg(L, dato);
            dato="";
        }
    }
    agregarArg(L, dato);
}


/** Esta funcion busca la tupla por su PK y solo retorna true si la encentra, en ese caso el puntero recibido queda apuntando a
dicho registro.
Si no lo a encuentra pueden pasar dos cosas, o bien no existe el registro mayor y llega al final de la lista retornando false con
el puntero a NULL.
O tambien existe algun valor mayor a la pk y retorna false con el puntero en la posicion en la que debe insertar el nuevo registro.
**/
bool buscarTuplaPorPK(ListaTupla &sonda, string pk){
    while( sonda->sig!=NULL ){
        if( sonda->sig == NULL)
            return false;

        if( pk.compare(sonda->sig->celda->sig->info) > 0 ){
            sonda = sonda->sig;
            continue;
        }else{
            if( pk.compare(sonda->sig->celda->sig->info) == 0 ){ //si es igual retorna true y el puntero en la posicion del pk buscado
                sonda = sonda->sig;
                return true;
            }if( pk.compare(sonda->sig->celda->sig->info) < 0 ){
                return false;
            }
        }
    }
}

//Busca una columna por su nombre y retorna la posicion de la columna, o  retorna 1000 si no la encuentra
int buscarColumna(ListaColum L, string nombreColum){
    while( L != NULL ){
        if(L->nombre == nombreColum)
            return L->nroColum;
        else
            L = L->sig;
    }
    return 1000;
}

bool agregarTuplaOrdenada(ListaTupla &auxTupla, string pk, ListaArg listaValores){//agrega una nueva tupla de forma ordenada
    ListaTupla sonda = auxTupla;
    ListaCelda auxCelda;
    if( auxTupla->sig == NULL ){ //Si la primer tupla es NULL inserto en el primer lugar
        agregarTuplaFinal(auxTupla, listaValores);
        /****Aqui falta el codigo para las celdas con los datos***/
        return true;
    }
    if( buscarTuplaPorPK(sonda, pk)){//Si encontro un registro con la misma pk no se puede agregar el registro
        return false;
    }if( buscarTuplaPorPK(sonda, pk) == false && sonda != NULL ){
        auxTupla = sonda;
        agregarTuplaSiguiente(auxTupla, listaValores);
        return true;
    }
}

void agregarTuplaFinal(ListaTupla &auxTupla, ListaArg listaValores){
    if( auxTupla->sig == NULL ){
        ListaTupla nueva = new nodoListaTupla;//Nueva tupla
        nueva->ant = auxTupla;
        nueva->sig = NULL;
        nueva->indice = auxTupla->indice + 1;
        auxTupla->sig = nueva;
        auxTupla = nueva;
        nueva->celda = new nodoListaCelda;//Celda dummy para la nuevta tupla
        nueva->celda->ant = NULL;
        nueva->celda->sig = NULL;
        nueva->celda->nroCelda = 0;
        cargarTupla(auxTupla, listaValores);
        ordenarIndiceTupla(auxTupla);
    }else
        agregarTuplaFinal(auxTupla->sig, listaValores);
}

void agregarTuplaSiguiente(ListaTupla &auxTupla, ListaArg listaValores){
        ListaTupla nuevaTupla = new nodoListaTupla; //Nueva tupla
        nuevaTupla->ant = auxTupla;
        nuevaTupla->sig = auxTupla->sig;
        auxTupla->sig = nuevaTupla;
        if( nuevaTupla->sig != NULL)
            nuevaTupla->sig->ant = nuevaTupla;
        auxTupla = nuevaTupla;
        auxTupla->celda = new nodoListaCelda; //Celda dummy para la nuevta tupla
        auxTupla->celda->ant = NULL;
        auxTupla->celda->sig = NULL;
        auxTupla->celda->nroCelda = 0;
        cargarTupla(auxTupla, listaValores);
        ordenarIndiceTupla(auxTupla);
}

void cargarTupla(ListaTupla &auxTupla, ListaArg listaArg){
    int largo = lengthArg(listaArg);
    ListaCelda auxCelda;
    auxCelda = auxTupla->celda;
    while( auxCelda->sig!= NULL)
        auxCelda = auxCelda->sig;
    for( int i=1; i<= largo; i++ ){
        ListaCelda nueva = new nodoListaCelda;
        nueva->sig = NULL;
        nueva->ant = auxCelda;
        nueva->info = traerParametro(listaArg, i);
        nueva->nroCelda = auxCelda->nroCelda +1;
        auxCelda->sig = nueva;
        auxCelda = nueva;
    }
}

char traerOperador(string condicion){//Obtiene el operador para las comparaciones cursado en un string
    int largo = condicion.length();
    char arrCondicion[largo];
    strcpy(arrCondicion,condicion.c_str());
    char c;
    for(int i=0; i<largo; i++){
        c = arrCondicion[i];
        if( c=='*' || c=='<' || c=='>' || c=='!' || c=='=' || c== '-'){
            return c;
        }
    }
}

void ordenarIndiceTupla(ListaTupla &auxTupla){//Ordena los indices de las tuplas
    while( auxTupla != NULL && auxTupla->indice!=0 ){
        auxTupla->indice = auxTupla->ant->indice + 1;
        auxTupla = auxTupla->sig;
    }
}

void modificarCelda(ListaCelda &auxCelda, int nroCelda, string nuevoValor){
    if( auxCelda==NULL)
        return;
    if( auxCelda!=NULL && auxCelda->nroCelda!=nroCelda )
        modificarCelda(auxCelda->sig, nroCelda, nuevoValor);
    else
        auxCelda->info = nuevoValor;
}


void borrarColumnasTabla(ListaColum &auxColum){ //Borra todas las columnas de una tabla
    if( auxColum==NULL )
        return;
    if( auxColum->sig==NULL ){
        ListaColum borrar = auxColum;
        auxColum = NULL;
        delete borrar;
        return;
    }else{
        ListaColum borrar = auxColum->sig;
        auxColum->sig = borrar->sig;
        delete borrar;
        borrarColumnasTabla(auxColum->sig);
    }
}

void borrarTuplasTabla(ListaTupla &auxTupla){ //Borra todas las tuplas de una tabla
    if( auxTupla==NULL)
        return;
    if( auxTupla->sig == NULL ){
        ListaTupla borrar = auxTupla;
        auxTupla = NULL;
        delete borrar;
        return;
    }if( auxTupla->sig != NULL ){
        ListaTupla borrar = auxTupla->sig;
        auxTupla->sig = borrar->sig;
        delete borrar;
        borrarTuplasTabla(auxTupla->sig);
    }
}

void borrarListaArg(ListaArg &L){ //Borra todas los argumentos
    if( L==NULL )
        return;
    if( L->sig==NULL ){
        ListaArg borrar = L;
        L = NULL;
        delete borrar;
        return;
    }else{
        ListaArg borrar = L->sig;
        L->sig = borrar->sig;
        delete borrar;
        borrarListaArg(L->sig);
    }
}

void ordenarNroColum(ListaColum &auxColum){
    if( auxColum == NULL )
        return;
    if( auxColum->ant == NULL ){
        auxColum->nroColum = 0;
        ordenarNroColum( auxColum->sig );
    }else{
        auxColum->nroColum = auxColum->ant->nroColum +1 ;
        ordenarNroColum( auxColum->sig );
    }
}

void ordenarNroCelda(ListaCelda &auxCelda){
    if( auxCelda == NULL )
        return;
    if( auxCelda->ant == NULL ){
        auxCelda->nroCelda = 0;
        ordenarNroCelda( auxCelda->sig );
    }else{
        auxCelda->nroCelda = auxCelda->ant->nroCelda +1 ;
        ordenarNroCelda( auxCelda->sig );
    }
}




/** Busca el nodo en el arbol y lo retorna o retorna NULL si no lo encuentra**/
ABBTabla traerNodoTabla(string nombre, ABBTabla &t){
    if( t == NULL )
        return NULL;
    if( nombre == t->nombre )
            return t;
    if( nombre.compare(t->nombre) > 0 )
        return traerNodoTabla(nombre, t->der) ;
    if( nombre.compare(t->nombre) < 0 )
        return traerNodoTabla(nombre, t->izq);
}

/**Destruye un nodo tipo tabla */
void destruirNodo(ABBTabla nodo){
    nodo->izq = NULL;
    nodo->der = NULL;
    delete nodo;
}

void imprimirArbol( ABBTabla t){ //Recorre en orden y muestra los elementos en pantalla
    if( t != NULL ){
        imprimirArbol(t->izq);
        cout <<"  "<<t->nombre<<endl ;
        imprimirArbol(t->der);
    }
}

/** Busca la posicion de donde colgara el nuevo nodo (Posicion del nodo padre) ***/
ABBTabla obtenerPosicion(ABBTabla &t, string nombre){
    if( t == NULL) return t;                      //Si el arbol esta vacio y retorno NULL
    if( nombre.compare(t->nombre) < 0 && t->izq != NULL ) //Si es menor y hay mas elementos a la izquierda avanza
        return obtenerPosicion(t->izq, nombre);

    if( nombre.compare(t->nombre) < 0 && t->izq == NULL )//Si es menor y no hay mas elementos (es una hoja)
        return t;

    if( nombre.compare(t->nombre) > 0 && t->der != NULL )//Si es mayor y hay mas elementos a la derecha avanza
        return obtenerPosicion(t->der, nombre);

    if( nombre.compare(t->nombre) > 0 && t->der == NULL )//Si es mayor y no hay mas elementos (es una hoja)
        return t;
}

bool miembro(string x, ABBTabla t){ //Chequea si un elemento pertenece al ABBTabla
    if( t == NULL ) return false;
    else{
        if( x == t->nombre )
            return true;
        else
            return ( miembro(x, t->izq) || miembro(x, t->der) );
    }
}

ABBTabla crearNodoTabla(string nombre, ABBTabla nodoPadre){ //Crea un nodo de tipo tabla
    ABBTabla nuevaTabla = new nodoABBTabla;
    nuevaTabla->izq = NULL;
    nuevaTabla->der = NULL;
    nuevaTabla->cantColumnas = 0;
    nuevaTabla->padre = nodoPadre;
    nuevaTabla->nombre= nombre;
    //Crea la columna dummy en la tabla nueva
    ListaColum nuevaColum = new nodoListaColum;//Columna dummy
    nuevaColum->ant = NULL;
    nuevaColum->sig = NULL;
    nuevaTabla->columna = nuevaColum;
    //Crea la tupla dummy en la tabla nueva
    ListaTupla nuevaTupla = new nodoListaTupla;//Tupla dummy
    nuevaTupla->ant = NULL;
    nuevaTupla->sig = NULL;
    nuevaTupla->indice = 0;
    nuevaTabla->tupla = nuevaTupla;
    //Crea la celda dummy en la tupla
    ListaCelda nuevaCelda = new nodoListaCelda;
    nuevaCelda->ant = NULL;
    nuevaCelda->sig = NULL;
    nuevaCelda->nroCelda = 0;
    nuevaTabla->tupla->celda = nuevaCelda;
    return nuevaTabla;
}

void insertDer( string nombre, ABBTabla &t){
    t->der = crearNodoTabla(nombre, t);
}

void insertIzq(string nombre, ABBTabla &t){
    t->izq = crearNodoTabla(nombre, t);
}

void insertNodoTabla(ABBTabla &t, string nombre){ //Recibe el nombre de una tabla y la crea en la posicion correcta
    ABBTabla padre = obtenerPosicion(t, nombre);    //Obtiene la posicion del padre que le corresponde
    if( padre == NULL ){
        padre = crearNodoTabla(nombre, padre);
        t = padre;
        return;
    }
    if( nombre.compare(padre->nombre) > 0 ){
        insertDer(nombre, padre);
    }
    else
        insertIzq(nombre, padre);
}

void borrarNodoTabla(ABBTabla nodoEliminar){
    if( nodoEliminar->der && nodoEliminar->izq ){ /**Si tiene ambos hijos*/
        ABBTabla menor = obtenerMinimo( nodoEliminar->der ); //Obtiene el menimo que esta a la derecha
        nodoEliminar->nombre = menor->nombre; //Cambia los datos del nodo a eliminar por los del minimo que esta a la derecha
        menor->padre->izq = menor->izq;
        borrarNodoTabla(menor); //ahora hay que eliminar al menor que se copio para arriba
    }
    else if( nodoEliminar->izq ){ /**Si solo tiene un hijo de la izquierda*/
        reemplazar(nodoEliminar, nodoEliminar->izq);//Sustituye el nodo a borrar por el nodo de la izquierda
        destruirNodo(nodoEliminar);
    }
    else if( nodoEliminar->der ){ /**Si solo tiene un hijo de la derecha */
        reemplazar(nodoEliminar, nodoEliminar->der);//Sustituye el nodo a borrar por el nodo de la derecha
        destruirNodo(nodoEliminar);
    }
    else{ /**Si no tiene hijos ( el nodo a eliminar es una hoja ) */
      //  reemplazar(nodoEliminar, NULL);//Sustituye el nodo a eliminar por NULL
        destruirNodo(nodoEliminar);
    }
}

/** Funcion para obtener el nodo minimo **/
ABBTabla obtenerMinimo(ABBTabla t){//Determina el nodo más a la izquierda
    if( t == NULL )
        return NULL;
    if( t->izq ) //Si teine hijo izquierdo avanza más a la izquierda
        return obtenerMinimo(t->izq);
    else        //Si no tiene hijo izquierdo, llego al final y retorna el nodo minimo
        return t;
}

/** Funcion para reemplazar el nodo1 por el nodo2 **/
void reemplazar(ABBTabla nodo1, ABBTabla nodo2){
    if( nodo1->padre ){ //Si el nodo a reemplazar tiene padre
        if( nodo1->nombre == nodo1->padre->izq->nombre ){//Chequea que el padre este apuntando al hijo izquierdo
            nodo1->padre->izq = nodo2; //Ahora el hijo izquierdo del padre es nodo2
        }
        if( nodo1->nombre == nodo1->padre->der->nombre ){//Chequea que el padre este apuntando al hijo derecho
            nodo1->padre->der = nodo2; //Ahora el hijo derecho del padre es nodo2
        }
    }
    if( nodo2 )
        nodo2->padre = nodo1->padre;//Ahora nodo2 apunta a su nuevo padre
}

/** Copia los datos de la columna 1 en la columna 2 ***/
void copiarColumnas(ListaColum colum1, ListaColum colum2){
    if( colum1->sig != NULL ){
        if( colum2->sig == NULL )
            agregarColumna(colum2, colum1->sig->nombre);
        copiarColumnas(colum1->sig, colum2->sig);
    }
}

void agregarColumna(ListaColum L, string nombre){
    while( L->sig!=NULL)
        L = L->sig;
    ListaColum nuevaColum = new nodoListaColum;
    L->sig = nuevaColum;
    nuevaColum->nombre = nombre;
    nuevaColum->sig = NULL;
    nuevaColum->ant = L;
    nuevaColum->nroColum = nuevaColum->ant->nroColum+1;
    if( L->ant == NULL )  // Verifica si la columna a agregar debe ser PK o no
        nuevaColum->PK = true;
}


/** Obtiene los datos de una lista de celdas (registro) y los retorna como estring con formato valor1:valor2:valor3
EL string values debe ser vacio y el int pos indica la primer posicion a copiar **/
string traerDatosCeldas(ListaCelda L, string values, int pos){
    if( L==NULL )
        return values;
    else{
        if( L->nroCelda < pos )
            return traerDatosCeldas(L->sig, values, pos);
        if( L->nroCelda == pos ){
            return traerDatosCeldas(L->sig, values += L->info, pos);
        }else
            return traerDatosCeldas(L->sig, values +=":"+L->info, pos);
    }

}

/** Version simplificada para insertar datos en una tabla */
void insertarReg(string nombreTabla, string valoresTupla){
    extern ABBTabla t;                           //ListaTabla Global
    ABBTabla auxTabla;
    if( nombreTabla.empty() )  // Valida que el nombre de la tabla no este vacio
        return;
    if( valoresTupla.empty() )  // Valida que tenga valores
        return;
    ListaTupla auxTupla = NULL;
    ListaArg listaValores = crearListaArg();            //crea una lista de valores para recibir los argumentos
    cargarListaArg(listaValores, valoresTupla, ':');    //carga los valores en una lista
    auxTabla = traerNodoTabla(nombreTabla, t); //si la tabla existe devuelve el puntero a ella, si no el puntero es NULL
    if( auxTabla != NULL){
        if( auxTabla->cantColumnas == lengthArg(listaValores) ){//Chequea si la cantidad de valores pasados es igual a los campos que tiene a tabla
            auxTupla = auxTabla->tupla;
            string pk = traerParametro(listaValores, 1); //obtiene la pk cursada
            if(agregarTuplaOrdenada(auxTupla, pk, listaValores)){  //Devuelve true si pudo insertar la tupla de forma ordenada
                borrarListaArg(listaValores);
                return;
            }else{
                borrarListaArg(listaValores);
                return;
            }
        }
    }
    else{
        cout<<"> La tabla a copiar \""<<nombreTabla<<"\" no existe"<<endl;
    }
}

/** Si hay columnas repetidas entre la tabla1 y la tabla2 retorna true, no cuenta la PK) */
bool columnasRepetidas(ListaColum L1, ListaColum L2){
    if( L1 == NULL && L2 == NULL )
        return false;
    if( L1->nombre.compare(L2->nombre) == 0 && L1->nroColum > 1 )
        return true;
    else
        return columnasRepetidas( L1->sig, L2->sig );
}

/** Une las columnas de la tabla2 con las columnas de la tabla1 (Los agrega al final de la tabla1 )
El entero pos indica desde que posicion de la tabla2 va a empezar a copiar  **/
void unirColumnas(ListaColum L1, ListaColum L2, int pos){
    while( L2 != NULL ){
        while( L1->sig!= NULL )
            L1 = L1->sig;
        if( L2->nroColum >= pos ){
            agregarColumna(L1, L2->nombre);
        }
        L2 = L2->sig;
    }
}

//Cuenta la cantidad de columnas que tiene una tabla
int cantColumnas(ListaColum L){
    if( L == NULL )
        return 0;
    if( L->nroColum == 0 )
        return cantColumnas(L->sig);
    else
        return ( 1 + (cantColumnas(L->sig)));
}

// Trae el dato de la posicion pos
string traerDato(ListaCelda LC, int pos){
    string dato = "";
    if( LC == NULL ) return dato;
    if( LC->nroCelda == pos ) return LC->info;
    else    return traerDato(LC->sig, pos);
}

//Imprime un registro (lista del celdas )
void imprimirReg(ListaCelda LC){
    while( LC->sig != NULL ){
        LC = LC->sig;
        if( LC->ant->ant == NULL )
            cout<<"  "<< LC->info;
        if( LC->nroCelda > 1 && LC->sig != NULL )
            cout<<":"<< LC->info;
        if( LC->sig == NULL && LC->nroCelda > 1){
            cout<<":"<< LC->info <<endl;
        continue;
        }
        if( LC->sig == NULL )
            cout<<endl;
    }
}
