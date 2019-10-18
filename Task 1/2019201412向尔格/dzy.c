#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#define Move 70
#define Max_val 2047
#define bias 1023
#define GNN printf("!\n");

struct node {
	unsigned char db[8];
}a, b, c, d, Infinity;

int get_sign(struct node T) {
	if( (T.db[7] & (1 << 7))) return 1;
	return 0;
}

void Input() {
	scanf("%lf%lf", &a, &b);
}

unsigned long long Exp(struct node T) {

	unsigned long long Value = 0;
	for(int i = 4; i < 8; ++i) {
		if(T.db[6] & (1 << i)) Value += (1 << (i-4));
	}

	for(int i = 0;i < 7; ++i) {
		if(T.db[7] & (1 << i)) Value += (1 << (i+4));
	}
	return Value;
}

unsigned long long Val(struct node T) {
	unsigned long long Value = 0;
	unsigned long long leiji = 0;

	for(int i = 0;i < 6; ++i) {
		for(int j = 0;j < 8; ++j) {
			if(T.db[i] & (1 << j)) Value += (1LL << (j + leiji));
		}
		leiji += 8;
	}

	for(int i = 0;i < 4; ++i) if(T.db[6] & (1 << i)) Value += (1LL << (i + leiji));
	if(Exp(T)) Value += (1LL << 52);
	return Value;
}

struct node restore(unsigned long long ex, unsigned long long Value, unsigned long long Sig) {
	struct node temp;
	for(int i = 0;i < 8; ++i) temp.db[i] = 0;
	unsigned long long pos = 0;

	for(int i = 0;i < 6; ++i) {
		for(int j = 0;j < 8; ++j) {
			if(Value & (1LL << (pos + j))) temp.db[i] += (1 << j);
		} pos += 8;
	}

	for(int j = 0;j < 4; ++j) if(Value & (1LL << (pos+j))) temp.db[6] += (1 << j);
	for(int j = 4;j < 8; ++j) if(ex & (1 << (j-4))) temp.db[6] += (1 << j);
	for(int j = 0;j < 7; ++j) if(ex & (1 << (j+4))) temp.db[7] += (1 << j);

	if(Sig > 0) temp.db[7] += (1 << 7);
	return temp;
}

bool INF(struct node T) {
	if(Exp(T) == Max_val && Val(T) == 0) return true;
	return false;
}

void print(struct node T) {
	printf("%.50le\n", *(double*)&T);
}

int RD_MUL(__int128 c_val, int pos, unsigned long long newval) {
	bool flag = 0;
	for(int i = pos;i >= 0; --i) {
		if( (c_val >> i) & 1) flag = 1;
	}

	if(flag) return 1;
	else if(newval & 1) return 1;
	else return 0;
}

void comp(unsigned long long a_exp, unsigned long long b_exp) {
	if(a_exp < b_exp) return ;
	if(a_exp > b_exp || Val(a) > Val(b)) {
		d = a;
		a = b;
		b = d;
		return; 
	}
}

bool ISZERO(struct node T) {
	if(Exp(T) == 0 && Val(T) == 0) return true;
	return false;
}

unsigned long long get_val(__int128 c_val, int st, int End) {
	unsigned long long newval = 0;
	for(int i = st;i >= End; --i) if( (c_val >> i) & 1) newval += (1LL << i - End);
	return newval;
}

