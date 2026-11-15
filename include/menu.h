/**
 * @file menu.h
 * @brief Interaktív terminálos (TTY) menürendszer az RSA titkosítóhoz.
 */
#ifndef MENU_H
#define MENU_H

/**
 * @brief Elindítja a program interaktív, szöveges főmenüjét.
 * 
 * Ez a függvény egy végtelen ciklusban fut, megjeleníti a specifikációban kért 
 * menüpontokat, és biztonságosan bekéri a felhasználó választását a szabványos 
 * bemenetről (stdin). Lekezeli az érvénytelen karakteres bemeneteket a puffer 
 * ürítésével. A vezérlés addig marad ebben a függvényben, amíg a felhasználó 
 * a kilépés (4) opciót nem választja.
 */
void run_interactive_menu(void);

#endif // MENU_H