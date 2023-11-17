#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    struct Mesa mesa1;
    struct Nombres nom;
    int bandera, comprobar;
    do {
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
        system("cls");
        printf("ingrese el n%cmero de mesa\n",163);
        scanf("%d",&mesa1.num_mesa);
        comprobar=comprobar_mesa(&mesa1);
        if(comprobar!=1)
        {
            printf("CARGAR LA CANTIDAD DE VOTOS PARA CADA PARTIDO\n");
            rewind(a2);
            while(fread(&nom,sizeof(struct Nombres),1,a2))
            {
                printf("\nINGRESO PARA PARTIDO: %s\n",nom.frente);
                printf("Ingrese la cantidad de votos para el candidato a presidente: %s\n",nom.presi);
                scanf("%d",&mesa1.partidos[nom.id].votos_presidente);

                printf("Ingrese la cantidad de votos para el candidato a gobernador: %s\n",nom.gob);
                scanf("%d",&mesa1.partidos[nom.id].votos_gobernador);
                mesa1.partidos[nom.id].partido=nom.id;
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
        fflush(stdin);
        fclose(archivo);
        fclose(a2);
    } while (bandera==1);
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
    fflush(stdin);
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

int cargar_nompartido()
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

int mostrar_mesas_ordenadas()
{
    system("cls");
    FILE *archivo = fopen("mesas.bin","rb");
    if (archivo == NULL){
        printf("\nHubo un error\n");
        return -1;
    }
    FILE *a2 = fopen("nombres.bin","rb");
    if (a2 == NULL){
        printf("\nHubo un error\n");
        return -1;
    }
    FILE *data = fopen("Data.bin","rb");
    if(data == NULL){
        printf("Hubo un error\n");
        return -1;
    }
    admin adm;
    fread(&adm,sizeof(adm),1,data);
    int o=adm.cantp;
    struct Mesa mesa;
    struct Nombres nom;
    int tam=0;
    rewind(archivo);
    while(fread(&mesa,sizeof(struct Mesa),1,archivo))
    {
        tam++;
    }
    int vec[tam];
    rewind(archivo);
    for(int j=0;j<tam;j++)
    {
        fread(&mesa,sizeof(struct Mesa),1,archivo);
        vec[j]=mesa.num_mesa;
    }
    ordenar(vec,tam);
    rewind(archivo);
    printf("\n%d\n",adm.cantp);
    for(int k=0;k<tam;k++)
    {

        while(fread(&mesa,sizeof(struct Mesa),1,archivo))
        {
            if(mesa.num_mesa==vec[k])
            {
                rewind(a2);
                printf("-------------------------------------------------------------------------------------------------------\n");
                printf("| %-8s | %-20s | %-20s | %-20s | %-8s | %-8s |\n", "N. MESA", "PARTIDO", "PRESIDENTE","GOBERNADOR", "VOTOS P.", "VOTOS G.");
                printf("-------------------------------------------------------------------------------------------------------\n");
                for (int i = 0; i < o; ++i) {
                    fread(&nom, sizeof(struct Nombres), 1, a2);
                    printf("| %-8d | %-20s | %-20s | %-20s | %-8d | %-8d |\n", mesa.num_mesa, nom.frente, nom.presi, nom.gob, mesa.partidos[i].votos_presidente, mesa.partidos[i].votos_gobernador);
                }
                printf("-------------------------------------------------------------------------------------------------------\n");
            }
        }
        rewind(archivo);
    }
    system("pause");
    system("cls");
    fclose(archivo);
    fclose(a2);
    fclose(data);
}

void cambiar(int *xp, int *yp) {
    int temp = *xp;
    *xp = *yp;
    *yp = temp;
}

void ordenar(int arr[], int n) {
    for (int i = 0; i < n-1; i++)
        for (int j = 0; j < n-i-1; j++)
            if (arr[j] > arr[j+1])
                cambiar(&arr[j], &arr[j+1]);
}

int mostrar_mesa_id(int num_mesa){
    FILE *mesas = fopen("mesas.bin","rb");
    if(mesas == NULL){
        printf("Hubo un error\n");
        return -1;
    }
    FILE *nombres = fopen("nombres.bin","rb");
    if(nombres == NULL){
        printf("Hubo un error\n");
        return -1;
    }
    FILE *data = fopen("Data.bin","rb");
    if(data == NULL){
        printf("Hubo un error\n");
        return -1;
    }
    struct Mesa mesa;
    admin adm;
    int mesa_encontrada = 0;
    fread(&adm,sizeof(adm),1,data);
    int n = adm.cantp;
    int k=n-1;
    struct Nombres nombres_array[n];
    int vec[n];
    while (fread(&mesa, sizeof(mesa), 1, mesas)) {
        if (mesa.num_mesa == num_mesa) {
            mesa_encontrada = 1;
            int votos_totales_presi = 0;

            for (int i = 0; i < n; ++i) {
                votos_totales_presi += mesa.partidos[i].votos_presidente;
                vec[i]=mesa.partidos[i].votos_presidente;
            }
            ordenar(vec,n);

            fread(nombres_array, sizeof(struct Nombres), n, nombres);
            printf("-------------------------------------------------------------------------------------\n");
            printf("| %-8s | %-20s | %-20s | %-8s | %-13s |\n", "N. MESA", "PARTIDO", "PRESIDENTE", "VOTOS P.", "PORCENTAJE");
            printf("-------------------------------------------------------------------------------------\n");

            for (int i = 0; i < n; ++i) {
                if (vec[k]==mesa.partidos[i].votos_presidente){
                    float porcentaje_votado = (mesa.partidos[i].votos_presidente * 100.0) / votos_totales_presi;
                    printf("| %-8s | %-20s | %-20s | %-8d | %-12.2f%c |\n",
                           "", nombres_array[i].frente, nombres_array[i].presi,
                           mesa.partidos[i].votos_presidente, porcentaje_votado,37);
                    k--;
                    i=-1;
                }
            }
            printf("-------------------------------------------------------------------------------------\n");

            int votos_totales_gob = 0;

            for (int i = 0; i < n; ++i) {
                votos_totales_gob += mesa.partidos[i].votos_gobernador;
                vec[i]=mesa.partidos[i].votos_gobernador;
            }
            ordenar(vec,n);
            k=n-1;
            fread(nombres_array, sizeof(struct Nombres), n, nombres);
            printf("-------------------------------------------------------------------------------------\n");
            printf("| %-8s | %-20s | %-20s | %-8s | %-13s |\n", "N. MESA", "PARTIDO", "GOBERNADOR", "VOTOS G.", "PORCENTAJE");
            printf("-------------------------------------------------------------------------------------\n");


            for (int i = 0; i < n; ++i) {
                if (vec[k]==mesa.partidos[i].votos_gobernador) {
                    float porcentaje_votado = (mesa.partidos[i].votos_gobernador * 100.0) / votos_totales_gob;
                    printf("| %-8s | %-20s | %-20s | %-8d | %-12.2f%c |\n",
                           "", nombres_array[i].frente, nombres_array[i].gob,
                           mesa.partidos[i].votos_gobernador, porcentaje_votado,37);
                    k--;
                    i=-1;
                }
            }
            printf("-------------------------------------------------------------------------------------\n");
            fflush(stdin);
        }
    }
    if(mesa_encontrada != 1){
        printf("\v\vNO SE ENCONTR%c LA MESA N%cMERO: %d\n",224,233,num_mesa);
    }
    system("pause");
    system("cls");
    fclose(mesas);
    fclose(nombres);
    fclose(data);
}

void mostrar_votos_totales()
{
    FILE* mesa= fopen("mesas.bin","rb");if (mesa==NULL) printf("\n El archivo no existe mesas.bin\n");
    FILE* data=fopen("Data.bin","rb"); if (data==NULL) printf("\nEl archivo Data.bin no existe\n");
    FILE* nombres=fopen("nombres.bin","rb"); if (nombres==NULL) printf ("\nEl archivo nombres.bin no existe\n");
    struct Mesa recorrer;
    struct Partido acumulador;
    struct Nombres nom;
    admin adm;
    int n, votos_total_presi, votos_total_gob, i=0;
    fread(&adm, sizeof(admin), 1, data);
    n=adm.cantp;
    while(fread(&recorrer, sizeof(struct Mesa), 1, mesa) == 1)
    {
        printf("\nBlanco:%d, impug:%d\n",recorrer.votos_en_blanco, recorrer.votos_impugnados);
        for(i=0; i<n; i++)
        {
            votos_total_presi=votos_total_presi+recorrer.partidos[i].votos_presidente;
            printf("\nproceso:%d i=%i\n",votos_total_presi, i);
        }
        votos_total_presi=votos_total_presi+recorrer.votos_en_blanco+recorrer.votos_impugnados;

    }
    printf("FINAL!!!!%d!!!!",votos_total_presi);
    system("pause");
    fclose(mesa);
    fclose(nombres);
    fclose(data);
}

void Modificar_mesa(){
    system("cls");
    FILE *a2 = fopen("nombres.bin","rb");
    if(a2 == NULL){
        printf("Hubo un error\n");
        return -1;
    }
    long a;
    int comp;
    admin adm;
    struct Mesa mesa;
    struct Nombres nom;
    int nummesa;
    int opc,control=0;
    printf("\nINGRESE LA CLAVE MAESTRA\n");
    scanf("%s",&adm.contrasenia);
    comp=comparar_contrasenia(adm.contrasenia);
    if(comp!=0)
    {
        printf("\nNO TIENE PERMITIDO HACER ESTA ACCION\n");
        system("pause");
        return 0;
    }
    FILE *archivo=fopen("mesas.bin", "r+b");
    if(archivo==NULL)
    {
        printf("ARCHIVO NO ENCONTRADO");
        exit(-1);
    }
    system("cls");
    printf("\n INGRESE EL NUMERO DE MESA A MODIFICAR : ");
    fflush(stdin);
    scanf("%d", &nummesa);
    int comparar;
    rewind(archivo);
    while(fread(&mesa, sizeof(struct Mesa), 1 , archivo)){
        if(mesa.num_mesa==nummesa){
            a=ftell(archivo);
            fflush(stdin);
            printf("\nMESA ENCONTRADA\n");
            printf("CARGAR LA CANTIDAD DE VOTOS PARA CADA PARTIDO\n");
            rewind(a2);
            while(fread(&nom,sizeof(struct Nombres),1,a2))
            {
                printf("\nINGRESO PARA PARTIDO: %s\n",nom.frente);
                printf("Ingrese la cantidad de votos para el candidato a presidente: %s\n",nom.presi);
                scanf("%d",&mesa.partidos[nom.id].votos_presidente);

                printf("Ingrese la cantidad de votos para el candidato a gobernador: %s\n",nom.gob);
                scanf("%d",&mesa.partidos[nom.id].votos_gobernador);
                mesa.partidos[nom.id].partido=nom.id;
            }
            printf("Ingrese la cantidad de votos en blancos que tuvo la mesa\n");
            scanf("%d",&mesa.votos_en_blanco);

            printf("ingrese la cantidad de votos impugnados que tuvo la mesa\n");
            scanf("%d",&mesa.votos_impugnados);
            rewind(archivo);
            fseek(archivo,a-sizeof(struct Mesa),0);
            fwrite(&mesa, sizeof(struct Mesa), 1, archivo);
            fclose(archivo);
            printf("\nMESA MODIFICADA EXITOSAMENTE\n");
            system("pause");
            control=1;
        }
    }
    if(control==0)
    {
        printf("\nLA MESA NO SE ENCUENTRA INGRESADA\n");
        system("pause");
    }

    fclose(a2);
}

int comparar_contrasenia(char* x){
    int a;
    admin admin;
    FILE *t=fopen("Data.bin", "rb");
    rewind(t);
    fread(&admin,sizeof(admin), 1 , t);
    if(strcmp(x, admin.contrasenia)==0){
        a=0;
    }else{
        a=1;
    }
    fclose(t);
    return a;
}

void cambiar_contrasenia(){
    FILE *t=fopen("Data.bin", "r+b");
    char aux[10],aux2[10];
    admin admin;
    int control=0;
    while(control==0)
    {
        system("cls");
        printf("Ingrese la contrase%ca actual : ",164);
        fflush(stdin);
        scanf("%s",&aux);
        rewind(t);
        fread(&admin,sizeof(admin),1,t);
        strcpy(aux2,admin.contrasenia);
        int num=admin.cantp;
        if((strcmp(aux,aux2))==0)
        {
            printf("Contrase%ca verificada",164);
            printf("\n");
            system("PAUSE");
            fflush(stdin);
            printf("\n----------------------\n");
            printf("\nIngrese nueva contrase%ca : ",164);
            scanf("%s",&aux2);
            while(strlen(aux2) !=8){
                system("cls");
                printf("\nDefina una contrase%ca de 8 caracteres:",164);
                fflush(stdin);
                scanf("%s",&aux2);
            }
            strcpy(admin.contrasenia,aux2);
            admin.cantp=num;
            rewind(t);
            fwrite(&admin,sizeof(admin),1,t);
            printf("\nContrase%ca modificada con exito\n",164);
            printf("\n");
            system("pause");
            system("cls");
            control=1;

        }else
        {
            printf("\nLa contrase%ca actual es incorrecta\nIntente nuevamente\n",164);
            system("pause");
            control=1;
        }

    }
    fclose(t);
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
        system("cls");
        printf("Bienvenido al sistema de votos\nPor favor ingrese la accion que desea realizar\n");
        printf("1_Cargar mesa\n2_Mostrar Todas las mesas ordenadas\n3_Buscar una mesa\n4_Mostrar los votos totales\n5_Modificar mesa\n6_Modificar clave maestra\n_Salir\n");
        scanf("%d",&opcion);
        if(opcion==1)cargarMesa();
        if(opcion==2)mostrar_mesas_ordenadas();
        if(opcion==3){
            int num_mesa;
            printf("Ingrese el n%cmero de mesa que desea buscar:\n",163);
            scanf("%d",&num_mesa);
            system("cls");
            mostrar_mesa_id(num_mesa);
        }
        if(opcion==4) mostrar_votos_totales();
        if(opcion==5) Modificar_mesa();
        if(opcion==6) cambiar_contrasenia();
    } while (opcion != 7);
    return 0;
}
