#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>

#include <unistd.h>
//Descomentar si esta en windows
//#include <Windows.h> 

#define MAXSIZE 26


//////////////////////////PROTOTIPOS_FUNCIONES/////////////////////////

//Abre un maletin
void abrirMaletin(int);
/*Funcion para despejar la consola de manera portable obtenida de:
https://stackoverflow.com/questions/2347770/how-do-you-clear-the-console-screen-in-c#:~:text=just%20type%20clrscr()%3B%20function,function%20easy%20to%20clear%20screen.*/
void clearsc(){
    #if defined(__linux__) || defined(__unix__) || defined(__APPLE__)
        system("clear");
    #endif

    #if defined(_WIN32) || defined(_WIN64)
        system("cls");
    #endif
}
//Crea un usuario, crea un archivo por usuario
void crearUsr();
//Declara los valores dentro del arreglo de maletines y le asigna un premio aleatorio a cada uno
void generarMaletines(void);
//Genera la oferta del banco
int generarOferta(int);
//Ayuda a saber que maletin contiene que premio
int getPos(float);
//Inicia la sesion leyendo el archivo
int inicarSes(char*);
//Animacion de introduccion
void intro();
//El cilco principal del juego
void mainGame(void);
//Menu principal
void mainMenu(int*);
//Imprime el tablero principal de los maletines
void printBoard(void);
//Impreime las estadisticas del usuario en pantalla
void printEstadisticas();
//Funcion con propositos de depuracion
void printMaletin(int);
//Checa si el usuario ingresa un numero valido, dado por un rango del 1 a int x
void validateInput(int*, int);

/////////////////////////ESTRUCTURAS////////////////////////

typedef struct
{
    int num;  //Nuemero que aparece en cada maletin
    char symbol; // Maneja la X que se pone despues de abrir un maletin
    float premio; // El dinero que contiene cada maletin
    bool abierto; // Determina si el maletin ya fue abierto o no.
} maletin;

/////////////////////VARIABLES GLOBALES/////////////////////

float premios[MAXSIZE] = {0.01, 1, 5, 10, 25, 50, 75, 100, 200, 300, 
                        400, 500, 750, 1000, 5000, 10000, 25000, 50000,
                        75000, 100000, 200000, 300000, 400000, 500000,
                        750000, 1000000};

maletin maletines[MAXSIZE];

char usr[30];
//char *usr = (char*)malloc(30*sizeof(char));

///////////////////////////MAIN////////////////////////////

int main(void)
{
    int op;
    int acceso = 0;
    intro();
    do
    {
        clearsc();
        mainMenu(&op);
        switch (op)
        {
        case 1:
            clearsc();
            printf("Primero debes iniciar sesion...\n");
            acceso = inicarSes(usr); 
            if(acceso == 1){mainGame();}         
            break;
        case 2:
            crearUsr();
            break;
        case 3:
            printEstadisticas();
        default:
            break;
        }
    }while(op != 4);
    
}

////////////////////////DECLARACIONES////////////////////////

void abrirMaletin(int x)
{
    x--;
    maletines[x].num = 0;
    maletines[x].symbol = 'X';
    maletines[x].abierto = 1;
}

void crearUsr()
{ 
    FILE *ap;
	char c[20], usuario[20];
	char *NomArch;
	
	NomArch=(char*)malloc(30*sizeof(char));
	
    clearsc();

	getchar();
	printf("\n\n\tDame tu nombre de usuario:  ");
	gets(NomArch);
    strcpy(usuario, NomArch);
	NomArch=strcat(NomArch,".txt");
    if(access(NomArch, F_OK) == 0)
    {
        printf("Este usuario ya esta registrado\n");
        sleep(1);
        clearsc();
        return;
    }
	printf("\n\tDame tu contrasena:  ");
    gets(c);
    ap=fopen(NomArch,"w");

	if(ap==NULL){
		printf("ERROR: No se pudo crear el archivo");
	}
	else{	
        //Usuario
        //Contraseña
		fprintf(ap, "C%s\n", c);
        printf("Registro exitoso!\n");	
	}
    fclose(ap);
    //
    free(NomArch);
    sleep(1);
    clearsc();
}

