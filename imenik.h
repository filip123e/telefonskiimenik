#ifndef IMENIK_H
#define IMENIK_H

#include <stddef.h>
#include <stdio.h>

#define MAX_IME     50
#define MAX_PREZIME 50
#define MAX_BROJ    20
#define MAX_EMAIL   50
#define DATOTEKA    "imenik.bin"   

typedef struct {
    int  id;
    char ime[MAX_IME];
    char prezime[MAX_PREZIME];
    char broj[MAX_BROJ];
    char email[MAX_EMAIL];
} Kontakt;

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


extern Kontakt *kontakti;
extern size_t   brojKontakata;


void ucitajIzDatoteke(void);
void spremiUDatoteku(void);

void unosKontakta(void);
void pregledKontakata(void);
void azurirajKontakt(void);
void obrisiKontakt(void);
void sortirajKontakte(void);
void pretraziKontakt(void);
void arhivirajDatoteku(void);
void ocistiMemoriju(void);

int  usporedbaKontakta(const void*, const void*);
void pauziraj(void);

#endif 
