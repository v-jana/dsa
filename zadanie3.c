// zadanie3.c -- Zadanie 3 - Popolvar
// Jana Vrablova, 29.11.2016 00:37:30

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

typedef struct Vrchol{
	int vrchol;
	int suma;
}Vrchol;

//prvy index vo vertexes prazdny
typedef struct Halda{
	struct Vrchol *vrcholy;
	int pocet;
}Heap;


void inicializuj_haldu(struct Halda *halda, int velkost) //???
{
	halda->vrcholy = (struct Vrchol*) malloc (velkost * sizeof(struct  Vrchol));
	halda->pocet = 0;
}

void zmen(struct Halda *halda, int index1, int index2)
{
	struct Vrchol tmp;
	tmp = halda->vrcholy[index1];
	halda->vrcholy[index1] = halda->vrcholy[index2];
	halda->vrcholy[index2] = tmp;
}

void zmen_nahor(struct Halda *halda, int index)
{
	while(index > 1 && halda->vrcholy[index/2].suma > halda->vrcholy[index].suma){
		zmen(halda, index/2, index);
		index = index/2;
	}
}

void zmen_nadol(struct Halda *halda, int index )
{
	int min;

	while(index <= halda->pocet/2){
		min = index * 2;
		if((min + 1) <= halda->pocet && halda->vrcholy[min].suma > halda->vrcholy[min + 1].suma){
			min += 1;
		}
		if(halda->vrcholy[index].suma > halda->vrcholy[min].suma ){
			zmen(halda, index, min);
		} else {
			break;
		}
		index = min;
	}
}

void nastav_hodnotu(struct Halda *halda, int vrchol, int suma)
{
	int i;

	for(i = 1; i <= halda->pocet; i++){
		if(halda->vrcholy[i].vrchol == vrchol && halda->vrcholy[i].suma > suma){
			halda->vrcholy[i].suma = suma;
			zmen_nahor(halda, i);
			break;
		}
	}

}

void vloz_do_haldy(struct Halda *halda, int vrchol, int suma)
{
	halda->pocet++;
	halda->vrcholy[halda->pocet].vrchol = vrchol;
	halda->vrcholy[halda->pocet].suma = suma;
	zmen_nahor(halda, halda->pocet);
}

void vyber_minimum(struct Halda *halda, int *min)
{
	min[0] = halda->vrcholy[1].vrchol;
	min[1] = halda->vrcholy[1].suma;
	halda->vrcholy[1] = halda->vrcholy[halda->pocet];
	halda->pocet--;
	zmen_nadol(halda, 1);

}

int index_vrchola(int x, int y, int sirka)
{
	return x*sirka + y;
}

int cas_cesty(char **mapa, int x, int y, int zapnuty)
{

	if(mapa[x][y] == 'H'){
		return 2;
	}else if (mapa[x][y] == 'N'){
		return -1;
	}else {
		return 1;
	}
	return -1;
}

int je_v_mape(int x, int y, int n, int m)
{
	if(x < 0 || y < 0 || x > (n-1) || y > (m-1)) {
		return 0;
	} else {
		return 1;
	}

}

void uloz_kratsiu_cestu(struct Halda *halda, int index_novy, int index_stary, int cas, int *predchadzajuce_vrcholy, int *je_v_halde, int *suma_v_halde)
{
	if(cas == -1){
		return;
	} else if(je_v_halde[index_novy]){
		if(suma_v_halde[index_novy] > suma_v_halde[index_stary] + cas){
			suma_v_halde[index_novy] = suma_v_halde[index_stary] + cas;
			predchadzajuce_vrcholy[index_novy] = index_stary;
			nastav_hodnotu(halda, index_novy, suma_v_halde[index_novy]);
		}
	}else {
		vloz_do_haldy(halda, index_novy, suma_v_halde[index_stary] + cas);
		predchadzajuce_vrcholy[index_novy] = index_stary;
		suma_v_halde[index_novy] = suma_v_halde[index_stary] + cas;
		je_v_halde[index_novy] = 1;
	}
}

