#include "../s21_decimal.h"

// return 1 - TRUE, 0 - FALSE

int s21_is_equal(s21_decimal dec1, s21_decimal dec2) {
  int res = 0;
  if (null_mantissa(&dec1) && null_mantissa(&dec2)) {
    res = 1;
  } else if (get_sign(&dec1) == get_sign(&dec2)) {
    decimal_alignment(&dec1, &dec2);
    res = !compare_mantissa(&dec1, &dec2);
  }
  return res;
}