#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include "table.c"

myDouble POSITIVE_INF, NEGATIVE_INF, POSITIVE_ZERO, NEGATIVE_ZERO, NAN;

void InitProcess() {
    POSITIVE_INF = MakeDouble(0, ELEVEN_ONE, 0);
    NEGATIVE_INF = MakeDouble(1, ELEVEN_ONE, 0);
    POSITIVE_ZERO = MakeDouble(0, 0, 0);
    NEGATIVE_ZERO = MakeDouble(1, 0, 0);
    NAN = MakeDouble(1, ELEVEN_ONE, 1);
}

myDouble Negative(myDouble x) {
    x.byte[7] |= (1 << 7);
    return x;
}

myDouble Positive(myDouble x) {
    x.byte[7] &= SEVEN_ONE;
    return x;
}

int Min(int a, int b) {
    return a < b ? a : b;
}

void PrintUint128_t(__uint128_t x) {
    if (x < 0) {
        x = -x;
        putchar('-');
    }
    if (x > 9) PrintUint128_t(x / 10);
    putchar(x % 10 + '0');
}

__uint128_t Rounding(int pointPos, __uint128_t m, int haveRemainder) { // Execute round operatrion for a.
    if (m == 0 || pointPos <= FRAC_BITS) return m; // m == 0 or the number is in 52 bits.
    int roundPart = pointPos - FRAC_BITS - 1;
    __uint128_t tail = 0, half = 1, lastBit;
    half = half << roundPart;
    lastBit = half << 1;

    if (roundPart < 127)
        tail = m & ((~tail) >> (127 - roundPart));
    else
        tail = m;

    if (tail < half) return m - tail;
    if (tail > half) return m - tail + lastBit;
    if (haveRemainder) return m - tail + lastBit;
    if (lastBit & m) return m - tail + lastBit;
    return m - tail;
}

int Compare(int e1, __uint128_t m1, int e2, __uint128_t m2) { // Return |a1| > |a2|.
    if (e1 > e2) return 1;
    if (e1 < e2) return 0;
    return m1 > m2;
}

int FindHighOne(__uint128_t m) { // Find the highest bit 1 of m.
    unsigned long long a = m >> 64;
    if (a != 0)
        return 127 - __builtin_clzll(a);
    a = m & (-1ull);
    if (a != 0)
        return 63 - __builtin_clzll(a);
    return NO_ONE;
}

int FindSign(myDouble a) { // Find the sign bit of a.
    return (a.byte[7] & SIGN_BIT) >> 7;
}

int FindExp(myDouble a) { // Find the exp bits of a.
    return ((a.byte[7] & EXP_1) << 4) | ((a.byte[6] & EXP_2) >> 4);
}

__uint128_t FindFrac(myDouble a) { // Find the frac bits of a.
    __uint128_t m = a.byte[6] & FOUR_ONE;
    for (int i = 5; i >= 0; i--)
        m = (m << 8) | a.byte[i];
    return m;
}

int CompareDouble(myDouble a1, myDouble a2) { // Return a1 >= a2.
    int s1 = FindSign(a1), s2 = FindSign(a2);
    int e1 = FindExp(a1), e2 = FindExp(a2);
    __uint128_t m1 = FindFrac(a1), m2 = FindFrac(a2);

    if (!e1) e1 = 1 - BIAS;
    else {
        e1 = e1 - BIAS;
        m1 += LEADING_ONE;
    }
    if (!e2) e2 = 1 - BIAS;
    else {
        e2 = e2 - BIAS;
        m2 += LEADING_ONE;
    }

    if (s1 && !s2) return 0;                  // a1 = -, a2 = +.
    if (!s1 && s2) return 1;                  // a1 = +, a2 = -.
    if (s1) {                                 // a1 = -, a2 = -.
        if (e1 > e2) return 0;
        if (e1 < e2) return 1;
        return m1 <= m2;
    }
    else {                                    // a1 = +, a2 = +.
        if (e1 > e2) return 1;
        if (e1 < e2) return 0;
        return m1 >= m2;
    }
}

