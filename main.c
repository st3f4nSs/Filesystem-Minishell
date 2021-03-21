#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "file.h"
int main(void)
{
	int n, cnt, min, max, depth;
	char comanda[300], par1[70], par2[70], par3[70], par4[70];
	Directory *curent;

	cnt = 1;
	while (1) {
		fgets(comanda, 200, stdin);
		n = strlen(comanda);
		comanda[n-1] = '\0';
		if (strcmp(comanda, "create fs") == 0)
			create_fs(&curent);
		if (strcmp(comanda, "delete fs") == 0) {
			delete_fs(&curent);
			break;
		}
		if (strstr(comanda, "touch")) {
			parm(comanda, par1, par2, par3, par4);
			touch(&curent, par1, par2);
		}
		if (strstr(comanda, "mkdir")) {
			parm(comanda, par1, par2, par3, par4);
			mkdir(&curent, par1);
		}
		if (strstr(comanda, "cd")) {
			if (strcmp(comanda, "cd ..") == 0)
				cd_2(&curent);
			else {
				parm(comanda, par1, par2, par3, par4);
				cd_1(&curent, par1);
			}
		}
		if (strcmp(comanda, "ls") == 0)
			ls(curent);
		if (strcmp(comanda, "tree") == 0) {
			printf("%s\n", curent->name);
			if (curent->headFile)
				afisare_fisiere(curent->headFile, cnt);
			tree(curent->headDir, cnt);
		}
		if (strcmp(comanda, "pwd") == 0) {
			if (curent->parentDir == NULL)
				printf("/");
			else
				pwd(curent);
				printf("\n");
		}
		if (strstr(comanda, "rmdir")) {
			parm(comanda, par1, par2, par3, par4);
			rmdir(&curent, par1);
		} else
			if (strstr(comanda, "rm")) {
				parm(comanda, par1, par2, par3, par4);
				rm(&curent, par1);
			}
		if (strstr(comanda, "find")) {
			parm(comanda, par1, par2, par3, par4);
			depth = atoi(par1);
			min = atoi(par2);
			max = atoi(par3);
			prop(curent, atoi(par2), atoi(par3), par4);
			find(curent->headDir, depth, min, max, par4);
			printf("\n");
		}
	}
	return 0;
}
