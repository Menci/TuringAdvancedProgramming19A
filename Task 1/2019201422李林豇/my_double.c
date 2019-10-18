#include<stdio.h>
#include<math.h>
#include<string.h>
#include <stdint.h>  // uint64_t
#include <stdbool.h> // bool

#define init_varible() unsigned char _sign_1, _sign_2, _sign;int _exp_1, _exp_2, _exp;u_ll _frac_1, _frac_2, _frac;unsigned char _type_1, _type_2;int _bit_shift;struct big_int big_frac_1, big_frac_2, big_frac
#define lowbit(x) ((x)&(-(x)))
#define num_of_bytes  8
#define len_of_bytes  8
#define num_of_big_int 2

typedef unsigned long long u_ll;

const unsigned int _bias=1023;
const unsigned int _int_bit = 32;
const unsigned int _ll_bit = 64;
const unsigned int _sign_bit = 1;
const unsigned int _exp_bit = 11;
const unsigned int _frac_bit = 52;
const unsigned int _exp_frac_bit = 63;  //_exp_bit + _frac_bit;
const unsigned int _is_inf = 1;
const unsigned int _is_nan = 2;
const unsigned int _is_zero = 4;
const unsigned int _is_nothing = 0;
const unsigned int _exp_max =(1LL << 11) - 1; //(1LL << _exp_bit) - 1
const unsigned int char_max = (1 << 8) - 1;//(1 << len_of_bytes) - 1;

const u_ll _frac_max = (1LL << 52) - 1;//(1LL << _frac_bit) - 1
const u_ll _int_max = (1LL << 32) - 1;//(1LL << _int_bit) - 1
const u_ll _ll_max = -1;

int pre_log2[1<<len_of_bytes];

//pre_log2[i] means the result of log2(i)


struct big_int{
    u_ll _bitset[num_of_big_int];
};

u_ll _tmp;

//big_int are a interger larger than long long
//it ranges from 0 to 2^(_ll_bit * num_of_big_int)-1
//use big_int to calculate frac of double

unsigned char comp_big_int(struct big_int _X,struct big_int _Y)
{
    for (char i = num_of_big_int-1; i >=0 ; i--)
        if(_X._bitset[i]^_Y._bitset[i])
            return _X._bitset[i] > _Y._bitset[i];
}

//find the bigger big_int
//if ther first one is lager, it return 1, else return 0

struct big_int  u_ll_to_big_int(u_ll _Y)
{
    struct big_int _X;
    _X._bitset[0] = _Y;
    for (char i = 1; i < num_of_big_int;i++)
        _X._bitset[i] = 0;
    return _X;
}

//change unsigned long long into big_int

void left_shift_big_int(u_ll _bitset[], int _K)
{
    int _delta = _K / _ll_bit;
    _K -= _delta * _ll_bit;
    for (char i = num_of_big_int - 1; i > _delta; i--)
    {
        _bitset[i] = _bitset[i - _delta] << _K;
        if(_K)
            _bitset[i] |= _bitset[i - _delta - 1] >> (_ll_bit - _K);
    }
    _bitset[_delta] = _bitset[0]<<_K;
    for (char i = _delta - 1; i >= 0; i--)
        _bitset[i] = 0;
}

//left shift big_int

void right_shift_big_int(u_ll _bitset[], int _K)
{
    int _delta = _K / _ll_bit;
    _K -= _delta * _ll_bit;
    _tmp = (1LL << _K) - 1;
    for (char i = 0; i < num_of_big_int-_delta-1; i++)
    {
        _bitset[i] = _bitset[i + _delta] >> _K;
        _bitset[i] |= (_bitset[i + _delta+1]&_tmp) << (_ll_bit - _K);
    }
    _bitset[num_of_big_int-_delta-1] = _bitset[num_of_big_int-1] >> _K;
    for (char i = num_of_big_int-_delta; i <num_of_big_int; i++)
        _bitset[i] = 0;
}

//right shift big_int

