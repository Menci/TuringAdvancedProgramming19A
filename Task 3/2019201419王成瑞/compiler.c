#include <assert.h>
#include <stdio.h>

#include "compiler.h"

void operatorMove(int srcID, int tarID) {
    assert(srcID >= 0 && tarID >= 0);
    if(tarID < REGISTER_NUM) {
        printf("reg_%d = ", tarID);
    } else {
        printf("M[%d] = ", tarID - REGISTER_NUM);
    }
    if(srcID < REGISTER_NUM) {
        printf("reg_%d;\n", srcID);
    } else {
        printf("M[%d];\n", srcID - REGISTER_NUM);
    }
}

void operatorMovei(long val, int tarID) {
    assert(tarID >= 0);
    if(tarID < REGISTER_NUM) {
        printf("reg_%d = %ld;\n", tarID, val);
    } else {
        printf("M[%d] = %ld;\n", tarID - REGISTER_NUM, val);
    }
}

void operatorAdd(int srcID, int tarID) {
    assert(srcID >= 0 && tarID >= 0);
    if(tarID < REGISTER_NUM) {
        printf("reg_%d += ", tarID);
    } else {
        printf("M[%d] += ", tarID - REGISTER_NUM);
    }
    if(srcID < REGISTER_NUM) {
        printf("reg_%d;\n", srcID);
    } else {
        printf("M[%d];\n", srcID - REGISTER_NUM);
    }
}

void operatorAddi(long val, int tarID) {
    assert(tarID >= 0);
    if(tarID < REGISTER_NUM) {
        printf("reg_%d += %ld;\n", tarID, val);
    } else {
        printf("M[%d] += %ld;\n", tarID - REGISTER_NUM, val);
    }
}

void operatorMin(int srcID, int tarID) {
    assert(srcID >= 0 && tarID >= 0);
    if(tarID < REGISTER_NUM) {
        printf("reg_%d -= ", tarID);
    } else {
        printf("M[%d] -= ", tarID - REGISTER_NUM);
    }
    if(srcID < REGISTER_NUM) {
        printf("reg_%d;\n", srcID);
    } else {
        printf("M[%d];\n", srcID - REGISTER_NUM);
    }
}

void operatorMini(long val, int tarID) {
    assert(tarID >= 0);
    if(tarID < REGISTER_NUM) {
        printf("reg_%d -= %ld;\n", tarID, val);
    } else {
        printf("M[%d] -= %ld;\n", tarID - REGISTER_NUM, val);
    }
}

void operatorMul(int srcID, int tarID) {
    assert(srcID >= 0 && tarID >= 0);
    if(tarID < REGISTER_NUM) {
        printf("reg_%d *= ", tarID);
    } else {
        printf("M[%d] *= ", tarID - REGISTER_NUM);
    }
    if(srcID < REGISTER_NUM) {
        printf("reg_%d;\n", srcID);
    } else {
        printf("M[%d];\n", srcID - REGISTER_NUM);
    }
}

void operatorMuli(long val, int tarID) {
    assert(tarID >= 0);
    if(tarID < REGISTER_NUM) {
        printf("reg_%d *= %ld;\n", tarID, val);
    } else {
        printf("M[%d] *= %ld;\n", tarID - REGISTER_NUM, val);
    }
}

void operatorDiv(int srcID, int tarID) {
    assert(srcID >= 0 && tarID >= 0);
    if(tarID < REGISTER_NUM) {
        printf("reg_%d /= ", tarID);
    } else {
        printf("M[%d] /= ", tarID - REGISTER_NUM);
    }
    if(srcID < REGISTER_NUM) {
        printf("reg_%d;\n", srcID);
    } else {
        printf("M[%d];\n", srcID - REGISTER_NUM);
    }
}

void operatorDivi(long val, int tarID) {
    assert(tarID >= 0);
    if(tarID < REGISTER_NUM) {
        printf("reg_%d /= %ld;\n", tarID, val);
    } else {
        printf("M[%d] /= %ld;\n", tarID - REGISTER_NUM, val);
    }
}