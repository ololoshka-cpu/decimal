#include "../s21_decimal.h"

int s21_round(s21_decimal src, s21_decimal *dest) {
  int res = 1;
  if (dest != NULL) {
    res = 0;
    set_null(dest);
    int exp = get_exp(&src);
    while (exp > 1) {
      dec_div_10(&src);
      exp--;
    }
    if (exp) {
      int mod = dec_div_10(&src);
      if (mod >= 5) {
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