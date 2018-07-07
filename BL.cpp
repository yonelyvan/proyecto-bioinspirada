#include <bits/stdc++.h>
#define see(X) cout<<#X<<" "<<X<<endl;
using namespace std;

typedef struct{
	int x,y;
	int dx;//ancho
	int dy;//alto
}rectangulo;

typedef vector<rectangulo> vr;
typedef vector<int> vi;
typedef vector<float> vf;
typedef vector<vi> vvi;

vr generar_rectangulos(int n){
	vr rectangulos; 
	for (int i = 0; i < n; ++i){
		rectangulo r;
		r.x=0;
		r.y=0;
		r.dx = 20+rand()%12;
		r.dy = 15+rand()%20;
		rectangulos.push_back(r);
	}
	return rectangulos;
}





bool cont_x(rectangulo contenedor,rectangulo r, rectangulo k){
	int rx_min = r.x;
	int rx_max = r.x+r.dx;
	int x_min = contenedor.dx-k.dx;
	int x_max = contenedor.dx;
	if(rx_min==x_min){return true;}
	if(rx_min < x_min){
		if( (rx_max-x_min)>0 ){
			return true;
		}
	}
	if(rx_min > x_min){
		if( (x_max-rx_min)>0 ){
			return true;
		}
	}
	return false;
}

bool cont_xl(rectangulo contenedor,rectangulo r, rectangulo k){
	int rx_min = r.x;
	int rx_max = r.x+r.dx;
	int x_min = k.x;
	int x_max = k.x+k.dx;
	if(rx_min==x_min){return true;}
	if(rx_min < x_min){
		if( (rx_max-x_min)>0 ){
			return true;
		}
	}
	if(rx_min > x_min){
		if( (x_max-rx_min)>0 ){
			return true;
		}
	}
	return false;
}

bool cont_y(rectangulo r, rectangulo k){
	int rx_min = r.y;
	int rx_max = r.y+r.dy;
	int x_min = k.y;
	int x_max = k.y+k.dy;
	if(rx_min==x_min){return true;}
	if(rx_min < x_min){
		if( (rx_max-x_min)>0 ){
			return true;
		}
	}
	if(rx_min > x_min){
		if( (x_max-rx_min)>0 ){
			return true;
		}
	}
	return false;
}
//contenedor, rectangulos insertados, rectangulo a insertar
void busqueda_vertical(rectangulo contenedor,vr &R, rectangulo &k){
	int max_x = contenedor.dx;
	int	min_x = max_x - k.dx;	
	
	int x=min_x;
	int y=0;
	for (rectangulo r : R){
		int r_min = r.x;
		int r_max = r.x+r.dx;
		if(cont_x(contenedor,r,k)){
			if( (r.y+r.dy) > y){//rectangulo mas en la cima
				y = r.y + r.dy;
			}
		}
	}
	k.x=x;
	k.y=y;
}

void busqueda_vertical_l(rectangulo contenedor,vr &R, rectangulo &k){
	int max_x = k.x + k.dx;
	int	min_x = k.x;	
	
	int x=min_x;
	int y=0;
	for (rectangulo r : R){
		int r_min = r.x;
		int r_max = r.x+r.dx;
		if(cont_xl(contenedor,r,k)){
			if( (r.y+r.dy) > y){//rectangulo mas en la cima
				y = r.y + r.dy;
			}
		}
	}
	k.x=x;
	k.y=y;
}
//contenedor, rectangulos insertados, rectangulo a insertar
void busqueda_horizontal(rectangulo contenedor,vr &R, rectangulo &k){
	int max_y = k.y+k.dy;
	int min_y = k.y;
	int x = 0;
	int y = k.y;
	for (rectangulo &r : R){
		int r_min = r.y;
		int r_max = r.y+r.dy;
		if( cont_y(r,k) ){//hay un rectangulo
			if( (r.x+r.dx) > x){//rectangulo mas en la cima
				x = r.x + r.dx;
			}
		}
	}
	k.x=x;
	k.y=y;
}

//in: vr
//out: vr [x,y]especificos
int BL(rectangulo contenedor, vr &R,vi orden){
	vr rr;
	int max_x = contenedor.dx;
	int max_y = contenedor.dy;
	int fallos =0;
	for (int i = 0; i < orden.size(); ++i){
		int j = orden[i];
		if(rr.size() <= 0){//esta vacio
			rr.push_back(R[j]);
		}else{
			busqueda_vertical(contenedor, rr,R[j]);
			//cout<<"antes poss: "<<R[j].x<<" "<<R[j].y<<endl;
			busqueda_horizontal(contenedor, rr,R[j]);
			//cout<<"despues poss: "<<R[j].x<<" "<<R[j].y<<endl;
			busqueda_vertical_l(contenedor, rr,R[j]);
			//cout<<"final poss: "<<R[j].x<<" "<<R[j].y<<endl;
			if( (R[j].x+R[j].dx) > max_x || (R[j].y+R[j].dy) > max_y){
				fallos++;
			}
			rr.push_back(R[j]);
		}
	}
	return fallos;
	//cout<<"FALLOS: "<<fallos<<endl;
}




void plot_rectangulos(rectangulo contenedor,vr R){
	std::ofstream file("rectangulos"); 
	file<<contenedor.x<<","<<contenedor.y<<","<<contenedor.dx<<","<<contenedor.dy<<endl;
	for (auto r:R){
		file<<r.x<<","<<r.y<<","<<r.dx<<","<<r.dy<<endl;
		cout<<r.x<<","<<r.y<<","<<r.dx<<","<<r.dy<<endl;
	} 
	system("python p.py 'rectangulos' ");
}


void plot_rectangulos_por_orden(rectangulo contenedor,vr R,vi orden){
	BL(contenedor,R,orden);//aplicar algoritmo BL 
	std::ofstream file("rectangulos"); 
	file<<contenedor.x<<","<<contenedor.y<<","<<contenedor.dx<<","<<contenedor.dy<<endl;
	
	cout<<"\nRectangulos: "<<endl;
	for (rectangulo r : R){
		file<<r.x<<","<<r.y<<","<<r.dx<<","<<r.dy<<endl;
		cout<<r.x<<","<<r.y<<","<<r.dx<<","<<r.dy<<endl;
	} 
	system("python p.py 'rectangulos' ");
}






/*
int main(){
	srand (time(NULL));
	
	//rectanglo principal
	rectangulo contenedor;
	contenedor.x = 0;
	contenedor.y = 0;
	contenedor.dx = 200;
	contenedor.dy = 200;
	//
	int n=20;
	vr R = generar_rectangulos(n);

	vi orden;
	for (int i = 0; i < n; ++i){
		orden.push_back(i);
	}

	BL(contenedor,R,orden);

	plot_rectangulos(contenedor,R);

	return 0;
}*/