void Mul() {
	unsigned long long a_exp = Exp(a);
	unsigned long long b_exp = Exp(b);

	comp(a_exp, b_exp);
	a_exp = Exp(a), b_exp = Exp(b);

	unsigned long long c_exp = 0;
	bool if_nornor = 0, is_a = 0, is_b = 0;

	if(!a_exp) ++ a_exp, is_a = 1;
	if(!b_exp) ++ b_exp, is_b = 1;

	if(a_exp + b_exp < bias) if_nornor = 1, c_exp = bias - a_exp - b_exp;
	else c_exp = a_exp + b_exp - bias;

	if(c_exp >= Max_val) {
		c = Infinity;
		if((get_sign(a) * get_sign(b))) c.db[7] ^= (1 << 7);
		return;
	}

	if(is_a) -- a_exp;
	if(is_b) -- b_exp;

	unsigned long long a_val = Val(a); 
	unsigned long long b_val = Val(b); 	

	if(a_val == 0 || b_val == 0) {
		if(get_sign(a) != get_sign(b)) c.db[7] += (1 << 7);
		return;
	}

	unsigned long long newval = 0;
	__int128 c_val = (__int128)a_val * (__int128)b_val;
	int pos = -1, st = 104;

	for(int i = 120;i >= 0; --i) {
		if( (c_val >> i) & 1) {
			pos = i;
			break;
		}
	}

	if(!a_exp && !b_exp) {
		if(get_sign(a) != get_sign(b)) c.db[7] += (1 << 7);
		return ;
	}

	if(if_nornor == 0) {
		if(pos >= st) {
			c_exp += pos - st;
			int End = pos - 52;

			if(c_exp) newval = get_val(c_val, pos-1, End);
			else {
				++ End;
				newval = get_val(c_val, pos, End);
			}

			if( (c_val >> End-1) & 1) newval += RD_MUL(c_val, End-2, newval);
		}
		else {
			int sum = st - pos;
			c_val = (c_val << sum);
			if(c_exp > sum) {
				c_exp -= sum;
				int End = st - 52;
				newval = get_val(c_val, st-1, End);

				if(End <= st && ((c_val >> End-1) & 1)) newval += RD_MUL(c_val, End-2, newval);
			}
			else {
				int End = st - 51 + sum - c_exp;
				newval = get_val(c_val, st, End);

				if(End <= st+1 && ((c_val >> End-1) & 1)) newval += RD_MUL(c_val, End-2, newval);
				c_exp = 0;
			}
		}
	}

	else {
		if(pos >= st) {
			int sum = pos - st;
			if(sum > c_exp) {
				int End = pos - 52;

				c_exp = sum - c_exp;
				newval = get_val(c_val, pos-1, End);

				if( (c_val >> End-1) & 1) newval += RD_MUL(c_val, End-2, newval);
			}
			else {
				int End = pos - 51 + c_exp - sum;
				newval = get_val(c_val, pos, End);

				if(End <= 127 && ((c_val >> End-1) & 1)) newval += RD_MUL(c_val, End-2, newval);
				c_exp = 0;
			}
		}
		else {
			int sum = st - pos;
			c_val = (c_val << sum);
			int End = st - 51 + c_exp + sum;
			newval = get_val(c_val, st, End);

			if(End <= st+1 && ((c_val >> End-1) & 1)) newval += RD_MUL(c_val, End-2, newval);
			c_exp = 0;
		}
	}
	
	//print
	if(c_exp >= Max_val) {
		c = Infinity;
		if((get_sign(a) * get_sign(b))) c.db[7] ^= (1 << 7);
		return;
	}

	unsigned long long sigc = get_sign(a) ^ get_sign(b);
	c = restore(c_exp, newval, sigc);
	//print(c);
}

int MOL(unsigned long long a_val, unsigned long long b_val,unsigned long long newval) {
	if((((__int128)a_val) << Move) % ((__int128)b_val) != 0) return 1;
	if(newval & 1) return 1;
	return 0;
}

