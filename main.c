#include <stdio.h>
#include "imenik.h"

int main(void) {
    int opcija;
    ucitajIzDatoteke();//19
    //10
    do {
        printf("\n--- Telefonski imenik ---\n");
        printf("1. Unos kontakta\n");
        printf("2. Pregled kontakata\n");
        printf("3. Azuriraj kontakt\n");
        printf("4. Obrisi kontakt\n");
        printf("5. Sortiraj kontakte\n");
        printf("6. Pretrazi kontakt\n");
        printf("7. Arhiviraj imenik\n");
        printf("0. Izlaz\n");
        printf("Odabir: ");

        if (scanf("%d", &opcija) != 1) {
            while (getchar() != '\n');
            printf("Molimo unesite broj!\n");
            opcija = -1;
            continue;
        }
        while (getchar() != '\n');
        switch (opcija) {
        case UNOS: unosKontakta(); break;
        case PREGLED: pregledKontakata(); break;
        case AZURIRAJ: azurirajKontakt(); break;
        case OBRISI: obrisiKontakt(); break;
        case SORTIRAJ: sortirajKontakte(); break;
        case PRETRAZI: pretraziKontakt(); break;
        case ARHIVIRAJ: arhivirajDatoteku(); break;
        case IZLAZ: ocistiMemoriju(); break;
        default:
            printf("Nepoznata opcija!\n");
            pauziraj();
        }
    } while (opcija != IZLAZ);

    return 0;
}
