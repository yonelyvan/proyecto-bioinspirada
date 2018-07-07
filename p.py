import matplotlib.pyplot as plt
import numpy as np
import sys

def plot_main_rectangle(continer,v):
	x = v[0]
	y = v[1]
	dx = v[2]
	dy = v[3]
	print "Contenerdor: ",x," ",y," ",dx," ",dy
	rectangle = plt.Rectangle((x, y),dx, dy, fc='g')
	continer.gca().add_patch(rectangle);

def plot_rectangle(continer,v):
	x = v[0]
	y = v[1]
	dx = v[2]
	dy = v[3]
	rectangle = plt.Rectangle((x, y), dx, dy, fc='r')
	continer.gca().add_patch(rectangle);

def load_data(file):
	M = np.genfromtxt(file,delimiter=',')
	return M

def main():
	file = sys.argv[1] #nombre de archivo parametro
	M = load_data(file)
	[f,c] = M.shape
	
	plt.axes()

	plot_main_rectangle(plt, M[0]);#rectangulo principal
	for i in range(1,f):
		plot_rectangle(plt,M[i]);
	plt.axis('scaled')
	plt.show()



main()

#system("python p.py 'test' ");