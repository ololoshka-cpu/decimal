#include "../s21_decimal.h"

// returns 1 if dec1 > dec2, else 0;
int s21_is_greater(s21_decimal dec1, s21_decimal dec2) {
  int res = 0;
  int cmp = 0;
  int sign1 = get_sign(&dec1);
  int sign2 = get_sign(&dec2);
  if (sign2 > sign1 && !null_mantissa(&dec1)) {
    res = 1;
  } else if (sign1 == sign2) {
    decimal_alignment(&dec1, &dec2);
    cmp = compare_mantissa(&dec1, &dec2);
    if ((sign1 && cmp == 2) || (!sign1 && cmp == 1)) {
      res = 1;
    }
  }
  return res;
}