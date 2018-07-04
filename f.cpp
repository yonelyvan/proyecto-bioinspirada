#include <bits/stdc++.h>
using namespace std;

#define TAM_POB 4 //Tamaño de la Población: 4
#define TAM_CROM 10 //Tamaño de los Cromosomas: 5 [x,y] == [1010,1001]
#define ITERACIONES 2 //Cantidad de Iteraciones: 30
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

bool Mejor(individuo a, individuo b){
	if(a.fitness > b.fitness){//< minimizar, > maximizar
		return true; 
	} 
	return false;
}

string to_string_cromosoma(individuo I);
void imprimir_poblacion(string label ,poblacion &P);
void imprimir_fitness_poblacion(string label ,poblacion &P);


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
	int Ii=0;
	for(individuo I : P){
		cont += (I.fitness*100)/total;
		v_pro.push_back(cont);
		cout<<Ii<<") "<<to_string_cromosoma(I)<<" "<<I.fitness<<" Prob: "<<(I.fitness*100)/total<<endl;
		++Ii;
	}
	//seleccion
	cout<<"Seleccionados"<<endl;
	poblacion seleccionados;

	for (int i = 0; i < P.size(); ++i){
		int s = rand()%100;
		for (int j = 0; j < v_pro.size(); ++j){//verificando a q rango pertenece
			if( s <= v_pro[j] ){
				seleccionados.push_back(P[j]);
				cout<<j<<") "<<to_string_cromosoma(P[j])<<" "<<P[j].fitness<<endl;
				break;
			}
		}
	}
	return seleccionados;
}

poblacion seleccion(poblacion P){
	return ruleta(P);
	//return torneo(p);
}


poblacion cruzar(poblacion P){
	poblacion hijos;
	for (int i = 0; i < (P.size()/2); ++i){
		//se crea una mascara
		vi mascara;
		cout<<"Mascara : [";
		for (int i = 0; i < TAM_CROM; ++i){
			mascara.push_back(rand()%2);
			cout<<mascara[i];
		}
		cout<<"]"<<endl;
		int j1,j2;
		//eleccion de padres
		individuo p1;
		individuo p2;
		while(1){
			j1 = rand()%P.size();
			j2 = rand()%P.size();
			float p_cru = (rand()%100)/100;
			if( p_cru <= PROB_CRU){
				p1 = P[j1];
				p2 = P[j2];
				cout<<"padre1: "<<j1<<") "<<to_string_cromosoma(P[j1])<<endl;
				cout<<"padre2: "<<j2<<") "<<to_string_cromosoma(P[j2])<<endl;
				break;
			}
		}

		individuo h1;
		individuo h2;
		//cruzamiento
		for (int i = 0; i < TAM_CROM; ++i){
			if( mascara[i] ){
				h1.cromosoma.push_back(p2.cromosoma[i]);
				h2.cromosoma.push_back(p1.cromosoma[i]);
			}else{
				h1.cromosoma.push_back(p1.cromosoma[i]);
				h2.cromosoma.push_back(p2.cromosoma[i]);
			}
		}
		cout<<"hijo1: "<<to_string_cromosoma(h1)<<endl;
		cout<<"hijo2: "<<to_string_cromosoma(h2)<<endl;
		cout<<endl;
		hijos.push_back(h1);
		hijos.push_back(h2);
	}
	return hijos;
}

void mutar(poblacion &P){
	for (int i = 0; i < P.size(); ++i){
		float pm=rand()%100;
		if( pm <= PROB_MUT ){//para cada individuo
			int bit_mut = rand()%TAM_CROM;//
			cout<<"mutacion en individuo: "<< i<<") en el bit: "<<bit_mut<<endl;;
			if( P[i].cromosoma[bit_mut] ){
				P[i].cromosoma[bit_mut]=0;
			}else{
				P[i].cromosoma[bit_mut]=1;
			}
		}
	}
}


void run(){
	cout<<"Tamaño de la Población: "<<TAM_POB<<endl;
	cout<<"Tamaño de los Cromosomas: "<<TAM_CROM<<endl;
	cout<<"Cantidad de Iteraciones: "<<ITERACIONES<<endl;
	cout<<"Probabilidad de Cruzamiento: "<<PROB_CRU<<endl;
	cout<<"Probabilidad de Mutación: "<<PROB_MUT<<endl;

	cout<<"\nGenerando Población Inicial"<<endl;
	poblacion P = get_poblacion_inicial();
	imprimir_poblacion("Poblacion inicial:",P);

	for (int it = 0; it < ITERACIONES; ++it){
		cout<<"\n-----------Iteración: "<<it<<" -----------"<<endl;

		cout<<"Evaluando Individuos"<<endl;
		calcular_fitness(P);
		imprimir_fitness_poblacion("",P);

		cout<<"Selección de Individuos - Método de la Ruleta"<<endl;
		poblacion seleccionados = seleccion(P);
		
		cout<<"\nCruzamiento"<<endl;
		poblacion hijos = cruzar(P);
		calcular_fitness(hijos);
		for(individuo I :hijos){
			P.push_back(I);
		}
		//mutacion
		mutar(P);

		imprimir_fitness_poblacion("\nSelección de Siguiente Población",P);
		sort(P.begin(), P.end(),Mejor);
		poblacion nueva_poblacion;
		for (int i = 0; i < TAM_POB; ++i){
			nueva_poblacion.push_back(P[i]);
		}
		//nueva poblacion
		imprimir_fitness_poblacion("\nNueva poblacion",nueva_poblacion);
		P=nueva_poblacion;
	}
}


int main(){
	srand (time(NULL));//
	run();
	return 0;
}


void imprimir_poblacion(string label, poblacion &P){
	cout<<label<<endl;
	int i=0;
	for(individuo I:P){
		cout<<i<<to_string_cromosoma(I)<<endl;
		++i;
	}
}

void imprimir_fitness_poblacion(string label,poblacion &P){
	cout<<label<<endl;
	int i=0;
	for(individuo I:P){
		cout<<i<<") "<<to_string_cromosoma(I)<<I.fitness<<endl;
		++i;
	}
}

string to_string_cromosoma(individuo I){
	string s=" [";
	for(auto c:I.cromosoma){
		s += to_string(c);
	}
	s +="] ";
	return s;
}