int *dijkstra(char **mapa, int n, int m, int zaciatok, int ciel)
{
	struct Halda *halda;
	int *predchadzajuce_vrcholy;
	int *navstivene_vrcholy;
	int *je_v_halde;
	int *suma_v_halde;
	int i, k = 0, x, y;
	int velkost = m * n;
	int *min;
	int *cesta;
	int index;

	halda = (struct Halda*) malloc (1 * sizeof(struct Halda));
	inicializuj_haldu(halda, velkost + 1);
	predchadzajuce_vrcholy = (int*) malloc (velkost * sizeof(int));
	navstivene_vrcholy = (int*) calloc (velkost, sizeof(int));
	je_v_halde = (int*) calloc (velkost, sizeof(int));
	suma_v_halde = (int*) malloc (velkost* sizeof(int));
	cesta  = (int*) malloc (velkost * sizeof(int));
	for(i = 0; i < velkost; i++){
		suma_v_halde[i] = INT_MAX;
	}
	min = (int*) malloc (2 * sizeof(int));

	x = zaciatok/m;
	y = zaciatok%m;

	vloz_do_haldy(halda, zaciatok, cas_cesty(mapa, x, y, 0));
	je_v_halde[zaciatok] = 1;

	while(halda->pocet > 0){
		vyber_minimum(halda, min);
		x = min[0]/m;
		y = min[0]%m;
		navstivene_vrcholy[min[0]] = 1;

		if(je_v_mape(x+1, y, n, m) && ((navstivene_vrcholy[index_vrchola(x+1, y, m)]) == 0)){
			uloz_kratsiu_cestu(halda, index_vrchola(x+1, y, m), index_vrchola(x,y,m), cas_cesty(mapa, x+1, y, 0), predchadzajuce_vrcholy, je_v_halde, suma_v_halde);
		}
		if(je_v_mape(x-1, y, n, m) && (navstivene_vrcholy[index_vrchola(x-1, y, m)]) == 0){
			uloz_kratsiu_cestu(halda, index_vrchola(x-1, y, m), index_vrchola(x,y,m), cas_cesty(mapa, x-1, y, 0), predchadzajuce_vrcholy, je_v_halde, suma_v_halde);
		}
		if(je_v_mape(x, y+1, n, m) && (navstivene_vrcholy[index_vrchola(x, y+1, m)]) == 0){
			uloz_kratsiu_cestu(halda, index_vrchola(x, y+1, m), index_vrchola(x,y,m), cas_cesty(mapa, x, y+1, 0), predchadzajuce_vrcholy, je_v_halde, suma_v_halde);
		}
		if(je_v_mape(x, y-1, n, m) && (navstivene_vrcholy[index_vrchola(x, y-1, m)]) == 0){
			uloz_kratsiu_cestu(halda, index_vrchola(x, y-1, m), index_vrchola(x,y,m), cas_cesty(mapa, x, y-1, 0), predchadzajuce_vrcholy, je_v_halde, suma_v_halde);
		}

	}

	k = 1;
	cesta[k++] = suma_v_halde[ciel];
	cesta[k++] = predchadzajuce_vrcholy[ciel];
	index = predchadzajuce_vrcholy[ciel];
	while(index != zaciatok){
		cesta[k] = predchadzajuce_vrcholy[index];
		k++;
		index = predchadzajuce_vrcholy[index];
	}
	
	cesta[0] = k;

	free(predchadzajuce_vrcholy);
	free(navstivene_vrcholy);
	free(min);
	free(halda->vrcholy);
	free(halda);
	free(je_v_halde);
	free(suma_v_halde);

	return cesta;
}

int *najdi_najkratsiu(int **cesty)
{
	int najkratsia = INT_MAX;
	int *cesta;

	cesta = (int*) malloc (4* sizeof(int));
	// drak -> 1. -> 2. -> 3.
	if(cesty[0][1] + cesty[3][1] + cesty[5][1]  < najkratsia){
		najkratsia = cesty[0][1] + cesty[3][1] + cesty[5][1];
		cesta[0] = 0;
		cesta[1] = 1;
		cesta[2] = 2;
		cesta[3] = 3;
	}
	// drak -> 1. -> 3. -> (obratene)  2.
	if(cesty[0][1] + cesty[4][1] + cesty[5][1] < najkratsia){
		najkratsia = cesty[0][1] + cesty[4][1] + cesty[5][1];
		cesta[0] = 0;
		cesta[1] = 1;
		cesta[2] = 3;
		cesta[3] = 2;
	}
	// drak -> 2. -> (obratene) 1. -> 3.
	if(cesty[1][1] + cesty[3][1] + cesty[4][1] < najkratsia){
		najkratsia = cesty[1][1] + cesty[3][1] + cesty[4][1];
		cesta[0] = 0;
		cesta[1] = 2;
		cesta[2] = 1;
		cesta[3] = 3;
	}
	// drak -> 2. -> 3. -> (obratene) 1.
	if(cesty[1][1] + cesty[5][1] + cesty[4][1] < najkratsia){
		najkratsia = cesty[1][1] + cesty[5][1] + cesty[4][1];
		cesta[0] = 0;
		cesta[1] = 2;
		cesta[2] = 3;
		cesta[3] = 1;
	}
	// drak -> 3. -> (obratene) 1. -> 2.
	if(cesty[2][1] + cesty[4][1] + cesty[3][1] < najkratsia){
		najkratsia = cesty[2][1] + cesty[4][1] + cesty[3][1];
		cesta[0] = 0;
		cesta[1] = 3;
		cesta[2] = 1;
		cesta[3] = 2;
	}
	// drak -> 3. -> (obratene) 2. -> (obratene) 1.
	if(cesty[2][1] + cesty[5][1] + cesty[3][1]  < najkratsia){
		najkratsia = cesty[2][1] + cesty[5][1] + cesty[3][1];
		cesta[0] = 0;
		cesta[1] = 3;
		cesta[2] = 2;
		cesta[3] = 1;
	}
	return cesta;

}