void Div() {
	bool is_a = 0, is_b = 0, is_nornor = 0;
	unsigned long long a_exp, b_exp, c_exp, a_val, b_val, newval = 0;

	a_exp = Exp(a), b_exp = Exp(b);

	if(!a_exp) ++ a_exp, is_a = 1;
	if(!b_exp) ++ b_exp, is_b = 1;

	if(a_exp + bias >= b_exp) c_exp = a_exp + bias - b_exp;
	else c_exp = b_exp - a_exp - bias, is_nornor = 1;

	if(is_a) -- a_exp;
	if(is_b) -- b_exp;

	a_val = Val(a), b_val = Val(b);
	__int128 c_val = (((__int128)a_val) << Move) / (__int128)b_val;
	//normalized
	if(ISZERO(a)) {
		if( (get_sign(a) ^ get_sign(b))) c.db[7] += (1 << 7);
		return;
	}
	if(ISZERO(b)) {
		c = Infinity;
		if( (get_sign(a) ^ get_sign(b))) c.db[7] += (1 << 7);
		return;
	}

	int pos = -1;
	for(int i = 126;i >= 0; --i) {
		if((c_val >> i)&1) {
			pos = i; 
			break;
		}
	}

	if(pos == -1) {
		if( (get_sign(a) ^ get_sign(b))) c.db[7] += (1 << 7);
		return;
	}

	int st = Move;
	if(is_nornor == 0) {
		if(pos >= st) {//nor
			int sum = pos - st, End = pos - 52;
			c_exp += sum;

			if(c_exp) newval = get_val(c_val, pos-1, End);
			else newval = get_val(c_val, pos, End+1);
			if(!c_exp) ++ End;

			if(End <= st+1 && (c_val >> End-1)&1) newval += MOL(a_val, b_val, newval);
		}
		else {
			int sum = st - pos;
			c_val = (c_val << sum);
			if(c_exp > sum) {//nor
				int End = st - 52;
				newval += get_val(c_val, st-1, End);

				if(End <= st+1 && ((c_val >> End-1)&1)) newval += MOL(a_val, b_val, newval);
				c_exp -= sum;
			}
			else {//denor
				int End = st - 51 + sum - c_exp;//wait to assure
				newval += get_val(c_val, st, End);

				if(End <= st+1 && ((c_val >> End-1)&1)) newval += MOL(a_val, b_val, newval);
				c_exp = 0;
			}
		}
	}
	
	else {
		if(pos >= st) {
			int sum = pos - st;
			if(sum > c_exp) { // = wait to observe
				//nor
				int End = pos - 52;
				newval = get_val(c_val, pos-1, End);

				if(End <= st+1 && ((c_val >> End-1)&1)) newval += MOL(a_val, b_val, newval);
				c_exp = sum - c_exp;  //Wait Wait Wait
			}
			else {//denor
				int End = pos - 51 + c_exp - sum;
				newval = get_val(c_val, pos, End);

				if(End <= st+1 && ((c_val >> End-1)&1)) newval += MOL(a_val, b_val, newval);
				c_exp = 0;
			}
		}
		else {//denordenor
			int sum = st - pos;
			c_val = (c_val << sum);

			int End = st - 51 + c_exp + sum;
			newval = get_val(c_val, st, End);

			if(End <= st+1 && ((c_val >> End-1)&1)) newval += MOL(a_val, b_val, newval);
			c_exp = 0;
		}
	}

	if(c_exp >= Max_val) {
		c = Infinity;
		if( (get_sign(a) ^ get_sign(b))) c.db[7] += (1 << 7);
		return;
	}
	c = restore(c_exp, newval, (get_sign(a) ^ get_sign(b)));
}


unsigned long long ROUNDING(unsigned long long Value, int pos) {
	if(pos > 53) pos = 60;
	if(pos == 0) return Value;

	unsigned long long tmp = (Value >> pos);
	if(!(Value & (1LL << (pos-1)))) return tmp;

	bool flag = 0;
	for(int i = pos-2;i >= 0; --i) {
		if(Value & (1LL << i)) {
			flag = 1;
			break;
		}
	}

	if(flag) return tmp + 1;
	if(!flag) if(Value & (1LL << pos)) return tmp+1;
	return tmp;
}

unsigned long long RD_MINUS(unsigned long long Value, int pos) {
	unsigned long long tmp = (Value >> pos);
	for(int i = pos-1;i >= 0; --i) {
		if(Value & (1LL << i)) return tmp + 1;
	}
	return tmp;
}

void Add_Add() {
	unsigned long long a_exp, b_exp;
	a_exp = Exp(a); b_exp = Exp(b);

	if(INF(a) && INF(b)) {c = Infinity; c.db[0] += 1; return;}
	if(INF(a)) {c = a; return;}
	if(INF(b)) {c = b; return;}

	int cha = b_exp - a_exp;
	__int128 a_val = (__int128) Val(a);
	__int128 b_val = (__int128) Val(b);

	a_val = (a_val << Move);
	b_val = (b_val << Move);

	unsigned long long c_exp = b_exp;
	__int128 c_val = 0;

	if(a_exp == 0 && cha) -- cha;
	if(cha) {
		if(cha < 128) a_val = (a_val >> cha);
		else a_val = 0;
	}

	c_val = b_val - a_val;

	int pos = -1;
	for(int i = 52 + Move;i >= 0; --i) {
		if( (c_val >> i) & 1) {
			pos = i; break;
		}
	}

	unsigned long long newval = 0;
	if(pos == -1) {
		c_exp = 0;
		newval = 0;
	}

	else if(c_exp >= (52 + Move - pos)) {
		int End = pos-52, sum = 52 + Move - pos;
		if(c_exp > sum) {
			c_val = (c_val << sum);
			for(int i = 52 + Move - 1;i >= Move; --i) {
				if( (c_val >> i) & 1) newval += (1LL << i - Move);
			}

			if( (c_val >> Move-1)&1) newval += RD_MUL(c_val, Move-2, newval);
			c_exp -= sum;
		}

		else {
			c_val = (c_val << sum);
			for(int i = 52 + Move;i >= Move + 1; --i) {
				if( (c_val >> i) & 1) newval += (1LL << i - Move - 1);
			}

			if( (c_val >> Move)&1) newval += RD_MUL(c_val, Move-1, newval);
			c_exp = 0;
		}
	}

	else {
		int sum = 52 + Move - pos;
		int End = pos - 51 + sum - c_exp;
		for(int i = pos;i >= End; --i) {
			if( (c_val >> i)&1) newval += (1LL << i - End);
		}
		
		if( (c_val >> End-1) & 1) newval += RD_MUL(c_val, End-2, newval);
		c_exp = 0;
	}

	c = restore(c_exp, newval, get_sign(b));
}

