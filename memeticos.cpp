#include "util.cpp"

#define TAM_POB 16 //N 	Tamaño de la Población: 4
#define TAM_CROM 32 //Tamaño de los Cromosomas: 5
#define ITERACIONES 1000 //Cantidad de Iteraciones: 30
#define PROB_CRU 0.8 //Probabilidad de Cruzamiento: 0.9 
#define CRU_PUNTO 3 //Cruzamiento de un Punto - Punto 3
#define PROB_MUT 10 // 0.5-1% Probabilidad de Mutación: 0.05
		
int M = 3; //N=TAM_POB


vvi get_poblacion_inicial(){
	vvi pi;
	for (int i = 0; i < TAM_POB; ++i){
		vi t;
		for (int j = 0; j < TAM_CROM; ++j){
			t.push_back(j);
		}
		for (int j = 0; j < TAM_CROM; ++j){
			int n = rand()%TAM_CROM;
			int m = rand()%TAM_CROM;
			int temp=t[n];
			swap(t[n],t[m]);	
		}	
		pi.push_back(t);
	}
	return pi;
}




vi ruleta(rectangulo contenedor, vr R,vvi p){
	vf f = get_fitness( contenedor, R, p);
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


vi torneo(rectangulo contenedor, vr R,vvi p){
	vi seleccionados;
	vf f = get_fitness(contenedor,R,p);
	int participantes=3;	
	for (int ii = 0; ii < f.size(); ++ii){
		priority_queue< pair< float,int > > pq;
		for (int j = 0; j < participantes; ++j){
			int p_i = rand()%p.size();
			pq.push(make_pair( inf-f[p_i], p_i ));
		}
		seleccionados.push_back( pq.top().second );
	}	
	return seleccionados;
}

vi seleccion(rectangulo contenedor, vr R,vvi p){
	//return ruleta(contenedor, R,p);
	return torneo(contenedor, R,p);
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

vvi PBX(rectangulo contenedor, vr R,vi p1,vi p2){
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
	print_vi(puntos);

	h1=asignar(p1,h1,puntos);
	h2=asignar(p2,h2,puntos);
	hijos.push_back(h1);
	hijos.push_back(h2);
	cout<<"Hijos:"<<endl;
	//print_vvi_labels(hijos);
	imprimir_fit_poblacion(contenedor,R,hijos);
	return hijos;
}
/*PMX*/
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
			/*see("padres__________");
			see("cromosoma");
			see(i);
			print_vi_labels(p1);
			print_vi_labels(p2);
			*/
			int val_label_p1 = p1[i];  
			int val_label_p2 = p2[i];
			int dir1=find_poss(p1,val_label_p2);
			int dir2=find_poss(p2,val_label_p1);
			p1[dir1]=val_label_p1;
			p2[dir2]=val_label_p2;
			swap(p1[i],p2[i]); 
			/*
			see("hijos---temporales");
			print_vi_labels(p1);
			print_vi_labels(p2);
			see("_______________");
			*/
		}	
	}
	hijos.push_back(p1);
	hijos.push_back(p2);
	cout<<"Hijos:"<<endl;
	print_vvi_labels(hijos);

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
	cout<<"Hijos:"<<endl;
	print_vvi_labels(hijos);
	return hijos;
}

vvi cruzar(rectangulo contenedor, vr R, vi p1,vi p2){ //PBX
	return PBX(contenedor,R,p1,p2);	
	//return PMX(p1,p2);	
	//return CX(p1,p2);
}

//muta bit del individuo elegido aleatoriamente 


//muta bit del individuo elegido aleatoriamente 
vvi mutar(vvi p, float pro_mutacion){
	for (int i = 0; i < p.size(); ++i){
		float pm=rand()%100;
		if( pm <= pro_mutacion ){//para cada individuo
			int c1 = rand()%TAM_CROM;
			int c2 = rand()%TAM_CROM;
			cout<<"mutacion en individuo: "<< i+1<<") en: "<<c1<<" y "<<c2<<endl;;
			swap(p[i][c1], p[i][c2]);
		}
	}
	return p;
}


vi mejor_hijo(rectangulo contenedor, vr &R, vi I){
	vi temp=I;
	vvi mejores;
	for(int i = 0; i < 5; ++i){
		int c1 = rand()%TAM_CROM;
		int c2 = rand()%TAM_CROM;
		swap(temp[c1], temp[c2]);
		if( get_fitness(contenedor,R,temp) <= get_fitness(contenedor,R,I)  ){
			mejores.push_back(temp);
		}
		temp = I;
	}

	vi top=I;
	for (int i = 0; i < mejores.size(); ++i){
		if( get_fitness(contenedor,R,mejores[i]) < get_fitness(contenedor,R,top) ){
			top = mejores[i];
			top = mejor_hijo(contenedor,R,mejores[i]);
		}
	}
	return top;
}


