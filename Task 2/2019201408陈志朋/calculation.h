Double Convert_to_MyDouble (uint64_t num) {
	int i;
	Double Num;
	Num.sign = Num.exp = Num.frac = 0;
	uint64_t bin = (1ll << 51), tmp, Bin = 1;

	for (i = 0; i <= 51; ++i) {
		tmp = (num & bin);
		if (tmp) Num.frac |= Bin;
		bin >>= 1; Bin <<= 1;
	}
	
	bin = (1ll << 52);
	for (i = 0; i <= 10; ++i) {
		tmp = ((num & bin) >> 52);
		Num.exp |= tmp;
		bin <<= 1;
	}

	Num.sign = (!(!(num & bin)));
	return Num;
}
uint64_t Convert_to_StdDouble (Double num) {
	int i;
	uint64_t Num = 0;
	
	if (num.sign)Num = (1ll << 63);
	
	for (i = 0; i <= 10; ++i) 
		if (num.exp & (1 << i))
			Num |= (1ll << (i+52));
	
	for (i = 0; i <= 51; ++i)
		if (num.frac & (1ll << i))
			Num |= (1ll << (51 - i));
	return Num;
}
uint64_t ChangeMe (double tmp) {
	ull a = *((ull *)&tmp);
	return a;
}
double ChangeStd (uint64_t tmp) {
	double a = *((double *)&tmp);
	return a;
}