void pre_find_log2()
{
    pre_log2[1] = 0;
    for (int i = 2; i <= char_max;i++)
        pre_log2[i] = pre_log2[i >> 1] + 1;
}

//find array pre_log2[]

int find_log2(u_ll _bitset[])
{
    for (int i = num_of_big_int - 1; i >= 0;i--)
        if(_bitset[i])
        {
            unsigned char *_pointer = (unsigned char *) &_bitset[i];
            for (char j = num_of_bytes - 1; j >= 0; j--)
                if(_pointer[j])
                    return i * _ll_bit + j * len_of_bytes + pre_log2[_pointer[j]];
        }   
    return -1;
}

//find log2(X),when X is a big_int

void carry_big_int(u_ll _bitset[], int _K)
{
    if(!(_K^num_of_big_int))
        return;
    if(!(_bitset[_K]^_ll_max))
        carry_big_int(_bitset, _K + 1);
    ++_bitset[_K];
    return;
}

//plus 1 on the _Kth bit of a big_int

void discarry_big_int(u_ll _bitset[],int _K)
{
    if(!(_K^num_of_big_int))
        return;
    if(_bitset[_K])
        discarry_big_int(_bitset, _K + 1);
    _bitset[_K]--;
    return;
}

//minus 1 on the _Kth bit of a big_int

void big_int_add_u_ll(u_ll _bitset[],int _K,u_ll _Y)
{
    if(_bitset[_K]+_Y<_Y)
        carry_big_int(_bitset, _K + 1);
    _bitset[_K] += _Y;
    return; 
}

//plus _Y on the _Kth bit of a big_int(_Y is unsigned long long)

void big_int_minus_u_ll(u_ll _bitset[],int _K,u_ll _Y)
{
    if(_bitset[_K]-_Y>_bitset[_K])
        discarry_big_int(_bitset, _K + 1);
    _bitset[_K] -= _Y;
    return;
}

//plus _Y on the _Kth bit of a big_int(_Y is unsigned long long)

int if_big_int_carry(u_ll _bitset[],int _K,int remain)
{
    _K--;
    int _pos = _K / _ll_bit;
    _K -= _pos * _ll_bit;
    u_ll _max = (1LL << _K) | ((1LL << _K) - 1),this_pos;
    this_pos = _bitset[_pos] & _max;
    if(!(this_pos>>_K))
        return 0;
    if(this_pos^lowbit(this_pos))
        return 1;
    for (int i = 0; i < _pos;i++)
        if(_bitset[i])
            return 1;
    if(!remain)
        return 2;
    return 1;
}

//if a big_int should carry,it returns 1
//if a big_int should carry,it returns 0
//if it is up to the lowbit of big_int, it returns 2

unsigned char if_big_int_zero(struct big_int _X)
{
    for(char i = 0; i < num_of_big_int;i++)
        if(_X._bitset[i])
            return 0;
    return 1;
}

//if a big_int is zero,return 1 
// else return 0

int reserve_digit_big_int(u_ll _bitset[], int _K,int remain)
{
    int _log2 = find_log2(_bitset);
    if(_K<0)
    {
        for(int i=0;i<num_of_big_int;i++)
        {
            if(_bitset[i])
                break;
            if(i==num_of_big_int-1)
                return 0;
        }
        _bitset[0]=1;
        _bitset[1]=0;
        return _log2+1;
    }
    if(_K==_log2)
        return 0;
    if(_log2<_K)
    {
        left_shift_big_int(_bitset, _K - _log2);
        return _log2 - _K;
    }
    int _flag = if_big_int_carry(_bitset, _log2 - _K, remain);
    right_shift_big_int(_bitset, _log2 - _K);
    if(_flag&1)
    {
        big_int_add_u_ll(_bitset, 0, 1);
        if(find_log2(_bitset) == _K)
            return _log2 - _K;
        right_shift_big_int(_bitset, 1);
        return _log2 - _K + 1;
    }
    if(_flag&2&&(_bitset[0]&1))
    {
        big_int_add_u_ll(_bitset, 0, 1);
        if(find_log2(_bitset) == _frac_bit)
            return _log2 - _K;
        right_shift_big_int(_bitset, 1);
        return _log2 - _K + 1;
    }
    return _log2 - _K;
}

