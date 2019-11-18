#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#define SWAP(a, b, size)                \
    do {                                \
        COPY(swapBuffer, (a), size);    \
        COPY((a), (b), size);           \
        COPY((b), swapBuffer, size);    \
    } while (0)

#define COPY(a, b, size) memcpy((a), (b), (size))
#define INTRO_LOWER_BOUND 16

int my_log2(int x) {
    int ret = 0;
    while (x >>= 1) ++ret;
    return ret;
}

static inline void myHeapSort(void *base, size_t nmemb, int size,
                                int (*compar)(const void *, const void *),
                                void *swapBuffer) {
    if (nmemb <= 1) return;
    void *begin = base - size;
    for (int i = nmemb >> 1; i >= 1; --i) {
        for (int k = i, j = k << 1; j <= nmemb; k = j, j <<= 1) {
            void *heapFa = begin + k * size, *heapCh = begin + j * size;
            if ((j | 1) <= nmemb && compar(heapCh, (heapCh + size)) < 0)
                j |= 1, heapCh = begin + (j | 1) * size;
            if (compar(heapFa, heapCh) < 0) SWAP(heapFa, heapCh, size);
        }
    }
    for (int i = nmemb; i > 1; --i) {
        SWAP(base, begin + i * size, size);
        for (int k = 1, j = k << 1; j < i; k = j, j <<= 1) {
            void *heapFa = begin + k * size, *heapCh = begin + j * size;
            if ((j | 1) < i && compar(heapCh, (heapCh + size)) < 0)
                j |= 1, heapCh = begin + (j | 1) * size;
            if (compar(heapFa, heapCh) < 0) SWAP(heapFa, heapCh, size);
        }
    }
}

static inline void myInsertionSort(void *base, size_t nmemb, size_t size,
                                     int (*compar)(const void *, const void *),
                                     void *swapBuffer) {
    void *tmpBuf = malloc(size), *rlim = base + (nmemb * size);
    for (void *i = base + size; i < rlim; i = i + size) {
        void *j = i - size;
        COPY(tmpBuf, i, size);
        if(compar(base, tmpBuf) > 0) {
            for(j; j >= base; j -= size) 
                COPY(j + size, j, size);
            COPY(base, tmpBuf, size);
        }
        else {
            for(j; compar(j, tmpBuf) > 0; j -= size) 
                COPY(j + size, j, size);
            COPY(j + size, tmpBuf, size);
        }
    }
    if (tmpBuf != NULL) free(tmpBuf);
}

static inline int checkSpecialSeq(void *base, size_t nmemb, size_t size,
                       int (*compar)(const void *, const void *)) {
    int reverseNum = 0, eqNum = 0;
    for (void *i = base, *Rlim = base + (nmemb - 1) * size; i < Rlim; i += size) {
        int res = compar(i, i + size);
        if (res > 0) ++reverseNum;
        if (res == 0) ++eqNum;
    }
    if (!reverseNum) return 1;
    if (reverseNum + eqNum + 1 == nmemb) return -1; 
    return 0;
}

void reverseSequence(void *base, size_t nmemb, size_t size, void *swapBuffer) {
    for (int i = 0; i < (nmemb >> 1); ++i)
        SWAP(base + i * size, base + (nmemb - 1 - i) * size, size);
}

void myIntroSort(void *base, size_t nmemb, size_t size,
                       int (*compar)(const void *, const void *), int depLim,
                       void *swapBuffer) {
    if (nmemb <= 1) return;
    void *tmpBuf = malloc(size);
    while (nmemb > INTRO_LOWER_BOUND) {
        if (depLim == 0) {
            myHeapSort(base, nmemb, size, compar, swapBuffer);
            if (tmpBuf != NULL) free(tmpBuf);
            return;
        } else {
            --depLim;
            int medianPos;
            void *seqMid = base + (((nmemb >> 1) - 1) * size),
                 *seqEnd = base + (nmemb - 1) * size;
            if (compar(base, seqMid) <= 0) {
                if (compar(seqMid, seqEnd) <= 0)
                    medianPos = (nmemb >> 1) - 1;
                else {
                    if (compar(base, seqEnd) <= 0)
                        medianPos = nmemb - 1;
                    else
                        medianPos = 0;
                }
            } else {
                if (compar(seqMid, seqEnd) <= 0) {
                    if (compar(base, seqEnd) <= 0)
                        medianPos = 0;
                    else
                        medianPos = nmemb - 1;
                } else
                    medianPos = (nmemb >> 1) - 1;
            }
            void *frontPtr = base, *backPtr = base + (nmemb * size);
            COPY(tmpBuf, base + medianPos * size, size);
            while (1) {
                backPtr = backPtr - size;
                while (compar(frontPtr, tmpBuf) < 0) frontPtr = frontPtr + size;
                while (compar(tmpBuf, backPtr) < 0) backPtr = backPtr - size;
                if (frontPtr >= backPtr) break;
                SWAP(frontPtr, backPtr, size);
                frontPtr = frontPtr + size;
            }
            medianPos = ((frontPtr - base) / size);
            myIntroSort(base + (medianPos * size), nmemb - medianPos, size, compar, depLim,
                              swapBuffer);
            nmemb = medianPos;
        }
    }
    if (tmpBuf != NULL) free(tmpBuf);
    myInsertionSort(base, nmemb, size, compar, swapBuffer);
}

void Qsort(void *base, size_t nmemb, size_t size,
              int (*compar)(const void *, const void *)) {
    if (nmemb <= 1) return;
    void *swapBuffer = malloc(size);
    int seqSt = checkSpecialSeq(base, nmemb, size, compar);
    if(seqSt == 1) return;
    else if(seqSt == -1) {
        reverseSequence(base, nmemb, size, swapBuffer);
        return;
    } 
    myIntroSort(base, nmemb, size, compar, my_log2(nmemb) << 1,
                      swapBuffer);
    free(swapBuffer);
}
