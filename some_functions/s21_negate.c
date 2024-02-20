#include "../s21_decimal.h"

int s21_negate(s21_decimal src, s21_decimal *dest) {
  int res = 1;
  if (dest != NULL) {
    res = 0;
    set_null(dest);
    copy_decimal(&src, dest);
    set_sign(!get_sign(dest), dest);
  }
  return res;
}
