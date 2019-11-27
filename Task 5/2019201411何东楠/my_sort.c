#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#define MAX_BYTE (1000)
#define ADR(begAdr, i, size) ((begAdr) + ((i) - 1) * (size))

void *swapTmp[MAX_BYTE];

void swap(void *ele1Adr, void *ele2Adr, size_t size)
{
	memcpy(swapTmp, ele1Adr, size);
	memcpy(ele1Adr, ele2Adr, size);
	memcpy(ele2Adr, swapTmp, size);
}

void *base[MAX_BYTE];

int is_sorted(void *begAdr, size_t nmemb, size_t size, 
		int (*compar) (const void *, const void *))
{
	for (int i = 2; i <= (int) nmemb; ++i) {
		if (compar(ADR(begAdr, i, size), ADR(begAdr, i - 1, size)) < 0) return 0;
	}
	return 1;
}

int is_rev_sorted(void *begAdr, size_t nmemb, size_t size, 
		int (*compar) (const void *, const void *))
{
	for (int i = 2; i <= (int) nmemb; ++i) {
		if (compar(ADR(begAdr, i, size), ADR(begAdr, i - 1, size)) > 0) return 0;
	}
	return 1;
}

void bf_sort(void *begAdr, size_t nmemb, size_t size, 
		int (*compar) (const void *, const void *))
{
	for (int i = 1; i <= (int) nmemb; ++i) {
		int flag = i;

		for (int j = i + 1; j <= (int) nmemb; ++j) {
			if (compar(ADR(begAdr, j, size), ADR(begAdr, flag, size)) < 0) {
				flag = j;
			}
		}

		if (flag != i) {
			swap(ADR(begAdr, flag, size), ADR(begAdr, i, size), size);
		}
	}
}

void qsort(void *begAdr, size_t nmemb, size_t size, 
		int (*compar) (const void *, const void *))
{
	if ((int) nmemb <= 7) {
		if (is_sorted(begAdr, nmemb, size, compar)) 
			return ;

		if (is_rev_sorted(begAdr, nmemb, size, compar)) {
			for (int i = 1; i <= (int) nmemb / 2; ++i) {
				swap(ADR(begAdr, i, size), ADR(begAdr, nmemb - i + 1, size), size);
			}
		}
		return ;

	}

	if ((int) nmemb <= 7) {
		bf_sort(begAdr, nmemb, size, compar);
		return ;
	}

	swap(ADR(begAdr, 1, size), ADR(begAdr, nmemb / 2, size), size);
	memcpy(base, ADR(begAdr, 1, size), size);

	int i = 1, j = (int) nmemb;
	while (i < j) {
		while (i < j && compar(base, ADR(begAdr, j, size)) <= 0) j --;
		if (i < j) {
			memcpy(ADR(begAdr, i, size), ADR(begAdr, j, size), size);
			i ++;
		}

		while (i < j && compar(base, ADR(begAdr, i, size)) > 0) i ++;
		if (i < j) {
			memcpy(ADR(begAdr, j, size), ADR(begAdr, i, size), size);
			j --;
		}
	}

	memcpy(ADR(begAdr, i, size), base, size);

	int ed = i - 1;
	while (ed >= 1 && compar(ADR(begAdr, ed, size), base) == 0) ed --;
	qsort(ADR(begAdr, 1, size), ed, size, compar);

	int st = i + 1;
	while (st <= nmemb && compar(ADR(begAdr, st, size), base) == 0) st ++;
	qsort(ADR(begAdr, st, size), nmemb - st + 1, size, compar);

}

void my_sort(void *begAdr, size_t nmemb, size_t size, 
		int (*compar) (const void *, const void *))
{
	if (is_sorted(begAdr, nmemb, size, compar)) return ;
	if (is_rev_sorted(begAdr, nmemb, size, compar)) {
		for (int i = 1; i <= (int) nmemb / 2; ++i) {
			swap(ADR(begAdr, i, size), ADR(begAdr, nmemb - i + 1, size), size);
		}
		return ;
	}
	qsort(begAdr, nmemb, size, compar);
}

#undef ADR
#undef MAX_BYTE

int compar(const void* a, const void* b)
{
	if (*((int *)a) > *((int *)b)) return 1;
	if (*((int *)a) == *((int *)b)) return 0;
	return -1;
}

int main()
{
	freopen("input.in", "r", stdin);
	freopen("output.out", "w", stdout);

	int n, a[(int) 1e6 + 5] = {0};

	scanf("%d", &n);
	for (int i = 1; i <= n; ++i) 
		scanf("%d", a + i);
	my_sort((void *) (a + 1), n, sizeof(a[1]), compar);
	for (int i = 1; i <= n; ++i) 
		printf("%d%c", a[i], i == n ? '\n' : '\n');

}