void generarMaletines(void)
{
    int r;
    srand(time(NULL));
    for(int i = 0; i < MAXSIZE; i++)
    {
        do
        {
            r = rand()%MAXSIZE;
            if(premios[r] != 0)
            {
                maletines[i].premio = premios[r];
                maletines[i].symbol = ' ';
                premios[r] = 0;
                break;
            }
        }while(1);

        maletines[i].num = i+1;
        maletines[i].abierto = false;
    }
}

int generarOferta(int miMaletin)
{
    float oferta = maletines[miMaletin-1].premio;
    int cont = 0;
    for(int i = 0; i < MAXSIZE; i++)
    {
        if(maletines[i].abierto == false){
            oferta += maletines[i].premio;
            cont++;
        }
    }
    oferta /= cont;
    //printf("%f\n", oferta);
    return (int)oferta;
}

int getPos(float dinero)
{
    for (int i = 0; i < MAXSIZE; i++)
    {
        if(dinero == maletines[i].premio){
            return i;
        }
    }
    return 0;
}

int inicarSes(char usr[30])
{
    FILE *fp;
    char cadena[30], contra[30], con[30];
    int bandera = 0;
    printf("\n\nDame tu nombre de usuario: ");
    scanf("%s",cadena);
    strcpy(usr, cadena);
    strcat(cadena,".txt");
    fp=fopen(cadena,"r");

    if(fp==NULL) //la condicion verifica que exista contenido (archivo) en el apuntador
    {
        printf("Tu usario no existe, vete a registrar\n");
        fclose(fp);
        sleep(1);
        clearsc();
        return 0;
    }
    else
    {
        printf("\nYa eres usuario de nuestro juego!!!\n");
		while (!feof(fp))
        {
        	fscanf(fp, "%s", contra);
            if(contra[0] == 'C')
            {
                memmove(contra, contra+1, strlen(contra));
                break;
            }   
        }
        do
        {
            printf("\nDame tu contrasena: ");
            scanf("%s",con);
            if (strcmp(contra,con)==0){
                bandera = 1;
            } 
            printf("Contrasena incorrecta, intentelo de nuevo.\n");
   	    } while(bandera == 0);
        fclose(fp);
        printf("Bienvenido de nuevo");
        return 1;
    }
}

void printBoard()
{
    printf("\n\n\n");
    printf("\t\t\t------     ------     ------     ------     ------\n");
    printf("\t\t\t| %*d |     | %*d |     | %*d |     | %*d |     | %*d |\n", 2,  maletines[0].num, 2, maletines[1].num, 2, maletines[2].num, 2, maletines[3].num, 2,  maletines[4].num);
    printf("\t\t\t------     ------     ------     ------     ------         0.01 [%c]   1,000     [%c]\n", maletines[getPos(0.01)].symbol, maletines[getPos(1000)].symbol);
    printf("\t\t\t     ------     ------     ------     ------               1    [%c]   5,000     [%c]\n", maletines[getPos(1)].symbol, maletines[getPos(5000)].symbol);
    printf("\t\t\t     | %*d |     | %*d |     | %*d |     | %*d |               5    [%c]   10,000    [%c]\n", 2, maletines[5].num, 2, maletines[6].num, 2, maletines[7].num, 2, maletines[8].num, maletines[getPos(5)].symbol, maletines[getPos(10000)].symbol);
    printf("\t\t\t     ------     ------     ------     ------               10   [%c]   25,000    [%c]\n", maletines[getPos(10)].symbol, maletines[getPos(25000)].symbol);
    printf("\t\t\t------     ------     ------     ------     ------         25   [%c]   50,000    [%c]\n", maletines[getPos(25)].symbol, maletines[getPos(50000)].symbol);
    printf("\t\t\t| %*d |     | %*d |     | %*d |     | %*d |     | %*d |         50   [%c]   75,000    [%c]\n", 2, maletines[9].num, 2, maletines[10].num, 2, maletines[11].num, 2, maletines[12].num, 2, maletines[13].num, maletines[getPos(50)].symbol, maletines[getPos(75000)].symbol);
    printf("\t\t\t------     ------     ------     ------     ------         75   [%c]   100,000   [%c]\n", maletines[getPos(75)].symbol, maletines[getPos(100000)].symbol);
    printf("\t\t\t     ------     ------     ------     ------               100  [%c]   200,000   [%c]\n", maletines[getPos(100)].symbol, maletines[getPos(200000)].symbol);
    printf("\t\t\t     | %*d |     | %*d |     | %*d |     | %*d |               200  [%c]   300,000   [%c]\n", 2, maletines[14].num, 2, maletines[15].num, 2, maletines[16].num, 2, maletines[17].num, maletines[getPos(200)].symbol, maletines[getPos(300000)].symbol);
    printf("\t\t\t     ------     ------     ------     ------               300  [%c]   400,000   [%c]\n", maletines[getPos(300)].symbol, maletines[getPos(400000)].symbol);
    printf("\t\t\t------     ------     ------     ------     ------         400  [%c]   500,000   [%c]\n", maletines[getPos(400)].symbol, maletines[getPos(500000)].symbol);
    printf("\t\t\t| %*d |     | %*d |     | %*d |     | %*d |     | %*d |         500  [%c]   750,000   [%c]\n", 2, maletines[18].num, 2, maletines[19].num, 2, maletines[20].num, 2, maletines[21].num, 2, maletines[22].num, maletines[getPos(500)].symbol, maletines[getPos(750000)].symbol);
    printf("\t\t\t------     ------     ------     ------     ------         750  [%c]   1,000,000 [%c]\n", maletines[getPos(750)].symbol, maletines[getPos(1000000)].symbol);
    printf("\t\t\t           ------     ------     ------           \n");
    printf("\t\t\t           | %*d |     | %*d |     | %*d |           \n", 2, maletines[23].num, 2, maletines[24].num, 2, maletines[25].num);
    printf("\t\t\t           ------     ------     ------           \n");
}

