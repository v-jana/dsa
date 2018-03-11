// zadanie2.c -- Zadanie 2 - Socialna siet
// Jana Vrablova, 3.11.2016 11:34:31

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#define COUNT 10000
#define NAME_OF_PAGE 200
#define HASH_SEED 0

typedef struct fans{
	int height;
	int lcount;
	char *name;
	struct fans *parent;
	struct fans *lchild;
	struct fans *rchild;	
}FANS;

typedef struct root{
	int count;
	struct fans *firstNode;
	char *key;
}ROOT;

struct root *hashMap[COUNT];

unsigned int hash_function(const char *s, unsigned int seed)
{
    unsigned int hash = seed;
    while (*s)
    {
        hash = hash * 101  +  *s++;
    }
    return hash;
}

void init()
{
	int i;
	for (i = 0; i < COUNT; i++) {
		hashMap[i] = NULL;
	}
}

int max(int x, int y)
{
	if(x > y){
		return x;
	} else {
		return y;
	}	
}

int getHeight(struct fans *current)
{
	if(current == NULL){
		return 0;
	} else {
		return current->height;
	}
	
}

struct fans *rotateRight(struct fans *current)
{
	struct fans *left = current->lchild;
	struct fans *rleft = left->rchild;

	left->rchild = current;
	current->lchild = rleft;
	
	if(current->parent != NULL){
		if(current->parent->lchild == current){
			current->parent->lchild = left;
		} else{
			current->parent->rchild = left;
		}
	}
	left->parent = current->parent;
	current->parent = left;
	if(rleft != NULL){
		rleft->parent = current;
	}
	
	current->lcount = current->lcount - left->lcount - 1;
	
	current->height = max(getHeight(current->lchild), getHeight(current->rchild))+1;
	left->height = max(getHeight(left->lchild), getHeight(left->rchild))+1;

	return left;
}

struct fans *rotateLeft(struct fans *current)
{
	struct fans *right = current->rchild;
	struct fans *lright = right->lchild;

	right->lchild = current;
	current->rchild = lright;

	if(current->parent != NULL){
		if(current->parent->lchild == current){
			current->parent->lchild = right;
		} else{
			current->parent->rchild = right;
		}
	}
	right->parent = current->parent;
	current->parent = right;
	if(lright != NULL){
		lright->parent = current;
	}
	
	right->lcount = right->lcount + current->lcount + 1;

	current->height = max(getHeight(current->lchild), getHeight(current->rchild))+1;
	right->height = max(getHeight(right->lchild), getHeight(right->rchild))+1;

	return right;
}

int getBalanceFactor(struct fans *current)
{
	int height;
	if (current == NULL){
		return 0;
	} else{
		height = getHeight(current->lchild) - getHeight(current->rchild);
		return height;
	}
}

void inorder(struct fans *current)
{
	if(current == NULL){
		return;
	}
	
	inorder(current->lchild);
	printf("%s\n", current->name);
	inorder(current->rchild);
}

int insertTree(struct root *rootp, char *user)
{
	struct fans *current;
	struct fans *add;
	struct fans *previous;
	int balanceFactor;

	current = rootp->firstNode;
	
	if(rootp->firstNode == NULL) {
		previous = NULL;
	} else {
		while (current != NULL){ 
			if(strcmp(user,current->name) < 0){
				previous = current;
				current = current->lchild;
			} else if(strcmp(user, current->name) > 0){
				previous = current;
				current = current->rchild;
			} else {
				return 0;
			}
			
		}
	}
	
	add = (struct fans*) malloc (sizeof(struct fans));
	add->height = 1;
	add->lcount = 0;
	add->lchild = NULL;
	add->rchild = NULL;
	add->name = strdup(user);
	rootp->count++;
	
	if (previous == NULL){
		rootp->firstNode = add;
		rootp->firstNode->parent = NULL;
	}
	else{
		if(strcmp(user, previous->name) < 0){
			previous->lchild = add;
			previous->lchild->parent = previous;
		}else{
			previous->rchild = add;
			previous->rchild->parent = previous;
		}
	}
	
	
	previous = add;
	current = add->parent;
	while(current != NULL){
		current->height = max(getHeight(current->lchild), getHeight(current->rchild))+1;
		if(previous == current->lchild){
			current->lcount++;
		}
		
		balanceFactor = getBalanceFactor(current);
		
		if(balanceFactor > 1 && getBalanceFactor(current->lchild) >= 0){
			current = rotateRight(current);
		}
		if(balanceFactor > 1 && getBalanceFactor(current->lchild) < 0){
			current->lchild = rotateLeft(current->lchild);
			current = rotateRight(current);
		}
		if(balanceFactor < -1 && getBalanceFactor(current->rchild) <= 0){
			current = rotateLeft(current);
		}
		if(balanceFactor < -1 && getBalanceFactor(current->rchild) > 0){
			current->rchild = rotateRight(current->rchild);
			current = rotateLeft(current);
		}
		previous = current;
		current = current->parent;
	}
		rootp->firstNode = previous;
	return 1;
}