__uint128_t TurnTo52(__uint128_t m, int pointPos) { // Make the significand of the number to 52 bits.
    int len = pointPos - 52;
    if (len >= 0)
        return (m >> len) & GET_BITS;
    else
        return (m << (-len)) & GET_BITS;
}

int CheckInf(int e, __uint128_t m) { // Check if the number is equal to INF.
    return (e == ELEVEN_ONE) && (m == 0);
}

int CheckZero(int e, __uint128_t m) { // Check if the number is equal to zero.
    return (e == 0) && (m == 0);
}

int CheckNan(int e, __uint128_t m) { // Check if the number is equal to NaN.
    return (e == ELEVEN_ONE) && (m != 0);
}

myDouble Calc(int s, int e, __uint128_t m, int pointPos, int haveRemainder) { // Make the result to IEEE 754 format.
    int topOne = FindHighOne(m), min;
    if (topOne == -1)
        return s ? NEGATIVE_ZERO : POSITIVE_ZERO;

    if (pointPos < topOne) {
        e += topOne - pointPos;
        pointPos += topOne - pointPos;
    }

    if (pointPos > topOne && e > MIN_EXP) {
        min = Min(pointPos - topOne, e - MIN_EXP);
        pointPos -= min;
        e -= min;
    }

    if (e > MAX_EXP)
        return s ? NEGATIVE_INF : POSITIVE_INF;

    if (pointPos - topOne < 53 && e < MIN_EXP) {
        min = Min(53 + topOne - pointPos, MIN_EXP - e);
        pointPos += min;
        e += min;
    }

    if (pointPos - topOne == 53) {
        if (e < MIN_EXP)
            return MakeDouble(s, 0, 0);
        m = Rounding(pointPos, m, haveRemainder);
        m = TurnTo52(m, pointPos);
        return MakeDouble(s, 0, m);
    }

    if (pointPos > topOne) {
        if (e < MIN_EXP)
            return MakeDouble(s, 0, 0);
        m = Rounding(pointPos, m, haveRemainder);
        if (FindHighOne(m) == pointPos)
            return MakeDouble(s, e + BIAS, 0);
        m = TurnTo52(m, pointPos);
        return MakeDouble(s, 0, m);
    }

    m = Rounding(pointPos, m, haveRemainder);

    topOne = FindHighOne(m);

    if (topOne > pointPos) {
        e += topOne - pointPos;
        pointPos += topOne - pointPos;
    }

    if (e > MAX_EXP)
        return s ? NEGATIVE_INF : POSITIVE_INF;

    m = TurnTo52(m, pointPos);
    e = e + BIAS;
    m -= LEADING_ONE;

    return MakeDouble(s, e, m);
}

myDouble Add(myDouble a1, myDouble a2, int s3) { // Assume that always positive.
    int e1 = FindExp(a1), e2 = FindExp(a2);
    __uint128_t m1 = FindFrac(a1), m2 = FindFrac(a2);

    if (!e1)
        e1 = 1 - BIAS;
    else {
        e1 = e1 - BIAS;
        m1 += LEADING_ONE;
    }
    if (!e2)
        e2 = 1 - BIAS;
    else {
        e2 = e2 - BIAS;
        m2 += LEADING_ONE;
    }

    if (e1 < e2) {                             // Swap.
        int et;
        et = e1, e1 = e2, e2 = et;
        __uint128_t mt;
        mt = m1, m1 = m2, m2 = mt;
        myDouble at;
        at = a1, a1 = a2, a2 = at;
    }

    int de = e1 - e2, pointPos = 52;
    m1 <<= Min(REAL_FRAC_BITS + 1, de);
    pointPos += Min(REAL_FRAC_BITS + 1, de);
    int e3 = e1;
    __uint128_t m3 = m1 + m2;
    if ((m3 >> (pointPos + 1)) & 1) {
        pointPos++;
        e3++;
    }

    return Calc(s3, e3, m3, pointPos, 0);
}

