#include <stdlib.h>
#include <stdio.h>

#define	MAX_PROCESOS 100

// Estructura de los procesos
struct str_proc{
	int numero;
	int arribo;
	int rafaga;
	int estado; //0: no existe 1: listo, 2: ejecutando (no se va a usar), 3: finalizado
};

typedef struct str_proc proceso;

//Arreglo que contiene a cada proceso
struct str_procAT{
	proceso arr_procesos[MAX_PROCESOS];
	int tope;
};

typedef struct str_procAT AT_Procesos; // arreglo con tope de procesos

/*
//Posible uso para guardar procesos en estado listo 
struct str_procListoAT{
	proceso arr_procesos[MAX_PROCESOS];
	int tope;
};
typedef struct str_procListoAT AT_ProcesosListos;
*/

/*
//Uso solo de testing para verificar procesos y estados
void imprimirProcesos(proceso p){
    printf("------------------------\n");
	printf("Num Proceso: %d\n", p.numero);
	printf("Arribo: %d\n", p.arribo);
	printf("Rafaga: %d\n", p.rafaga);
	printf("Estado: %d\n", p.estado);
    printf("------------------------\n");
}
*/
//Imprime uso de CPU; procesos en ejecucion en un tiempo determinado.
bool imprimirUsoCpu(AT_Procesos &atp){
	int i = 0;
	bool ejecuta = false;
	bool finProceso = false;
	while (i < atp.tope && ejecuta != true){
		//Busca primer proceso en cola en estado listo y lo ejecuta.
		if (atp.arr_procesos[i].estado == 1){
			printf("Ejecutando P%d\n",atp.arr_procesos[i].numero);
			ejecuta = true;
			atp.arr_procesos[i].rafaga--;
			//Si el proceso ya se consumio, pasa a finalizado.			
			if(atp.arr_procesos[i].rafaga == 0){
				atp.arr_procesos[i].estado = 3;
				finProceso = true;
			}
		}
		i++;
	}
	//Si no habia proceso para ejecutar, se muestra estado CPU LIBRE
	if(ejecuta !=true){
		printf("CPU Libre\n");
	}
	return finProceso;
}

//Ordena procesos segun la rafaga
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
	p.numero = 1;
	int num_proc, procesosTerminados = 0, arribo, rafaga, tiempoCPU = 0, tiempo = 0;
	char enter;
	
	//Solicita el ingreso de los procesos y los carga en el array
	printf("Seleccione cantidad de procesos a ejecutar: ");
	scanf("%d%c",&num_proc,&enter);
	printf("\n");
	while (num_proc > 0){
		printf("Proceso %d:\n\n",p.numero);
		printf("Ingrese el tiempo de arribo: ");
		scanf("%d%c",&p.arribo,&enter);
		printf("Ingrese el tiempo de rafaga: ");
		scanf("%d%c",&p.rafaga,&enter);
		printf("\n");
		p.estado = 0;
		procesos.arr_procesos[procesos.tope] = p;
		procesos.tope++;
		p.numero++;
		num_proc--;
		tiempoCPU += p.rafaga;
	}
	//Por cada segundo recorre el array de procesos para ponerlos en estado listo

	while(procesosTerminados != procesos.tope){ // resolver de otra forma cuanto tiempo revisar procesos.
		for(int j=0;j<procesos.tope;j++){
			if(procesos.arr_procesos[j].arribo == tiempo && procesos.arr_procesos[j].estado != 3){
				procesos.arr_procesos[j].estado = 1;
			}
		}
		//ordena procesos por rafaga, y ejecuta los listos
		ordenaRafaga(procesos);
		printf("----------------\n");
		printf("%d: ",tiempo);		
		if(imprimirUsoCpu(procesos)){
			procesosTerminados++;
		}
		tiempo++;
	}
}
