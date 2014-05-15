/**
	\file readdir.c
	\brief Ce programme évalue l'utilisation de readdir en fonction du nombre de fichier présent dans le repertoire.

	Ce programme va donc accéder à différent répertoire contenant pour le premier un fichier text et pour le dernier 100 fichiers text, en comparant les temps nécessaire à chacun.

*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <errno.h>
#include <dirent.h>

#include "benchmark.h"

void exit_error(char *s) 
{
	perror(s);
	exit(EXIT_FAILURE);
}

int main (int argc, char *argv[])  {

	int a = system("./dir_init.sh");
	if (a==-1)
	exit_error("creation des files");
	// Déclare un timer, ainsi qu'un recorder qui va contenir les résultats de l'exécution du programme

	timer * t = timer_alloc();
	recorder * read_rec = recorder_alloc("readdir.csv");

	int i;
	DIR * dirp;
	struct dirent *dp;

	for(i=0;i<100;i++)
	{
		char name[20];
		sprintf(name, "file/file%d", i);
		
		dirp = opendir(name);
		
		if(dirp==NULL)
		{exit_error("opendir");}
		
		start_timer(t);
		dp=readdir(dirp);
		write_record(read_rec, i, stop_timer(t));
	
	}

	int err = closedir(dirp);
	if(err<<0)
	{exit_error("closedir");}

	recorder_free(read_rec);
	timer_free(t);
	
return EXIT_SUCCESS;
}
	
	

