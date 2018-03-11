// zadanie1.c -- Zadanie 1 - Spravca pamati
// Jana Vrablova, 29.9.2016 15:32:59

#include <stdio.h>

char *p;

int abs(int a) 
{
  if (a < 0) {
    return -a;
  }
  return a;
}

int memory_free(void *valid_ptr)
{
  char *ptr = p + sizeof(int), *predchadzajuci=NULL;
  char *platny, *dalsi;
  int velkost = *(int*)p;

  platny = valid_ptr;
  platny -= sizeof(int); 
  dalsi = platny + sizeof(int) + (*(int*)platny);

  while (ptr != platny){
	  predchadzajuci = ptr;
	  ptr = ptr + sizeof(int) + abs(*((int*)ptr));  
  } 

  if (predchadzajuci!=NULL && (*(int*)predchadzajuci) < 0){  /*spojenie s predchadzajucim volnym blokom*/
      (*(int*)predchadzajuci) = abs(*(int*)predchadzajuci);
	  (*(int*)predchadzajuci) += ((*(int*)ptr) + sizeof(int));
	  ptr = predchadzajuci;
  }
  
  if((dalsi) - p < velkost && (*(int *)dalsi) < 0){  
	  (*(int *)dalsi) = abs(*(int *)dalsi);
	  (*(int*)ptr) += (*(int *)dalsi) + sizeof(int);
  }

  (*(int*)ptr) = -(*(int*)ptr);
  
  return 0;
}


/*trivialna implementacia*/
void *memory_alloc(unsigned int size)
{
  int *hlavicka, *dalsia;
  int velkost = *((int*)p), zvysok;

  for (hlavicka=((int*)p) + 1; ((char*)hlavicka) - p < velkost; hlavicka=(int*)(((char*)hlavicka) + sizeof(int) + abs(*hlavicka))){
	if((*hlavicka) >= 0 || size > abs(*hlavicka)){
		continue;
    } 
    if((*hlavicka) < 0){
		if (size > (abs(*hlavicka) - sizeof(int))) {
			(*hlavicka)= abs(*hlavicka);
		} else {
			zvysok = abs(*hlavicka) - size - sizeof(int);
			(*hlavicka)= size;
			dalsia = (int*)(((char*)hlavicka) + sizeof(int) + (*hlavicka));
			(*dalsia) = -zvysok;
		}
		return hlavicka+1;
	}  	
  }
  return NULL;
}


int memory_check(void *ptr)
{
	int *platny;
	
	for (platny =((int*)p) + 1; platny < (int*)ptr - 1; platny =(int*)(((char*)platny) + abs(*platny) + sizeof(int)))
	{
		continue;
	} 

	if(platny != ((int*)ptr) - 1){
		return 0;
	}
	
	if (ptr == NULL || (((int*)ptr) -1) <= 0){
		return 0;
	}
  
  return 1;
}


void memory_init(void *ptr, unsigned int size)
{
  int *hlavicka;
  
  p = ptr;
  hlavicka = ptr;
  *hlavicka = size - sizeof(int);
  *(hlavicka + 1) = -((*hlavicka) - sizeof(int));
}


// Tlacidlo 'Spustit' vyskusa vasho spravcu pamati na testovacich scenaroch.
// Vlastna funkcia main() nie je potrebna, a nebude sa spustat.
