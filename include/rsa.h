/**
 * @file rsa.h
 * @brief RSA adatszerkezetek és alapvető memóriakezelési függvények.
 */
#ifndef RSA_H
#define RSA_H

#include <stdbool.h>
#include "bn.h"

/**
 * @brief Nyilvános kulcs (.pub) memóriabeli reprezentációja.
 */
typedef struct {
    struct bn n; /**< Az RSA modulus (n) */
    struct bn e; /**< A nyilvános kitevő (e - public exponent) */
} rsa_public_key;

/**
 * @brief Privát kulcs (.priv) memóriabeli reprezentációja.
 */
typedef struct {
    struct bn n; /**< Az RSA modulus (n) */
    struct bn d; /**< A titkos kitevő (d - private exponent) */
} rsa_private_key;

/**
 * @brief Egy teljes RSA kulcspár, ami a generáláskor a memóriában egyben létezik.
 */
typedef struct {
    struct bn n;      /**< A közös modulus (n) */
    struct bn e;      /**< A nyilvános kitevő (e) */
    struct bn d;      /**< A titkos kitevő (d) */
    int bit_length;   /**< A kulcs hossza bitekben (pl. 1024) */
} rsa_keypair;

/**
 * @brief Dinamikusan lefoglal és inicializál egy új RSA kulcspárt.
 * 
 * Ez a függvény heap memóriát allokál az rsa_keypair struktúrának, és inicializálja
 * a benne található bignum változókat. A memóriacímet a dupla indirekciós pointeren
 * keresztül adja vissza. Használat után a free_rsa_keypair() hívása kötelező!
 * 
 * @param bit_length A generálandó kulcs hossza bitekben (pl. 1024).
 * @param out_keypair Dupla pointer, ahova a függvény a lefoglalt címet írja.
 * @return true Ha a memóriafoglalás és az inicializálás sikeres volt.
 * @return false Ha a memóriafoglalás sikertelen (nincs elég memória).
 */
bool generate_rsa_keypair(int bit_length, rsa_keypair **out_keypair);

/**
 * @brief Felszabadítja a dinamikusan lefoglalt RSA kulcspárt.
 * 
 * @param keypair A felszabadítandó kulcspár memóriacíme. NULL pointer esetén nem csinál semmit.
 */
void free_rsa_keypair(rsa_keypair *keypair);

#endif // RSA_H