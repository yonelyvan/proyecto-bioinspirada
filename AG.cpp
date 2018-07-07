#include "BL.cpp"

#define TAM_POB 16 //Tamaño de la Población: 4
#define TAM_CROM 32 //Tamaño de los Cromosomas: 5 [x,y] == [1010,1001]
#define ITERACIONES 1000 //Cantidad de Iteraciones: 30
#define PROB_CRU 0.9 //Probabilidad de Cruzamiento: 0.9 
#define PROB_MUT 5 // 0.5-1% Probabilidad de Mutación: 0.05
int inf = 1e9;


typedef struct{
	vi cromosoma;
	double fitness;
}individuo;

typedef vector<individuo> poblacion;

bool Mejor(individuo a, individuo b){
	if(a.fitness < b.fitness){//< minimizar, > maximizar
		return true; 
	} 
	return false;
}


//=========imprimir============
string to_string_cromosoma(individuo I){
	string s=" [ ";
	for(auto c:I.cromosoma){
		s += to_string(c);
		s +=" ";
	}
	s +="] ";
	return s;
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
//=====================

poblacion get_poblacion_inicial(){
	poblacion P;
	for (int i = 0; i < TAM_POB; ++i){
		individuo I;//orden de rectangulos
		for (int i = 0; i < TAM_CROM; ++i){
			I.cromosoma.push_back(i);
		}
		for (int j = 0; j < TAM_CROM; ++j){
			int n = rand()%TAM_CROM;
			int m = rand()%TAM_CROM;
			swap(I.cromosoma[n],I.cromosoma[m]);	
		}
		P.push_back(I);
	}
	return P;
}

void calcular_fitness(rectangulo contenedor, vr R,poblacion &P){
	for(individuo &I:P){
		int fallos = BL(contenedor,R,I.cromosoma);
		int rect_no_introducidos = fallos;
		I.fitness = rect_no_introducidos +1;
	}	
}

poblacion ruleta(poblacion P){
	float total = 0;
	for(individuo I : P){ 
		total +=I.fitness; 
	}

	float cont=0;
	vf v_pro;//ruleta
	int Ii=0;
	for(individuo I : P){
		cont += (I.fitness*100.0)/total;
		v_pro.push_back(cont);
		cout<<Ii<<") "<<to_string_cromosoma(I)<<" "<<I.fitness<<" Prob: "<<(I.fitness*100.0)/total<<endl;
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


//=========cruzamiento=========
int find_poss(vi v,int key){
	for (int i = 0; i < v.size(); ++i){
		if( key==v[i] ){
			return i;
		}
	}
	return -1;
} 

bool find(vi v,int key){
	for (int i = 0; i < v.size(); ++i){
		if( key==v[i] ){
			return true;
		}
	}
	return false;
}

/*--PBX*/
vi asignar(vi padre, vi hijo, vi puntos){
	int cro_i =0;
	for (int i = 0; i < TAM_CROM; ++i){
		if( !find(puntos,i) ){//si no es un punto seleccionado
			while(cro_i<TAM_CROM){
				if( !find(hijo,padre[cro_i]) ){//si no existe cromosoma en el hijo
					hijo[i]=padre[cro_i];
					break;
				}
				++cro_i;
			}
		}
	}
	return hijo;
}

vvi PBX(vi p1,vi p2){
	int num_puntos=3;
	vvi hijos;
	vi puntos;
	vi h1=p1;
	vi h2=p2;
	//puntos random, swap 
	for (int i = 0; i < num_puntos; ++i){
		int pi = rand()%TAM_CROM;
		if( !find(puntos, pi) ){
			puntos.push_back(pi);
			swap(h1[pi],h2[pi]);
		}else{
			--i;
		}

	}
	//vacios
	for (int i = 0; i < TAM_CROM; ++i){
		if( !find(puntos,i)){
			h1[i]=inf;
			h2[i]=inf;
		}
	}
	cout<<"seleccionados: ";
	//print_vi(puntos);

	h1=asignar(p1,h1,puntos);
	h2=asignar(p2,h2,puntos);
	hijos.push_back(h1);
	hijos.push_back(h2);
	cout<<"Hijos:"<<endl;
	//print_vvi_labels(hijos);
	return hijos;
}

vvi PMX(vi p1,vi p2){
	vi puntos;
	vvi hijos;
	/* punto s aleatorios*/
	int point1 = 1;//rand()%TAM_CROM;
	int point2 = 4;//rand()%TAM_CROM;
	while(point1==point2){
		point2=rand()%TAM_CROM;
	}
	if(point2<point1){swap(point1,point2);}
	cout<<"Puntos: "<<point1<<" "<<point2<<endl;

	for (int i = 0; i < TAM_CROM; ++i){
		if( point1<=i && i<point2){ //sublista
			int val_label_p1 = p1[i];  
			int val_label_p2 = p2[i];
			int dir1=find_poss(p1,val_label_p2);
			int dir2=find_poss(p2,val_label_p1);
			p1[dir1]=val_label_p1;
			p2[dir2]=val_label_p2;
			swap(p1[i],p2[i]); 
		}	
	}
	hijos.push_back(p1);
	hijos.push_back(p2);
	cout<<"Hijos:"<<endl;
	//print_vvi_labels(hijos);

	return hijos;
}

vvi CX(vi p1,vi p2){
	vvi hijos;
	vi h1;
	vi h2;
	for (int i = 0; i < TAM_CROM; ++i){
		h1.push_back(inf);
		h2.push_back(inf);
	}

	int poss=0;
	int poss_h=poss;
	for (int i = 0; i < TAM_CROM; ++i){//bucle
		h1[poss]=p1[poss];
		h2[poss]=p2[poss];
		poss = find_poss(p1,p2[poss]);
		if(poss==0){
			break;
		}
	}
	/**/
	for (int i = 0; i < TAM_CROM; ++i){//los q faltan
		if(h1[i]==inf){
			h1[i]=p2[i];
			h2[i]=p1[i];
		}
	}

	hijos.push_back(p1);
	hijos.push_back(p2);
	//cout<<"Hijos:"<<endl;
	//print_vvi_labels(hijos);
	return hijos;
}


poblacion cruzar(poblacion &S){
	poblacion hijos;
	for (int i = 0; i < TAM_POB/2; ++i){
		individuo p1;
		individuo p2;
		while(1){
			int j1 = rand()%S.size();
			int j2 = rand()%S.size();
			float p_cru = (rand()%100)/100;
			if( p_cru <= PROB_CRU){
				p1 = S[j1];
				p2 = S[j2];
				cout<<"padre1: "<<j1<<") "<<to_string_cromosoma(S[j1])<<endl;
				cout<<"padre2: "<<j2<<") "<<to_string_cromosoma(S[j2])<<endl;
				break;
			}
		}
		vvi H = PBX(p1.cromosoma,p2.cromosoma);	
		//vvi H =  PMX(p1.cromosoma,p2.cromosoma);	
		//vvi H = CX(p1.cromosoma,p2.cromosoma);
		individuo h1, h2;
		h1.cromosoma = H[0];
		h2.cromosoma = H[1];
		hijos.push_back(h1);
		hijos.push_back(h2);
		
		cout<<"hijo1: "<<to_string_cromosoma(h1)<<endl;
		cout<<"hijo2: "<<to_string_cromosoma(h2)<<endl;
		cout<<endl;
	}
	return hijos;
}

//===========================

//muta bit del individuo elegido aleatoriamente 
void mutar(poblacion &P){
	int i =0;
	for (individuo &I : P){
		float pm=rand()%100;
		if( pm <= PROB_MUT ){//para cada individuo
			int c1 = rand()%TAM_CROM;
			int c2 = rand()%TAM_CROM;
			cout<<"mutacion en individuo: "<< i<<") en: "<<c1<<" y "<<c2<<endl;;
			swap(I.cromosoma[c1], I.cromosoma[c2]);
		}
		++i;
	}
}




void run(){
	cout<<"Tamaño de la Población: "<<TAM_POB<<endl;
	cout<<"Tamaño de los Cromosomas: "<<TAM_CROM<<endl;
	cout<<"Cantidad de Iteraciones: "<<ITERACIONES<<endl;
	cout<<"Probabilidad de Cruzamiento: "<<PROB_CRU<<endl;
	cout<<"Probabilidad de Mutación: "<<PROB_MUT<<endl;

	//rectanglo principal
	rectangulo contenedor;
	contenedor.x = 0;
	contenedor.y = 0;
	contenedor.dx = 150;
	contenedor.dy = 150;
	vr R = generar_rectangulos(TAM_CROM);//vector de rectangulos

	poblacion P = get_poblacion_inicial(); //orden de insercion
	imprimir_poblacion("Poblacion inicial:",P);
	plot_rectangulos_por_orden (contenedor,R,P[0].cromosoma);

	for (int it = 0; it < ITERACIONES; ++it){
		cout<<"\n-----------Iteración: "<<it<<" -----------"<<endl;
		cout<<"Evaluando Individuos"<<endl;
		calcular_fitness(contenedor,R,P);
		imprimir_fitness_poblacion("",P);

		cout<<"Selección de Individuos - Método de la Ruleta"<<endl;
		poblacion seleccionados = seleccion(P);
		
		cout<<"\nCruzamiento"<<endl;
		poblacion hijos = cruzar(seleccionados);
		calcular_fitness(contenedor,R,hijos);
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
	plot_rectangulos_por_orden (contenedor,R,P[0].cromosoma);
}






int main(){
	//srand (time(NULL));
	run();
	return 0;
}


