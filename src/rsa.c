#include <stdlib.h>
#include "rsa.h"
#include "debugmalloc.h"

bool generate_rsa_keypair(int bit_length, rsa_keypair **out_keypair) {
    // 1. Dinamikus memóriafoglalás a heapen (dupla indirekció a hívónak)
    *out_keypair = (rsa_keypair*)malloc(sizeof(rsa_keypair));
    
    if (*out_keypair == NULL) {
        return false;
    }

    (*out_keypair)->bit_length = bit_length;
    
    // 2. A bignum változók inicializálása (kinullázása) a bn.h függvényével
    bignum_init(&(*out_keypair)->n);
    bignum_init(&(*out_keypair)->e);
    bignum_init(&(*out_keypair)->d);

    // TODO: Később ide jön a prímkeresés és a modulusszámítás matematikája

    return true;
}

void free_rsa_keypair(rsa_keypair *keypair) {
    // A free hívása előtt érdemes ellenőrizni a NULL pointert
    if (keypair != NULL) {
        free(keypair);
    }
}