#include <stdlib.h>
#include <stdio.h>

#define	MAX_PROCESOS 20

struct str_proc{
	int numero;
	int arribo;
	int rafaga;
	int estado; //0: no existe 1: listo, 2: ejecutando, 3: finalizado
};

typedef struct str_proc proceso;

struct str_procAT{
	proceso arr_procesos[MAX_PROCESOS];
	int tope;
};

typedef struct str_procAT AT_Procesos; // arreglo con tope de procesos

struct str_procListoAT{
	proceso arr_procesos[MAX_PROCESOS];
	int tope;
};

typedef struct str_procListoAT AT_ProcesosListos;

void imprimirProcesos(proceso p){
    printf("------------------------\n");
	printf("Num Proceso: %d\n", p.numero);
	printf("Arribo: %d\n", p.arribo);
	printf("Rafaga: %d\n", p.rafaga);
	printf("Estado: %d\n", p.estado);
    printf("------------------------\n");
}

void imprimirProcesoEnUso(AT_Procesos &atp){
	int i = 0;
	bool ejecuta = false;
	while (i < atp.tope && ejecuta != true){
		if (atp.arr_procesos[i].estado == 1){
			printf("Ejecutando P%d\n",atp.arr_procesos[i].numero);
			ejecuta = true;
			atp.arr_procesos[i].rafaga--; 
			if(atp.arr_procesos[i].rafaga == 0){
				atp.arr_procesos[i].estado = 3;
			}
		}
		i++;
	}
	if(ejecuta !=true){
		printf("CPU Libre\n");
	}
}

void ordenaRafaga(AT_Procesos &atp){
	proceso temp;
	for(int i=0; i<atp.tope-1; i++){
		for(int j=0;j<atp.tope-1;j++){
			if(atp.arr_procesos[j].rafaga>atp.arr_procesos[j+1].rafaga){
				temp = atp.arr_procesos[j];
				atp.arr_procesos[j] = atp.arr_procesos[j+1];
				atp.arr_procesos[j+1] = temp;
			}
		}
	}
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
		p.estado = 0;
		procesos.arr_procesos[procesos.tope] = p;
		procesos.tope++;
		num_proc--;
	}
	
	for(int i=0;i < 50; i++){ // resolver de otra forma cuanto tiempo revisar procesos.
		for(int j=0;j<procesos.tope;j++){
			if(procesos.arr_procesos[j].arribo == i && procesos.arr_procesos[j].estado != 3){
				procesos.arr_procesos[j].estado = 1;
			}
		}
		ordenaRafaga(procesos);
		printf("----------------\n");
		printf("%d: ",i);		
		imprimirProcesoEnUso(procesos);
	}
}
