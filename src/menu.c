#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "menu.h"
#include "debugmalloc.h"

void run_interactive_menu(void) {
    bool fut = true;
    int valasztas;

    while (fut) {
        printf("\n=== RSA Titkosito Rendszer ===\n\n");

        printf("1. Uj kulcspar generalasa\n");
        printf("2. Fajl titkositasa\n");
        printf("3. Fajl visszafejtese\n");
        printf("4. Kilepes\n\n");
        
        printf("Valassz egy funkciot: ");

        if (scanf("%d", &valasztas) != 1) {
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
            printf("[Hiba] Ervenytelen bemenet, szamot kerek!\n");
            continue;
        }

        switch (valasztas) {
            case 1:
                printf("\n-> [Keszul] Kulcsgeneralas...\n");
                // TODO: Bekérni a fájlneveket és a bitméretet
                break;
            case 2:
                printf("\n-> [Keszul] Titkositas...\n");
                break;
            case 3:
                printf("\n-> [Keszul] Visszafejtes...\n");
                break;
            case 4:
                fut = false;
                printf("\nKilepes...\n");
                break;
            default:
                printf("\n[Hiba] Ervenytelen opcio. Valassz 1 es 4 kozott!\n");
                break;
        }
    }
}