void printEstadisticas()
{
    clearsc();
    char usr[30], ch[30], usr2[30], x;
    int partidas = 0, total = 0;
    printf("\n\n\tDame tu nombre de usuario...\n\n");
    scanf("%s", usr);
    strcpy(usr2, usr);
    strcat(usr,".txt");
    FILE* ptr = fopen(usr,"r");

    if(ptr==NULL) //la condicion verifica que exista contenido (archivo) en el apuntador
    {
        printf("Tu usario no existe, vete a registrar\n");
        fclose(ptr);
        sleep(1);
        clearsc();
        return;
    }else{
        printf("\n\t%s\n", usr2);
        printf("\n\tHistorial de partidas: \n");
        while(!feof(ptr))
        {
            fscanf(ptr, "%s", ch);
            if(ch[0] == '$')
            {
                printf("\n\t%*d: %s", 3, partidas+1, ch);
                memmove(ch, ch+1, strlen(ch));
                total += atoi(ch);
                partidas++;
            }
        }
        printf("\n\n\tTotal partidas jugadas: %d\n", partidas);
        printf("\n\n\tTotal ganado: $%d\n", total);
        sleep(2);
        fclose(ptr);
        printf("\n\n\tPresione cualquier tecla para regresar al menu...\n");
        getchar();
        scanf("%c", &x);
    }

}

void printMaletin(int x)
{
    x--;
    printf("El maletin %d contiene: $%.2f y su estado abierto es: %d\n", maletines[x].num, maletines[x].premio, maletines[x].abierto);
}

void validateInput(int *x, int range)
{
    bool valid = false;
    do
    {   
        scanf("%d", x);
        for(int i = 1; i <= range; i++)
        {
            if(*x == i)
            {   
                valid = true;
                break;
            }
        }
        if(!valid)
        {   
            getchar();
            printf("Opcion no valida, intente de nuevo...\n");
        }
    } while (!valid);
}

