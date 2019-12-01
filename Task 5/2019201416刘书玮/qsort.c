#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<math.h>
#define ULL unsigned long long
ULL _Rand_Seed_1 = 137, _Rand_Seed_2 = 139;
void _Rand_Start(){
    int i = 100;
    while(i-- >= 0){
        _Rand_Seed_1 = _Rand_Seed_1 * _Rand_Seed_1;
        _Rand_Seed_2 = _Rand_Seed_2 * _Rand_Seed_1;
    }
}
ULL _Rand_Rand(ULL Mod){
    _Rand_Seed_1 = _Rand_Seed_1 * _Rand_Seed_1;
    _Rand_Seed_2 = _Rand_Seed_2 * _Rand_Seed_1;
    return _Rand_Seed_2 * _Rand_Seed_1 % Mod;
}
ULL _Main_Space[100000];
void _Qsort_Swap(void *_First, void *_Second, size_t _Size)
{
    memcpy(_Main_Space, _Second, _Size);
    memcpy(_Second, _First, _Size);
    memcpy(_First, _Main_Space, _Size);
}
void _Qsort_Insert_Sort(void *_Base, size_t _Number, size_t _Size, int (*compar)(const void *, const void *))
{
    int i,j,k;
    for(i = 1; i < _Number; ++ i){
        for(j = 0; j < i; ++ j){
            if(compar(_Base + _Size * i, _Base + _Size * j) <= 0){
                memcpy(_Main_Space, _Base + i * _Size, _Size);
                memmove(_Base + (j + 1) * _Size, _Base + j * _Size, _Size * (i - j ));
                memcpy(_Base + j * _Size, _Main_Space, _Size);
                break;
            }
        }
    }
}
int _Qsort_Bulb_Sort(void *_Base, size_t _Number, size_t _Size, int (*compar)(const void *, const void *), int _Times){
    int i,j,cnt;
    for(i = 0; i + i< _Number; ++i){
        if(compar(_Base + _Size * i, _Base + _Size * (_Number - i - 1)) > 0)
            _Qsort_Swap(_Base + _Size * i, _Base + _Size * (_Number - i - 1), _Size);
    }
    for(i = 0; i < _Times; ++i){
        cnt = 0;
        for(j = 0; j < _Number - 1; ++j){
            if(compar(_Base + _Size * j, _Base + _Size * j + _Size) > 0){
                _Qsort_Swap(_Base + _Size * j, _Base + _Size * j + _Size, _Size);
                cnt ++;
            }
        }
        if(!cnt) break;
    }
    return i;
}
void _Qsort_Quick_Sort(void *_Base, size_t _Number, size_t _Size, int (*compar)(const void *, const void *),int _Depth)
{
    if(_Number <= 1) return;
    if(_Number <= 32){
        _Qsort_Insert_Sort(_Base, _Number, _Size, compar);
        return;
    }

    void *_Point_1 = _Base;
    void *_Point_2 = _Base + (_Number - 1) * _Size;
    _Qsort_Swap(_Base, _Base + _Rand_Rand(_Number) * _Size, _Size);
    int tot=0;
    while(_Point_1 != _Point_2){
        int tmp;
        while(_Point_1 != _Point_2){
            tmp = compar(_Point_1, _Point_2);
            tot += (tmp == 0);
            if(tmp > 0){
                _Qsort_Swap(_Point_1, _Point_2, _Size);
                break;
            }
            _Point_2 -= _Size;
        }
        while(_Point_1 != _Point_2){
            tmp = compar(_Point_1, _Point_2);
            tot += (tmp == 0);
            if(tmp >= 0){
                _Qsort_Swap(_Point_1, _Point_2, _Size);
                break;
            }
            _Point_1 += _Size;
        }
    }
    if(tot + 1 == _Number) return;
    _Qsort_Quick_Sort(_Base, ((ULL)_Point_1 - (ULL)_Base) / _Size, _Size, compar, _Depth + 1);
    _Qsort_Quick_Sort(_Point_1 + _Size, _Number - ((ULL)_Point_1 - (ULL)_Base) / _Size - 1, _Size, compar, _Depth + 1);
}
void _Qsort(void *_Base, size_t _Number, size_t _Size, int (*compar)(const void *, const void *),int _Depth){
    if(_Qsort_Bulb_Sort(_Base, _Number, _Size, compar, 1) < 1) return;
    _Qsort_Quick_Sort(_Base, _Number, _Size, compar, 0);
}
int main(){
}