//round a big_int to _K+1 bits left, and return the number of bits lost

void big_int_add_pow2(u_ll _bitset[],int _K)
{
    int _pos =_K / _ll_bit;
    _K -= _pos * _ll_bit;
    _bitset[_pos] |= 1LL << _K;
} 

//plus 2^_K to a big_int

struct big_int  add_big_int(struct big_int _X,struct big_int _Y)
{
    for (int i = 0; i < num_of_big_int;i++)
        big_int_add_u_ll(_X._bitset, i, _Y._bitset[i]);
    return _X;
}

//return _X + _Y (two big_int)

struct big_int  minus_big_int(struct big_int _X,struct big_int _Y)
{
    for (int i = 0; i < num_of_big_int;i++)
        big_int_minus_u_ll(_X._bitset, i, _Y._bitset[i]);
    return _X;
}

//return _X - _Y (two big_int)

struct big_int times_big_int(struct big_int _X,struct big_int _Y)
{
    __int128 _A, _B;
    struct big_int _ANS;
    memset(_ANS._bitset, 0, sizeof(_ANS));

    for (int i = 0; i <num_of_big_int;i++)
        if(_X._bitset[i])
            for (int j = 0; i + j < num_of_big_int;j++)
                if(_Y._bitset[j])
                {
                    _A = _X._bitset[i];
                    _B = _Y._bitset[j];
                    _A = _A * _B;
                    _B = _A & _ll_max;
                    _A >>= _ll_bit;
                    big_int_add_u_ll(_ANS._bitset, i + j, _B);
                    if(i+j+1!=num_of_big_int)
                        big_int_add_u_ll(_ANS._bitset, i + j + 1, _A);
                }
    return _ANS;
}

//return _X * _Y (two big_int)

u_ll _remain;

//the remainder of divide

struct big_int divide_big_int(struct big_int _X,struct big_int _Y)
{
    struct big_int _ANS;
    memset(_ANS._bitset,0,sizeof(_ANS));
    left_shift_big_int(_Y._bitset , _ll_bit+10);
    
    for(int i = _ll_bit+10; i >= 0;i--)
    {
        if(comp_big_int(_X,_Y))
        {
            _X=minus_big_int(_X,_Y);
            big_int_add_pow2(_ANS._bitset, i);
        }
        right_shift_big_int(_Y._bitset,1);
    }
    _remain=_X._bitset[0];
    return _ANS;
}

//return _X / _Y (two big_int)

struct my_double{
    unsigned char _bitset[num_of_bytes];
};

// the new double

u_ll my_double_to_u_ll(struct my_double _X)
{
    u_ll _tmp;
    unsigned char *_pointer = (unsigned char *) &_tmp;
    for (int i = 0; i < num_of_bytes;i++)
        _pointer[i] = _X._bitset[i];
    return _tmp;
}

// change my_double to a interger byte by byte

struct my_double u_ll_to_my_double(u_ll _ULL)
{
    struct my_double _X;
    unsigned char *_pointer = (unsigned char *) &_ULL;
    for (int i = 0; i < num_of_bytes;i++)
        _X._bitset[i] = _pointer[i];
    return _X;
}

// change  a interger to my_double  byte by byte

struct my_double make_my_double(u_ll _sign,u_ll _exp,u_ll _frac)
{
    return u_ll_to_my_double(_sign << _exp_frac_bit | _exp << _frac_bit | _frac);
}

// use the 3 parts of double in IEEE to make my_double 

u_ll find_sign(struct my_double _X)
{
    _tmp = my_double_to_u_ll(_X);
    return _tmp >> _exp_frac_bit;
}

//return the sign of my_double

