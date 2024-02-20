#include "../s21_decimal.h"

int s21_from_decimal_to_int(s21_decimal src, int *dest) {
  int res = 0;
  *dest = 0;
  int exp = get_exp(&src);
  while (exp != 0) {
    dec_div_10(&src);
    exp--;
  }
  if (src.bits[1] == 0 && src.bits[2] == 0) {
    if (get_bit(31, &src) == 1 && (get_sign(&src) == 0)) {
      res = 1;
    } else {
      *dest = src.bits[0];
      if (get_sign(&src)) {
        *dest *= -1;
      }
    }
  } else {
    res = 1;
  }
  return res;
}