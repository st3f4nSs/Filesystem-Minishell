#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "file.h"
/*
 * Functie pentru crearea, initializarea directorului root;
 * Alocam memorie pentru director si nume;
 * Initializam lista de fisiere, de directoare
si parintele directorului cu NULL;
 */
void create_fs(Directory **curent)
{
	Directory *nou;

	nou = (Directory *)malloc(sizeof(Directory));
	nou->name = (char *)malloc(3*sizeof(char));
	strcpy(nou->name, "/");
	nou->headDir = NULL;
	nou->headFile = NULL;
	nou->parentDir = NULL;
	*curent = nou;
}
/*
 * Functie pentru stergerea/eliberarea memoriei directorului root;
 * Eiberam memoria pentru numele directorului("/");
 * Eliberam memoria pentru director;
 */
void delete_fs(Directory **curent)
{
	free((*curent)->name);
	free(*curent);
}
/*
 * Functie care insereaza un fisier in lista, in ordine
lexicografica;
 * Alocam memorie  pentru noul element din lista, pentru fisier in sine,
pentru numele fisierului si pentru datele (continutul) fisierului;
 * Initializam campurile fisierului cu valorile corespunzatoare;
 * Avem 3 cazuri atunci cand inseram in lista de fisiere:
 * 1) Daca lista este vida atunci inseram elem si modificam capul
de lista;
 * 2) Daca elementul pe care vrem sa-l introducem este mai mic(lexicografic)
decat primul element, atunci adaugam elementul nou pe prima pozitie
si modificam capul de lista;
 * 3) Daca nu este nici 1) si nici 2), elementul trebuie adaugat fie la
mijloc fie la final atunci ne deplasam pana la cel mai mare element
mai mic decat elementul nostru si il adaugam dupa acesta;
 */
void touch(Directory **curent, char nume[], char text[])
{
	int dim1, dim2;
	File_list *nou, *p;

	dim1 = strlen(nume);
	dim2 = strlen(text);
	nou = (File_list *)malloc(sizeof(File_list));
	nou->fisier = (File *)malloc(sizeof(File));
	nou->fisier->name = (char *)malloc(sizeof(char)*(dim1+2));
	nou->fisier->data = (char *)malloc(sizeof(char)*(dim2+2));
	nou->fisier->dir = *curent;
	strcpy(nou->fisier->name, nume);
	strcpy(nou->fisier->data, text);
	nou->fisier->size = dim2;
	if ((*curent)->headFile == NULL) {
		nou->next = NULL;
		(*curent)->headFile = nou;
	} else {
		if (strcmp((*curent)->headFile->fisier->name, nume) > 0) {
			nou->next = (*curent)->headFile;
			(*curent)->headFile = nou;
		} else {
			p = (*curent)->headFile;
			while (p->next) {
				if (strcmp(p->next->fisier->name, nume) > 0)
					break;
				p = p->next;
			}
			nou->next = p->next;
			p->next = nou;
		}
	}
}
/*
 * Functie care insereaza un director in lista de directoare, in ordine
lexicografica;
 * Alocam memorie  pentru noul element din lista, pentru director in sine
si pentru numele directorului;
 * Initializam campurile directorului cu valorile corespunzatoare;
 * Avem 3 cazuri atunci cand inseram in lista de directoare:
 * 1) Daca lista este vida atunci inseram elementul si modificam capul
de lista;
 * 2) Daca elementul pe care vrem sa-l introducem este mai mic( lexicografic)
decat primul element, atunci adaugam elementul nou pe prima pozitie
si modificam capul de lista;
 * 3)Daca nu este nici 1) si nici 2), elementul trebuie adaugat fie la
mijloc fie la final atunci ne deplasam pana la cel mai mare element
mai mic decat elementul nostru si il adaugam dupa acesta;
 */
