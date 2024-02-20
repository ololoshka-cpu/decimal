#include "../s21_decimal.h"

int s21_floor(s21_decimal src, s21_decimal *dest) {
  int res = 1;
  if (dest != NULL) {
    res = 0;
    int exp = get_exp(&src);
    set_null(dest);
    int mod = 0;
    while (exp > 1) {
      mod += dec_div_10(&src);
      exp--;
    }
    if (exp) {
      mod += dec_div_10(&src);
      if (get_sign(&src) && mod) {
        s21_decimal temp = {0};
        temp.bits[0] = 1;
        sum_mantissa(&src, &temp, dest);
        set_exp(get_exp(&src), dest);
        set_sign(get_sign(&src), dest);
      } else {
        copy_decimal(&src, dest);
      }
    }
  }
  return res;
}