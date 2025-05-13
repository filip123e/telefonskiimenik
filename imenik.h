#ifndef IMENIK_H
#define IMENIK_H

#define MAX_IME 50
#define MAX_BROJ 20

typedef struct {
    int id;
    char ime[MAX_IME];
    char broj[MAX_BROJ];
} Kontakt;

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

#endif