void mainGame(void)
{
    clearsc();
    generarMaletines();
    int miMaletin, op, turnos = 6, ganancias, numMiMalet;
    char usrArch[30];

    ///////////Proceso de inicio////////////
    printf("\n\n\t\tBienvenido a Deal or no Deal, %s!!!!!\n\n\n\t\tPara comenzar, eliga su maletin...\n\n", usr);
    printBoard();
    validateInput(&miMaletin, 26);
    numMiMalet = maletines[miMaletin-1].num;
    maletines[miMaletin-1].num = 0;
    maletines[miMaletin-1].abierto = true;
    clearsc();

    //Proceso principal del juego
    for(int i = turnos; i >= 0; i--)
    {
        if(i == 4){continue;}
        for(int j = i+1; j > 0; j--)
        {
            clearsc();
            printf("\t\tElige %d maletines...\n\n", j);
            do
            {   
                printBoard();
                printf("\n\t\tQue maletin deseas abrir...\n");
                validateInput(&op, 26);
                if(maletines[op-1].abierto == true)
                {   
                    clearsc();
                    printf("\t\tEste maletin ya fue abierto, intente de nuevo...\n\n");
                }
                getchar();
            }while(maletines[op-1].abierto == true);   

            printf("\t\t      ____      \n");
	        printf("\t\t.____| __ |____.\n");
	        printf("\t\t|              |\n");
	        printf("\t\t|  %*.2f  |\n", 10, maletines[op-1].premio);
	        printf("\t\t|              |\n");
	        printf("\t\t|______________|\n");
            abrirMaletin(op);
            sleep(1);
        }

        ///////////Ofertar///////////
        clearsc();
        printf("\n\t\tUsted tiene una oferta del banco...\n\n");
        printf("\t\tLa oferta es: $%d\n\n", generarOferta(miMaletin));
        printf("\t\t¿Desea aceptarla?\n");
        printf("\n\t\t1......Si   2......No\n\n");
        validateInput(&op, 2);
        if(op == 1)
        {
            ganancias = generarOferta(miMaletin);
            break;
        }
    }
    clearsc();
    int restantes = 0;
    for(int i = 0; i < 26; i++)
    {
        if(maletines[i].num != 0){
            restantes++;
        }
    }

    //Si son los ultimos dos maletines, desicion final
    if (restantes == 2){
        int maletRest = 0;
        for(int i = 0; i < 26; i++)
        {
            if(maletines[i].abierto == 0){maletRest = maletines[i].num;}
        }
        printf("\n\tEs momento de la decision final.\n\n\tDesea quedarse con su maletin, o prefiere el restante.\n\n");
        printf("\n\tUsted puede ganar [%.2f] o [%.2f]\n", maletines[numMiMalet-1].premio, maletines[maletRest-1].premio);
        printf("\t\t      ____                ____      \n");
	    printf("\t\t.____| __ |____.    .____| __ |____.\n");
	    printf("\t\t|              |    |              |\n");
	    printf("\t\t|      %*d      |    |      %*d      |\n", 2, numMiMalet, 2, maletRest);
	    printf("\t\t|              |    |              |\n");
	    printf("\t\t|______________|    |______________|\n");

        printf("\n\n\t1.........Me quedo con el mio\t2.........Elijo el restante\n\n");

        validateInput(&op, 2);
        switch (op)
        {
        case 1:
            ganancias = (int)maletines[numMiMalet-1].premio;
            break;
        case 2:
            ganancias = (int)maletines[maletRest-1].premio;
            break;
        }
    }

    clearsc();
    printf("\n\n\n\t\tFELICIDADES!!!!!\n\n\t\tUsted se lleva a casa $%d\n\n", ganancias);


    //Registrar ganancias en el archivo
    int partidas = 0;
    strcpy(usrArch, usr);
    strcat(usrArch, ".txt");
    FILE* ptr = fopen(usrArch, "a+");
    fprintf(ptr, "\n$%d", ganancias);
    fclose(ptr);

    sleep(2);
    clearsc();
}