void mkdir(Directory **curent, char nume[])
{
	Dir_list *nou, *p;
	int dim;

	dim = strlen(nume);
	nou = (Dir_list *)malloc(sizeof(Dir_list));
	nou->director = (Directory *)malloc(sizeof(Directory));
	nou->director->name = (char *)malloc(sizeof(char)*(dim+2));
	nou->director->parentDir = *curent;
	nou->director->headFile = NULL;
	nou->director->headDir = NULL;
	strcpy(nou->director->name, nume);
	if ((*curent)->headDir == NULL) {
		nou->next = NULL;
		(*curent)->headDir = nou;
	} else {
		if (strcmp((*curent)->headDir->director->name, nume) > 0) {
			nou->next = (*curent)->headDir;
			(*curent)->headDir = nou;
		} else {
			p = (*curent)->headDir;
			while (p->next) {
				if (strcmp(p->next->director->name, nume) > 0)
					break;
				p = p->next;
			}
			nou->next = p->next;
			p->next = nou;
		}
	}
}
/*
 * Functie care identifica parametrii unei functii apelate si ii
stocheaza in p1, p2, p3 si p4;
 * Am scindat sirul( comanda) in mai multe subsiruri folosindu-ma de
functia strtok si am luat drept separatori sirul format din TAB
si SPATIU;
 */
void parm(char sir[], char p1[], char p2[], char p3[], char p4[])
{
	char *p;
	int cnt;

	cnt = 1;
	p = strtok(sir, "\n ");
	while (p) {
		if (cnt == 2)
			strcpy(p1, p);
		if (cnt == 3)
			strcpy(p2, p);
		if (cnt == 4)
			strcpy(p3, p);
		if (cnt == 5)
			strcpy(p4, p);
		cnt++;
		p = strtok(NULL, "\n ");
	}
}
/*
 * Functia ls listeaza intai fisierele apoi directoarele separate
prin spatiu;
 * Aceasta functie parcurge cele 2 liste:  lista de fisiere si
 lista de directoare a directorului curent si le afiseaza;
 */
void ls(Directory *cap)
{
	File_list *i;
	Dir_list *j;

	i = cap->headFile;
	while (i) {
		printf("%s ", i->fisier->name);
		i = i->next;
	}
	j = cap->headDir;
	while (j) {
		printf("%s ", j->director->name);
		j = j->next;
	}
	printf("\n");
}
/*
 * Functia cd_1 este de forma cd <directoar> si are rolul
de a ne muta in directorul dorit, daca acesta exista. In
caz contrar ne afiseaza un mesaj de eroare;
 * Parcuregem lista de directoare si daca gasim un director
cu numele cautat retinem pointerul catre acela si ii asociem
directorului curent noua valoare;
 */
void cd_1(Directory **curent, char nume[])
{
	Dir_list *i;
	Directory *p;
	int bec;

	bec = 0;
	i = (*curent)->headDir;
	while (i) {
		if (strcmp(i->director->name, nume) == 0) {
			bec = 1;
			p = i->director;
			break;
		}
		i = i->next;
	}
	if (bec == 1)
		*curent = p;
	else
		printf("Cannot move to '%s': No such directory!\n", nume);
}
/*
 * Functia cd_2 ne duce catre directorul precedent, cd ..;
 * Daca directorul curent nu este directorul root, atunci
cand vom apela cd .. ne vom muta in directorul parinte al
directorului curent;
 */
void cd_2(Directory **curent)
{
	if ((*curent)->parentDir != NULL)
		*curent = (*curent)->parentDir;
}
/*
 * Functie folosita pentru printarea numarului de spatii in cadrul
functiei tree;
 */
void spatii(int numar)
{
	int i;

	for (i = 1; i <= 4 * numar; i++)
		printf(" ");
}
/*
 * Functie care afiseaza lista de fisiere;
 * Utilizata in cadrul functiei tree;
 */
void afisare_fisiere(File_list *nod, int nr_sp)
{
	File_list *i;

	i = nod;
	while (i) {
		spatii(nr_sp);
		printf("%s\n", i->fisier->name);
		i = i->next;
	}
}
/*
 * Functia afiseaza intr-o maniera recursiva directoarele
si fisierele asociate acestora punand un numar de 4*nivel spatii,
unde nivel se refera la adancimea grafului;
 */
