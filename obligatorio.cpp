#include <stdlib.h>
#include <stdio.h>

#define	MAX_PROCESOS 20

// Estructura de los procesos
struct str_proc{
	int numero;
	int arribo;
	int rafaga;
	int estado; //0: no existe 1: listo, 2: ejecutando (no se va a usar), 3: finalizado
	int tiempoEspera;
	int tiempoRetorno;
};

typedef struct str_proc proceso;

//Arreglo que contiene a cada proceso
struct str_procAT{
	proceso arr_procesos[MAX_PROCESOS];
	int tope;
};

typedef struct str_procAT AT_Procesos; // arreglo con tope de procesos

void imprimirProcesos(AT_Procesos atp){
	proceso temp;
	//Ordenar procesos por numero
	for(int i=0; i<atp.tope-1; i++){
		for(int j=0;j<atp.tope-1;j++){
			if(atp.arr_procesos[j].numero>atp.arr_procesos[j+1].numero){
				temp = atp.arr_procesos[j];
				atp.arr_procesos[j] = atp.arr_procesos[j+1];
				atp.arr_procesos[j+1] = temp;
			}
		}
	}
	for(int i=0; i<atp.tope; i++){
		printf("------------------------\n");
		printf("Proceso %d:\n\n", atp.arr_procesos[i].numero);
		printf("Tiempo de espera: %d\n", atp.arr_procesos[i].tiempoEspera);
		printf("Tiempo de retorno: %d\n", atp.arr_procesos[i].tiempoRetorno);
		printf("------------------------\n");	
	}
}
  
//Imprime uso de CPU; procesos en ejecucion en un tiempo determinado.
bool imprimirUsoCpu(AT_Procesos &atp){
	int i=0;
	bool ejecuta = false;
	bool finProceso = false;
	int nombreProcEjecutando;
	while (i < atp.tope && ejecuta != true){
		//Busca primer proceso en cola en estado listo y lo ejecuta.
		if (atp.arr_procesos[i].estado == 1 || atp.arr_procesos[i].estado == 2){
			printf("Ejecutando P%d\n",atp.arr_procesos[i].numero);
			atp.arr_procesos[i].estado = 2;
			ejecuta = true;
			atp.arr_procesos[i].rafaga--;
			nombreProcEjecutando = atp.arr_procesos[i].numero;
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
	//Recorro procesos para sumar tiempo de espera y retorno, vuelvo a listo  
	//el proceso anterior que se estuviera ejecutando 
	for(int i=0; i<atp.tope; i++){
		if((atp.arr_procesos[i].estado == 2) && (nombreProcEjecutando != atp.arr_procesos[i].numero)){
			atp.arr_procesos[i].estado = 1;
			atp.arr_procesos[i].tiempoEspera++;
			atp.arr_procesos[i].tiempoRetorno++;
		}else if(atp.arr_procesos[i].estado == 1){
			atp.arr_procesos[i].estado = 1;
			atp.arr_procesos[i].tiempoEspera++;
			atp.arr_procesos[i].tiempoRetorno++;
		}else if(atp.arr_procesos[i].estado == 2){
			atp.arr_procesos[i].tiempoRetorno++;
		}	

	}
	return finProceso;
}

//Asegura que el proceso ejecute FCFS en caso de competencia de Burst Time
void ordenaProcesos(AT_Procesos &atp){
	proceso temp;
	for(int i=0; i<atp.tope-1; i++){
		for(int j=0;j<atp.tope-1;j++){
			if(atp.arr_procesos[j].arribo>atp.arr_procesos[j+1].arribo){
				temp = atp.arr_procesos[j];
				atp.arr_procesos[j] = atp.arr_procesos[j+1];
				atp.arr_procesos[j+1] = temp;
			}
		}
	}
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
//Devuelve resultado final Tiempo de Espera Promedio
float promEspera(AT_Procesos atp){
	float sumaTiemEspera;
	for(int i=0;i<atp.tope;i++){
		sumaTiemEspera += atp.arr_procesos[i].tiempoEspera;
	}
	return (sumaTiemEspera / atp.tope);
}
//Devuelve resultado final Tiempo de Espera Promedio
float promRetorno(AT_Procesos atp){
	float sumaTiemRetorno;
	for(int i=0;i<atp.tope;i++){
		sumaTiemRetorno += atp.arr_procesos[i].tiempoRetorno;
	}
	return (sumaTiemRetorno / atp.tope);
}

//Solicita al usuario los Procesos a ingresar y los carga en el array de procesos.
void solicitaProcesos(AT_Procesos &atp){	
	proceso p;
	p.numero = 1;
	int num_proc,arribo,rafaga;
	char enter;
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
		p.tiempoEspera = 0;
		p.tiempoRetorno = 1;
		atp.arr_procesos[atp.tope] = p;
		atp.tope++;
		p.numero++;
		num_proc--;
	}
}

//Acciones que se realizan por cada segundo hasta que finalicen todos los procesos ingresados.
void ejecPorSegundo(AT_Procesos &atp){
	int procesosTerminados = 0, tiempo = 0;
	char enter;
	//Inicia en cero y por cada segundo recorre el array de procesos para ponerlos en estado listo, termina cuando todos los procesos finalizan
	while(procesosTerminados != atp.tope){ // podria resolverse de otra forma...
		for(int j=0;j<atp.tope;j++){
			if(atp.arr_procesos[j].arribo == tiempo || atp.arr_procesos[j].estado == 2){
				atp.arr_procesos[j].estado = 1;
			}
		}
		//ordena procesos por rafaga, y ejecuta los listos
		ordenaProcesos(atp);
		printf("----------------\n");
		printf("%d: ",tiempo);		
		if(imprimirUsoCpu(atp)){
			procesosTerminados++;
		}
		tiempo++;
	}
}

main(){
	AT_Procesos procesos;
	procesos.tope = 0;

	solicitaProcesos(procesos);
	ejecPorSegundo(procesos);
	imprimirProcesos(procesos);
	
	printf("Tiempo promedio de espera: %g\n\n", promEspera(procesos));
	printf("Tiempo promedio de retorno: %g\n", promRetorno(procesos));
}