void mainMenu(int *op)
{
    printf("\t\t\t\t\t ______       ______.        ___        ___    \n");
	printf("\t\t\t\t\t | .__ \\     |  ____|       /   \\       | |   \n");
	printf("\t\t\t\t\t | |  \\ |    |  |_.        / /_\\ \\      | | \n");
	printf("\t\t\t\t\t | |   | |   |  __|       /  ___  \\     | |   \n");
	printf("\t\t\t\t\t | :__/ |    |  |___.    /  /   \\  \\    | |___. \n");
	printf("\t\t\t\t\t |_____/     |______|   /__/     \\__\\   |_____| \n\n");
	printf("\n");
	printf("\t\t\t\t\t\t   _______       _______       \n");
	printf("\t\t\t\t\t\t  /   _   \\     |   __  \\       \n");
	printf("\t\t\t\t\t\t /   / \\   \\    |  |__|  |     \n");
	printf("\t\t\t\t\t\t|   |   |   |   |   _   /     \n");
	printf("\t\t\t\t\t\t \\   \\_/   /    |  | \\  \\      \n");
	printf("\t\t\t\t\t\t  \\_______/     |__|  \\__\\     \n\n");
	printf("\n");
	printf("\t\t\t   ____   ____     _______        ______     ______.       ___       ___        \n");
	printf("\t\t\t  |    \\  |  |    /   _   \\       | .__ \\   |  ____|      /   \\      | |      \n");	
	printf("\t\t\t  |     \\ |  |   /   / \\   \\      | |  \\ |  |  |_.       / /_\\ \\     | |             \n");	
	printf("\t\t\t  |  |\\  \\|  |  |   |   |   |     | |  | |  |  __|      /  ___  \\    | |              \n");	
	printf("\t\t\t  |  | \\     |   \\   \\_/   /      | :../ |  |  |___.   /  /   \\  \\   | |___.           \n");	
	printf("\t\t\t  |__|  \\____|    \\_______/       |_____/   |______|  /__/     \\__\\  |_____|        \n");	
	printf("\n\n");
	printf("\t\t\t      ____                ____                ____                ____    \n");
	printf("\t\t\t.____| __ |____.    .____| __ |____.    .____| __ |____.    .____| __ |____.         \n");
	printf("\t\t\t|    Manuel    |    |  David Ariel |    |    Daniel    |    |  Alan Uriel  |         \n");
	printf("\t\t\t|  Rodriguez   |    |   Garduno    |    |    Escobar   |    |   Carrasco   |         \n");
	printf("\t\t\t|  Urdapilleta |    |    Lopez     |    |    Flores    |    |     Ruiz     |         \n");
	printf("\t\t\t|______________|    |______________|    |______________|    |______________|         \n");
    sleep(2);
    printf("\n\n\n\t\t\t\tEliga una opción...\n\n\n");
    printf("\t\t\t\t\t\t1............Jugar\n\n");
    printf("\t\t\t\t\t\t2............Registrarse\n\n");
    printf("\t\t\t\t\t\t3............Estadísticas\n\n");
    printf("\t\t\t\t\t\t4............Salir\n\n");
    validateInput(op, 4);
}

void intro(void)
{
    clearsc();
    printf("\n");
	printf("\t________      _____  ________.  ____    ___   ___              ___   ________    ____    ___    ____    _______           ________\n");
	printf("\t|  ._.  |     |   |  |  _____|  |   \\   |  |  \\  \\            /  /  |  _____|    |   \\   |  |   |   |   |      \\         /  ____  \\ \n");
	printf("\t|  |_| /_     |   |  |  |       |    \\  |  |   \\  \\          /  /   |  |         |    \\  |  |   |   |   |  __   \\       /  /    \\  \\\n");
	printf("\t|   ___  \\    |   |  |  | __    |     \\ |  |    \\  \\        /  /    |  |__.      |     \\ |  |   |   |   | |  \\   \\     /  /      \\  \\\n");
	printf("\t|  |   \\  \\   |   |  |   __|    |  |\\  \\|  |     \\  \\      /  /     |   __|      |  |\\  \\|  |   |   |   | |   |   |   |  |        |  |\n");
	printf("\t|  |    |  |  |   |  |  |       |  | \\     |      \\  \\    /  /      |  |         |  | \\     |   |   |   | |__/   /     \\  \\      /  /\n");
	printf("\t|  |___/  /   |   |  |  |____   |  |  \\    |       \\  \\__/  /       |  |____     |  |  \\    |   |   |   |       /       \\  \\____/  /\n");
	printf("\t|________/    |___|  |_______|  |__|   \\___|        \\______/        |_______|    |__|   \\___|   |___|   |______/         \\________/ \n\n\n");
    sleep(3);
    clearsc();
}