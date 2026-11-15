#include <stdio.h>
#include <stdlib.h>

#include "debugmalloc.h"
#include "menu.h"

int main(int argc, char *argv[]) {
    if (argc == 1) {
        run_interactive_menu();
    } else {
        printf("CLI mod elinditva %d argumentummal. (Fejlesztes alatt)\n", argc - 1);
        // TODO: process_cli_arguments(argc, argv);
    }

    return EXIT_SUCCESS;
}