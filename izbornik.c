#include <stdio.h>
#include "imenik.h"
#include "izbornik.h"

void prikaziIzbornik(void) {
    int izbor;
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
        scanf("%d", &izbor);

        switch (izbor) {
            case 1: unosKontakta(); break;
            case 2: pregledKontakata(); break;
            case 3: azurirajKontakt(); break;
            case 4: obrisiKontakt(); break;
            case 5: sortirajKontakte(); break;
            case 6: pretraziKontakt(); break;
            case 7: arhivirajDatoteku(); break;
            case 0: break;
            default: printf("Nepoznata opcija!\n");
        }
    } while (izbor != 0);
}
