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

struct nodoListaTabla{
    string nombre;
    int cantColumnas;
    ListaColum columna;
    ListaTupla tupla;
    nodoListaTabla * ant;
    nodoListaTabla * sig;
};
typedef nodoListaTabla * ListaTabla;

//**** GLOBALES *****//
ListaTabla LTabla = new nodoListaTabla;

//** OPERACIONES ***/
TipoRet createTable(string nombreTabla);
TipoRet dropTable(string nombreTabla);
TipoRet addCol(string nombreTabla, string nombreCol);
TipoRet dropCol(string nombreTabla, string nombreCol);
TipoRet insertInto(string nombreTabla, string valoresTupla);
TipoRet deleteFrom(string nombreTabla, string condicionEliminar);
TipoRet update(string nombreTabla, string condicionModificar, string columnaModificar, string valorModificar );
TipoRet printDataTable(string nombreTabla);

/** FUNCIONES Y PROCEDIMIENTOS AUXILIARES */
void leerComando(ListaTabla T, string comando ); //Interpreta el comando de entrada
void mostrarAyuda(); // Imprime la Ayuda con los comandos validos
string traerParametro(ListaArg L, int n);  //Trae del a posicion n un valor tipo string de la lista de parametros
ListaArg crearListaArg();               //Crea una lista para guardar parametros
void imprimirArg(ListaArg L);           //Imprime los argumentos de la lista
void borrarListaArg(ListaArg &L);
void agregarArg(ListaArg L, string arg); //Agrega un nuevo parametro al final de la lista de parametros
void cargarListaArg(ListaArg L, string allArg, char separador);
ListaTabla buscarTabla(ListaTabla &LTabla, string nombreTabla); //Si la tabla existe devuelve el puntero a ella, si no el puntero es NULL
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

int test = 0;

int main(){
    extern ListaTabla LTabla;
    LTabla->cantColumnas = 0;
    LTabla->ant = NULL;
    LTabla->sig = NULL;

    string comando;

    while(comando!="exit"){ //mantiene la terminal esperando ordenes
        getline(cin, comando);
        if( comando.empty()!=true )
            leerComando(LTabla, comando);
        else
            cout << "  No ha ingresado ningun comando" <<endl;
    }
    cout<<endl<< "  Bye..." <<endl;

    return 0;
}

TipoRet createTable(string nombreTabla){
    TipoRet res = OK;
    extern ListaTabla LTabla;               //ListaTabla Global
    ListaTabla aux = LTabla;
    while( aux->sig!=NULL ){
        if( aux->sig->nombre == nombreTabla ){//Valida si la tabla ya existe
            res = ERROR;
            return res;
        }else{
            aux = aux->sig;
        }
    }
    ListaTabla nuevaTabla = new nodoListaTabla;
    nuevaTabla->nombre = nombreTabla;
    nuevaTabla->sig = NULL;
    aux->sig = nuevaTabla;
    nuevaTabla->ant = aux;
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
    return res;
}

TipoRet dropTable(string nombreTabla){
    TipoRet res = OK;
    extern ListaTabla LTabla;               //Variable tipo listaTabla Global
    ListaTabla auxTabla = LTabla, borrarTabla;
    ListaColum auxColum, borrarColum;
    ListaTupla auxTupla, borrarTupla;
    ListaCelda auxCelda, borrarCelda;
    while( auxTabla->sig!=NULL ){
        if( auxTabla->sig->nombre == nombreTabla ){//Busca la tabla a borrar y para una posicion antes
            auxColum = auxTabla->sig->columna;     //Apunta a las columnas a eliminar
            borrarColumnasTabla(auxColum); //Borra la lista de columnas de la tabla
            auxTupla = auxTabla->sig->tupla; //Apunta al comienzo de las tuplas a borrar
            while( auxTupla != NULL ){//Recorro todas las tuplas y borro sus celdas
                auxCelda = auxTupla->celda; //Apunta a la lista de celdas de la tupla actual
                borrarCeldasTupla(auxCelda);//Elimina la lista de celdas de la tupla actual
                auxTupla = auxTupla->sig;   //Avanza a la siguiente tupla
            }
            auxTupla = auxTabla->sig->tupla; //Apunta al comienzo de las tuplas a borrar
            borrarTuplasTabla(auxTupla);  //Borra todas la tuplas de una tabla
            //Por ultimo borra la tabla
            borrarTabla = auxTabla->sig;
            if( auxTabla->sig->sig != NULL ) //Verifica si hay un nodo despues del que voy a borrar
                auxTabla->sig->sig->ant = auxTabla;
            auxTabla->sig = auxTabla->sig->sig;
            delete borrarTabla;
            return res;
        }
        auxTabla = auxTabla->sig;
    }
    res = ERROR;
    return res;
}

