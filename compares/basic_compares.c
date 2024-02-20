#include "../s21_decimal.h"

// returns 0, if mantissa1 == mantissa2, 1 if mantissa1 > mantissa2, else 2
int compare_mantissa(s21_decimal *dec1, s21_decimal *dec2) {
  int res = 0;
  int ptr1 = 0;
  int ptr2 = 0;
  for (int i = 95; i >= 0 && !res; i--) {
    ptr1 = get_bit(i, dec1);
    ptr2 = get_bit(i, dec2);
    if (ptr1 > ptr2) {
      res = 1;
    } else if (ptr1 < ptr2) {
      res = 2;
    }
  }
  return res;
}

int null_mantissa(s21_decimal *dec) {
  return !(!!dec->bits[0] + !!dec->bits[1] + !!dec->bits[2]);
}