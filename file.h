#ifndef __FILE_H__
#define __FILE_H__

typedef struct Directory {
	char *name;
	struct File_list *headFile;
	struct Dir_list *headDir;
	struct Directory *parentDir;
} Directory;
typedef struct File {
	char *name;
	int size;
	char *data;
	Directory *dir;
} File;
typedef struct File_list {
	File *fisier;
	struct File_list *next;
} File_list;
typedef struct Dir_list {
	Directory *director;
	struct Dir_list *next;
} Dir_list;
void create_fs(Directory **curent);
void delete_fs(Directory **curent);
void touch(Directory **curent, char nume[], char text[]);
void mkdir(Directory **curent, char nume[]);
void parm(char sir[], char p1[], char p2[], char p3[], char p4[]);
void ls(Directory *cap);
void cd_1(Directory **curent, char nume[]);
void cd_2(Directory **curent);
void spatii(int numar);
void afisare_fisiere(File_list *nod, int nr_sp);
void tree(Dir_list *curent, int cnt);
void rm(Directory **curent, char nume[]);
void rm_d(Directory **curent, char nume[]);
void del_file_list(Directory **curent);
void del_dir_list(Directory **curent);
void pwd(Directory *curent);
void delete(Dir_list **curent);
void rmdir(Directory **curent, char nume[]);
void prop(Directory *curent, int min_size, int max_size, char subsir[]);
void find(Dir_list *curent, int depth, int min, int max, char subsir[]);

#endif