TipoRet addCol(string nombreTabla, string nombreCol){
    TipoRet res = NO_IMPLEMENTADA;
    extern ListaTabla LTabla;               //ListaTabla Global
    ListaTabla auxTabla = LTabla;
    ListaColum auxColum = NULL;
    ListaColum nuevaColum = NULL;
    auxTabla = buscarTabla(LTabla, nombreTabla); //Si existe la tabla, se para apuntando sobre ella
    if( auxTabla!=NULL ){
        if( auxTabla->tupla->sig == NULL ){ // Chequea que la tabla no tenga ningun registro cargado **/
            auxColum = auxTabla->columna; //Puntero auxiliar para recorrer las columnas
            while( auxColum->sig != NULL ){//Recorre la lista de columnas y cheque que no exista una columna con el mismo nombre
                auxColum = auxColum->sig;
                if( auxColum->nombre == nombreCol ){
                    cout<<"  ¡Operacion no valida!. Ya existe una columna llamada \""<<nombreCol<<"\""<<endl;
                    res = ERROR;
                    return res;
                }
            }
            //Si no se encontro ninguna columna con el mismo nombre:
            nuevaColum = new nodoListaColum;
            auxColum->sig = nuevaColum;
            nuevaColum->nombre = nombreCol;
            nuevaColum->sig = NULL;
            nuevaColum->ant = auxColum;
            nuevaColum->nroColum = auxColum->nroColum+1;
            auxTabla->cantColumnas++;
            if( auxColum->ant == NULL )  // Verifica si la columna a agregar debe ser PK o no
                nuevaColum->PK = true;
            res = OK;
            return res;
        }else{
            cout<<"  ¡Operacion no valida!. La tabla ya tiene registros cargados"<<endl;
            res = ERROR;
            return res;
        }

    }
    cout<<"  La tabla \""<<nombreTabla<<"\" no existe."<<endl;
    res = ERROR;
    return res;
}

TipoRet dropCol(string nombreTabla, string nombreCol){
    TipoRet res = NO_IMPLEMENTADA;
    int nroColBuscada;
    extern ListaTabla LTabla;               //ListaTabla Global
    ListaTabla auxTabla = LTabla;           //puntero auxiliar tipo ListaTabla
    ListaColum auxColum;
    ListaColum borraColum;
    ListaTupla auxTupla;
    ListaCelda auxCelda;
    ListaCelda borraCelda;
    /** Busca si existe la tabla **/
    auxTabla = buscarTabla(LTabla, nombreTabla); //si la tabla existe devuelve el puntero a ella, si no el puntero es NULL
    if( auxTabla!=NULL ){
            auxColum = auxTabla->columna;

            while( auxColum->sig != NULL ){  //Recorre la lista de columnas y chequea que no exista una columna con el mismo nombre
                if( auxColum->sig->nombre == nombreCol ){
                    nroColBuscada = buscarColumna(auxColum, nombreCol); //Obtiene el numero de la columna, si no lo encuentra retorna 1000
                   // nroColBuscada = auxColum->sig->nroColum; //guarda el numero de la columna buscada
                    if( auxColum->sig->PK==true && auxTabla->cantColumnas>1 ){//si hay mas de una columna no se pude borrar la pk
                        cout<<"  La columna \""<<nombreCol<<"\" es Clave Primaria y hay otras columnas que se identifican por ella."<<endl;
                        res = ERROR;
                        return res;
                    }
                    else{
                    /******* LO MODIFICADO ********************/
                        borrarColumna( auxColum, nroColBuscada); //Borra la columna
                        ordenarNroColum(auxTabla->columna);    //Ordena los numeros de las columnas
                    /*******************************************/
                        auxTabla->cantColumnas--;         //Decrementa en uno la cantidad de columnas que tiene la tabla
                        auxTupla = auxTabla->tupla->sig;//arranca en la tupla 1
                        while( auxTupla != NULL ){ //Busca en todas las tuplas
                            auxCelda = auxTupla->celda;
                            borrarCelda(auxCelda, nroColBuscada);//borra la celda
                            ordenarNroCelda( auxTupla->celda ); //ordena el indice de la celda
                            auxTupla = auxTupla->sig;       //Avanza a la  siguiente tupla
                        }
                        cout<<"  Columna \""<<nombreCol<<"\" eliminada"<<endl;
                        res = OK;
                        return res;
                    }
                }
                auxColum = auxColum->sig;
            }
            cout<<"  La Columna \""<<nombreCol<<"\" no existe."<<endl;
            res = ERROR;
            return res;
    }else{  //Si la tabla no existe
        cout<<"  La tabla \""<<nombreTabla<<"\" no existe."<<endl;
        res = ERROR;
        return res;
    }
}

