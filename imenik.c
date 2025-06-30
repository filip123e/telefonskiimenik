#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include "imenik.h"


Kontakt *kontakti = NULL;
size_t   brojKontakata = 0;


static int generirajId(void) {
    int maxId = 0;
    for (size_t i = 0; i < brojKontakata; ++i)
        if (kontakti[i].id > maxId) maxId = kontakti[i].id;
    return maxId + 1;
}


void ucitajIzDatoteke(void) {
    FILE *f = fopen(DATOTEKA, "rb");
    if (!f) {
        if (errno != ENOENT) perror("Greska pri otvaranju datoteke");
        return;                       
    }

    fseek(f, 0L, SEEK_END);
    long velicina = ftell(f);         
    if (velicina <= 0) { fclose(f); return; }
    rewind(f);                        

    size_t zapisa = (size_t)velicina / sizeof(Kontakt);
    kontakti = malloc(zapisa * sizeof(Kontakt));
    if (!kontakti) { perror("Greska malloc"); fclose(f); return; }

    if (fread(kontakti, sizeof(Kontakt), zapisa, f) != zapisa) {
        perror("Greska pri čitanju");
        free(kontakti); kontakti = NULL; brojKontakata = 0;
    } else {
        brojKontakata = zapisa;
    }
    fclose(f);
}

void spremiUDatoteku(void) {
    FILE *f = fopen(DATOTEKA, "wb");       
    if (!f) { perror("Greska pri pisanju"); return; }

    if (fwrite(kontakti, sizeof(Kontakt), brojKontakata, f) != brojKontakata)
        perror("Greska pri upisu");
    fclose(f);
}


void unosKontakta(void) {
    Kontakt novi;
    novi.id = generirajId();

    printf("Unesite ime: ");     scanf(" %49[^\n]",  novi.ime);
    printf("Unesite prezime: "); scanf(" %49[^\n]",  novi.prezime);
    printf("Unesite broj: ");    scanf(" %19[^\n]",  novi.broj);
    printf("Unesite email: ");   scanf(" %49[^\n]",  novi.email);

    Kontakt *tmp = realloc(kontakti, (brojKontakata + 1) * sizeof(Kontakt));
    if (!tmp) { perror("Greska realloc"); return; }
    kontakti = tmp;
    kontakti[brojKontakata++] = novi;

    spremiUDatoteku();
}

void pregledKontakata(void) {
    printf("\n--- Svi kontakti ---\n");
    for (size_t i = 0; i < brojKontakata; ++i)
        printf("ID: %d | Ime: %s | Prezime: %s | Broj: %s | Email: %s\n",
               kontakti[i].id, kontakti[i].ime, kontakti[i].prezime,
               kontakti[i].broj, kontakti[i].email);
    pauziraj();
}

void azurirajKontakt(void) {
    int id;
    printf("Unesite ID kontakta za ažuriranje: ");
    scanf("%d", &id);

    for (size_t i = 0; i < brojKontakata; ++i) {
        if (kontakti[i].id == id) {
            printf("Novi broj: ");  scanf(" %19[^\n]", kontakti[i].broj);
            printf("Novi email: "); scanf(" %49[^\n]", kontakti[i].email);

            FILE *f = fopen(DATOTEKA, "rb+");
            if (!f) { perror("Greska otvaranja datoteke"); pauziraj(); return; }

            long offset = (long)i * sizeof(Kontakt);
            if (fseek(f, offset, SEEK_SET) == 0)
                fwrite(&kontakti[i], sizeof(Kontakt), 1, f);
            else
                perror("fseek");

            fclose(f);

            printf("Kontakt ažuriran.\n");
            pauziraj();
            return;
        }
    }
    printf("Kontakt nije pronađen.\n");
    pauziraj();
}

