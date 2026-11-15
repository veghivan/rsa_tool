#include <stdlib.h>
#include <stdio.h>
#include "rsa.h"
#include "debugmalloc.h"

/**
 * @brief Legeneral egy veletlenszeru, adott bithosszusagu primszamot.
 * 
 * @param prime_out A generalt primszam kimeneti parametere.
 * @param bits A kivan bithossz (az RSA kulcsmeret fele).
 */
static void generate_large_prime(struct bn *prime_out, int bits);

/**
 * @brief Kiszamitja a modularis inverzet (res = a^-1 mod m).
 * 
 * A Kiterjesztett Euklideszi algoritmust hasznalja. Mivel a bignum konyvtar
 * csak elojel nelkuli (unsigned) szamokat kezel, az algoritmus tartalmaz
 * egy alulcsordulas elleni vedelmet a kivonasoknal.
 * 
 * @param res Eredmeny (az inverz).
 * @param a Az invertalando ertek.
 * @param m A modulus.
 */
static void bignum_mod_inverse(struct bn *res, struct bn *a, struct bn *m);

/**
 * @brief Segedfuggveny: Modulo szamitas bignumokkal (res = a % m)
 */
static void bignum_modulo(struct bn *a, struct bn *m, struct bn *res);

/**
 * @brief Segedfuggveny: Modularis gyorshatvanyozas (res = base^exp % mod)
 */
static void bignum_pow_mod(struct bn *base, struct bn *exp, struct bn *mod, struct bn *res);

/**
 * @brief Miller-Rabin primteszt algoritmusa.
 * @param n A tesztelendo szam.
 * @param k A teszt iteracioinak szama (megbizhatosag).
 * @return true ha valoszinuleg prim, false ha biztosan osszetett.
 */
static bool miller_rabin(struct bn *n, int k);

bool generate_rsa_keypair(int bit_length, rsa_keypair **out_keypair) {
    *out_keypair = (rsa_keypair*)malloc(sizeof(rsa_keypair));
    if (*out_keypair == NULL) return false;

    (*out_keypair)->bit_length = bit_length;
    bignum_init(&(*out_keypair)->n);
    bignum_init(&(*out_keypair)->e);
    bignum_init(&(*out_keypair)->d);

    struct bn p, q, p_minus_1, q_minus_1, phi_n, egy;
    bignum_init(&p); bignum_init(&q);
    bignum_init(&p_minus_1); bignum_init(&q_minus_1);
    bignum_init(&phi_n); bignum_init(&egy);

    bignum_from_int(&egy, 1);

    printf("-> [Matek] %d bites primek keresese...\n", bit_length / 2);
    generate_large_prime(&p, bit_length / 2);
    generate_large_prime(&q, bit_length / 2);

    // Modulus kiszamitasa: n = p * q 
    bignum_mul(&p, &q, &(*out_keypair)->n);

    //Euler-fele phi fuggveny: phi(n) = (p - 1) * (q - 1) 
    bignum_sub(&p, &egy, &p_minus_1);
    bignum_sub(&q, &egy, &q_minus_1);
    bignum_mul(&p_minus_1, &q_minus_1, &phi_n);

    // Publikus kitevo (e) fixalasa a 65537 (0x10001) szabvanyos ertekre 
    bignum_from_int(&(*out_keypair)->e, 65537);

    // Privat kitevo (d) szamitasa: e modularis inverze modulo phi(n). (d * e = 1 mod phi(n)) 
    printf("-> [Matek] Privat kitevo (d) kiszamitasa...\n");
    bignum_mod_inverse(&(*out_keypair)->d, &(*out_keypair)->e, &phi_n);

    return true;
}

void free_rsa_keypair(rsa_keypair *keypair) {
    if (keypair != NULL) {
        free(keypair);
    }
}

static void generate_large_prime(struct bn *prime_out, int bits) {
    struct bn egy, kettes;
    bignum_from_int(&egy, 1);
    bignum_from_int(&kettes, 2);
    bool is_prime = false;
    
    while (!is_prime) {
        bignum_init(prime_out);
        
        // Bitenkenti random generalas (tokeltes platform- es memoriastruktura-fuggetlenseg) 
        for (int i = 0; i < bits; i++) {
            bignum_mul(prime_out, &kettes, prime_out); // Elotolas (bit shift)
            
            /* Az MSB (elso bit) mindig 1, hogy ne legyen rovidebb a szam.
               Az LSB (utolso bit) mindig 1, hogy ne legyen paros. */
            if (i == 0 || i == bits - 1 || (rand() % 2 != 0)) {
                bignum_add(prime_out, &egy, prime_out);
            }
        }
        
        /* 10 koros Miller-Rabin elegendo a biztos primhez. 
           Ha elhasal, a while ciklus ujrakezdi. */
        is_prime = miller_rabin(prime_out, 10);
    }
}

