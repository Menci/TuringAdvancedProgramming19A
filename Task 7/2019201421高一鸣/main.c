#include <stdio.h>
#include <stdlib.h>

#include "treap.h"
int main() {
  int n;
  scanf("%d", &n);
  while (n--) {
    int opt, x;
    scanf("%d%d", &opt, &x);
    switch (opt) {
      case 0:
        Insert(x);
        break;
      case 1:
        Delete(getRank(root, x));
        break;
      case 2:
        printf("%d\n", getKth(x, root));
        break;
      case 3:
        printf("%d\n", getRank(root, x - 1));
        break;
      case 4:
        printf("%d\n", findPre(x, root));
        break;
      case 5:
        printf("%d\n", findNxt(x, root));
        break;
    }
  }
  clearTreap(root);
}