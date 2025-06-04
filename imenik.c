#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "imenik.h"

Kontakt* kontakti = NULL;
size_t brojKontakata = 0;

void ucitajIzDatoteke(void) {
    FILE* f = fopen(DATOTEKA, "r");
    if (!f) {
        perror("Greska pri otvaranju datoteke");
        return;
    }

    Kontakt k;
    while (fscanf(f, "%d;%49[^;];%49[^;];%19[^;];%49[^\n]\n", &k.id, k.ime, k.prezime, k.broj, k.email) == 5) {
        Kontakt* temp = realloc(kontakti, (brojKontakata + 1) * sizeof(Kontakt));
        if (!temp) {
            perror("Greska pri realokaciji memorije");
            fclose(f);
            return;
        }
        kontakti = temp;
        kontakti[brojKontakata++] = k;
    }
    fclose(f);
}

void spremiUDatoteku(void) {
    FILE* f = fopen(DATOTEKA, "w");
    if (!f) {
        perror("Greska pri pisanju datoteke");
        return;
    }
    for (size_t i = 0; i < brojKontakata; ++i)
        fprintf(f, "%d;%s;%s;%s;%s\n", kontakti[i].id, kontakti[i].ime, kontakti[i].prezime, kontakti[i].broj, kontakti[i].email);
    fclose(f);
}

static int generirajId(void) {
    int maxId = 0;
    for (size_t i = 0; i < brojKontakata; i++)
        if (kontakti[i].id > maxId) maxId = kontakti[i].id;
    return maxId + 1;
}

void unosKontakta(void) {
    Kontakt novi;
    novi.id = generirajId();

    printf("Unesite ime: ");
    scanf(" %49[^\n]", novi.ime);
    printf("Unesite prezime: ");
    scanf(" %49[^\n]", novi.prezime);
    printf("Unesite broj: ");
    scanf(" %19[^\n]", novi.broj);
    printf("Unesite email: ");
    scanf(" %49[^\n]", novi.email);

    Kontakt* temp = realloc(kontakti, (brojKontakata + 1) * sizeof(Kontakt));
    if (!temp) {
        perror("Greska pri realokaciji");
        return;
    }
    kontakti = temp;
    kontakti[brojKontakata++] = novi;
    spremiUDatoteku();
}

void pregledKontakata(void) {
    printf("\n--- Svi kontakti ---\n");
    for (size_t i = 0; i < brojKontakata; ++i)
        printf("ID: %d | Ime: %s | Prezime: %s | Broj: %s | Email: %s\n",
               kontakti[i].id, kontakti[i].ime, kontakti[i].prezime, kontakti[i].broj, kontakti[i].email);
    pauziraj();
}

void azurirajKontakt(void) {
    int id;
    printf("Unesite ID kontakta za azuriranje: ");
    scanf("%d", &id);

    for (size_t i = 0; i < brojKontakata; ++i) {
        if (kontakti[i].id == id) {
            printf("Novi broj: ");
            scanf(" %19[^\n]", kontakti[i].broj);
            printf("Novi email: ");
            scanf(" %49[^\n]", kontakti[i].email);
            spremiUDatoteku();
            printf("Kontakt azuriran.\n");
            pauziraj();
            return;
        }
    }

    printf("Kontakt nije pronadjen.\n");
    pauziraj();
}

void obrisiKontakt(void) {
    int id;
    printf("Unesite ID za brisanje: ");
    scanf("%d", &id);

    for (size_t i = 0; i < brojKontakata; ++i) {
        if (kontakti[i].id == id) {
            for (size_t j = i; j < brojKontakata - 1; ++j)
                kontakti[j] = kontakti[j + 1];
            brojKontakata--;
            spremiUDatoteku();
            printf("Kontakt obrisan.\n");
            pauziraj();
            return;
        }
    }

    printf("Kontakt nije pronadjen.\n");
    pauziraj();
}

int usporedbaKontakta(const void* a, const void* b) {
    return strcmp(((Kontakt*)a)->ime, ((Kontakt*)b)->ime);
}

void sortirajKontakte(void) {
    if (brojKontakata > 0) {
        qsort(kontakti, brojKontakata, sizeof(Kontakt), usporedbaKontakta);
        printf("Kontakti sortirani.\n");
    }
    pauziraj();
}

void pretraziKontakt(void) {
    if (brojKontakata == 0) {
        printf("Nema kontakata.\n");
        pauziraj();
        return;
    }

    sortirajKontakte(); 

    char trazenoIme[MAX_IME];
    char trazenoPrezime[MAX_PREZIME];
    char trazeniEmail[MAX_EMAIL];

    printf("Unesite ime za pretragu (prazno za preskoci): ");
    fgets(trazenoIme, sizeof(trazenoIme), stdin);
    trazenoIme[strcspn(trazenoIme, "\n")] = 0;

    printf("Unesite prezime za pretragu (prazno za preskoci): ");
    fgets(trazenoPrezime, sizeof(trazenoPrezime), stdin);
    trazenoPrezime[strcspn(trazenoPrezime, "\n")] = 0;

    printf("Unesite email za pretragu (prazno za preskoci): ");
    fgets(trazeniEmail, sizeof(trazeniEmail), stdin);
    trazeniEmail[strcspn(trazeniEmail, "\n")] = 0;

    int found = 0;
    for (size_t i = 0; i < brojKontakata; i++) {
        int match = 1;
        if (strlen(trazenoIme) > 0 && strcmp(kontakti[i].ime, trazenoIme) != 0)
            match = 0;
        if (strlen(trazenoPrezime) > 0 && strcmp(kontakti[i].prezime, trazenoPrezime) != 0)
            match = 0;
        if (strlen(trazeniEmail) > 0 && strcmp(kontakti[i].email, trazeniEmail) != 0)
            match = 0;

        if (match) {
            printf("ID: %d | Ime: %s | Prezime: %s | Broj: %s | Email: %s\n",
                   kontakti[i].id, kontakti[i].ime, kontakti[i].prezime, kontakti[i].broj, kontakti[i].email);
            found = 1;
        }
    }

    if (!found)
        printf("Nema kontakata koji odgovaraju pretrazi.\n");

    pauziraj();
}

void arhivirajDatoteku(void) {
    if (rename(DATOTEKA, "imenik.txt") == 0)
        printf("Datoteka arhivirana kao 'imenik.txt'\n");
    else
        perror("Greska kod preimenovanja");
    pauziraj();
}

void ocistiMemoriju(void) {
    if (kontakti != NULL) {
        free(kontakti);
        kontakti = NULL;
        brojKontakata = 0;
    }
}

inline void pauziraj(void) {
    printf("\nPritisnite Enter za nastavak...");
    getchar(); 
}
