#include "../s21_decimal.h"

int get_float_exp(unsigned int);
void set_float_to_decimal(unsigned int, int, s21_decimal *);
void set_exp10(int, s21_decimal *);

//================================================================================

int s21_from_float_to_decimal(float src, s21_decimal *dest) {
  int res = 0;
  set_null(dest);
  unsigned int num = *(unsigned int *)&src;
  int exp = get_float_exp(num);

  set_sign(src < 0, dest);
  src = fabsf(src);
  if (!(-96 < exp && exp < 96)) {
    res = 1;
  } else {
    int exp10 = 0;
    while (src < 1e+6 && exp10 < 28) {
      src *= 10;
      exp10++;
    }
    if (exp10) {
      if ((int)src) {
        src = (int)src;
      } else {
        src = roundf(src);
      }
      while (fmodf(src, 10) == 0) {
        src /= 10;
        exp10--;
      }
    }
    num = *(unsigned int *)&src;
    exp = get_float_exp(num);
    set_float_to_decimal(num, exp, dest);
    set_exp10(exp10, dest);
  }
  return res;
}

//==================================================================================

//==================================================================================

int get_float_exp(unsigned int src) { return ((src & 0x7FF00000) >> 23) - 127; }

void set_float_to_decimal(unsigned int src, int exp, s21_decimal *dest) {
  int temp = 1 << 23;
  temp |= src & 0x7FFFFF;
  dest->bits[0] = temp;
  for (int i = 0; i < abs(exp - 23); i++) {
    exp < 23 ? shift_right(dest) : shift_left(dest);
  }
}

void set_exp10(int exp, s21_decimal *dest) { dest->bits[3] |= (exp << 16); }