void tree(Dir_list *curent, int cnt)
{
	if (curent != NULL) {
		spatii(cnt);
		printf("%s\n", curent->director->name);
		afisare_fisiere(curent->director->headFile, cnt+1);
		tree(curent->director->headDir, cnt+1);
		tree(curent->next, cnt);
	}
}
/*
 * Functie care sterge un fisier cu numele "nume", daca exista
din directorul curent;
 * Daca lista este nevida, pronim de la al doilea element al listei
si pentru fiecare element verificam daca urmatorul element este cel cautat.
Ne uitam la urmatorul element pentru a putea realiza stergerea, avand nevoie
de adresa elementului precedent, a elementului cautat;
 * Cand gasim un fisier cu numele nume il stergem, modificam legaturile
dintre pointeri, si dealocam memoria;
 * Cea de-a doua parte a acestui algoritm verifica daca primul element este
cel cautat, caz in care il stergem, modificam capul de lista si dealocam
memoria;
 */
void rm(Directory **curent, char nume[])
{
	File_list *i, *p;
	int bec;

	bec = 0;
	if ((*curent)->headFile) {
		i = (*curent)->headFile;
		while (i->next) {
			if (strcmp(i->next->fisier->name, nume) == 0) {
				bec = 1;
				p = i->next;
				i->next = p->next;
				free(p->fisier->name);
				free(p->fisier->data);
				free(p->fisier);
				free(p);
			} else
				i = i->next;
		}
		if (strcmp((*curent)->headFile->fisier->name, nume) == 0) {
			bec = 1;
			i = (*curent)->headFile;
			(*curent)->headFile = i->next;
			free(i->fisier->name);
			free(i->fisier->data);
			free(i->fisier);
			free(i);
		}
	}
	if (bec == 0)
		printf("Cannot remove '%s': No such file!\n", nume);
}
/*
 * Functie care sterge un director cu numele "nume", daca acesta exista
din directorul curent;
 * Functia urmeaza acelasi algoritm ca functia "rm", doar ca asupra
directoarelor si nu a fisierelor;
 * Functie folosita pentru implementarea RMDIR;
 */
void rm_d(Directory **curent, char nume[])
{
	Dir_list *i, *p;

	if ((*curent)->headDir) {
		i = (*curent)->headDir;
		while (i->next) {
			if (strcmp(i->next->director->name, nume) == 0) {
				p = i->next;
				i->next = p->next;
				free(p->director->name);
				free(p->director);
				free(p);
			} else
				i = i->next;
		}
		if (strcmp((*curent)->headDir->director->name, nume) == 0) {
			i = (*curent)->headDir;
			(*curent)->headDir = i->next;
			free(i->director->name);
			free(i->director);
			free(i);
		}
	}
}
/*
 * Functie care dealoca memoria pentru o lista de fisiere;
 * Parcurgem lista si stocam intr-o alta variabila nodul curent,
pentru a-l putea sterge, trecem la nodul urmator si apoi dealocam
memoria pentru nodul curent(stocat in vriabila "j"). Aceasta operatie
se efectueaza cat timp nu am ajuns la capatul de lista;
 * la final dupa ce lista este complet dealocata atribuim capului de lista
valoarea NULL, semnificand faptul ca lista este vida;
 * Functie folosita pentru RMDIR;
 */
void del_file_list(Directory **curent)
{
	File_list *i, *j;

	i = (*curent)->headFile;
	while (i) {
		j = i;
		i = i->next;
		free(j->fisier->name);
		free(j->fisier->data);
		free(j->fisier);
		free(j);
	}
	(*curent)->headFile = NULL;
}
/*
 * Functie care dealoca memoria pentru o lista de directoare;
 * Parcurgem lista si stocam intr-o alta variabila nodul curent,
pentru a-l putea sterge, trecem la nodul urmator si apoi dealicam
memoria pentru nodul curent(stocat in vriabila "j");
 * la final dupa ce lista este complet dealocata atribuim capului de lista
valoarea NULL, semnificand faptul ca lista este vida;
 * Functie folosita pentru RMDIR;
 */
