#ifndef IMENIK_H
#define IMENIK_H

#include <stddef.h>
#include <stdio.h>

//3,4,5,15
#define MAX_IME 50
#define MAX_PREZIME 50
#define MAX_BROJ 20
#define MAX_EMAIL 50
#define DATOTEKA "imenik.txt"

typedef struct {
    int id; //2
    char ime[MAX_IME];  //15
    char prezime[MAX_PREZIME]; //15
    char broj[MAX_BROJ]; //15
    char email[MAX_EMAIL]; //15
} Kontakt;

//4
typedef enum {
    IZLAZ = 0,
    UNOS,
    PREGLED,
    AZURIRAJ,
    OBRISI,
    SORTIRAJ,
    PRETRAZI,
    ARHIVIRAJ
} IzbornikOpcije;
//8
extern Kontakt* kontakti;
extern size_t brojKontakata;
//13
void ucitajIzDatoteke(void);
void spremiUDatoteku(void);
void unosKontakta(void);//1
void pregledKontakata(void);//1
void azurirajKontakt(void);//1
void obrisiKontakt(void);//1
void sortirajKontakte(void);//23
void pretraziKontakt(void);//24
void arhivirajDatoteku(void);//21
void ocistiMemoriju(void);//18

int usporedbaKontakta(const void*, const void*);
//9
void pauziraj(void); 


#endif
