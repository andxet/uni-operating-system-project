//
//  miacoda_cli.c
//  helpdesk
//
//  Created by Andrea Peretti on 07/04/12.
//  Copyright (c) 2012 Università degli studi di Torino. All rights reserved.
//

#include "miacoda_cli.h"
#include "coda.h"

#include <unistd.h>
#include <stdio.h>

//Funzioni ottimizzate per il client
int c_coda_ini(){
	if(coda_esiste() == -1)
		return -2; //La coda non esiste!
	coda = coda_aggancia();
	if(coda == -1)
		return -1; //Errore nel collegarsi alla coda
	return 0;
}

int c_coda_telefona(){//Invia al server 
	coda_messaggio mess = coda_messaggio_componi(M_SERVER, getpid(), RICH_OP);
	return coda_spedisci(coda, mess);
}

int c_coda_invia_rich(int da_spedire){
	if(op == 0)
		return -1;//L'operatore a cui inviare la richiesta non è stato definito.
	coda_messaggio mess = coda_messaggio_componi(op, getpid(), da_spedire);
	return coda_spedisci(coda, mess);
}

int c_coda_attendi_op(){
	coda_messaggio mess;
	int err = coda_ricevi(coda, getpid(), &mess);
	if(mess.dato == HD_CLOSED)
		return -2;//L'helpdesk è chiuso
	if(mess.dato != OP_CIAO)
		return -1;//Messaggio inatteso
	op = mess.sender;
	return err;
}

int c_coda_ricevi(int * ricevuto){
	coda_messaggio mess;
	int err = coda_ricevi(coda, getpid(), &mess);
	*ricevuto = mess.dato;
	return err;
}