u_ll find_exp(struct my_double _X)
{
    _tmp = my_double_to_u_ll(_X);
    return (_tmp >> _exp_frac_bit) & 1 ? (_tmp ^ (1LL << _exp_frac_bit) ) >> _frac_bit : _tmp >> _frac_bit;
}

//return the exponent of my_double

u_ll find_frac(struct my_double _X)
{
    _tmp = my_double_to_u_ll(_X);
    return _tmp & _frac_max;
}

//return the mantissa of my_double

unsigned char is_special(struct my_double _X)
{
    unsigned int _exp;
    u_ll _frac;
    _exp = find_exp(_X);
    _frac = find_frac(_X);
    if (_exp ^ _exp_max)
    {
        if(!_exp&&!_frac)
            return _is_zero;
        return _is_nothing;
    }
    if(_frac)
        return _is_nan;
    return _is_inf;
}

//jugde whether the big_int is nan,inf,zero or nothing special

unsigned char comp_my_double(struct my_double _X,struct my_double _Y)
{
    u_ll _frac_1, _frac_2;
    unsigned int _exp_1, _exp_2;

    _exp_1 = find_exp(_X);   _exp_2 = find_exp(_Y);
    _frac_1 = find_frac(_X);   _frac_2 = find_frac(_Y);
    
    if(_exp_1!=_exp_2)
        return _exp_1 > _exp_2;
    if(_frac_1!=_frac_2)
        return _frac_1 > _frac_2;
    return 2;
}

//to compare two big_int _X and _Y

struct my_double add_my_double(struct my_double _X,struct my_double _Y)
{
    init_varible();

    if(!comp_my_double(_X,_Y))
    {
        struct my_double __TMP;
        __TMP = _X;
        _X = _Y;
        _Y = __TMP;
    }

    _sign_1 = find_sign(_X);   _sign_2 = find_sign(_Y); 
    _type_1 = is_special(_X);   _type_2 = is_special(_Y);
    _frac_1 = find_frac(_X);   _frac_2 = find_frac(_Y);
    _exp_1 = find_exp(_X);   _exp_2 = find_exp(_Y);
   
    if(_exp_1)
        _frac_1 |= (1LL << _frac_bit);
    else
        _exp_1++;
    if(_exp_2)
        _frac_2 |= (1LL << _frac_bit);
    else
        _exp_2++;
    _exp = _exp_2;
    big_frac_1 = u_ll_to_big_int(_frac_1);
    big_frac_2 = u_ll_to_big_int(_frac_2);

    if (((_type_1 | _type_2) & _is_nan) == _is_nan)
        return _type_1 == _is_nan ? _X : _Y;
    
    _bit_shift = _exp_1 - _exp_2;
    
    _sign = _sign_1;
    if (_bit_shift > _frac_bit + 1)
        return _X;

    if(_sign_1^_sign_2)
    {
        if ((_type_1 && _type_2) == _is_inf)
            return make_my_double(0, _exp_max, _frac_max);
        
        left_shift_big_int(big_frac_1._bitset,_bit_shift);
        big_frac = minus_big_int(big_frac_1,big_frac_2);

        if(if_big_int_zero(big_frac))
            return make_my_double(0, 0, 0);
        struct big_int _TMP=big_frac;
        _exp += reserve_digit_big_int(big_frac._bitset,_frac_bit,0);
        _frac = big_frac._bitset[0] ;

        if(_exp +(int)_frac_bit< 0)
            return make_my_double(_sign, 0, 0);
        if(_exp <= 0)
        {
            int _log2=find_log2(_TMP._bitset);
            if(reserve_digit_big_int(_TMP._bitset,_frac_bit+_exp-1,0)>_log2-(_frac_bit+_exp-1))
                left_shift_big_int(_TMP._bitset,1);
            _frac=_TMP._bitset[0];
            return make_my_double(_sign, 0, _frac);
        }
        if(_exp>=(int)_exp_max)
            return make_my_double(_sign, _exp_max, 0);

        _frac ^= (1LL << _frac_bit);
        return make_my_double(_sign, _exp, _frac);
    }
    else
    {
        if ((_type_1 && _type_2) == _is_inf)
            return make_my_double(_sign_1, _exp_max, 0);
        
        left_shift_big_int(big_frac_1._bitset, _bit_shift);
        big_frac = add_big_int(big_frac_1, big_frac_2);
        struct big_int _TMP=big_frac;
        _exp += reserve_digit_big_int(big_frac._bitset, _frac_bit,0);
        _frac = big_frac._bitset[0];

        if(_exp>=(int)_exp_max)
            return make_my_double(_sign, _exp_max, 0);
            
        if(_exp +(int)_frac_bit< 0)
            return make_my_double(_sign, 0, 0);

        if(_exp <= 0)
        {
            int _log2=find_log2(_TMP._bitset);
            if(reserve_digit_big_int(_TMP._bitset,_frac_bit+_exp-1,0)>_log2-(_frac_bit+_exp-1))
                left_shift_big_int(_TMP._bitset,1);
            _frac=_TMP._bitset[0];
            return make_my_double(_sign, 0, _frac);
        }

        _frac ^= (1LL << _frac_bit);
        return make_my_double(_sign, _exp, _frac);
    }
}

