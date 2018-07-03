#include <bits/stdc++.h>
using namespace std;

#define TAM_POB 4 //Tamaño de la Población: 4
#define TAM_CROM 8 //Tamaño de los Cromosomas: 5 [x,y] == [1010,1001]
#define ITERACIONES 50  //Cantidad de Iteraciones: 30
#define PROB_CRU 0.9 //Probabilidad de Cruzamiento: 0.9 
#define CRU_PUNTO 4 //Cruzamiento de un Punto - Punto 3
#define PROB_MUT 5 // 0.5-1% Probabilidad de Mutación: 0.05

typedef vector<int> vi;
typedef vector<float> vf;
typedef vector<vi> vvi;

typedef struct{
	vi cromosoma;
	double fitness;
}individuo;

typedef vector<individuo> poblacion;


poblacion get_poblacion_inicial(){
	poblacion P;
	for (int i = 0; i < TAM_POB; ++i){
		individuo I;
		for (int i = 0; i < TAM_CROM; ++i){
			int c = rand()%2;
			I.cromosoma.push_back(c);//cromosoma
		}	
		P.push_back(I);
	}
	return P;
}

void calcular_fitness(poblacion &P){
	for(individuo &I : P){
		int x =0;
		int y =0;
		for (int i = 0; i < I.cromosoma.size(); ++i){
			if( i < TAM_CROM/2 ){
				x += (pow(2,i))*I.cromosoma[i];
			}else{
				y += (pow(2,i-TAM_CROM/2))*I.cromosoma[i];
			}
		}//[x,y] calculdos
		I.fitness = x+y;
	}
}



poblacion ruleta(poblacion P){
	int total = 0;
	for(individuo I : P){ 
		total +=I.fitness; 
	}

	float cont=0;
	vf v_pro;//ruleta

	for(individuo I : P){
		cont += (I.fitness*100)/total;
		v_pro.push_back(cont);
		cout<<cont<<endl;
	}
	//seleccion
	poblacion seleccionados;

	for (int i = 0; i < P.size(); ++i){
		int s = rand()%100;
		for (int j = 0; j < v_pro.size(); ++j){//verificando a q rango pertenece
			if( s <= v_pro[j] ){
				seleccionados.push_back(P[j]);
				//cout<<j<<") ["
				break;
			}
		}
	}
	return seleccionados;
}



string to_string_cromosoma(individuo I)
void imprimir_poblacion(poblacion &P);
void imprimir_fitness_poblacion(poblacion &P);


int main(){
	poblacion P = get_poblacion_inicial();
	//imprimir_poblacion(P);
	calcular_fitness(P);
	imprimir_fitness_poblacion(P);
	cout<<"seleccionados"<<endl;
	poblacion seleccionados = ruleta(P); 
	//imprimir_poblacion(seleccionados);

	return 0;
}







string to_string_cromosoma(individuo I){
	string s=" [";
	for(auto c:I.cromosoma){
		s += to_string(c);
	}
	s +="] ";
}

void imprimir_poblacion(poblacion &P){
	for (int i = 0; i < P.size(); ++i){
		cout<<i<<") [";
		for(int c : P[i].cromosoma){
			cout<<c;
		}
		cout<<"]"<<endl;
	}
}

void imprimir_fitness_poblacion(poblacion &P){
	cout<<endl;
	cout<<"Fitness :"<<endl;
	int i=0;
	for(individuo I:P){
		cout<<i<<to_string_cromosoma(I)<<I.fitness<<endl;
		++i;
	}
}

