#ifndef _GETKEY_H_
#define _GETKEY_H_

typedef struct{
	int press;
	int count;
	int flg;
	int toggle;
}Button;

extern Button up;
extern Button down;
extern Button right;
extern Button left;
extern Button shift;
extern Button enter;
extern Button control;
extern Button tab;
extern Button del;
extern Button alt;
extern Button backspace;
extern Button esc;
extern Button key_q;
extern Button key_a;
extern Button key_z;
extern Button key_w;
extern Button key_s;
extern Button key_x;
extern Button key_e;
extern Button key_d;
extern Button key_c;
extern Button key_r;
extern Button key_f;
extern Button key_v;
extern Button key_t;
extern Button key_g;
extern Button key_b;
extern Button key_y;
extern Button key_h;
extern Button key_n;
extern Button key_u;
extern Button key_j;
extern Button key_m;
extern Button key_i;
extern Button key_k;
extern Button key_o;
extern Button key_l;
extern Button key_p;
extern Button one;
extern Button two;
extern Button three;
extern Button four;
extern Button five;
extern Button six;
extern Button seven;
extern Button eight;
extern Button nine;
extern Button zero;

void getKey(char *keyStat);
int pushCount(Button *any);
int is(int n);

#endif