myDouble Minus(myDouble a1, myDouble a2, int s3) { // Assume that always the bigger one minus the smaller one.
    int e1 = FindExp(a1), e2 = FindExp(a2);
    __uint128_t m1 = FindFrac(a1), m2 = FindFrac(a2);

    if (!e1) e1 = 1 - BIAS;
    else {
        e1 = e1 - BIAS;
        m1 += LEADING_ONE;
    }
    if (!e2) e2 = 1 - BIAS;
    else {
        e2 = e2 - BIAS;
        m2 += LEADING_ONE;
    }

    int de = e1 - e2, pointPos = 52;
    m1 <<= Min(54, de);
    pointPos += Min(54, de);
    int e3 = e1;
    __uint128_t m3 = m1 - m2;

    return Calc(s3, e3, m3, pointPos, 0);
}

myDouble Addition(myDouble a1, myDouble a2) { // a1 + a2
    int s1 = FindSign(a1), s2 = FindSign(a2);
    int e1 = FindExp(a1), e2 = FindExp(a2);
    __uint128_t m1 = FindFrac(a1), m2 = FindFrac(a2);

    int isInf1 = CheckInf(e1, m1), isInf2 = CheckInf(e2, m2);
    int isZero1 = CheckZero(e1, m1), isZero2 = CheckZero(e2, m2);

    if (CheckNan(e1, m1) || CheckNan(e2, m2)) return NAN;
    if (isInf1 && isInf2) {
        if (s1 == s2) return s1 ? NEGATIVE_INF : POSITIVE_INF;
        return NAN;
    }
    if (isInf1) return s1 ? NEGATIVE_INF : POSITIVE_INF;
    if (isInf2) return s2 ? NEGATIVE_INF : POSITIVE_INF;
    if (isZero1) return a2;
    if (isZero2) return a1;

    if (!e1) e1 = 1 - BIAS;
    else {
        e1 = e1 - BIAS;
        m1 += LEADING_ONE;
    }
    if (!e2) e2 = 1 - BIAS;
    else {
        e2 = e2 - BIAS;
        m2 += LEADING_ONE;
    }

    if (s1 == s2) {
        return Add(a1, a2, s1);
    }
    else {
        if (!Compare(e1, m1, e2, m2)) { // Make |a1| > |a2|.
            int st;
            st = s1, s1 = s2, s2 = st;
            myDouble at;
            at = a1, a1 = a2, a2 = at;
        }
        return Minus(a1, a2, s1);
    }
}

myDouble Subtraction(myDouble a1, myDouble a2) { // a1 - a2
    int s2 = FindSign(a2);
    int e2 = FindExp(a2);
    __uint128_t m2 = FindFrac(a2);

    a2 = MakeDouble(s2 ^ 1, e2, m2);
    return Addition(a1, a2);
}

myDouble Multiplication(myDouble a1, myDouble a2) { // a1 * a2
    int s1 = FindSign(a1), s2 = FindSign(a2);
    int e1 = FindExp(a1), e2 = FindExp(a2);
    __uint128_t m1 = FindFrac(a1), m2 = FindFrac(a2);

    int isInf1 = CheckInf(e1, m1), isInf2 = CheckInf(e2, m2);
    int isZero1 = CheckZero(e1, m1), isZero2 = CheckZero(e2, m2);

    if (CheckNan(e1, m1) || CheckNan(e2, m2)) return NAN;
    if (isInf1 && isInf2) {
        if (s1 == s2) return POSITIVE_INF;
        return NEGATIVE_INF;
    }
    if ((isInf1 && isZero2) || (isZero1 && isInf2)) return NAN;

    if (!e1)
        e1 = 1 - BIAS;
    else {
        e1 = e1 - BIAS;
        m1 += LEADING_ONE;
    }
    if (!e2)
        e2 = 1 - BIAS;
    else {
        e2 = e2 - BIAS;
        m2 += LEADING_ONE;
    }

    int s3 = s1 ^ s2;
    int e3 = e1 + e2;
    __uint128_t m3 = m1 * m2; // m3 = REAL_m * (2 ^ 104)
    int pointPos = 104;

    if ((m3 >> 105) & 1) { // Check if REAL_m >= 2.
        e3++;
        pointPos = 105;
    }
    else
        pointPos = 104;

    return Calc(s3, e3, m3, pointPos, 0);
}