void obrisiKontakt(void) {
    int id;
    printf("Unesite ID za brisanje: ");
    scanf("%d", &id);

    size_t pos = brojKontakata;     
    for (size_t i = 0; i < brojKontakata; ++i)
        if (kontakti[i].id == id) { pos = i; break; }

    if (pos == brojKontakata) {
        printf("Kontakt nije pronađen.\n");
        pauziraj();
        return;
    }

    for (size_t j = pos; j < brojKontakata - 1; ++j)
        kontakti[j] = kontakti[j + 1];
    --brojKontakata;

  
    const char *tempFile = "imenik.tmp";
    FILE *tmp = fopen(tempFile, "wb");
    if (!tmp) { perror("tmp fopen"); pauziraj(); return; }

    fwrite(kontakti, sizeof(Kontakt), brojKontakata, tmp);
    fclose(tmp);

   
    remove(DATOTEKA);                 
    if (rename(tempFile, DATOTEKA) != 0)
        perror("rename(tmp → imenik.bin)");

    printf("Kontakt obrisan.\n");
    pauziraj();
}


int usporedbaKontakta(const void *a, const void *b) {
    return strcmp(((const Kontakt*)a)->ime, ((const Kontakt*)b)->ime);
}

void sortirajKontakte(void) {
    if (brojKontakata > 0) {
        qsort(kontakti, brojKontakata, sizeof(Kontakt), usporedbaKontakta);
       
        spremiUDatoteku();
        printf("Kontakti sortirani.\n");
    }
    pauziraj();
}

void pretraziKontakt(void) {
    if (brojKontakata == 0) {
        printf("Nema kontakata.\n");
        pauziraj(); return;
    }

    char trazenoIme[MAX_IME] = "";
    char trazenoPrezime[MAX_PREZIME] = "";
    char trazeniEmail[MAX_EMAIL] = "";

    printf("Ime (Enter za preskoči): ");     fgets(trazenoIme, sizeof(trazenoIme), stdin);
    printf("Prezime (Enter za preskoči): ");  fgets(trazenoPrezime, sizeof(trazenoPrezime), stdin);
    printf("Email (Enter za preskoči): ");    fgets(trazeniEmail, sizeof(trazeniEmail), stdin);

    trazenoIme[strcspn(trazenoIme, "\n")]         = 0;
    trazenoPrezime[strcspn(trazenoPrezime, "\n")] = 0;
    trazeniEmail[strcspn(trazeniEmail, "\n")]     = 0;

    int found = 0;
    for (size_t i = 0; i < brojKontakata; ++i) {
        int ok = 1;
        if (*trazenoIme     && strcmp(kontakti[i].ime,     trazenoIme)     != 0) ok = 0;
        if (*trazenoPrezime && strcmp(kontakti[i].prezime, trazenoPrezime) != 0) ok = 0;
        if (*trazeniEmail   && strcmp(kontakti[i].email,   trazeniEmail)   != 0) ok = 0;

        if (ok) {
            printf("ID: %d | %s %s | %s | %s\n",
                   kontakti[i].id, kontakti[i].ime, kontakti[i].prezime,
                   kontakti[i].broj, kontakti[i].email);
            found = 1;
        }
    }
    if (!found) printf("Nema rezultata.\n");
    pauziraj();
}


void arhivirajDatoteku(void)
{
    const char *backup = "imenik.bak";

 
    spremiUDatoteku();

    
    if (remove(backup) == 0)
        printf("Stari backup '%s' je obrisan.\n", backup);
    else if (errno != ENOENT)        
        perror("remove(backup)");

   
    if (rename(DATOTEKA, backup) == 0)
        printf("Datoteka arhivirana kao '%s'\n", backup);
    else {
        perror("rename → backup");
        pauziraj();
        return;
    }

   
    spremiUDatoteku();

    pauziraj();
}


void ocistiMemoriju(void) {
    free(kontakti);
    kontakti = NULL;
    brojKontakata = 0;
}

inline void pauziraj(void) {
    printf("Pritisnite Enter za nastavak...");
    while (getchar() != '\n');
    getchar();
}
