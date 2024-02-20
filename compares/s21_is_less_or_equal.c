#include "../s21_decimal.h"

// return 1 if de1 <= dec2, else 0
int s21_is_less_or_equal(s21_decimal dec1, s21_decimal dec2) {
  return !s21_is_greater(dec1, dec2);
}