int *vytvor_cestu(char **mapa, int n, int m, int *beings)
{
	int i, k = 1;
	int **cesty;
	int *najratsia_kombinacia;
	int *g_drak1;
	int *cela_cesta;
	int *spatna_cesta;
	int p;

	g_drak1 =  dijkstra(mapa, n, m, 0, beings[0]);

	cesty = (int**) malloc (12 * sizeof(int*));

	i = 0;

	cesty[i++] = dijkstra(mapa, n, m, beings[0], beings[1]);
	cesty[i++] = dijkstra(mapa, n, m, beings[0], beings[2]);
	cesty[i++] = dijkstra(mapa, n, m, beings[0], beings[3]);
	cesty[i++] = dijkstra(mapa, n, m, beings[1], beings[2]);
	cesty[i++] = dijkstra(mapa, n, m, beings[1], beings[3]);
	cesty[i++] = dijkstra(mapa, n, m, beings[2], beings[3]);

	najratsia_kombinacia = najdi_najkratsiu(cesty);

	cela_cesta = (int*) malloc (n*m * sizeof(int));
	spatna_cesta = (int*) malloc (n*m * sizeof(int));
	cela_cesta[1] = najratsia_kombinacia[0];
	for(i = 2; i < g_drak1[0]; i++){
		cela_cesta[i] = g_drak1[g_drak1[0]-i+1];
	}
	k = g_drak1[0];

	if(najratsia_kombinacia[1] == 1){
		for(i = 2; i < cesty[0][0]; i++){
			cela_cesta[k++] = cesty[0][cesty[0][0]-i+1];
		}
		p = 1;
	}
	if(najratsia_kombinacia[1] == 2){
		for(i = 2; i < cesty[1][0]; i++){
			cela_cesta[k++] = cesty[1][cesty[1][0]-i+1];
		}
		p = 2;
	}
	if(najratsia_kombinacia[1] == 3){
		for(i = 2; i <  cesty[2][0]; i++){
			cela_cesta[k++] = cesty[2][cesty[2][0]-i+1];
		}
		p = 3;
	}
	free(spatna_cesta);
	spatna_cesta = dijkstra(mapa, n, m, beings[p], beings[najratsia_kombinacia[2]]);
	for(i = 2; i < spatna_cesta[0]; i++){
			cela_cesta[k++] = spatna_cesta[spatna_cesta[0]-i+1];
	}
	p = najratsia_kombinacia[2];
	free(spatna_cesta);
	spatna_cesta = dijkstra(mapa, n, m, beings[p], beings[najratsia_kombinacia[3]]);
	for(i = 2; i < spatna_cesta[0]; i++){
			cela_cesta[k++] = spatna_cesta[spatna_cesta[0]-i+1];
    }

	cela_cesta[k++] = beings[najratsia_kombinacia[3]];
	cela_cesta[0] = k;

	free(g_drak1);
	for(i = 0; i < 6; i++){
		free(cesty[i]);
	}
	free(cesty);
	free(najratsia_kombinacia);
	free(spatna_cesta);

	return cela_cesta;
}

int *zachran_princezne(char **mapa, int n, int m, int t, int *dlzka_cesty)
{
	int i, j, p = 0;
	int *beings;
	int *cesta, *finalna;

	beings = (int*) malloc (4 * sizeof(int));
	for (i = 0; i < 4; i++) {
		beings[i] = -1;
	}
	
	for(i = 0; i < n; i++){
		for(j = 0; j < m; j++){
			switch(mapa[i][j]){
				case 'D': beings[0] = index_vrchola(i, j, m); break;
				case 'P': beings[1 + p] = index_vrchola(i, j, m); p++; break;
			}
		}
	}

	cesta = vytvor_cestu(mapa, n, m, beings);
	finalna = (int*) malloc (cesta[0]*2 * sizeof(int));

	for(i = 2; i < cesta[0]; i++){
		finalna[(i-2)*2] = cesta[i]%m;
		finalna[(i-2)*2+1] = cesta[i]/m;
	}

	*dlzka_cesty = (cesta[0]-2);
  printf("%d\n", *dlzka_cesty);

	free(cesta);
	free(beings);
	
	return finalna;
}


// Tlacidlo 'Spustit' vyskusa vasu socialnu siet na testovacich scenaroch.
// Vlastna funkcia main() nie je potrebna, a nebude sa spustat.