void Bin_to_Char(char *num, ull val, int start, unsigned int flag, int maxlen) {
	int i, j;
	for (i = 0; i < start; ++i) num[i] = '0';
	for (i = start, j = Max_Frac_Bin - 1; j >= 0; ++i, --j) {
		if (val & (1ll << j)) num[i] = '1';
		else num[i] = '0';
	}
	if (flag) num[i++] = '1';
	else num[i++] = '0';
	for (; i < maxlen; ++i)
		num[i] = '0';
}
void Round(int *val, int *Max) {
	int i;
	int start = (*Max) - (Max_Frac_Bin + 1);
	if (start < 0)return;
	else if (val[start] == 0) return;
	else {
		bool flag = 0;
		for (i = start - 1; i >= 0; --i)
			if (val[i]) {flag = 1; break;}
		if (val[start + 1])flag = 1;
		if (flag) {
			val[start + 1] += 1;
			for (i = start + 1; i <= *Max; ++i) {
				val[i + 1] += val[i] / 2;
				val[i] %= 2;
			}
			if (val[(*Max) + 1]) (*Max)++;
		}
	}
}
void Char_to_Bin (ull *num, int *val, int Max) {
	int i, j;
	for (i = Max - 1, j = 0; j < Max_Frac_Bin; --i, ++j)
		(*num) |= ((1ll << j) * val[i]);
}
Double Add (Double a, Double b) {
	int i, Max, last;
	char vala[Max_Frac_Bin * 2 + 5], valb[Max_Frac_Bin * 2 + 5];
	int vals[Max_Frac_Bin * 2 + 5];
	unsigned int val;
	Double tmp;
	
	if (a.exp > b.exp) {
		tmp = b; b = a; a = tmp;
	}
	if (a.exp != 0)val = b.exp - a.exp;
	else if (b.exp != 0)val = b.exp - a.exp - 1;
	else val = 0;
	if (val >= Max_Frac_Bin + 2) return b;
	tmp.sign = tmp.exp = tmp.frac = 0;
	Bin_to_Char(vala, a.frac, 0  , a.exp, Max_Frac_Bin + val + 1);
	Bin_to_Char(valb, b.frac, val, b.exp, Max_Frac_Bin + val + 1);
	Max = Max_Frac_Bin + val;
	tmp.exp = b.exp;
	memset(vals, 0, sizeof(vals));
	for (i = 0; i <= Max; ++i) {
		vals[i] += (vala[i] - '0') + (valb[i] - '0');
		vals[i+1] += vals[i] / 2;
		vals[i] %= 2;
	}
	if (vala[Max] == '0' && valb[Max] == '0' && vals[Max] == 1) tmp.exp++;
	if (vals[Max + 1]) {
		Max++;
		if (tmp.exp < inf) tmp.exp++;
	}
	last = Max;
	Round(vals, &Max);
	if (Max != last && tmp.exp < inf) tmp.exp++;
	ull Frac = 0;
	Char_to_Bin(&Frac, vals, Max);
	tmp.frac = Frac;
	return tmp;
}
bool judge(Double a, Double b) {
	if (a.exp > b.exp) return true;
	else if (a.exp < b.exp) return false;
	else {
		int i;
		for (i = 0; i <= 63; ++i) {
			if ((a.frac & (1ll << i)) > (b.frac & (1ll << i))) return true;
			if ((a.frac & (1ll << i)) < (b.frac & (1ll << i))) return false;
		}
	}
	return false;
}
void Char_to_Bin1 (ull *num, int *val, int Max, int exp) {
	int i, j, start;
	if (exp > 0) start = Max - 1;
	else {
		Max -= exp;
		start = Max;
	}
	for (i = start, j = 0; j < Max_Frac_Bin && i >= 0; --i, ++j)
		(*num) |= ((1ll << j) * val[i]);
}
Double Subtraction(Double a, Double b) {
	int exp = 0;
	int i, j, Max, last;
	char vala[Max_Frac_Bin * 2 + 5], valb[Max_Frac_Bin * 2 + 5];
	int vals[Max_Frac_Bin * 2 + 5];
	unsigned int val;
	ull Frac = 0;
	Double tmp;
	tmp.exp = tmp.frac = 0;
	tmp.sign = 1;
	
	if (judge(a, b)) {
		tmp = b; b = a; a = tmp;
		tmp.sign = tmp.exp = tmp.frac = 0;
	}
	if (a.exp != 0)val = b.exp - a.exp;
	else if (b.exp != 0)val = b.exp - a.exp - 1;
	else val = 0;
	
	if (val >= Max_Frac_Bin + 2){
		b.sign = tmp.sign;
		return b;
	}
	Bin_to_Char(vala, a.frac, 0  , a.exp, Max_Frac_Bin + val + 1);
	Bin_to_Char(valb, b.frac, val, b.exp, Max_Frac_Bin + val + 1);
	
	Max = Max_Frac_Bin + val;
	
	memset(vals, 0, sizeof(vals));
	for (i = 0; i <= Max; ++i) {
		vals[i] += (valb[i] - '0') - (vala[i] - '0');
		if (vals[i] < 0) {
			vals[i] = 1; 
			for (j = i + 1; j <= Max; ++j)
				if (valb[j] == '0')valb[j] = '1';
				else {
					valb[j] = '0'; break;
				}
		}
	}
	if (b.exp) exp = b.exp;
	else exp = 1;
	while (exp > 0 && Max > 0 && vals[Max] == 0) {
		exp--; Max--;
	}
	last = Max;
	Round(vals, &Max);
	if (Max != last) exp++;
	Char_to_Bin1(&Frac, vals, Max, exp);
	tmp.frac = Frac;
	if (exp < 0) tmp.exp = 0;
	else tmp.exp = exp;
	return tmp;
}
bool judge_nan(Double num) {
	return (num.exp == nan && num.frac != 0);
}
bool judge_inf(Double num) {
	return (num.exp == inf && num.frac == 0);
}
Double Add_Solve (Double a, Double b) {
	if (judge_nan(a)) return a;
	if (judge_nan(b)) return b;
	if (judge_inf(a)) return a;
	if (judge_inf(b)) return b;
	Double val;
	if (a.sign == b.sign) {
		val = Add(a, b);
		val.sign = a.sign;
	}
	else {
		if (a.sign == 1) {
			val      = Subtraction(a, b);
			val.sign = val.sign ^ 1;
		}
		else {
			val = Subtraction(a, b);
		}
	}
	if (val.exp == inf) val.frac = 0;
	return val;
}
Double Subtraction_Solve (Double a, Double b) {
	b.sign ^= 1;
	return Add_Solve(a, b);
}
Double Multiplication (Double a, Double b) {
	bool flag = 0;
	int i, j, Max, last, exp;
	char vala[Max_Frac_Bin * 2 + 5], valb[Max_Frac_Bin * 2 + 5];
	int vals[Max_Frac_Bin * 4 + 5];
	unsigned int val;
	Double tmp;

	tmp.sign = tmp.exp = tmp.frac = 0;
	tmp.sign = a.sign ^ b.sign;
	if (a.exp && b.exp) exp = a.exp + b.exp - Bias;
	else if (!a.exp && !b.exp) {
		tmp.exp = tmp.frac = 0;
		return tmp;
	}
	else exp = a.exp + b.exp - Bias + 1;
	Bin_to_Char(vala, a.frac, 0, a.exp, Max_Frac_Bin);
	Bin_to_Char(valb, b.frac, 0, b.exp, Max_Frac_Bin);
	Max = Max_Frac_Bin;
	memset(vals, 0, sizeof(vals));
	for (i = 0; i <= Max; ++i) {
		for (j = 0; j <= Max; ++j) {
			vals[i + j] += (vala[i] - '0') * (valb[j] - '0');
		}
	}
	Max <<= 1;
	for (i = 0; i <= Max; ++i) {
		vals[i + 1] += vals[i] / 2;
		vals[i] %= 2;
	}
	while (vals[Max + 1]) {
		Max++; exp++;
		vals[Max + 1] += vals[Max] / 2;
		vals[Max] %= 2;
	}
	while (vals[Max] == 0) {
		Max--; exp--;
	}
	while (exp < 1 && Max < Max_Frac_Bin * 4 + 4) {
		exp++; Max++;
		vals[Max] = 0;
		flag = 1;
	}
	if (flag) exp--;
	last = Max;
	Round(vals, &Max);
	if (Max != last && exp < inf) exp++;
	ull Frac = 0;
	Char_to_Bin(&Frac, vals, Max);
	tmp.frac = Frac;
	if (exp >= inf) {
		tmp.exp = inf;
		tmp.frac = 0;
		return tmp;
	}
	else tmp.exp = exp;
	if (tmp.frac == 0)tmp.exp = 0;
	return tmp;
}
Double Multiplication_Solve (Double a, Double b) {
	Double val;
	if (judge_nan(a)) return a;
	if (judge_nan(b)) return b;
	if (judge_inf(a)) {
		val = a;
		val.sign = a.sign ^ b.sign;
		return val;
	}
	if (judge_inf(b)) {
		val = b;
		val.sign = a.sign ^ b.sign;
		return val;
	}
	val = Multiplication(a, b);
	return val;
}
void out (__int128 x) {
	if (x < 10){
		putchar(x + '0');
		return;
	}
	out(x / 10);
	putchar((x % 10) + '0');
}
#define bias 70
__int128 Double_to_int128 (long long val, int exp) {
	int i, j;
	__int128 num = 0;
	for (i = 0, j = Max_Frac_Bin - 1; i < Max_Frac_Bin; ++i, --j) {
		if (val & (1ll << i)) num |= (1ll << j);
	}
	if (exp) num |= (1ll << 52);
	return num;
}
void int128_to_int (__int128 val, int *num) {
	int i;
	__int128 Bin = 1;
	for (i = 0; i <= Max_Frac_Bin + bias; ++i, Bin <<= 1) {
		if (val & Bin) num[i] = 1;
	}
}
void Round1(int *val, int *Max, __int128 Mod) {
	int i;
	int start = (*Max) - (Max_Frac_Bin + 1);
	if (start < 0)return;
	else if (val[start] == 0) return;
	else {
		bool flag = 0;
		for (i = start - 1; i >= 0; --i)
			if (val[i]) {flag = 1; break;}
		if (val[start + 1] || Mod != 0)flag = 1;
		if (flag) {
			val[start + 1] += 1;
			for (i = start + 1; i <= *Max; ++i) {
				val[i + 1] += val[i] / 2;
				val[i] %= 2;
			}
			if (val[(*Max) + 1]) (*Max)++;
		}
	}
}
Double Division(Double a, Double b) {
	Double tmp;
	int num[200], exp = 0, Max = bias, i, j, last, tmpadd = 0;
	unsigned long long Frac = 0;
	memset(num, 0, sizeof(num));
	tmp.sign = a.sign ^ b.sign;
	exp = ((int)a.exp) - ((int)b.exp) + Bias;
	if (a.exp == 0 && b.exp != 0) exp++;
	else if (a.exp != 0 && b.exp == 0) exp--;
	__int128 a1 = 0, b1 = 0;
	a1 = Double_to_int128(a.frac, a.exp);
	b1 = Double_to_int128(b.frac, b.exp);
	a1 = (a1 << bias);
	int128_to_int(a1 / b1, num);
	for (i = 0, j = 0; i + Max <= Max_Frac_Bin + bias; ++i, ++j) {
		if (num[i + Max]) tmpadd=j;
	}
	Max += tmpadd;
	exp += tmpadd;
	while (Max > 0 && num[Max] == 0){
		Max--; exp--;
	}
	bool flag = 0;
	while (exp < 1 && Max < Max_Frac_Bin + 80) {
		exp++; Max++;
		num[Max] = 0;
		flag = 1;
	}
	if (flag) exp--;
	last = Max;
	Round1(num, &Max, a1 % b1);
	if (Max != last) exp++;
	Char_to_Bin(&Frac, num, Max);
	tmp.frac = Frac;
	tmp.exp  = exp;
	if (exp >= inf) {
		tmp.frac = 0;
		tmp.exp  = inf;
	}
	if (exp < 0) tmp.exp = tmp.sign = tmp.frac = 0;
	return tmp;
}
bool judge_zero(Double num) {
	return (num.frac == 0);
}
Double Division_Solve (Double a, Double b) {
	Double val;
	if (judge_nan(a))return a;
	if (judge_nan(b))return b;
	if (judge_zero(b)){
		val.exp  = nan;
		val.frac = val.sign = 0;
		return val;
	}
	val = Division(a, b);
	return val;
}
uint64_t Calc (uint64_t aa, uint64_t bb, char op) {
	Double a = Convert_to_MyDouble(aa);
	Double b = Convert_to_MyDouble(bb);
	Double result;

	if      (op == '+') result = Add_Solve(a, b);
	else if (op == '-') result = Subtraction_Solve(a, b);
	else if (op == '*') result = Multiplication_Solve(a, b);
	else if (op == '/') result = Division_Solve(a, b);

	return Convert_to_StdDouble(result);
}
