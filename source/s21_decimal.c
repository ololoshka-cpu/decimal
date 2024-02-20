#include "../s21_decimal.h"

int get_bit(int i, s21_decimal *value) {
  int res = -1;
  if (0 <= i && i <= 127) {
    res = (value->bits[i / 32] & (1 << (i % 32))) != 0;
  }
  return res;
}

void set_bit(int i, int x, s21_decimal *value) {
  if (x) {
    value->bits[i / 32] |= (1 << (i % 32));
  } else {
    value->bits[i / 32] &= ~(1 << (i % 32));
  }
}

void set_null(s21_decimal *dest) {
  dest->bits[0] &= 0x00000000;
  dest->bits[1] &= 0x00000000;
  dest->bits[2] &= 0x00000000;
  dest->bits[3] &= 0x00000000;
}

void set_sign(int sign, s21_decimal *dest) {
  if (sign) {
    dest->bits[3] |= (1 << 31);
  } else {
    dest->bits[3] &= ~(1 << 31);
  }
}

int get_exp(s21_decimal *src) { return (src->bits[3] & 0xFF0000) >> 16; }

int get_sign(s21_decimal *src) { return !!(src->bits[3] & 0x80000000); }

void shift_left(s21_decimal *dec) {
  int tmp_31 = dec->bits[0] >> 31, tmp_63 = dec->bits[1] >> 31;
  dec->bits[0] <<= 1;
  dec->bits[1] <<= 1;
  dec->bits[2] <<= 1;
  set_bit(32, tmp_31, dec);
  set_bit(64, tmp_63, dec);
}

void shift_right(s21_decimal *dec) {
  int tmp_32 = dec->bits[1] & 0x00000001;
  int tmp_64 = dec->bits[2] & 0x00000001;
  dec->bits[0] >>= 1;
  dec->bits[1] >>= 1;
  dec->bits[2] >>= 1;
  set_bit(31, tmp_32, dec);
  set_bit(63, tmp_64, dec);
}

void copy_decimal(s21_decimal *src, s21_decimal *dest) {
  dest->bits[0] = src->bits[0];
  dest->bits[1] = src->bits[1];
  dest->bits[2] = src->bits[2];
  dest->bits[3] = src->bits[3];
}

void set_exp(int exp, s21_decimal *dest) {
  int sign = get_sign(dest);
  dest->bits[3] = 0;
  dest->bits[3] += exp << 16;
  set_sign(sign, dest);
}

void decimal_alignment(s21_decimal *dec1, s21_decimal *dec2) {
  int overflow = 0;
  int exp1 = get_exp(dec1);
  int exp2 = get_exp(dec2);
  if (exp1 > exp2) {
    while (exp1 != exp2 && !overflow) {
      overflow = dec_mul_10(dec2);
      exp2++;
    }
  } else {
    while (exp1 != exp2 && !overflow) {
      overflow = dec_mul_10(dec1);
      exp1++;
    }
  }
  if (overflow) {
    exp1 = get_exp(dec1);
    exp2 = get_exp(dec2);
    while (exp1 != exp2) {
      dec_div_10(exp1 > exp2 ? dec1 : dec2);
      exp1 > exp2 ? exp1-- : exp2--;
    }
  }
}