vvi busqueda_local(rectangulo contenedor, vr R,vvi h){
	vvi mejores_hijos;
	for (int i = 0; i < h.size(); ++i){//para cada hijo
		mejores_hijos.push_back( mejor_hijo( contenedor, R,h[i]) );
	}	
	return mejores_hijos;
}


void run(){
	cout<<"Tamaño de la Población: "<<TAM_POB<<endl;
	cout<<"Tamaño de los Cromosomas: "<<TAM_CROM<<endl;
	cout<<"Cantidad de Iteraciones: "<<ITERACIONES<<endl;
	cout<<"Probabilidad de Cruzamiento: "<<PROB_CRU<<endl;
	cout<<"Cruzamiento de un Punto - Punto1: "<<CRU_PUNTO<<endl;
	cout<<"Probabilidad de Mutación: "<<PROB_MUT<<endl;
	//rectanglo principal
	rectangulo contenedor;
	contenedor.x = 0;
	contenedor.y = 0;
	contenedor.dx = 150;
	contenedor.dy = 150;
	vr R = generar_rectangulos(TAM_CROM);//vector de rectangulos

	cout<<"Generando soluciones"<<endl;
	vvi p = get_poblacion_inicial();
	imprimir_poblacion(p);

	cout<<"\nGenerando Población Inicial"<<endl;
	vf p_fit1 =  get_fitness(contenedor,R,p);
	priority_queue< pair< float,vi > > ppq;

	plot_rectangulos_por_orden (contenedor,R,p[0]);///
	for (int i = 0; i < p.size(); ++i){
		int fi=p_fit1[i];
		vi v_ind1=p[i];
		ppq.push( make_pair( inf-fi, v_ind1 ) );
	}

	vvi pp;
	for (int i = 0; i < p.size()/3; ++i){
		pp.push_back( ppq.top().second );
		ppq.pop();
	}
	p=pp;
	imprimir_poblacion(p);


	for (int it = 0; it < ITERACIONES; ++it){
	cout<<"\n-----------Iteración: "<<it<<" -----------"<<endl;

		cout<<"Evaluando Individuos"<<endl;
		imprimir_fit_poblacion(contenedor,R,p);

		cout<<"Selección de Individuos"<<endl;
		imprimir_fit_ruleta_poblacion(contenedor,R,p);
		vi seleccionados = seleccion(contenedor,R,p);
		//print_vi(seleccionados);
		for (int i = 0; i < (seleccionados.size()/2); ++i){
			cout<<"\nCruzamiento"<<endl;
			int pp;//
			int mm;//
			float p_cru;//
			while(1){
				p_cru = (rand()%100)/100;
				pp = rand()%seleccionados.size();
				mm = rand()%seleccionados.size();
				if( p_cru <= PROB_CRU){
					cout<<"padre1: "<<pp+1<<endl;
					cout<<"padre2: "<<mm+1<<endl;
					print_vi_labels(p[pp]);
					print_vi_labels(p[mm]);
					vvi hijos = cruzar(contenedor,R,p[pp], p[mm]);//cruse
					cout<<"Busqueda local"<<endl;
					vvi mejores_hijos = busqueda_local(contenedor,R,hijos);
					
					imprimir_fit_poblacion(contenedor,R,mejores_hijos);

					p.push_back(mejores_hijos[0]); 
					p.push_back(mejores_hijos[1]);
					
					break;
				}

			}
		}
		
		//p = mutar(p,PROB_MUT);
		cout<<"Selección de Siguiente Población"<<endl;
		imprimir_fit_poblacion(contenedor,R,p);

		//
		vf p_fit =  get_fitness(contenedor,R,p);
		priority_queue< pair< float,vi > > pq;

		for (int i = 0; i < p.size(); ++i){
			int fi=p_fit[i];
			vi v_ind=p[i];
			pq.push( make_pair( inf-fi, v_ind ) );
		}

		vvi pp;
		for (int i = 0; i < p.size()/2; ++i){
			pp.push_back( pq.top().second );
			pq.pop();
		}
		p=pp;
	}
	plot_rectangulos_por_orden (contenedor,R,p[0]);
}





int  main(){
	run();
	//test();
}
/*obtener M soluciones M>=3*N (N tamño de poblacion)*/