myDouble Division(myDouble a1, myDouble a2) { // a1 / a2
    int s1 = FindSign(a1), s2 = FindSign(a2);
    int e1 = FindExp(a1), e2 = FindExp(a2);
    __uint128_t m1 = FindFrac(a1), m2 = FindFrac(a2);

    int isInf1 = CheckInf(e1, m1), isInf2 = CheckInf(e2, m2);
    int isZero1 = CheckZero(e1, m1), isZero2 = CheckZero(e2, m2);

    if (CheckNan(e1, m1) || CheckNan(e2, m2)) return NAN;
    if (isInf1 && isInf2) return NAN;                                  // INF / INF
    if (isZero1 && isZero2) return NAN;                                // 0 / 0
    if (isInf2) return (s1 ^ s2) ? NEGATIVE_ZERO : POSITIVE_ZERO;      // x / INF
    if (isZero1) return (s1 ^ s2) ? NEGATIVE_ZERO : POSITIVE_ZERO;     // 0 / x (x != 0)
    if (isZero2) return (s1 ^ s2) ? NEGATIVE_INF : POSITIVE_INF;       // x / 0 (x != 0)
    if (isInf1) return (s1 ^ s2) ? NEGATIVE_INF : POSITIVE_INF;        // INF / x

    if (!e1)
        e1 = 1 - BIAS;
    else {
        e1 = e1 - BIAS;
        m1 += LEADING_ONE;
    }
    if (!e2)
        e2 = 1 - BIAS;
    else {
        e2 = e2 - BIAS;
        m2 += LEADING_ONE;
    }

    int s3 = s1 ^ s2;
    int e3 = e1 - e2;
    m1 <<= 74;                                      // Add 0s to the end of the number to make the result more precise.
    __uint128_t m3 = m1 / m2;
    int pointPos = 74;

    int haveRemainder = 0;
    if (m3 != m1 * m2) haveRemainder = 1;

    return Calc(s3, e3, m3, pointPos, haveRemainder);
}

void Print(myDouble a) {
    if (FindSign(a)) {
        a = Positive(a);
        putchar('-');
    }

    int e = FindExp(a);
    __uint128_t m = FindFrac(a);
    if (CheckInf(e, m)) {
        printf("inf");
        return;
    }
    if (CheckNan(e, m)) {
        printf("nan");
        return;
    }

    int notZero = 0, zeroNumber = 0;
    for (int i = MAX_10_EXP, j; i >= 0; i--) {
        for (j = 9; j > 0; j--)
            if (CompareDouble(a, positivePow10[i][j])) {
                a = Subtraction(a, positivePow10[i][j]);
                notZero = 1;
                putchar(j + '0');
                break;
            }
        if (!j && notZero)
            putchar('0');
    }
    if (!notZero)
        putchar('0');

    __uint128_t decimal = 0;
    for (int i = 1, j; i <= 20; i++) {                                  // Maxbits == 20.
        for (j = 9; j > 0; j--)
            if (CompareDouble(a, negativePow10[i][j])) {
                a = Subtraction(a, negativePow10[i][j]);
                decimal = decimal * 10 + j;
                break;
            }
            if (!j && !decimal) zeroNumber++;
            if (!j) decimal *= 10;
    }

    if (decimal == 0) return;
    putchar('.');
    while (zeroNumber--)
        putchar('0');
    PrintUint128_t(decimal);
    return;
}