#include "../s21_decimal.h"

// returns 1 if dec1 < dec2, else 0
int s21_is_less(s21_decimal dec1, s21_decimal dec2) {
  return !s21_is_greater_or_equal(dec1, dec2);
}