static void bignum_mod_inverse(struct bn *res, struct bn *a, struct bn *m) {
    struct bn t, newt, r, newr, q, tmp, tmp2;
    struct bn zero, egy;

    bignum_init(&t); 
    bignum_from_int(&newt, 1);
    
    bignum_assign(&r, m); 
    bignum_assign(&newr, a);
    
    bignum_init(&zero);
    bignum_from_int(&egy, 1);

    while (bignum_cmp(&newr, &zero) != 0) {
        // Hanyados szamitasa: q = r / newr 
        bignum_div(&r, &newr, &q);
        
        // Maradek frissitese: r_next = r - q * newr 
        bignum_mul(&q, &newr, &tmp);
        bignum_sub(&r, &tmp, &tmp);
        bignum_assign(&r, &newr);
        bignum_assign(&newr, &tmp);
        
        // Ideiglenes t ertek: tmp = q * newt 
        bignum_mul(&q, &newt, &tmp);
        
        // Biztonsagos modulo szamitas bignumokkal: tmp = tmp - (tmp / m) * m 
        struct bn div_res;
        bignum_div(&tmp, m, &div_res);
        bignum_mul(&div_res, m, &tmp2);
        bignum_sub(&tmp, &tmp2, &tmp);
        
        /* t_next = t - tmp mod m. Unsigned vedelem alulcsordulas ellen: 
           ha a kivonando (tmp) nagyobb, mint a kisebbitendo (t), hozzaadjuk a modulust. */
        if (bignum_cmp(&t, &tmp) < 0) {
            bignum_add(&t, m, &t); 
        }
        
        bignum_sub(&t, &tmp, &tmp);
        bignum_assign(&t, &newt);
        bignum_assign(&newt, &tmp);
    }
    
    // Ha az r > 1, akkor 'a' es 'm' nem relativ primek (nincs inverz) 
    if (bignum_cmp(&r, &egy) > 0) {
        bignum_init(res); 
        return;
    }
    
    bignum_assign(res, &t);
}

static void bignum_modulo(struct bn *a, struct bn *m, struct bn *res) {
    struct bn q, tmp;
    bignum_div(a, m, &q);
    bignum_mul(&q, m, &tmp);
    bignum_sub(a, &tmp, res);
}

static void bignum_pow_mod(struct bn *base, struct bn *exp, struct bn *mod, struct bn *res) {
    struct bn base_copy, exp_copy, tmp, zero, kettes;
    bignum_assign(&base_copy, base);
    bignum_assign(&exp_copy, exp);
    
    bignum_from_int(res, 1);
    bignum_init(&zero);
    bignum_from_int(&kettes, 2);

    // Gyorshatvanyozas algoritmusa (Square-and-Multiply) 
    while (bignum_cmp(&exp_copy, &zero) > 0) {
        struct bn exp_mod_2;
        bignum_modulo(&exp_copy, &kettes, &exp_mod_2);
        
        // Ha az aktualis kitevo bit 1 (paratlan) 
        if (bignum_cmp(&exp_mod_2, &zero) != 0) {
            bignum_mul(res, &base_copy, &tmp);
            bignum_modulo(&tmp, mod, res);
        }
        
        // Kitevo felezese (jobbra shift) es alap negyzetre emelese 
        bignum_div(&exp_copy, &kettes, &exp_copy);
        
        bignum_mul(&base_copy, &base_copy, &tmp);
        bignum_modulo(&tmp, mod, &base_copy);
    }
}

static bool miller_rabin(struct bn *n, int k) {
    struct bn zero, egy, kettes, n_minus_1, d, x, a;
    bignum_init(&zero);
    bignum_from_int(&egy, 1);
    bignum_from_int(&kettes, 2);
    
    bignum_sub(n, &egy, &n_minus_1);
    
    // Biztonsagi ellenorzesek kis szamokra es parosokra 
    if (bignum_cmp(n, &kettes) < 0) return false;
    if (bignum_cmp(n, &kettes) == 0) return true;
    
    struct bn mod2;
    bignum_modulo(n, &kettes, &mod2);
    if (bignum_cmp(&mod2, &zero) == 0) return false; // Paros szam sosem prim

    // n - 1 felirasa 2^s * d formaban 
    bignum_assign(&d, &n_minus_1);
    int s = 0;
    bignum_modulo(&d, &kettes, &mod2);
    
    while (bignum_cmp(&mod2, &zero) == 0) {
        bignum_div(&d, &kettes, &d);
        s++;
        bignum_modulo(&d, &kettes, &mod2);
    }

    /* Miller-Rabin teszt 'k' korben. 
       Kis primeket hasznalunk alapnak (a), ez kriptografiai szempontbol elegendo. */
    int prime_bases[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29};
    int limit = (k > 10) ? 10 : k;
    
    for (int i = 0; i < limit; i++) {
        bignum_from_int(&a, prime_bases[i]);

        // x = a^d % n 
        bignum_pow_mod(&a, &d, n, &x);

        if (bignum_cmp(&x, &egy) == 0 || bignum_cmp(&x, &n_minus_1) == 0) {
            continue;
        }

        bool pass = false;
        for (int r = 1; r < s; r++) {
            // x = x^2 % n 
            bignum_pow_mod(&x, &kettes, n, &x);
            
            if (bignum_cmp(&x, &n_minus_1) == 0) {
                pass = true;
                break;
            }
        }
        
        if (!pass) return false; // Ha egyik feltetel sem teljesult, biztosan osszetett!
    }
    
    return true; // Valoszinuleg prim
}