/**
	\file writev.c
	\brief Ce programme évalue le temps utilisé par writev pour écrire selon un pattern précis, par rapport à l'utilisation de write + lseek.


	writev utilisera des buffers de taille définie dans lesquels seront copiés les strings d'une carte de visite.
	write copiera directement les strings, puis utilisera lseek pour atteindre la zone où sera notée l'information suivante.
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <errno.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <sys/uio.h>

#include "benchmark.h"

// Constantes de longueur des buffers
// Ne pas oublier le '\0' terminal
#define LNOM 16
#define LPRENOM 16
#define LNAISSANCE 11
#define LETAT_CIVIL 13
#define LPROFESSION 26
#define LORGANISATION 52
#define LTELEPHONE 14
#define IOVEC_FIELDS 7

struct carte_visite 
{
	char nom[LNOM];
	char prenom[LPRENOM];
	char naissance[LNAISSANCE];
	char etat_civil[LETAT_CIVIL];
	char profession[LPROFESSION];
	char organisation[LORGANISATION];
	char telephone[LTELEPHONE];
};


void exit_error(char *s) 
{
	perror(s);
	exit(EXIT_FAILURE);
}

int main (int argc, char *argv[]) 
{
	timer * t = timer_alloc();
	recorder* writev_rec = recorder_alloc("writev.csv");
	recorder* lseek_rec = recorder_alloc("write+lseek.csv");

	int i, j, err, err1;

	// WARNING : the following sections are NOT protected against buffer overflows
	// Do NOT modify this section without keeping it in mind and do NOT use manual input
	char* nom = "Obiwan";
	int nom_len = strlen(nom) + 1;
	int nom_os = LNOM - nom_len;
	char* prenom = "Kenobi";
	int prenom_len = strlen(prenom) + 1;
	int prenom_os = LPRENOM - prenom_len;
	char* naissance = "42/42/1337";
	int naissance_len = strlen(naissance) + 1;
	int naissance_os = LNAISSANCE - naissance_len;
	char* etat_civil = "Célibataire";
	int etat_civil_len = strlen(etat_civil) + 1;
	int etat_civil_os = LETAT_CIVIL - etat_civil_len;
	char* profession = "Chevalier Jedi";
	int profession_len = strlen(profession) + 1;
	int profession_os = LPROFESSION - profession_len;
	char* organisation = "Ordre des Jedis";
	int organisation_len = strlen(organisation) + 1;
	int organisation_os = LORGANISATION - organisation_len;
	char* telephone = "0400/00.00.00";
	int telephone_len = strlen(telephone) + 1;
	int telephone_os = LTELEPHONE - telephone_len;

	struct carte_visite carte;
	struct iovec io_vector[IOVEC_FIELDS];

	sprintf(carte.nom , "%s", nom);
	sprintf(carte.prenom , "%s", prenom);
	sprintf(carte.naissance , "%s", naissance);
	sprintf(carte.etat_civil , "%s", etat_civil);
	sprintf(carte.profession , "%s", profession);
	sprintf(carte.organisation , "%s", organisation);
	sprintf(carte.telephone , "%s", telephone);

	io_vector[0].iov_base = (void*) &carte.nom;
	io_vector[0].iov_len = LNOM;
	io_vector[1].iov_base = (void*) &carte.prenom;
	io_vector[1].iov_len = LPRENOM;
	io_vector[2].iov_base = (void*) &carte.naissance;
	io_vector[2].iov_len = LNAISSANCE;
	io_vector[3].iov_base = (void*) &carte.etat_civil;
	io_vector[3].iov_len = LETAT_CIVIL;
	io_vector[4].iov_base = (void*) &carte.profession;
	io_vector[4].iov_len = LPROFESSION;
	io_vector[5].iov_base = (void*) &carte.organisation;
	io_vector[5].iov_len = LORGANISATION;
	io_vector[6].iov_base = (void*) &carte.telephone;
	io_vector[6].iov_len = LTELEPHONE;
	// end of dangerous section

	int fd = open("tmp.dat", O_WRONLY|O_CREAT|O_TRUNC, S_IRUSR|S_IWUSR);
	if (fd == -1)
	{
		perror("in open()");
		exit(EXIT_FAILURE);
	}


	for (i = 1 ; i <= 256 ; i++)
	{
		// debut benchmark writev
		start_timer(t);
		for(j=0;j<i;j++)
		{
			// instructions writev
			err = writev(fd, io_vector, IOVEC_FIELDS);
		}
		write_record(writev_rec, j, stop_timer(t));

		if ( err == -1 )
		{
			perror("in writev()");
			exit(EXIT_FAILURE);
		}

		// debut benchmark write+lseek

		// Remark : the manpage for writev advise not to use it with the stdio functions. This code ignores the warning
		start_timer(t);
		for(j=0;j<i;j++)
		{
			// instructions write+lseek
			err = write(fd, nom, nom_len);
			err1 = lseek(fd, nom_os, SEEK_CUR);
			err = write(fd, prenom, prenom_len);
			err1 = lseek(fd, prenom_os, SEEK_CUR);
			err = write(fd, naissance, naissance_len);
			err1 = lseek(fd, naissance_os, SEEK_CUR);
			err = write(fd, etat_civil, etat_civil_len);
			err1 = lseek(fd, etat_civil_os, SEEK_CUR);
			err = write(fd, profession, profession_len);
			err1 = lseek(fd, profession_os, SEEK_CUR);
			err = write(fd, organisation, organisation_len);
			err1 = lseek(fd, organisation_os, SEEK_CUR);
			err = write(fd, telephone, telephone_len);
			err1 = lseek(fd, telephone_os, SEEK_CUR);
		}
		write_record(lseek_rec, j, stop_timer(t));

		if ( err == -1 )
		{
			perror("in write()");
			exit(EXIT_FAILURE);
		}

		if ( err1 == -1 )
		{
			perror("in lseek()");
			exit(EXIT_FAILURE);
		}
	}


	// end of benchmarks
	// cleaning

	err = close(fd);
	if ( err == -1 )
	{
		perror("in close()");
		exit(EXIT_FAILURE);
	}

	recorder_free(writev_rec);
	recorder_free(lseek_rec);
	timer_free(t);
	
	return EXIT_SUCCESS;
}
	
	