// return _X + _Y (my_double)

struct my_double minus_my_double(struct my_double _X,struct my_double _Y)
{
    _Y._bitset[num_of_bytes-1] ^= (1LL << (len_of_bytes-1));
    return add_my_double(_X, _Y);
}

// return _X - _Y (my_double)

struct my_double times_my_double(struct my_double _X,struct my_double _Y)
{
    init_varible();

    _sign_1 = find_sign(_X);   _sign_2 = find_sign(_Y); 
    _type_1 = is_special(_X);   _type_2 = is_special(_Y);
    _frac_1 = find_frac(_X);   _frac_2 = find_frac(_Y);
    _exp_1 = find_exp(_X);   _exp_2 = find_exp(_Y);
    
    if(_exp_1)
        _frac_1 |= (1LL << _frac_bit);
    else
        _exp_1++;
    if(_exp_2)
        _frac_2 |= (1LL << _frac_bit);
    else
        _exp_2++;
    big_frac_1 = u_ll_to_big_int(_frac_1);
    big_frac_2 = u_ll_to_big_int(_frac_2);
    _sign = _sign_1 ^ _sign_2;
    if((_type_1|_type_2)&_is_nan)
        return _type_1 & _is_nan ? _X : _Y;
    
    if(!((_type_1|_type_2)^(_is_inf|_is_zero)))
    {
        _sign = _sign_1^_sign_2;
        return make_my_double(_sign, _exp_max, _frac_max);
    }

    if((_type_1|_type_2)&_is_zero)
    {
        _sign=_sign_1^_sign_2;
        return make_my_double(_sign, 0, 0);
    }
    big_frac = times_big_int(big_frac_1,big_frac_2);
    struct big_int _TMP=big_frac;
    int debug=reserve_digit_big_int(big_frac._bitset,_frac_bit,0);
    _exp = _exp_1 + _exp_2 - _bias + debug - _frac_bit;

    if(_exp>=(int)_exp_max)
        return make_my_double(_sign, _exp_max, 0);

    if(_exp + (int)_frac_bit< 0)
        return make_my_double(_sign, 0, 0);
    if(_exp <= 0)
    {
        int _log2=find_log2(_TMP._bitset);
        if(reserve_digit_big_int(_TMP._bitset,(int)_frac_bit+_exp-1,0)>_log2-(_frac_bit+_exp-1))
            left_shift_big_int(_TMP._bitset,1);
        _frac=_TMP._bitset[0];
        return make_my_double(_sign, 0, _frac);
    }
    _frac=big_frac._bitset[0];
    _frac ^= 1LL << _frac_bit ; 
    return make_my_double(_sign, _exp, _frac);
}

// return _X * _Y (my_double)

struct my_double divide_my_double(struct my_double _X,struct my_double _Y)
{
    init_varible();

