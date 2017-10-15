#include <stdlib.h>
#include <stdio.h>

#define	MAX_INT 1000

// Estructura de los procesos
struct str_proc{
	int numero;
	int arribo;
	int rafaga;
	int estado; //0: no existe 1: listo, 2: ejecutando, 3: finalizado
	int tiempoEspera;
	int tiempoRetorno;
};
typedef struct str_proc proceso;

//Arreglo que contiene a cada proceso
struct str_procAT{
	proceso arr_procesos[MAX_INT];
	int tope;
};
typedef struct str_procAT AT_Procesos; // arreglo con tope de procesos

//Arreglo para hacerr gantt
struct str_gantt{
	int numProceso;
	int tiempoEjProceso;
};
typedef struct str_gantt gantt;

struct str_ganttAT{
	gantt arr_gantt[MAX_INT];
	int tope;
};
typedef struct str_ganttAT AT_Gantt;

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
  
//Realiza la ejecucion de un proceso en un tiempo determinado
bool imprimirUsoCpu(AT_Procesos &atp, AT_Gantt &atgantt, int tiempo){
	int i=0;
	bool ejecuta = false;
	bool finProceso = false;
	int nombreProcEjecutando;
	while (i < atp.tope && ejecuta != true){
		//Busca primer proceso en cola en estado listo y lo ejecuta.
		if (atp.arr_procesos[i].estado == 1){ //|| atp.arr_procesos[i].estado == 2
			atp.arr_procesos[i].estado = 2;
			ejecuta = true;
			atp.arr_procesos[i].rafaga--;
			nombreProcEjecutando = atp.arr_procesos[i].numero;
			//Si el proceso ya se consumio, pasa a finalizado (estado 3)			
			if(atp.arr_procesos[i].rafaga == 0){
				atp.arr_procesos[i].estado = 3;
				finProceso = true;
			}
			//Agrego proceso ejecutado en un tiempo determiando al arreglo Gantt
			atgantt.arr_gantt[atgantt.tope].numProceso = atp.arr_procesos[i].numero;
			atgantt.arr_gantt[atgantt.tope].tiempoEjProceso = tiempo;
			atgantt.tope++;
		}
		i++;
	}
	//Si no habia proceso para ejecutar, se agrega un estado libre en gantt
	if(ejecuta != true){
		atgantt.arr_gantt[atgantt.tope].numProceso = 0;
		atgantt.arr_gantt[atgantt.tope].tiempoEjProceso = tiempo;
		atgantt.tope++;
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
	//Al terminar la funcion, retorna si un proceso finalizo
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

//Acciones que se realizan por cada segundo hasta que finalicen todos los procesos ingresados.
void ejecPorSegundo(AT_Procesos &atp, AT_Gantt &atgantt){
	int procesosTerminados = 0, tiempo = 0;
	char enter;
	//Inicia en cero y por cada segundo recorre el array de procesos para ponerlos en estado listo, termina cuando todos los procesos finalizan
	while(procesosTerminados != atp.tope){ // podria resolverse de otra forma...
		for(int j=0;j<atp.tope;j++){
			if(atp.arr_procesos[j].arribo == tiempo || atp.arr_procesos[j].estado == 2){
				atp.arr_procesos[j].estado = 1;
			}
		}
		//ordena procesos por rafaga y arribo
		ordenaProcesos(atp);	
		//Realiza una ejecucion de procesador; devuelve si finalizo un proceso
		if(imprimirUsoCpu(atp,atgantt,tiempo)){
			procesosTerminados++;
		}
		tiempo++;
	}
}

//Solicita al usuario los Procesos a ingresar y los carga en el array de procesos.
void solicitaProcesos(AT_Procesos &atp){	
	proceso p;
	p.numero = 1;
	int num_proc,arribo,rafaga;
	char enter;
	bool okProceso = false, okArribo = false, okBurst = false;
	do{
		printf("Seleccione cantidad de procesos a ejecutar: ");
		scanf("%d%c",&num_proc,&enter);
		if(num_proc > 0){
			okProceso = true;
		}
		else{
			printf("Error: La cantidad total de procesos a ejecutar debe ser mayor a cero.\n\n");
		}
	}while(!okProceso);
	
	printf("\n");

	while (num_proc > 0){
		printf("Proceso %d:\n\n",p.numero);

		do{
			printf("Ingrese el tiempo de arribo: ");
			scanf("%d%c",&p.arribo,&enter);
			if(p.arribo >= 0){
				okArribo = true;
			}
			else{
				printf("Error: El tiempo de arribo debe ser mayor o igual a cero.\n\n");
			}
		}while(!okArribo);

		do{
			printf("Ingrese el tiempo de rafaga: ");
			scanf("%d%c",&p.rafaga,&enter);
			if(p.rafaga > 0){
				okBurst = true;
				printf("\n");
				p.estado = 0;
				p.tiempoEspera = 0;
				p.tiempoRetorno = 1;
				atp.arr_procesos[atp.tope] = p;
				atp.tope++;
				p.numero++;
				num_proc--;
			}
			else{
				printf("Error: El tiempo de rafaga debe ser mayor a cero\n\n");
			}
		}while(!okBurst);
	}
}

//Imprime Gantt
void imprimirGantt(AT_Gantt &gantt){
	printf("\n\n");
	for(int i = 0; i < gantt.tope; i++){
		printf("----");
	}
	printf("-\n");
	for(int i = 0; i < gantt.tope; i++){
		if(i == 0){
			if(gantt.arr_gantt[i].numProceso == 0){
				printf("|LIB");
			}else{
				printf("|P%d ",gantt.arr_gantt[i].numProceso);
			}
		}else if(gantt.arr_gantt[i].numProceso != gantt.arr_gantt[i-1].numProceso){
			if(gantt.arr_gantt[i].numProceso == 0){
				printf("|LIB");
			}else{
				printf("|P%d ",gantt.arr_gantt[i].numProceso);
			}
		}else{
			printf("    ");
		}
	}
	printf("|\n");
	for(int i = 0; i < gantt.tope; i++){
		printf("----");
	}
	printf("-\n");
	for(int i = 0; i < gantt.tope; i++){
		if(i == 0){
			printf("%d   ",gantt.arr_gantt[i].tiempoEjProceso);
		}else if(gantt.arr_gantt[i].numProceso != gantt.arr_gantt[i-1].numProceso){
			printf("%d   ",gantt.arr_gantt[i].tiempoEjProceso);
		}else if(i == gantt.tope-1){
			printf("   %d",gantt.arr_gantt[i].tiempoEjProceso+1);
		}else{
			printf("    ");
		}
	}
	printf("\n\n\n");
}
	
//Devuelve resultado final Tiempo de Espera Promedio
float promEspera(AT_Procesos atp){
	float sumaTiemEspera;
	for(int i=0;i<atp.tope;i++){
		sumaTiemEspera += atp.arr_procesos[i].tiempoEspera;
	}
	return (sumaTiemEspera / atp.tope);
}

//Devuelve resultado final Tiempo de Retorno Promedio
float promRetorno(AT_Procesos atp){
	float sumaTiemRetorno;
	for(int i=0;i<atp.tope;i++){
		sumaTiemRetorno += atp.arr_procesos[i].tiempoRetorno;
	}
	return (sumaTiemRetorno / atp.tope);
}

void imprimirBienvenida() {
	printf ( "---------------------------------------------------------\n" );
	printf ( "------     Obligatorio 1 - Sistemas Operativos     ------\n" );
	printf ( "------        Tecnologo en Informatica             ------\n" );
	printf ( "------                 Grupo 4                     ------\n" );
	printf ( "------  Planificador de Procesos SJF Expropiativo  ------\n" );
	printf ( "---------------------------------------------------------\n" );
}

//Mensaje de despedida
void imprimirDespedida(){
	printf("\n\n¡Muchas gracias por usar nuestro programa!\n\n\n");
	printf("Emmanuel Picca\nLeonardo Faller\nTomas Garcia\nGerman Arenaza\n");
}	

main(){
	AT_Procesos procesos;
	AT_Gantt gantt;
	bool salida = false;
	
	imprimirBienvenida();
	while (!salida){
		procesos.tope = 0;
		gantt.tope = 0;

		solicitaProcesos(procesos);
		ejecPorSegundo(procesos,gantt);
		imprimirProcesos(procesos);
		imprimirGantt(gantt);
		printf("Tiempo promedio de espera: %g\n\n", promEspera(procesos));
		printf("Tiempo promedio de retorno: %g\n\n\n", promRetorno(procesos));

		char opcionSalida[2];
		printf("¿Desea continuar? (S/N): ");
		scanf("%s", opcionSalida);
		switch (opcionSalida[0]){ //sigue derecho si se presiona cualquier otra tecla
			case 'S': case 's':{
				salida = false;
				break;
			}
			case 'N': case 'n':{
				salida = true;
				imprimirDespedida();
				break;
			}
		}
	}
}
