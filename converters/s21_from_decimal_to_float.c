#include "../s21_decimal.h"

void print_float(float);

int s21_from_decimal_to_float(s21_decimal src, float *dest) {
  *dest = 0;
  int sign = get_sign(&src);
  for (int i = 0; i < 96; i++) {
    *dest += get_bit(i, &src) * pow(2, i);
  }
  *dest /= pow(10, get_exp(&src));
  if (sign) {
    *dest *= -1;
  }
  return 0;
}