void del_dir_list(Directory **curent)
{
	Dir_list *i, *j;

	i = (*curent)->headDir;
	while (i) {
		j = i;
		i = i->next;
		free(j->director->name);
		free(j->director);
		free(j);
	}
	(*curent)->headDir = NULL;
}
/*
 * Functie care afiseaza calea la care ne aflam;
 * Am folosit recursivitate pe stiva;
 * Am afisat parintele directorului curent, atat timp cat directorul
curent este diferit de root, intr-o maniera recursiva;
 */
void pwd(Directory *curent)
{
	if (curent->parentDir) {
		pwd(curent->parentDir);
		printf("/%s", curent->name);
	}
}
/*
 * Functie care sterge recursiv un director;
 * In urma apelarii acestei functii nu se va sterge directorul,
lista de fisiere si lista de directoare asociata acestuia, adica
nu va sterge primul nivel de sub director si directorul;
 * Functie utilizata in RMDIR
 */
void delete(Dir_list **curent)
{
	if (*curent != NULL) {
		delete(&((*curent)->director->headDir));
		del_file_list(&((*curent)->director));
		del_dir_list(&((*curent)->director));
		delete(&((*curent)->next));
	}
}
/*
 * Functie rmdir sterge intreg directorul precizat, daca
acesta exista, in caz contrar afiseaza un mesaj;
 * Cauta directirul cu numele "nume" si in cazul in care exista
foloseste functiile precizate mai sus si il sterge;
 * Ultima parte a programului: functia delete sterge toate nivelele
directorului in afara de primul nivel si de director in sine. Functia
del_dir_list sterge lista de fisiere de pe primul nivel. Functia
del_dir_list sterge lista de directoare de pe primul nivel. Functia
rm_d finalizeaza operatia de stergere, stergand doar directorul;
 */
void rmdir(Directory **curent, char nume[])
{
	Dir_list *i;
	Directory *retine;
	int bec;

	bec = 0;
	i = (*curent)->headDir;
	while (i) {
		if (strcmp(i->director->name, nume) == 0) {
			bec = 1;
			retine = i->director;
			break;
		}
		i = i->next;
	}
	if (bec == 1) {
		delete(&(retine->headDir));
		del_dir_list(&(retine));
		del_file_list(&(retine));
		rm_d(&*curent, retine->name);
	} else
		printf("Cannot remove '%s': No such directory!\n", nume);
}
/*
 * Functie care afiseaza fisierele care au o anumita proprietate, pentru
functia find;
 * Parcurgem lista de fisiere si le afisam pe acelea cu respectiva
proprietate;
 */
void prop(Directory *curent, int min_size, int max_size, char subsir[])
{
	File_list *i;
	File *j;

	i = curent->headFile;
	while (i) {
		j = i->fisier;
		if (j->size <= max_size && j->size >= min_size) {
			if (strstr(j->data, subsir) != NULL)
				printf("%s ", i->fisier->name);
		}
		i = i->next;
	}
}
/*
 * Cauta intr-o maniera recurisva fisierele care au dimensiunea intre 2 numere,
care contin subsirul "subsir" si care se afla la o adancime de maxim depth;
 * Pe langa aceasta functie in MAIN mai afisaam si din lista de fisiere de
pe nivelul 0, fisierele cu proprietate prop, daca exista;
 */
void find(Dir_list *curent, int depth, int min, int max, char subsir[])
{
	if (curent != NULL && depth != 0) {
		prop(curent->director, min, max, subsir);
		find(curent->director->headDir, depth - 1, min, max, subsir);
		find(curent->next, depth, min, max, subsir);
	}
}
