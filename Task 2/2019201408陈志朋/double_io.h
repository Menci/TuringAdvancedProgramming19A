typedef struct {
	unsigned long long sign :  1;
	unsigned long long exp  : 11;
	unsigned long long frac : 52;
} Double;

int Start = 0;
int max (int a, int b) {
	if (a > b) return a;
	else return b;
}
void print (long long num) {
	if (num < 10) {
		printf("%lld.",num);
		return;
	}
	print(num / 10);
	printf("%lld", num % 10);
}
void Print_Bin (Double Num) {
	if (Num.sign == 1) putchar('-');
	printf("1.");
	int i;
	long long val = (Num.frac);
	for (i = 0; i <= 51; ++i){
		printf ("%lld", val % 2);
		val /= 2;
	}
	printf("*2^%d\n", ((int)Num.exp - Bias));
}
int Get_Point (char *num) {
	int len = strlen( num ), i;
	for (i = 0; i < len; ++i)
		if (num[i] == '.') return i;
	return len;
}
void Set_Sign (Double *Num, int Negative) {
	if (Negative == 1) Num->sign = 0;
	else Num->sign = 1;
	return;
}
void Set_Frac (Double *Num, int ID) {
	Num->frac = ( (Num->frac) | ( 1ll << ID) );
}
void Update_Frac_Int (Double *Num, long long Val) {
	if (Val == 1) {
		if (Start == 0) return;
		Set_Frac(Num, Start);
		Start += 1;
		return;
	}
	Update_Frac_Int(Num, Val/2);
	if (Val % 2 == 1) Set_Frac(Num, Start);
	Start += 1;
}
void Update_Frac_Double (Double *Num, unsigned long long Val) {
	for (; Start <= Max_Frac_Bin; ++Start) {
		Val *= 2;
		if (Val / Max_Frac_Num == 1)
			Set_Frac(Num, Start);
		Val %= Max_Frac_Num;
	}
}
void Update_Frac_Double_LeadingZero (Double *Num, unsigned long long Val) {
	int exp = 0;
	short Zero = 1;
	Start = 0;
	for (; Start <= Max_Frac_Bin;) {
		Val *= 2;
		if (Zero) {
			if (Val / Max_Frac_Num == 1) Zero = 2;
			exp -= 1;
		}
		else if (Val / Max_Frac_Num == 1)
			Set_Frac(Num, Start);
		if (!Zero) ++Start;
		if (Zero == 2) Zero = 0;
		Val %= Max_Frac_Num;
	}
	Num->exp = exp + Bias;
}
long long Char_to_Dec_Int (char *num, int Left, int Right) {
	long long tmp = 0;
	int i;
	for (i = Left; i < Right && tmp <= Max_Frac; ++i)
		tmp = tmp * 10 + num[i] - '0';
	return tmp;
}
unsigned long long Char_to_Dec_Double (char *num, int Left, int Right) {
	unsigned long long tmp = 0;
	int i, sum;
	for (i = Left, sum = 1; i < Right && sum <= Max_Frac_Dec; ++i, ++sum)
		tmp = tmp * 10 + num[i] - '0';
	while (sum <= Max_Frac_Dec) {
		tmp *= 10; sum++;
	}
	return tmp;
}
void Scanf (Double *Num) {
	int Index_Point, Negative, len = 0, exp = 0;
	char num[Max_Len], val;
	unsigned long long Dec_Int, Dec_Double;
	
	Start = 0;
	Num->exp = 0;
	Num->frac = 0;
	Num->sign = 0;
	
	for (val = getchar(); val < '0' || val >'9'; val = getchar());
	for (; (val >= '0' && val <= '9') || (val == '.'); val = getchar())
		num[len++] = val;
	
	if (num[0] == '-') Negative = -1;
	else Negative = 1;
	Set_Sign(Num, Negative);

	Index_Point = Get_Point( num );
	if (Negative == 1) Dec_Int = Char_to_Dec_Int(num, 0, Index_Point);
	else Dec_Int = Char_to_Dec_Int(num, 1, Index_Point);
	
	if (Dec_Int != 0) {
		Update_Frac_Int(Num, Dec_Int);
		Num->exp = Start + Bias;
		Dec_Double = Char_to_Dec_Double(num, Index_Point + 1, len);
		Update_Frac_Double(Num, Dec_Double);
	}
	else {
		Dec_Double = Char_to_Dec_Double(num, Index_Point + 1, len);
		Update_Frac_Double_LeadingZero(Num, Dec_Double);
	}
}
void Bin_to_Dec_Int (Double Num, int *t, int *len) {
	int i, j, x;
	long long Val = Num.frac, tmp = 1;
	int exp = Num.exp - Bias;
	for (i = 1; i <= exp && Val; ++i) {
		tmp = (tmp << 1) + (Val & 1);
		Val >>= 1;
	}
//	printf("%lld\n", tmp);
	for (*len = 0; tmp; tmp /= 10) 
		t[++(*len)] = tmp % 10;
//	for (j = (*len); j >= 1; --j)printf("%d", t[j]); printf("\n");
	for (; i <= exp; ++i) {
		x = 0;
		for (j = 1; j <= (*len); ++j) {
			t[j] = t[j] * 2 + x;
			x = t[j] / 10;
			t[j] %= 10;
		}
		if (x) t[++(*len)] = x;
//		for (j = (*len); j >= 1; --j)printf("%d", t[j]); printf("\n");
	}
}
long long Bin_to_Dec_Double (Double Num) {
	long long tmp = 0;
	long long Bin = (Max_Frac_Num >> 1);
	long long Val = Num.frac;
	long long sum[55], bin_double[2][55];
	int exp = Num.exp - Bias;
	int i = 0, j, Now = 0, Next;
	for (i = 1; i <= Max_Frac_Bin; ++i)
		bin_double[0][i] = bin_double[1][i] = sum[i] = 0;
	for (i = -1; i - 1 >= exp; --i, Bin >>= 1) {
		Next = Now ^ 1;
		for (j = 1; j < Max_Frac_Bin; ++j)
			if (bin_double[Now][j] != 0) bin_double[Next][j+1] = bin_double[Now][j];
			else bin_double[Next][j+1] = 0;
		if ((Bin & 1) == 1)bin_double[Next][1] = 1;
		else bin_double[Next][1] = 0;
		Now = Next;
	}
	if (exp < 0) Val = ((Val << 1) | 1);
	for (i = 0; i + 1 <= exp; ++i) Val >>= 1;
	for (; Bin > 1; Bin >>= 1, Val >>= 1){
		if ((Val & 1) == 1) {
			tmp += Bin;
			for (j = 1; j <= Max_Frac_Bin; ++j)
				sum[j] += bin_double[Now][j];
		}
		Next = Now ^ 1;
		for (j = 1; j < Max_Frac_Bin; ++j)
			if (bin_double[Now][j] != 0) bin_double[Next][j+1] = bin_double[Now][j];
			else bin_double[Next][j+1] = 0;
		if ((Bin & 1) == 1)bin_double[Next][1] = 1;
		else bin_double[Next][1] = 0;
		Now = Next;
	}
	for (j = Max_Frac_Bin; j >= 2; --j)
		sum[j-1] += sum[j] / 2;
	return (tmp + (sum[1] / 2));
}
void Printf (Double Num, int Precision) {
	long long Val;
	long long Dec_Int = 0;
	long long Dec_Double = 0;
	int exp = Num.exp - Bias, t[300], len;
	int Double[20], i;
	
	if (Num.exp == inf && Num.frac == 0){
		if (Num.sign == 1) putchar('-');
		printf("inf");
		return;
	}
	if (Num.exp == nan && Num.frac == 0){
		if (Num.sign == 1) putchar('-');
		printf("nan");
		return;
	}
	
	if (exp >= 0) {
		Bin_to_Dec_Int(Num, t, &len);
		Dec_Double = Bin_to_Dec_Double(Num);
	}
	else {
		t[len = 1] = 0;
		Dec_Double = Bin_to_Dec_Double(Num);
	}
	
	for (i = 1; i <= 18; ++i)Double[i] = 0;
	if (Num.sign == 1) putchar('-');
	
	for (i = 1; Dec_Double; Dec_Double /= 10, ++i) {
		Double[i] = Dec_Double % 10;
	}
	if (Double[18 - Precision] >= 5)
		Double[18 - Precision + 1]++;
	for (i = 18 - Precision + 1; i <= 18; ++i) {
		Double[i+1] += Double[i] / 10;
		Double[i] %= 10;
	}
	if (Double[19]) {
		t[len + 1] = 0;
		for (i = 1; i <= len; ++i){
			t[i + 1] += t[i] / 10;
			t[i] %= 10;
		}
		if (t[len + 1])len++;
	}
	for (i = len; i >= 1; --i)printf("%d", t[i]);
	putchar('.');
	for (i = 18; i >= 18 - Precision + 1; --i)
		printf("%d", Double[i]);
}
