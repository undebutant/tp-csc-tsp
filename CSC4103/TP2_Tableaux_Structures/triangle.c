#include <stdio.h>
#include <stdlib.h>



// Trouve le rang ou inserer la valeur val dans le tableau tab, de taille top
int sort_get_index (float tab[], int top, float val) {
	for (int i = 0 ; i < top ; i++) {
		if (tab[i] > val) {
			return i;
		}
	}
	return top;
}


// Insere dans tab de taille top la valeur val a l'indice i
void sort_insert_at (float tab[], int i, int top, float val) {
	for (int a = top ; a > i ; a--) {
		tab[a] = tab[a - 1];
	}
	tab[i] = val;

}

// Tri par insertion (cf avant)
void sort_insert (float tab[], int top, float val) {
	int i = sort_get_index (tab, top, val);
	sort_insert_at (tab, i, top, val);
}


// Def du nb de colonnes
int nb_columns () {
	return 129;
}


// Def du nb de lignes
int nb_lines () {
	return 65;
}


// Initialisation de la grid par le syymbole pixel
void grid_init (char grid[], char pixel) {
	for (int i = 0 ; i < nb_columns() * nb_lines() ; i++) {
		grid[i] = pixel;
	}
}


// Affichage de la grid
void grid_display (char grid[]) {
	printf("\n\n");
	for (int i = 0 ; i < nb_columns() * nb_lines() ; i++) {
		if (i % nb_columns() == 0) {
			printf("\n");
		}
		printf("%c", grid[i]);
	}
	printf("\n\n");
}


// Changement du caractere (x,y) de la grid par pixel, attention on part de (0,0) en bas à gauche !
void plot_point (char grid[], int x, int y, char pixel) {
	// Gestion des out of bonds sur l'appel
	if (x < 0 || x > nb_columns()) { printf("x out of bond in plot_point\n\n"); exit(EXIT_FAILURE); }
	if (y < 0 || y > nb_lines()) { printf("y out of bond in plot_point\n\n"); exit(EXIT_FAILURE); }
	
	grid[(nb_lines() - y - 1) * nb_columns() + x] = pixel;	
}


// Tracer d'une ligne verticale de y0 a y1 en x, avec arrondis des floats fy0 et fy1
void plot_vline (char grid[], int x, float fy0, float fy1, char pixel) {
	int y0 = (int) (fy0 + 0.5);
	int y1 = (int) (fy1 + 0.5);
	
	if (fy0 - (int) fy0 == 0.5) { y0 -= 1; }

	for (int i = 0 ; i < y1 - y0 + 1 ; i++) {
		grid[(nb_lines() - y1 + i - 1) * nb_columns() + x] = pixel;	
	}
}


// On definit un point
struct point
{
	float x;
	float y;
};


// Parcourt d'une ligne verticale et identification des intersections avec les cotes du polygone definit par les points
void plot_poly_sweep (char grid[], struct point p[], int nb_points_tab, int x, char pixel) {
	
	//printf("\nSweep %d : ", x);
	int j = nb_points_tab - 1;

	float vlines [4 * nb_points_tab];
	int top = 0;

	for (int i = 0 ; i < nb_points_tab ; i++) {
		if ( (x >= p[i].x && x <= p[j].x) || (x >= p[j].x && x <= p[i].x) ) {
			if ((p[i].x <= p[j].x && p[i].x <= x && x < p[j].x) || (p[j].x <= p[i].x && p[j].x <= x && x < p[i].x)) {
				float y = p[i].y + (x - p[i].x) * (p[j].y - p[i].y) / (p[j].x - p[i].x);
				sort_insert (vlines, top, y);
				top += 1;
			}
		}
		j = i;
	}
	
	for (int k = 0 ; k < top - 1 ; k += 2) {
		plot_vline (grid, x, vlines[k], vlines[k + 1], pixel);
	}	
}


// Usage du sweep sur toutes les colonnes
void plot_poly (char grid[], struct point p[], int nb_points_tab, char pixel) {
	for (int i = 0 ; i < nb_columns() ; i++) {
		plot_poly_sweep (grid, p, nb_points_tab, i, pixel);
	}
}


void plot_triangle (char grid[], struct point p1, struct point p2, struct point p3, char pixel) {
	struct point p[3] = {p1, p2, p3};
	plot_poly (grid, p, 3, pixel);
}


struct point line_middle(struct point p1, struct point p2) {
	struct point p3 = {(p1.x + p2.x)/2, (p1.y + p2.y)/2};
	return p3;
}


void sierpinski (char grid[], struct point p1, struct point p2, struct point p3, int n, char pixel){
	if(n==0){
		plot_triangle(grid, p1, p2, p3, pixel);
	}
	else
	{
		struct point p4 = line_middle(p1, p2);
		struct point p5 = line_middle(p2, p3);
		struct point p6 = line_middle(p1, p3);
		sierpinski(grid, p4, p1, p6, (n-1), pixel);
		sierpinski(grid, p5, p6, p3, (n-1), pixel);
		sierpinski(grid, p2, p4, p5, (n-1), pixel);
	}
}



int main (int argc, char *argv[])
{

	char grid [nb_columns()*nb_lines()];
	grid_init(grid, ' ');

	//struct point p[6] = {{ 2, 13 }, { 10, 13 }, { 30, 7 }, { 10, 1 }, { 2, 1 }, { 18, 7 }};
	//plot_poly (grid, p, 6, '#');
	
	struct point p1 = {0, 0};
	struct point p2 = {64, 64};
	struct point p3 = {128, 0};

	//plot_triangle (grid, p1, p2, p3, '#');
	
	int rang;
	scanf("%d", &rang);
	sierpinski (grid, p1, p2, p3, rang, '#');

	grid_display(grid);

}





