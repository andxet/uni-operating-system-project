/*###############################################################################
##						-= Progetto di Sistemi Operativi =-					 	          ##
## --------------------------------------------------------------------------- ##
##  File:	       	operazioni_client.c                                       ##
##  Created by:	   Fiori, Peretti, Polto                                     ##
## --------------------------------------------------------------------------- ##
##  Descrizione:    Descrizione del file                                       ##
##                                                                             ##
###############################################################################*/

#include <stdio.h>
#include "costanti.h"
#include "stato_helpdesk.h"
#include "semafori.h"
#include "util.h"
#include "miacoda_cli.h"

stato_helpdesk stato_hd;
int key, coda;
/*
	Valori di ritorno :
		-Se la coda è chiusa : -1
		-Se HelpDesk chiuso  : -2
*/
int avviaClient()
{
	/* Il client deve verificare lo stato dell' HD per sapere se inviare o no la richiesta
	 * Se aperto fare un random e inviare la richiesta
	*/
	if(verificaHD())
	{//HelpDesk aperto
		key = KEY_START + gen_rand(MAX_N_OP);	//Decido in modo random a che operatore collegarmi
		//Vado al suo semaforo
		semaforo semOpScelto = collega_semaforo(key);
		s_wait(key);
		//Ho ottenuto l'accesso, mi collego alla coda e deposito il messaggio
		int esitoAggancio = c_coda_aggancia();
		if (esitoAggancio < 0)
		{
			s_signal(key);
			exit(-1);
		}
		
		
		int richiesta = gen_rand(N_MAX_RICH);
		c_coda_invia_rich(richiesta);
		int risposta = c_coda_attendi_op(); //Attendo che l'operatore svolga la mia richiesta
		if(risposta != 0)
			log("L'OPERATORE MI HA DATO UNA RISPOSTA NON VALIDA...");
		else
			log("Ho ricevuto la risposta che desideravo.");
		//Lascio posto per altri
		//s_signal(key); -> Lo fa già l'operatore
		
		log("Termino la chiamata!");
		
		exit(0);
	}
	else
	{//HelpDesk chiuso
		stampaLog("HelpDesk chiuso..");
		exit(0);
	}
	
}


int verificaHD()//Verifica se HD è aperto o no
{
	//Mi collego alla memoria condivisa e controllo il valore, se è aperto o no HD
	semaforo semHelpdesk = collega_semaforo(SEM_HD);
	s_wait(SEM_HD);
	if(stato_hd->aperto <= 1)
		return 0;//CHIUSO
	else
		return 1;//APERTO
	s_signal(SEM_HD);
	
}