TipoRet insertInto(string nombreTabla, string valoresTupla){
    TipoRet res = OK;
    extern ListaTabla LTabla;                           //ListaTabla Global
    ListaTabla auxTabla = NULL;
    ListaTupla auxTupla = NULL;
    ListaArg listaValores = crearListaArg();            //crea una lista de valores para recibir los argumentos
    cargarListaArg(listaValores, valoresTupla, ':');    //carga los valores en una lista
    auxTabla = buscarTabla(LTabla, nombreTabla); //si la tabla existe devuelve el puntero a ella, si no el puntero es NULL
    if( auxTabla != NULL){
        if( auxTabla->cantColumnas == lengthArg(listaValores) ){//Chequea si la cantidad de valores pasados es igual a los campos que tiene a tabla
            auxTupla = auxTabla->tupla;
            string pk = traerParametro(listaValores, 1); //obtiene la pk cursada
            if(agregarTuplaOrdenada(auxTupla, pk, listaValores)){  //Devuelve true si pudo insertar la tupla de forma ordenada
             //   cargarTupla(auxTupla, listaValores);
                cout<<"  Nuevo registro agregado con exito"<<endl;
                borrarListaArg(listaValores);
                return res;
            }else{
                cout<<"  No se pudo insertar el registro"<<endl;
                res = ERROR;
                borrarListaArg(listaValores);
                return res;
            }
        }
    }
}

