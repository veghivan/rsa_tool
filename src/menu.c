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
            case 1: {
                printf("\n-> [Keszul] Kulcsgeneralas...\n");
                
                int bit_length = 1024; // Az alapértelmezett érték előre beállítva
                char buffer[32];

                printf("Add meg a bitmeretet [Alapertelmezett: 1024]: ");
                
                // A főmenü scanf("%d") hívása bent hagyta az Entert (\n) a bemeneti pufferben.
                // Ezt ki kell ürítenünk, mielőtt a sorolvasó fgets-t meghívjuk.
                int c; while ((c = getchar()) != '\n' && c != EOF);

                if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
                    // Ha az első karakter nem Enter (\n), akkor a felhasználó írt be valamit
                    if (buffer[0] != '\n') {
                        if (sscanf(buffer, "%d", &bit_length) != 1 || bit_length <= 0) {
                            printf("[Hiba] Ervenytelen szam! Visszateres a fomenube...\n");
                            break;
                        }
                    }
                }

                rsa_keypair *keys = NULL;
                
                if (generate_rsa_keypair(bit_length, &keys)) {
                    printf("[Siker] %d bites RSA kulcspar lefoglalva a memoriaban!\n", keys->bit_length);
                    
                    // TODO: Fájlnevek bekérése és kimentés a file_io modullal
                    
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