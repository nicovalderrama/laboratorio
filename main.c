#include <stdio.h>
#include <stdlib.h>

typedef struct{
	char contrasenia[10];
    int cantp;
}admin;

struct Nombres{
    int id;
    char frente[100];
    char presi[100];
    char gob[100];
};

struct Partido{
    int partido;
    int votos_presidente;
    int votos_gobernador;
};

struct Mesa{
    int num_mesa;
    struct Partido partidos[10];
    int votos_en_blanco;
    int votos_impugnados;
};

int cargarMesa(){
    FILE *archivo = fopen("mesas.bin","a+b");
    if (archivo == NULL){
        printf("\nHubo un error\n");
        return -1;
    }
    FILE *a2 = fopen("nombres.bin","rb");
    if (a2 == NULL){
        printf("\nHubo un error\n");
        return -1;
    }
    struct Mesa mesa1;
    struct Nombres nom;
    int bandera, comprobar;
    do {
        system("cls");
        printf("ingrese el n%cmero de mesa\n",163);
        scanf("%d",&mesa1.num_mesa);
        comprobar=comprobar_mesa(&mesa1);
        if(comprobar!=1)
        {
            printf("CARGAR LA CANTIDAD DE VOTOS PARA CADA PARTIDO\n");
        /*for (int i = 0; i < 5; ++i) {
            printf("Ingrese el nombre del partido\n");
            printf("1_ La Libertad Avanza\n\v2_ Uni%cn por la patria\n\v3_Juntos por el cambio\n\v4_Frente de izquierda\n\v5_Hacemos por nuestro pa%cs\n",162,161);
            do {
                scanf("%d", &mesa1.partidos[i].partido);
                if(mesa1.partidos[i].partido < 1 || mesa1.partidos[i].partido > 5){
                    printf("Ingrese una opci%cn correcta\n",162);
                }
            } while (mesa1.partidos[i].partido < 1 || mesa1.partidos[i].partido > 5);*/
            rewind(a2);
            while(fread(&nom,sizeof(struct Nombres),1,a2))
            {
                printf("\nINGRESO DEL PARTDIDO: %s\n",nom.frente);
                printf("Ingrese la cantidad de votos para el candidato a presidente: %s\n",nom.presi);
                scanf("%d",&mesa1.partidos[nom.id].votos_presidente);

                printf("Ingrese la cantidad de votos para el candidato a gobernador: %s\n",nom.gob);
                scanf("%d",&mesa1.partidos[nom.id].votos_gobernador);
            }
            printf("Ingrese la cantidad de votos en blancos que tuvo la mesa\n");
            scanf("%d",&mesa1.votos_en_blanco);

            printf("ingrese la cantidad de votos impugnados que tuvo la mesa\n");
            scanf("%d",&mesa1.votos_impugnados);

            fseek(archivo,0,2);
            fwrite(&mesa1, sizeof(mesa1),1,archivo);

            printf("\nDesea cargar otra mesa?\n1_SI\n2_NO\n");
            scanf("%d",&bandera);
        }
        else
        {
            printf("\nLA MESA YA FUE CARGADA ANTERIORMENTE\n\n");
            bandera=0;
            system("pause");
        }
    } while (bandera==1);
    fclose(archivo);
    fclose(a2);
    system("cls");
    return 0;
}

int comprobar_mesa(struct Mesa *aux)
{
    FILE *archivo = fopen("mesas.bin", "rb");
    if (archivo == NULL)
    {
        printf("Error al abrir el archivo");
        return 1;
    }
    struct Mesa mesa_actual;
    fseek(archivo, 0, 0);
    while (fread(&mesa_actual, sizeof(struct Mesa), 1, archivo) == 1)
    {
        if (mesa_actual.num_mesa == aux->num_mesa)
        {
            fclose(archivo);
            return 1;//existe
        }
    }
    fclose(archivo);
    return 0;
}

void UnicaVez(){
	admin admin;
	char aux[10];
	int n=0;
	FILE* t;
	t= fopen("Data.bin", "ab");
	if(t==NULL)
	{
		printf("ARCHIVO: No encontrado\n");
		exit (-1);
	}
	printf("EL PROGRAMA PERMITE EL INGRESO DE UNA CLAVE MAESTRA Y LA CANTIDAD DE PARTIDOS QUE USARA EL PROGRAMA, UNA UNICA VEZ\n\n");
	printf("Debe ingresar una clave de 8 digitos\n\n");
	printf("Ingrese la clave maestra:");
	fflush(stdin);
	scanf("%s",&aux);
	while(strlen(aux) !=8){
		system("cls");
		printf("Defina una contrase%ca valida:",164);
		fflush(stdin);
		scanf("%s",&aux);
		fflush(stdin);
	}
	strcpy(admin.contrasenia,aux);
	printf("\nIngrese la cantidad de partidos con la que se manejara el programa:");
	scanf("%d",&admin.cantp);
	rewind(t);
	fwrite(&admin,sizeof(admin),1,t);
	fclose(t);
	while(n<admin.cantp)
    {
        printf("\nIngrese los datos del Partido %d",n+1);
        cargar_nompartido();
        n++;
    }
	system("pause");

}

void cargar_nompartido()
{
    FILE *archivo = fopen("nombres.bin","a+b");
    if (archivo == NULL){
        printf("\nHubo un error\n");
        return -1;
    }
    struct Nombres nombre;
    int n=0;

    rewind(archivo);
    while(fread(&nombre, sizeof(struct Nombres), 1, archivo))
    {
        n++;
    }
    printf("\nIngrese nombre del Partido Politico\n");
    fflush(stdin);
    gets(nombre.frente);
    printf("\nIngrese nombre del Candidato a Presidente\n");
    fflush(stdin);
    gets(nombre.presi);
    printf("\nIngrese nombre del Candidato a Gonernador\n");
    fflush(stdin);
    gets(nombre.gob);
    nombre.id=n;
    fseek(archivo,0,2);
    fwrite(&nombre,sizeof(struct Nombres),1,archivo);
    fclose(archivo);
}

int main() {
    FILE* t = fopen("Data.bin", "r+b");
	if(t==NULL){
		fclose(t);
		UnicaVez();
	}
	system("cls");
    int opcion;
    do {
        printf("Bienvenido al sistema de votos\nPor favor ingrese la accion que desea realizar\n");
        printf("1_Cargar mesa\n4_Salir\n");
        scanf("%d",&opcion);
        if(opcion==1)cargarMesa();
    } while (opcion != 4);
    return 0;
}
