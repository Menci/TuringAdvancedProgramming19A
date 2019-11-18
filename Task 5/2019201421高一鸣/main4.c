
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
	for (int gap = len / 2; gap >= 1; gap /= 2) {
		for (int i = 0 + gap,j; i < len; i += gap) {
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
void* partition(void *arr, size_t size,void* left, void* right) {
	void* i = left, *j = right;
	//printf("%d\n",(int)(right-left));
	//fflush(stdout);
	swap1(left,left+(int)((right-left)/size/2)*size,size);
	
	memcpy(tmp, left, size);
	
	while (i < j) {
		while (i < j && (compar(j, tmp)>0))
			j-=size;
		if (i < j) {
			memcpy(i,j,size);
			i+=size;
		}
		while (i < j &&( compar(i , tmp)<=0))
			i+=size;
		if (i < j) {
			memcpy(j,i,size);
			j-=size;
		}
	}
	memcpy(i, tmp, size);
	return i;
}
void* partition2(void *arr, size_t size,void* left, void* right) {
	long long *i = left, *j = right,tmpp;
	//swap1(left,left+(int)((right-left)/size/2)*size,size);

	//memcpy(tmp, left, size);
	tmpp=*(long long*)left;
	
	while (i < j) {
		while (i < j && (compar(j, &tmpp)>0))
			j--;
		if (i < j) {
			*i=*j;
			i++;
		}
		while (i < j &&( compar(i , &tmpp)<=0))
			i++;
		if (i < j) {
			*j=*i;
			j--;
		}
	}
	*i=tmpp;
	return i;
}
void sort(void *arr, void* left, void* right) {
	if(right-left<=8*size) {
		//Shell_sort(left, (right-left)/size+1, size, compar);
		insertSort(left, (right-left)/size+1, size, compar);
		return;
	}
	
	void* j;
	if(size==8) j=partition2(arr, size, left, right);
	else j = partition(arr, size, left, right);
	
	if(j-size>=left) sort(arr,left, j - size);
	if(right>=j+size) sort(arr,j + size, right);
}
void sortOneByte(void *base, size_t nmemb) {
	int num[256];
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
	sort(sbase,sbase,sbase+255);
	char *tmp=sbase;
	for(int i=0;i<=255;i++) {
		for(int j=1;j<=num[i];j++) {
			*(char *)base=*tmp;
			base++;
		}
		tmp++;
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
	int flag=0;
	void *tmp=base;
	for(int i=0;i<nmemb-1;i++) {
		if(compar(tmp,tmp+size)>0) flag=1;
		tmp+=size;
	}
	if(!flag) return;
	flag=0;
	for(int i=0;i<nmemb-1;i++) {
		if(compar(base+i*size,base+i*size+size)<0) flag=1;
	}
	if(!flag) {
		void *b1,*b2;
		b1=base,b2=base+(nmemb-1)*size;
		while(b1<=b2) {
			swap1(b1,b2,size);
			b1+=size,b2-=size;
		}
		return;
	}
	
	sort(base,base,base+(nmemb-1)*size);
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
