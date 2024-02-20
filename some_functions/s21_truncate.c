#include "../s21_decimal.h"

int s21_truncate(s21_decimal src, s21_decimal *dest) {
  int res = 1;
  if (dest != NULL) {
    res = 0;
    set_null(dest);
    copy_decimal(&src, dest);
    int exp = get_exp(dest);
    while (exp) {
      dec_div_10(dest);
      set_exp(--exp, dest);
    }
  }
  return res;
}