TipoRet deleteFrom(string nombreTabla, string condicion){
    TipoRet res = OK;
    extern ListaTabla LTabla;               //ListaTabla Global
    ListaTabla auxTabla = NULL;                //Tabla auxiliar
    ListaTupla auxTupla = NULL;
    ListaCelda auxCelda = NULL;
    string msjRespuesta;                    //Respuesta a retornar
    int regAfectados = 0;                //Cuenta la cantidad de elementos encontrados
    char operador = traerOperador(condicion); //obtiene el operador de la condicion a buscar
    ListaArg listaCondicion = crearListaArg(); //crea una lista con las condiciones
    cargarListaArg(listaCondicion, condicion, operador); // separa ambas partes de la condicion
    string nombreColumna    = traerParametro(listaCondicion,1);
    string valorCondicion   = traerParametro(listaCondicion,2);
    auxTabla = buscarTabla(LTabla, nombreTabla); //si la tabla existe devuelve el puntero a ella, si no el puntero es NULL
    if( auxTabla != NULL){
        int nroColumna = buscarColumna(auxTabla->columna, nombreColumna); //si el nombre de la columna existe retorna su posicion, si no retorna 1000
        if( nroColumna != 1000 ){ //Chequea que exista la columna
            auxTupla = auxTabla->tupla->sig; //Puntero auxiliar para recorrer las tuplas
            while( auxTupla!= NULL ){
                auxCelda = auxTupla->celda;
                if( operador!='-' ){
                    bool encontrado = false;
                    encontrado = compararCelda(auxCelda, nroColumna, operador, valorCondicion);
                    if( encontrado == true ){
                        cout<<"  Registro borrado-> "<< auxCelda->sig->info <<":"<< auxCelda->sig->sig->info<<":"<<auxCelda->sig->sig->sig->info  <<endl;
                        borrarCeldasTupla(auxCelda);
                        borrarTupla(auxTupla);
                        regAfectados++; //Cuenata los registros encontrados
                    }
                }
                if( operador=='-' ){
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
                cout<< "  No existe ninguna celda con el valor \""<<valorCondicion;
            else
                cout<< "  Registros afectados "<<regAfectados;//Imprime la cantidad de registros afectados si encuentra alguno
            cout<<msjRespuesta<<endl; //Imprime en pantalla la respuesta
            return res;
        }
        else{
            cout<<"  No hay campo con ese nombre";
            borrarListaArg(listaCondicion);
            res = ERROR;
            return res;
        }

    }
}

TipoRet update(string nombreTabla, string condicionModificar, string columnaModificar, string valorModificar ){
    TipoRet res = OK;
    extern ListaTabla LTabla;               //ListaTabla Global
    ListaTabla auxTabla = NULL;             //Tabla auxiliar
    ListaTupla auxTupla = NULL;
    ListaCelda auxCelda = NULL;
    int regAfectados   = 0;                //Cuenta los registros afectados
    char operador = traerOperador(condicionModificar); //obtiene el operador de la condicion a buscar
    ListaArg listaCondicion = crearListaArg();           //Crea una lista con las condiciones
    cargarListaArg(listaCondicion, condicionModificar, operador); // Separa ambas partes de la condicion
    string columnaCondicion = traerParametro(listaCondicion,1);// Nombre de la columna por la cual filtrar
    string valorCondicion   = traerParametro(listaCondicion,2); //Valor que debe cumplir el filtro
    auxTabla = buscarTabla(LTabla, nombreTabla); //si la tabla existe devuelve el puntero a ella, si no el puntero es NULL
    if( auxTabla != NULL){
        int nroColumnaMod = buscarColumna(auxTabla->columna, columnaModificar); //si el nombre de la columna existe retorna su posicion, si no retorna 1000
        if( nroColumnaMod == 1 ){
            cout<<"  No se puede modificar la clave primaria"<<endl;
            return res = ERROR;
        }
        int nroColumnaCond = buscarColumna(auxTabla->columna, columnaCondicion);
        if( nroColumnaCond != 1000  && nroColumnaMod != 1000 ){ //Chequea que existan ambas columnas en la tabla
            auxTupla = auxTabla->tupla->sig; //Puntero auxiliar para recorrer las tuplas
            while( auxTupla!= NULL ){
                auxCelda = auxTupla->celda;
                if( operador != '-' ){
                //if( !columnaCondicion.empty() ){
                    if( compararCelda( auxCelda, nroColumnaCond, operador, valorCondicion) ){
                        cout<<"  Registro modifcado-> "<< auxCelda->sig->info<<":"<< auxCelda->sig->sig->info<<":"<<auxCelda->sig->sig->sig->info<<endl;
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
                cout<<"  Actualizando todos los registros"<<endl;
            cout<<"  Registros afectados "<<regAfectados<<endl;
            return res;
        }
        else{
            cout<< "No hay campo con ese nombre"<<endl;
            res = ERROR;
            borrarListaArg(listaCondicion);
            return res;
        }
    }
}

TipoRet printDataTable(string nombreTabla){
    TipoRet res = OK;
    extern ListaTabla LTabla;               //ListaTabla Global
    ListaTabla auxTabla = LTabla;
    while( auxTabla!=NULL ){
        if( auxTabla->nombre == nombreTabla ){ //Si existe la tabla, para apuntando sobre ella
                cout<<"  Tabla "<<auxTabla->nombre<<endl;
                ListaColum auxColum = auxTabla->columna; //Puntero auxiliar para recorrer las columnas
                /** Recorre la lista campos */
                while( auxColum->sig != NULL ){ //Recorre la lista de columnas y chequea que no exista una columna con el mismo nombre
                    auxColum = auxColum->sig;
                    if( auxColum->ant->ant == NULL )
                        cout<<"  "<< auxColum->nombre;
                    if( auxColum->nroColum > 1 && auxColum->sig != NULL )
                        cout<<":"<< auxColum->nombre;
                    if( auxColum->sig == NULL && auxColum->nroColum > 1)
                        cout<<":"<< auxColum->nombre <<endl;
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
                        if( auxCelda->sig == NULL && auxCelda->nroCelda > 1)
                            cout<<":"<< auxCelda->info <<endl;
                    }
                }
                return res;
        }else{
            auxTabla = auxTabla->sig;
        }
    }
    cout<<"  La tabla \""<<nombreTabla<<"\" no existe."<<endl;
    res = ERROR;
    return res;
}

void mostrarAyuda(){
    cout <<endl<< "AYUDA DE COMANDOS: "<<endl;
    cout << "Nota: todos los comandos son 'case sensitive' "<<endl;
    cout << "  help  _______________________________________________________________________* IMPRIME LA AYUDA EN PANTALLA *" <<endl<<endl;
    cout << "  createTable(nombreTabla) ____________________________________________________* CREA UNA NUEVA TABLA *"<<endl<<endl;
    cout << "  Ejemplo:    createTable(Empleados)"<<endl<<endl;
    cout << "  dropTable(nombreTabla) ______________________________________________________* ELIMINA UNA TABLA EXISTENTE *" <<endl<<endl;
    cout << "  Ejemplo:    dropTable(Productos)"<<endl<<endl;
    cout << "  addCol(nombreTabla, nombreCol) ______________________________________________* AGREGA UNA NUEVA COLUMNA A LA TABLA EXISTENTE *" <<endl<<endl;
    cout << "  Ejemplo:    addCol(Empleados, Apellido)"<<endl<<endl;
    cout << "  dropCol(nombreTabla, nombreCol) _____________________________________________* ELIMINA UNA COLUMNA DE UNA TABLA *" <<endl<<endl;
    cout << "  Ejemplo:    dropCol(Proyectos, idProyecto)"<<endl<<endl;
    cout << "  insertInto(nombreTabla, valoresTupla) _______________________________________* INSERTA UN NUEVO REGISTRO EN LA TABLA *" <<endl<<endl;
    cout << "  Ejemplo:    insertInto(Personas, 3333111: Telma: Perez)"<<endl<<endl;
    cout << "  deleteFrom(nombreTabla, condicionEliminar) __________________________________* ELIMINA UN REGISTRO DE UNA TABLA *" <<endl<<endl;
    cout << "  Ejemplo:    deleteFrom(Personas, Perez)"<<endl<<endl;
    cout << "  update(nombreTabla, condicionModificar, columnaModificar, valorModificar) ___* caracterIZA UN CAMPO DE UNA TABLA *" <<endl<<endl;
    cout << "  Ejemplo:    update(Personas, Nombre=Pepe: CI: 1555000)"<<endl<<endl;
    cout << "  printDataTable(Clientes) _________________________________________________* IMPRIME TODOS LOS REGISTROS DE UNA TABLA *" <<endl<<endl;
    cout << "  Ejemplo:    printDataTable(Clientes)"<<endl<<endl;
}

/****************   LEE EL INGRESO DE LOS COMANDOS   ***********************/
void leerComando(ListaTabla LTabla, string comando){
    TipoRet res;
    string caracter;
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

    if( test == 0 ){
    	createTable("empleados");
    	addCol("empleados","id");
    	addCol("empleados","nombre");
    	addCol("empleados","apellido");
    	insertInto("empleados","11:Juan:Alvarez");
    	insertInto("empleados","2:Marina:Arismendi");
    	insertInto("empleados","9:Manuel:Urrutia");
    	insertInto("empleados","5:Emiliano:Carvajal");
    	test = 1;
    }

    //Se cargan los argumentos recibidos en una lista
    ListaArg listaArg = crearListaArg();
    cargarListaArg(listaArg, allArg, ',');

    string nombreTabla = traerParametro(listaArg,1); //Obtiene el nombre de la tabla

    if( sentencia=="createTable" ){ //createTable( nombreTabla )
        res = createTable(nombreTabla);
        if( res == 0 )
            cout<< "  Query OK -> Se creo la tabla \""<<nombreTabla<<"\""<<endl<<endl;
        if( res == 1)
            cout<< "  Query ERROR -> La tabla "<<nombreTabla<<" ya existe"<<endl<<endl;
    }

    if( sentencia=="dropTable" ){ // dropTable( nombreTabla )
        res = dropTable(nombreTabla);
        if( res == 0 )
            cout<< "  Query OK -> La Tabla \""<<nombreTabla<<"\" fue eliminada"<<endl<<endl;
        if( res == 1 )
            cout<< "  Query ERROR -> La tabla \""<<nombreTabla<<"\" no existe"<<endl<<endl;
    }

    if( sentencia == "addCol" ) {//addCol( nombreTabla, nombreCol )
        string nombreColumna = traerParametro(listaArg, 2);//obtiene e nombre de la columna ha agregar
        res = addCol(nombreTabla, nombreColumna);
        if(  res == 0 )
            cout<< "  Query OK -> La columna \""<<nombreColumna<<"\" ha sido creada"<<endl<<endl;
        if( res == 1)
            cout<< "  Query ERROR -> No se puedo crear la columna \""<<nombreColumna<<"\""<<endl<<endl;
        if( res == 2 )
            cout<< "  Query ERROR -> No se realizo la operacion"<<endl<<endl;
    }

    if( sentencia == "dropCol" ){ //   dropCol( nombreTabla, nombreCol)
        string nombreColumna = traerParametro(listaArg, 2);
        res = dropCol(nombreTabla, nombreColumna);
        if(  res == 0 )
            cout<< "  Query OK -> La columna \""<<nombreColumna<<"\" ha sido eliminada"<<endl<<endl;
        if( res == 1)
            cout<< "  Query ERROR -> No se puedo eliminar la columna \""<<nombreColumna<<"\""<<endl<<endl;
        if( res == 2 )
            cout<< "  Query ERROR -> No se realizo la operacion"<<endl<<endl;
    }

    if( sentencia == "insertInto" ){// insertInto( nombreTabla,valoresTupla")
        string valoresTupla = traerParametro(listaArg, 2);
        res = insertInto(nombreTabla, valoresTupla);
        if(  res == 0 )
            cout<< "  Query OK -> Nuevo resitro en la tabla \""<<nombreTabla<<"\""<<endl<<endl;
        if( res == 1)
            cout<< "  Query ERROR -> No se puedo agregar el registro en la tabla \""<<nombreTabla<<"\""<<endl<<endl;
        if( res == 2 )
            cout<< "  Query ERROR -> No se realizo la operacion"<<endl<<endl;

    }
    if( sentencia == "deleteFrom" ){ //deleteFrom( nombreTabla, condicionEliminar )
        string condicionEliminar = traerParametro(listaArg, 2);
        res = deleteFrom(nombreTabla, condicionEliminar);
        if(  res == 0 )
            cout<< "  Query OK "<<endl<<endl;
        if( res == 1)
            cout<< "  Query ERROR -> Se produjo un error al intentar eliminar registros"<<endl<<endl;
        if( res == 2 )
            cout<< "  Query ERROR -> No se realizo la operacion"<<endl<<endl;

    }
    if( sentencia == "update" ){ // update( nombreTabla, condicionModificar, columnaModificar, valorModificar)
        string condModificar = traerParametro(listaArg, 2);
        string columna = traerParametro(listaArg, 3);
        string valoModificar = traerParametro(listaArg, 4);
        res = update(nombreTabla, condModificar, columna, valoModificar);
        if(  res == 0 )
            cout<< "  Query OK"<<endl<<endl;
        if( res == 1)
            cout<< "  Query ERROR -> Error al actulizar el registro"<<endl<<endl;
        if( res == 2 )
            cout<< "  Query ERROR -> No se realizo la operacion"<<endl<<endl;
    }

    if( sentencia == "printDataTable" ){  // printDataTable( nombreTabla );
        res = printDataTable(nombreTabla);
        if( res == 0 )
            cout<< "  Query OK"<<endl<<endl;
        if( res == 1 )
            cout<< "  Query ERROR -> No se pudo mostrar la tabla\""<< nombreTabla <<"\""<<endl<<endl;
    }
    if( sentencia == "help"  )//  mostrarAyuda()
        mostrarAyuda();
    if(sentencia!="createTable" && sentencia!="dropTable" && sentencia!="addCol" && sentencia!="dropCol" && sentencia!="insertInto" && sentencia!="deleteFrom" && sentencia!="update" && sentencia!="printDataTable" && sentencia!="help" && sentencia!="exit" )
        cout << "  ¡EL comando '" << comando <<"' no es valido!"<<endl<<endl;
}

//Obtiene el parametro en la posicion n de la lista. Nota: debe recibir un n valido
string traerParametro(ListaArg L, int n){
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
            if( valor.compare(L->info) == 0 ) return true;
            else
                return false;
        }
        if( operador == '<' ){
            if( valor.compare(L->info) < 0 )
                return true;
            else
                return false;
        }

        if( operador == '>' ){
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

ListaTabla buscarTabla(ListaTabla &LTabla, string nombreTabla){ //Busca una tabla por su nombre
    if( LTabla == NULL)
        return NULL;
    if( LTabla->nombre != nombreTabla )
        return buscarTabla( LTabla->sig, nombreTabla);
    else
        return LTabla;
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
    if( auxTupla->sig==NULL ){
        ListaTupla borrar = auxTupla;
        auxTupla = NULL;
        delete borrar;
        return;
    }else{
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
	}else{//void agregarCeldaFinal(ListaCelda &auxCelda, string dato);//agrega nueva celda al final de la lista
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

