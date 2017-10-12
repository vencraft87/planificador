#include <stdlib.h>
#include <stdio.h>

#define	MAX_PROCESOS 20

struct str_proc{
	int numero;
	int arribo;
	int rafaga;

};

typedef struct str_proc proceso;

struct str_procAT{
	proceso arr_procesos[MAX_PROCESOS];
	int tope;
};

typedef struct str_procAT AT_Procesos; // arreglo con tope de procesos

void imprimirProcesos(proceso p){
    printf("------------------------\n");
	printf("Num Proceso: %d\n", p.numero);
	printf("Arribo: %d\n", p.arribo);
	printf("Rafaga: %d\n", p.rafaga);
    printf("------------------------\n");
}

main(){
	AT_Procesos procesos;
	procesos.tope = 0;
	proceso p;
	
	
	int num_proc, arribo, rafaga;
	char enter;
	printf("Seleccione cantidad de procesos a ejecutar: ");
	scanf("%d%c",&num_proc,&enter);
	printf("\n");
	
	int i=1;
	while (num_proc > 0){
		p.numero = i;
		printf("Proceso %d:\n\n",i++);
		printf("Ingrese el tiempo de arribo: ");
		scanf("%d%c",&p.arribo,&enter);
		printf("Ingrese el tiempo de rafaga: ");
		scanf("%d%c",&p.rafaga,&enter);
		printf("\n");
		
		procesos.arr_procesos[procesos.tope] = p;
		procesos.tope++;
		
		num_proc--;

	}
	
    printf("------------------------\n");
    for (int i=0; i < procesos.tope; i++){
        imprimirProcesos(procesos.arr_procesos[i]);
        printf("------------------------\n");
    }
}
