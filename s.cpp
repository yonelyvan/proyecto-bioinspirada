#include <bits/stdc++.h>
#define see(X) cout<<#X<<" "<<X<<endl;

using namespace std;

typedef vector<int> vi;
typedef vector<float> vf;
typedef vector<vi> vvi;



#define TAM_POB 4 //Tamaño de la Población: 4
#define TAM_CROM 10 //Tamaño de los Cromosomas: 5
#define ITERACIONES 50  //Cantidad de Iteraciones: 30
#define PROB_CRU 0.9 //Probabilidad de Cruzamiento: 0.9 
#define CRU_PUNTO 4 //Cruzamiento de un Punto - Punto 3
#define PROB_MUT 5 // 0.5-1% Probabilidad de Mutación: 0.05




void print_vi(vi v);
void print_vf(vf v);
void print_vvi(vvi v);
void imprimir_poblacion(vvi p);
void imprimir_fit_poblacion(vvi p);
void imprimir_fit_ruleta_poblacion(vvi p);
int suma_vf(vf v);


vvi get_poblacion_inicial(){
	vvi pi;
	for (int i = 0; i < TAM_POB; ++i){
		vi t;
		for (int i = 0; i < TAM_CROM; ++i){
			int c = rand()%2;
			t.push_back(c);
		}	
		pi.push_back(t);
	}
	return pi;
}

vf get_fitness(vvi p){
	vf f;
	for (int i = 0; i < p.size(); ++i){
		int x =0;
		int y =0;
		for (int j = 0; j < p[0].size(); ++j){
			if( j < TAM_CROM/2 ){
				x=x+(pow(2,j))*p[i][j];
			}else{
				y=y+(pow(2,j-TAM_CROM/2))*p[i][j];
			}
		} 
		 //(-(pow(acu,2))/10)+3*acu;
		//see(x);
		//see(y);
		f.push_back(x-y);//
	}
	return f;
}




vi ruleta(vvi p){
	vf f = get_fitness(p);
	int total = suma_vf(f);
	float cont=0;
	vf v_pro;//ruleta
	for (int i = 0; i < f.size(); ++i){
		cont += (f[i]*100.0)/total;
		v_pro.push_back(cont); 
	}
	//seleccion
	vi seleccionados;

	for (int i = 0; i < p.size(); ++i){
		int s= rand()%100;
		for (int j = 0; j < v_pro.size(); ++j){//verificando a q rango pertenece
			if( s <= v_pro[j] ){
				seleccionados.push_back(j);
				break;
			}
		}
	}
	return seleccionados;
}

vi torneo(vvi p){
	vi seleccionados;
	vf f = get_fitness(p);
	int participantes=3;	
	for (int ii = 0; ii < f.size(); ++ii){
		priority_queue< pair< float,int > > pq;
		for (int j = 0; j < participantes; ++j){
			int p_i = rand()%p.size();
			pq.push(make_pair( f[p_i], p_i ));
		}
		seleccionados.push_back( pq.top().second );
	}	
	return seleccionados;
}

vi seleccion(vvi p){
	//return ruleta(p);
	return torneo(p);
}


vvi cruzar(vi a,vi b, vi mascara){
	vvi hijos;
	vi h1;
	vi h2;
	for (int i = 0; i < a.size(); ++i){
		if( mascara[i] ){
			h1.push_back(a[i]);
			h2.push_back(b[i]);
		}else{
			h1.push_back(b[i]);
			h2.push_back(a[i]);
		}
	}
	hijos.push_back(h1);
	hijos.push_back(h2);
	return hijos;
}

//muta bit del individuo elegido aleatoriamente 
vvi mutar(vvi p, float pro_mutacion){
	for (int i = 0; i < p.size(); ++i){
		float pm=rand()%100;
		if( pm <= pro_mutacion ){//para cada individuo
			int bit_mut = rand()%p[0].size();//
			//see(bit_mut);
			cout<<"mutacion en individuo: "<< i+1<<") en el bit: "<<bit_mut<<endl;;
			if( p[i][bit_mut] ){
				p[i][bit_mut]=0;
			}else{
				p[i][bit_mut]=1;
			}
		}
		//see(pm);
	}
	return p;
}


