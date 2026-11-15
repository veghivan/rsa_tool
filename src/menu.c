#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "menu.h"
#include "debugmalloc.h"
#include "rsa.h"

void run_interactive_menu(void) {
    bool fut = true;
    int valasztas;

    while (fut) {
        printf("\n=== RSA Titkosito Rendszer ===\n");
        printf("1. Uj kulcspar generalasa\n");
        printf("2. Fajl titkositasa\n");
        printf("3. Fajl visszafejtese\n");
        printf("4. Kilepes\n");
        printf("Valassz egy funkciot: ");

        if (scanf("%d", &valasztas) != 1) {
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
            printf("[Hiba] Ervenytelen bemenet, szamot kerek!\n");
            continue;
        }

        switch (valasztas) {
            case 1: { 
                printf("\n-> [Keszul] Kulcsgeneralas...\n");
                
                int bit_length = 0;
                printf("Add meg a bitmeretet (vagy 0 az alapertelmezett 1024-hez): ");
                
                if (scanf("%d", &bit_length) != 1) {
                    int c; while ((c = getchar()) != '\n' && c != EOF);
                    printf("[Hiba] Szamot kerek! Visszateres a fomenube...\n");
                    break; 
                }
                
                if (bit_length <= 0) {
                    bit_length = 1024; // Alapértelmezett érték
                }

                rsa_keypair *keys = NULL;
                
                // Itt jön a dupla indirekció élesben: átadjuk a pointer memóriacímét (&keys)
                if (generate_rsa_keypair(bit_length, &keys)) {
                    printf("[Siker] %d bites RSA kulcspar lefoglalva a memoriaban!\n", keys->bit_length);
                    
                    // TODO: Fájlnevek bekérése és kimentés a file_io modullal
                    
                    // Használat után azonnal felszabadítjuk
                    free_rsa_keypair(keys);
                    printf("[Kesz] Memoria biztonsagosan felszabaditva.\n");
                } else {
                    printf("[Hiba] Nem sikerult memoriat foglalni a kulcsparnak!\n");
                }
                break;
            }
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