void deleteFromTree(struct root *rootp, char *user)
{
	struct fans *current, *change, *parent, *previous;
	struct fans *left, *right, *child;
	char *tmpname;
	int balanceFactor, wasLeft = 0;
	
	current = rootp->firstNode;
	while(current != NULL){
		if(strcmp(user, current->name)  < 0){
			current = current->lchild;
		} else if(strcmp(user, current->name) > 0){
			current = current->rchild;
		} else{
			left = current->lchild;
			right = current->rchild;
			break;
		}
	}	
	
	
	if(current != NULL){
		rootp->count--;
		if(left != NULL && right != NULL){
			change = right;
			while(change->lchild != NULL){
				change = change->lchild;
			}
			tmpname = current->name;
			current->name = change->name;
			change->name = tmpname;
			current = change;
			left = current->lchild;
			right = current->rchild;
		}
		if(current == rootp->firstNode){
			rootp->firstNode = (left != NULL) ? left : right;
		}
		
		if(left == NULL && right == NULL){
			if(current->parent != NULL){
				if(current == current->parent->lchild){
					wasLeft = 1;
					current->parent->lchild = NULL;
				}else{
					current->parent->rchild =NULL;
				}
			}
			parent = current->parent;
			free(current->name);
			free(current);
		} else if(left == NULL || right == NULL){
			child = (left != NULL) ? left : right;
			if(current->parent != NULL){
				if(current == current->parent->lchild){
					wasLeft = 1;
					current->parent->lchild = child;
				}else{
					current->parent->rchild = child;
				}
			}
			child->parent = current->parent;
			parent = current->parent;
			free(current->name);
			free(current);
		}

		previous = NULL;
		current = parent;
		while(current != NULL){
			current->height = max(getHeight(current->lchild), getHeight(current->rchild))+1;
			if(wasLeft || (previous != NULL && previous == current->lchild)){
				current->lcount--;
				wasLeft = 0;
			}
			
			balanceFactor = getBalanceFactor(current);
			
			if(balanceFactor > 1 && getBalanceFactor(current->lchild) >= 0){
				current = rotateRight(current);
			}
			if(balanceFactor > 1 && getBalanceFactor(current->lchild) < 0){
				current->lchild = rotateLeft(current->lchild);
				current = rotateRight(current);
			}
			if(balanceFactor < -1 && getBalanceFactor(current->rchild) <= 0){
				current = rotateLeft(current);
			}
			if(balanceFactor < -1 && getBalanceFactor(current->rchild) > 0){
				current->rchild = rotateRight(current->rchild);
				current = rotateLeft(current);
			}
			previous = current;
			current = current->parent;
		}
		if(previous != NULL){
			rootp->firstNode = previous;
		}
	}
}

int getMapIndex(char *page, int inserting) 
{
	unsigned int start, index;
	
	start = index = hash_function(page, HASH_SEED) % COUNT;
	while(1){
		if(hashMap[index] == NULL){
			if (inserting) {
				return index;
			} else{
				return -1;
			}
		} else if(strcmp(hashMap[index]->key, page) == 0){
			return index;
		} else{
			if (index < COUNT - 1){
				index++;
			} else{
				index = 0;
			}
			if (index == start){
				return -1;
			}
			continue;
		
		}
	}
}

void like(char *page, char *user)
{
	int index;
	struct root *rootp;

	if ((index = getMapIndex(page, 1)) == -1){
		printf("Mame plnu mapu.\n");
	}
	else {
		if(hashMap[index] == NULL){
			rootp = (struct root*) malloc (sizeof(struct root));	
			hashMap[index] = rootp;
			rootp->key = strdup(page);
			rootp->count = 1;
			rootp->firstNode = NULL;
			insertTree(rootp, user);
		}else{
			insertTree(hashMap[index], user);
		}
	}	
	
}

void unlike(char *page, char *user)
{
	
	int index;
	index = getMapIndex(page, 0);
	if(index == -1){
		return;
	}
	deleteFromTree(hashMap[index], user);
}

char *find(struct fans *current, int k)
{	
	if(current == NULL){
		return NULL;
	}
	
	while(current != NULL){
		if(k == current->lcount){
			return current->name;
		} else if(k < current->lcount){
			current = current->lchild;
		} else{
			k = k - current->lcount - 1;
			current = current->rchild;
		}
	}
	return NULL;
}

char *getuser(char *page, int k)
{
	int index;
	index = getMapIndex(page, 0);
	k--;
	if(index == -1){
		return NULL;
	}
	struct fans *current = hashMap[index]->firstNode;
	
	return find(current, k);
}

void inorderLcount(struct fans *root)
{
	if(root == NULL){
		return;
	}
	inorderLcount(root->lchild);
	printf("%s %d\n", root->name, root->lcount);
	inorderLcount(root->rchild);
}

// Tlacidlo 'Spustit' vyskusa vasu socialnu siet na testovacich scenaroch.
// Vlastna funkcia main() nie je potrebna, a nebude sa spustat.

