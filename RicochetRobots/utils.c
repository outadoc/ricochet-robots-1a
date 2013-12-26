//
//  utils.c
//  RicochetRobots
//
//  Created by Baptiste on 29/11/13.
//  Copyright (c) 2013 outadev. All rights reserved.
//

#include "utils.h"

//
// Génère un entier pseudo aléatoire compris entre min (compris) et max (non compris).
//
int rand_between(int min, int max) {
    return rand() % (max - min) + min;
}

//
// Retourne la longueur du string le plus grand parmi un tableau de strings
//
int max_strlen(char **str, int n) {
    if(str == NULL || n <= 0) return 0;
    
    int i;
    int maxlen = 0;
    
    for(i = 0; i < n; i++) {
        int currlen = (int) strlen(str[i]);
        if(currlen > maxlen) maxlen = currlen;
    }
    
    return maxlen;
}