#include <stdio.h>
#include <math.h>
#include <string.h>
char tmp[10000];
size_t size;
void *base;
void swap1(void *a,void *b,size_t size) {
	memcpy(tmp, a, size);
	memcpy(a, b, size);
	memcpy(b, tmp,size);
}
void swap(int a,int b) {
	int tt=a;
	a=b,b=tt;
}
int (*compar)(const void *, const void *);
int (*compar2)(const void *, const void *);
void Shell_sort(void *a, int len,size_t size,int (*compar)(const void *, const void *)) {
	int i,j,gap;  //步长
	for (gap = len / 2; gap >= 1; gap /= 2) {
		for (i = 0 + gap; i < len; i += gap) {
			memcpy(tmp,a+i*size,size);
			j = i - gap;
			while (j >= 0 && compar(a+j*size,tmp)>0) {
				memcpy(a+(j+gap)*size,a+j*size,size);
				j -= gap;
			}
			memcpy(a+(j+gap)*size,tmp,size);
		}
	}
}
void insertSort(void *a, int len,size_t size,int (*compar)(const void *, const void *)) {
	int i,j;
	for (i = 1 ; i < len; i++) {
		memcpy(tmp,a+i*size,size);
		j = i-1;
		while (j >= 0 && compar(a+j*size,tmp)>0) {
			memcpy(a+(j+1)*size,a+j*size,size);
			j -= 1;
		}
		memcpy(a+(j+1)*size,tmp,size);
	}
}
int partition(void *arr, size_t size,int left, int right) {
	int i = left, j = right;
	swap1(arr+left*size,arr+((left+right)/2)*size,size);
	memcpy(tmp, arr+left*size, size);
	while (i < j) {
		while (i < j && (compar((arr+j*size), tmp)>0))
			j--;
		if (i < j) {
			memcpy(arr+i*size,arr+j*size,size);
			i++;
		}
		while (i < j &&( compar((arr+i*size) , tmp)<=0))
			i++;
		if (i < j) {
			memcpy(arr+j*size,arr+i*size,size);
			j--;
		}
	}
	memcpy(arr+i*size, tmp, size);
	return i;
}
void sort(void *arr, size_t size, int left, int right) {
	if(left > right)
		return;
	if(right-left<=10) {
		Shell_sort(arr+left*size, right-left+1, size, compar);
//		insertSort(arr+left*size, right-left+1, size, compar);
		return;
	}
	
	int j = partition(arr, size, left, right);
	sort(arr, size,left, j - 1);
	sort(arr, size,j + 1, right);
}
void sortOneByte(void *base, size_t nmemb) {
	int num[255];
	memset(num,0,sizeof(num));
	char sbase[255];
	for(int i=0;i<=255;i++) {
		sbase[i]=i;
	}
	for(int j=0;j<nmemb;j++) {
		int x=*((char *)base+j);
		if(x<0) x+=256;
		num[x]++;
	}
	sort(sbase,1,0,255);
	char *tmp=sbase;
	for(int i=0;i<=255;i++) {
		for(int j=1;j<=num[i];j++) {
			*(char *)base=*tmp;
			base++;
		}
		tmp++;
	}
}
int cmpId(const void *A, const void *B) {
	int AA=*(int *)A,BB=*(int *)B;
	return compar2(base+size*AA,base+size*BB);
}
int base2[2000010];
char tmpp[770000000];
void sortBigByte(void *base, size_t nmemb, size_t size) {
	for(int i=0;i<nmemb;i++) {
		base2[i]=i;
	}
	compar2=compar;
	compar=cmpId;
	sort(base2,sizeof(int),0,nmemb-1);
	return;
	memcpy(tmpp,base,nmemb*size);
	for(int i=0;i<nmemb;i++) {
		memcpy(base+i*size,tmpp+base2[i]*size,size);
	}
}
void My_sort(void *_base, size_t nmemb, size_t _size,int (*cmp)(const void *, const void *)) {
	size=_size,base=_base;
//	qsort(base, nmemb, size, (*compar));
//	Shell_sort(base, nmemb, size, compar);
	compar=cmp;
	if(size==1) {
		sortOneByte(base,nmemb);
		return;
	}
	if(size>=3000) {
		sortBigByte(base,nmemb,size);
		return;
	}
	int flag=0;
	void *tmp=base;
	for(int i=0;i<nmemb-1;i++) {
		if(compar(tmp,tmp+size)>0) flag=1;
		tmp+=size;
	}
	if(!flag) {
		return;
	}
	/*flag=0;
	for(int i=0;i<nmemb-1;i++) {
		if(compar(base+i*size,base+i*size+size)<0) flag=1;
	}
	if(!flag) return;*/
	sort(base,size,0,nmemb-1);
}
int a[2000010]={3,2,4};
int cmp(const void* a,const void* b) {
	if( *((int*)(a))==*((int*)(b))) return 0;
	if( *((int*)(a))>*((int*)(b))) return 1;
	return -1;
}
int main(int argc, char *argv[]) {
	int len=2000000;
	len--;
	for(int i=0;i<=len;i++) {
		a[i]=rand();
	}
	My_sort(a,len+1,sizeof(int),cmp);
//	Qsort2(a, 0, len-1);
//	for(int i=0;i<=20;i++) printf("%d ",a[i]);
}