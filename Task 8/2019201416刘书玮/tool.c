#define CHAR_MAX 255
void _add(char *_out_base, int place, char data)
{
	*( _out_base + place / 8) &= CHAR_MAX - (1 << (7 - place % 8));
	if(data) *( _out_base + place / 8) |= 1 << (7 - place % 8);
}
int get_val(char *_base, int place)
{
	if( *(_base + place / 8) & (1 << (7 - (place & 7)))) return 1;
	return 0;
}