void Add() {
	unsigned long long a_exp = Exp(a);
	unsigned long long b_exp = Exp(b);
	comp(a_exp, b_exp);

	if(get_sign(a) != get_sign(b)) {
		Add_Add();
		return;
	}

	if(INF(a)) {c = a; return;}
	if(INF(b)) {c = b; return;}

	a_exp = Exp(a); b_exp = Exp(b);
	
	int cha = b_exp - a_exp;
	__int128 a_val = (__int128) Val(a);
	__int128 b_val = (__int128) Val(b);
	unsigned long long newval = 0, c_exp = b_exp;

	a_val = (a_val << Move), b_val = (b_val << Move);

	if(a_exp == 0 && cha != 0) -- cha;
	if(cha) {
		if(cha < 128) a_val >>= cha;
		else a_val = 0;
	}
	
	__int128 c_val = a_val + b_val;

	if(a_exp || b_exp) {
		int st = Move + 52 + 1;
		if( (c_val >> st) & 1){
			++ c_exp;

			if(c_exp >= Max_val) {
				c = Infinity;
				if(get_sign(b)) c.db[7] += (1 << 7);
				return;
			}

			int End = st - 52;
			for(int i = st - 1; i >= End; --i) {
				if( (c_val >> i) & 1) newval += (1LL << i-End);
			}
			if( (c_val >> End-1) & 1) newval += RD_MUL(c_val, End-2, newval);
		}
		else {
			-- st;
			int End = st - 52;
			for(int i = st - 1;i >= End; --i) {
				if( (c_val >> i) & 1) newval += (1LL << i-End);
			}
			if( (c_val >> End-1) & 1) newval += RD_MUL(c_val, End-2, newval);
		}
	}

	else {
		int st = Move + 52;
		if( (c_val >> st) & 1) {
			++ c_exp;
			int End = st - 52;

			for(int i = st - 1; i >= End; --i) {
				if( (c_val >> i) & 1) newval += (1LL << i-End);
			}
			if( (c_val >> End-1) & 1) newval += RD_MUL(c_val, End-2, newval);
		}

		else {
			int End = st - 52;
			for(int i = st - 1; i >= End; --i) {
				if( (c_val >> i) & 1) newval += (1LL << i-End);
			}

			if( (c_val >> End-1) & 1) newval += RD_MUL(c_val, End-2, newval);
		}
	}
	
	c = restore(c_exp, newval, get_sign(b));
}

void Init() {
	for(int i = 0;i < 8; ++i) a.db[i] = b.db[i] = c.db[i] = Infinity.db[i] = 0;
	for(int i = 4;i < 8; ++i) Infinity.db[6] += (1 << i);
	for(int i = 0;i < 7; ++i) Infinity.db[7] += (1 << i);
}

bool Is_nan(struct node T) {
	if(Exp(T) == Max_val && Val(T) != 0) return true;
	return false;
}

uint64_t bona(uint64_t ee, uint64_t ff, char op) {
	uint64_t leiji = 0;
	Init();
	for(int i = 0;i < 8; ++i) {
		for(int j = 0;j < 8; ++j) {
			if( (ee >> (j+leiji))&1) a.db[i] += (1 << j);
			if( (ff >> (j+leiji))&1) b.db[i] += (1 << j);
		} leiji += 8;
	}

	if(Is_nan(a)) return ee;
	if(Is_nan(b)) return ff;

    if (op == '+') Add();
	else if (op == '-') {
		b.db[7] ^= (1 << 7);
		Add();
	}
	else if(op == '*') Mul();
	else Div();

	leiji = 0;
	uint64_t result = 0;

	for(int i = 0;i < 8; ++i) {
		for(int j = 0;j < 8; ++j) {
			if(c.db[i] & (1 << j)) result += (1LL << (uint64_t)(j + leiji));
		} leiji += 8;
	}

    return result;
}
