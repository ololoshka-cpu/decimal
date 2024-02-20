#include "../s21_decimal.h"

// returns (number % 10)
int dec_div_10(s21_decimal *src) {
  int tmp = 0;
  s21_decimal res = {0};
  set_exp(get_exp(src) - 1, &res);
  int sign = get_sign(src);
  for (int i = 95; i >= 0; i--) {
    tmp *= 2;
    shift_left(&res);
    tmp += get_bit(i, src);
    if (tmp > 9) {
      res.bits[0] += 1;
      tmp -= 10;
    }
  }
  set_null(src);
  copy_decimal(&res, src);
  set_sign(sign, src);
  return tmp;
}

// returns overflow information
int dec_mul_10(s21_decimal *src) {
  int overflow = !!(src->bits[2] & 0xE0000000);
  if (!overflow) {
    int sign = get_sign(src);
    s21_decimal dec8 = {0};
    s21_decimal dec2 = {0};
    s21_decimal res = {0};
    copy_decimal(src, &dec8);
    copy_decimal(src, &dec2);
    shift_left(&dec8);
    shift_left(&dec8);
    shift_left(&dec8);
    shift_left(&dec2);
    overflow = sum_mantissa(&dec8, &dec2, &res);
    int exp = get_exp(src);
    if (!overflow) {
      set_null(src);
      copy_decimal(&res, src);
      set_exp(exp + 1, src);
      set_sign(sign, src);
    }
  }
  return overflow;
}

// returns overflow information
int sum_mantissa(s21_decimal *dec1, s21_decimal *dec2, s21_decimal *res) {
  int overflow = 0;
  int bit1 = 0;
  int bit2 = 0;
  for (int i = 0; i < 96; i++) {
    bit1 = get_bit(i, dec1);
    bit2 = get_bit(i, dec2);
    set_bit(i, (overflow + bit1 + bit2) % 2, res);
    overflow = (overflow + bit1 + bit2) / 2;
  }
  return overflow;
}
