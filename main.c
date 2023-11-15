#include <stdio.h>
#include <stdlib.h>

struct Partido{
    int partido;
    int votos_presidente;
    int votos_gobernador;
};

struct Mesa{
    int num_mesa;
    struct Partido partidos[5];
    int votos_en_blanco;
    int votos_impugnados;
};

int cargarMesa(){
    FILE *archivo = fopen("mesas.bin","a+b");
    if (archivo == NULL){
        printf("\nHubo un error\n");
        return -1;
    }
    struct Mesa mesa1;
    int bandera, comprobar;
    do {
        printf("ingrese el n%cmero de mesa\n",163);
        do
        {
            scanf("%d",&mesa1.num_mesa);
            comprobar=comprobar_mesa(&mesa1);
        }
        while(comprobar==1);
        printf("CARGAR LA CANTIDAD DE VOTOS PARA CADA PARTIDO\n");
        for (int i = 0; i < 5; ++i) {
            printf("Ingrese el nombre del partido\n");
            printf("1_ La Libertad Avanza\n\v2_ Uni%cn por la patria\n\v3_Juntos por el cambio\n\v4_Frente de izquierda\n\v5_Hacemos por nuestro pa%cs\n",162,161);
            do {
                scanf("%d", &mesa1.partidos[i].partido);
                if(mesa1.partidos[i].partido < 1 || mesa1.partidos[i].partido > 5){
                    printf("Ingrese una opci%cn correcta\n",162);
                }
            } while (mesa1.partidos[i].partido < 1 || mesa1.partidos[i].partido > 5);

            printf("ingrese la cantidad de votos para presidente\n");
            scanf("%d",&mesa1.partidos[i].votos_presidente);

            printf("Ingrese la cantidad de votos para gobernador\n");
            scanf("%d",&mesa1.partidos[i].votos_gobernador);
        }
        printf("Ingrese la cantidad de votos en blancos que tuvo la mesa\n");
        scanf("%d",&mesa1.votos_en_blanco);

        printf("ingrese la cantidad de votos impugnados que tuvo la mesa\n");
        scanf("%d",&mesa1.votos_impugnados);

        fwrite(&mesa1, sizeof(mesa1),1,archivo);
        fclose(archivo);
        printf("Desea cargar otra mesa?\n1_SI\n2_NO");
        scanf("%d",&bandera);
    } while (bandera==1);


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
            printf("\nLa mesa ya existe\n");
            fclose(archivo);
            return 1;
        }
    }
    fclose(archivo);
    return 0;
}

int main() {
    int opcion;
    do {
        printf("Bienvenido al sistema de votos\nPor favor ingrese la accion que desea realizar\n");
        printf("1_Cargar mesa\n4_Salirz");
        scanf("%d",&opcion);
        if(opcion==1)cargarMesa();
    } while (opcion != 4);
    return 0;
}