void run(){
	cout<<"Tamaño de la Población: "<<TAM_POB<<endl;
	cout<<"Tamaño de los Cromosomas: "<<TAM_CROM<<endl;
	cout<<"Cantidad de Iteraciones: "<<ITERACIONES<<endl;
	cout<<"Probabilidad de Cruzamiento: "<<PROB_CRU<<endl;
	cout<<"Cruzamiento de un Punto - Punto: "<<CRU_PUNTO<<endl;
	cout<<"Probabilidad de Mutación: "<<PROB_MUT<<endl;

	cout<<"\nGenerando Población Inicial"<<endl;
	vvi p = get_poblacion_inicial();
	imprimir_poblacion(p);

	//mascara
	vi mascara;
	for (int i = 0; i < TAM_CROM; ++i){
		mascara.push_back(rand()%2);
	}
	cout<<"MACARA: "; print_vi(mascara);
	for (int it = 0; it < ITERACIONES; ++it){
	cout<<"\n-----------Iteración: "<<it<<" -----------"<<endl;

		cout<<"Evaluando Individuos"<<endl;
		imprimir_fit_poblacion(p);

		cout<<"Selección de Individuos - Método de la Ruleta"<<endl;
		imprimir_fit_ruleta_poblacion(p);
		vi seleccionados = seleccion(p);
		//print_vi(seleccionados);
		for (int i = 0; i < (seleccionados.size()/2); ++i){
			cout<<"\nCruzamiento"<<endl;
			int pp=seleccionados[i*2];
			int mm=seleccionados[i*2+1];
			float p_cru = (rand()%100)/100;
			if( p_cru <= PROB_CRU){
				cout<<"padre: "<<pp+1<<endl;
				cout<<"madre: "<<mm+1<<endl;
				print_vi(p[pp]);
				print_vi(p[mm]);
				vvi hijos = cruzar(p[pp], p[mm], mascara);
				p.push_back(hijos[0]); 
				p.push_back(hijos[1]); 
			}else{
				while(1){
					p_cru = (rand()%100)/100;
					pp = rand()%seleccionados.size();
					mm = rand()%seleccionados.size();
					if( p_cru <= PROB_CRU){
						cout<<"padre: "<<pp+1<<endl;
						cout<<"madre: "<<mm+1<<endl;
						print_vi(p[pp]);
						print_vi(p[mm]);
						vvi hijos = cruzar(p[pp], p[mm], mascara);
						p.push_back(hijos[0]); 
						p.push_back(hijos[1]); 
						break;
					}
				}
			}
		}

		p = mutar(p,PROB_MUT);

		cout<<"Selección de Siguiente Población"<<endl;
		imprimir_fit_poblacion(p);

		//
		vf p_fit =  get_fitness(p);
		priority_queue< pair< float,vi > > pq;

		for (int i = 0; i < p.size(); ++i){
			int key=p_fit[i];
			vi v_ind=p[i];
			pq.push( make_pair( key, v_ind ) );
		}

		vvi pp;
		for (int i = 0; i < p.size()/2; ++i){
			pp.push_back( pq.top().second );
			pq.pop();
		}
		p=pp;
		//see("Iteracion 1");
		//see("Evaluando individuos");
		//imprimir_fit_poblacion(p);
	}
}




int  main(){
	run();
	//test();
}

/*
	vvi poblacion_inicial = get_poblacion_inicial();
	print_vvi(poblacion_inicial);

	vi f = get_fitness(poblacion_inicial);
	print_vi(f);

	see("cruzar");
	vvi hijos = cruzar(poblacion_inicial[0],poblacion_inicial[1],CRU_PUNTO);
	print_vvi(hijos);

	see(suma_vf(f));
	see("seleccionados")
	vi s=seleccion(poblacion_inicial);
	print_vi(s);

	see("mutar poblacion");
	poblacion_inicial=mutar(poblacion_inicial, PROB_MUT);
	print_vvi(poblacion_inicial);
*/













void print_vi(vi v){
	for (int i = 0; i < v.size(); ++i){
		cout<<v[i]<<" ";
	}
	cout<<endl;
}

void print_vf(vf v){
	for (int i = 0; i < v.size(); ++i){
		cout<<v[i]<<" ";
	}
	cout<<endl;
}

void print_vvi(vvi v){
	for (int i = 0; i < v.size(); ++i){
		print_vi(v[i]);
	}
}

int suma_vf(vf v){
	int s=0;
	for (int i = 0; i < v.size(); ++i){
		s+=v[i];
	}
	return s;
}

void imprimir_poblacion(vvi p){
	for (int i = 0; i < p.size(); ++i){
		cout<<i+1<<") ";
		print_vi(p[i]);
	}
}


void imprimir_fit_poblacion(vvi p){
	vf vfitness = get_fitness(p);
	for (int i = 0; i < p.size(); ++i){
		cout<<i+1<<") ";
		for (int j = 0; j < p[0].size(); ++j){
			cout<<p[i][j]<<" ";
		}
		cout<<" - "<<vfitness[i]<<endl;;
	}
}


void imprimir_fit_ruleta_poblacion(vvi p){
	vf f = get_fitness(p);
	int total = suma_vf(f);
	vf v_pro;//ruleta
	for (int i = 0; i < f.size(); ++i){
		v_pro.push_back((f[i]*100.0)/total); 
	}

	for (int i = 0; i < p.size(); ++i){
		cout<<i+1<<") ";
		for (int j = 0; j < p[0].size(); ++j){
			cout<<p[i][j]<<" ";
		}
		cout<<" - "<<f[i]<<" - "<<v_pro[i]<<endl;;
	}
}


	