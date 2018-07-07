#include <bits/stdc++.h>
#include "BL.cpp"
#define see(X) cout<<#X<<" "<<X<<endl;

using namespace std;

typedef vector<int> vi;
typedef vector<float> vf;
typedef vector<vi> vvi;
typedef map<int,int> mii;


int inf = 9999;



vf get_fitness(rectangulo contenedor, vr R,vvi p){
	vf f;
	for (int i = 0; i < p.size(); ++i){
		float s =0;
		int fallos = BL(contenedor,R,p[i]);
		int rect_no_introducidos = fallos;
		f.push_back(rect_no_introducidos +1);
	}
	return f;
}



float get_fitness(rectangulo contenedor, vr R,vi I){
	int fallos = BL(contenedor,R,I);
	int rect_no_introducidos = fallos;
	return rect_no_introducidos;
}

/****************/

void print_vi(vi v){
	for (int i = 0; i < v.size(); ++i){
		cout<<v[i]<<" ";
		//cout<<labels[v[i] ]<<" ";
	}
	cout<<endl;
}

void print_vi_labels(vi v){
	for (int i = 0; i < v.size(); ++i){
		cout<<v[i]<<" ";
		//cout<<labels[v[i] ]<<" ";
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

void print_vvi_labels(vvi v){
	for (int i = 0; i < v.size(); ++i){
		print_vi_labels(v[i]);
	}
}

int suma_vf(vf v){
	int s=0;
	for (int i = 0; i < v.size(); ++i){
		s+=v[i];
	}
	return s;
}

int suma_vi(vi v){
	int s=0;
	for (int i = 0; i < v.size(); ++i){
		s+=v[i];
	}
	return s;
}

void imprimir_poblacion(vvi p){
	for (int i = 0; i < p.size(); ++i){
		cout<<i+1<<") ";
		print_vi_labels(p[i]);
	}
}


void imprimir_fit_poblacion(rectangulo contenedor, vr R,vvi p){
	vf vfitness = get_fitness(contenedor,R,p);
	for (int i = 0; i < p.size(); ++i){
		cout<<i+1<<") ";
		for (int j = 0; j < p[0].size(); ++j){
			cout<<p[i][j]<<" ";
			//cout<<labels[p[i][j]]<<" ";
		}
		cout<<" - "<<vfitness[i]<<endl;;
	}
}


void imprimir_fit_ruleta_poblacion(rectangulo contenedor, vr R, vvi p){
	vf f = get_fitness(contenedor,R,p);
	int total = suma_vf(f);
	vf v_pro;//ruleta
	for (int i = 0; i < f.size(); ++i){
		v_pro.push_back((f[i]*100.0)/total); 
	}

	for (int i = 0; i < p.size(); ++i){
		cout<<i+1<<") ";
		for (int j = 0; j < p[0].size(); ++j){
			cout<<p[i][j]<<" ";
			//cout<<labels[p[i][j]]<<" ";

		}
		cout<<" - "<<f[i]<<" - "<<v_pro[i]<<endl;;
	}
}


bool find(vi v,int key){
	for (int i = 0; i < v.size(); ++i){
		if( key==v[i] ){
			return true;
		}
	}
	return false;
} 

int find_poss(vi v,int key){
	for (int i = 0; i < v.size(); ++i){
		if( key==v[i] ){
			return i;
		}
	}
	return -1;
} 

