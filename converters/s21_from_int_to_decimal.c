#include "../s21_decimal.h"

void set_int_to_decimal(int, s21_decimal *);

//===================================================================

int s21_from_int_to_decimal(int src, s21_decimal *dest) {
  int sign = 0;
  set_null(dest);
  if (src < 0) {
    sign = 1;
    if (src != -2147483648) {
      src *= -1;
    }
  }
  set_sign(sign, dest);
  set_int_to_decimal(src, dest);
  return 0;
}

//===================================================================

//===================================================================

void set_int_to_decimal(int src, s21_decimal *dest) { dest->bits[0] = src; }