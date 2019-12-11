#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int sum_malloc, sum_free;
char ch[100];
int main () {
    freopen ("test.out", "r", stdin);
	sum_malloc = 0;
	sum_free   = 0;
    while (scanf ("%s", ch) != EOF) {
        if (ch[0] == 'm') {
            sum_malloc++;
            scanf ("%s", ch);
        }
        else if (ch[0] == 'f') {
			sum_free++;
			scanf ("%s", ch);
		}
    }
	if (sum_malloc != sum_free) puts("Wrong");
	else puts("Correct");
    return 0;
}