    _sign_1 = find_sign(_X);   _sign_2 = find_sign(_Y); 
    _type_1 = is_special(_X);   _type_2 = is_special(_Y);
    _frac_1 = find_frac(_X);   _frac_2 = find_frac(_Y);
    _exp_1 = find_exp(_X);   _exp_2 = find_exp(_Y);
    _sign=_sign_1^_sign_2;
    if(_exp_1)
        _frac_1 |= (1LL << _frac_bit);
    else
        _exp_1++;
    if(_exp_2)
        _frac_2 |= (1LL << _frac_bit);
    else
        _exp_2++;
    
    big_frac_1 = u_ll_to_big_int(_frac_1);
    big_frac_2 = u_ll_to_big_int(_frac_2);

    if((_type_1|_type_2)&_is_nan)
        return _type_1 & _is_nan ? _X : _Y;
    if(_type_1==_is_zero&&_type_2==_is_zero)
        return make_my_double(_sign, _exp_max, _frac_max);
    if(_type_2==_is_zero)
        return make_my_double(_sign, _exp_max, 0);
    if(_type_1==_is_zero)
        return make_my_double(_sign, 0, 0);
    if((_type_1&_type_2)==_is_inf)
        return make_my_double(_sign, _exp_max, _frac_max);
    
    left_shift_big_int(big_frac_1._bitset, _ll_bit+10);
    int _tmp=reserve_digit_big_int(big_frac_2._bitset,_frac_bit,0);
    big_frac = divide_big_int(big_frac_1,big_frac_2);
    struct big_int _TMP=big_frac;
    _exp = _exp_1 - _exp_2 + _bias + reserve_digit_big_int(big_frac._bitset,_frac_bit,_remain) - _ll_bit-10 + _frac_bit - _tmp;

    if(_exp>=(int)_exp_max)
        return make_my_double(_sign, _exp_max, 0);
    if(_exp +(int)_frac_bit < 0)
        return make_my_double(_sign, 0, 0);

    if(_exp <= 0)
    {
        int _log2=find_log2(_TMP._bitset);
        if(reserve_digit_big_int(_TMP._bitset,_frac_bit+_exp-1,0)>_log2-(_frac_bit+_exp-1))
            left_shift_big_int(_TMP._bitset,1);
        _frac=_TMP._bitset[0];
        return make_my_double(_sign, 0, _frac);
    }

    _frac = big_frac._bitset[0];
    _frac ^= 1LL << _frac_bit; 
    return make_my_double(_sign, _exp, _frac);
}

// return _X / _Y (my_double)

struct my_double convert_to_my_double(uint64_t a)
{
    struct my_double _X;
    unsigned char *_pointer = (unsigned char *) &a;
    for (int i = 0; i < num_of_bytes;i++)
       _X._bitset[i] =  _pointer[i];
    return _X;
}

uint64_t convert_from_my_double(struct my_double _X)
{
    uint64_t a;
    unsigned char *_pointer = (unsigned char *) &a;
    for (int i = 0; i < num_of_bytes;i++)
        _pointer[i] = _X._bitset[i] ;
    return a;
}

uint64_t your_calculate_function(uint64_t a, uint64_t b, char op) {
    static bool initialized = false;
    if (!initialized) {
        pre_find_log2();
        initialized = true;
    }

    struct my_double x = convert_to_my_double(a);
    struct my_double y = convert_to_my_double(b);
    struct my_double result;
    if (op == '+') result = add_my_double(x, y);
    if (op == '-') result = minus_my_double(x, y);
    if (op == '*') result = times_my_double(x, y);
    if (op == '/') result = divide_my_double(x, y);
    return convert_from_my_double(result);
}
int main()
{
    uint64_t a=0x0001e5453fa35235LL,b=0x800926caddc7dedeLL;
    char op='-';
    uint64_t tmp=your_calculate_function(a,b,op);
    printf("%.8le",*(double*)&tmp);
    return 0;
}
/*
1100001010
*/