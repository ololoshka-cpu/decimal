#include "../s21_decimal.h"
#include "long_decimal.h"

// установить биты по заданной строке типа
// "- 00000001000000000000000000000000 00000000000000000011100000000000
// 00000000000100000000000000000000"
void l_decimal_set(long_decimal* l_dec, int exp, const char* num_str) {
  l_set_zero(l_dec);
  l_set_exp(l_dec, exp);

  if (*num_str && (*num_str == '+' || *num_str == '-')) {
    if (*num_str == '-') l_set_sign((l_dec), MINUS);
    while (*num_str && !(*num_str == '0' || *num_str == '1')) num_str++;
  }

  const char* str_end = num_str;
  while (*(str_end++))
    ;
  str_end -= 2;

  for (int i = 0; i < (L_BITS - 1) * 32 && str_end >= num_str; i++, str_end--) {
    if (*str_end == ' ') str_end--;
    if (str_end >= num_str && *str_end - '0' == 1) l_set_bit(l_dec, i, 1);
  }
}

// переписывание результата из long_decimal в decimal
void l_convert_long_to_decimal(long_decimal* l_dec, s21_decimal* dec) {
  dec->bits[3] = l_dec->bits[L_BITS - 1];
  for (int i = 0; i < 3; i++) dec->bits[i] = l_dec->bits[i];
}

// обнулить число
void l_set_zero(long_decimal* l_dec) {
  for (int i = 0; i < L_BITS; i++) l_dec->bits[i] = 0;
}

// инициализация длинного значениями decimal
void l_initialize(long_decimal* l_dec, s21_decimal* dec) {
  l_set_zero(l_dec);
  // copy from dec
  l_dec->bits[0] = dec->bits[0], l_dec->bits[1] = dec->bits[1];
  l_dec->bits[2] = dec->bits[2], l_dec->bits[L_BITS - 1] = dec->bits[3];
  // initialize another 'bits'
  for (int i = 3; i < L_BITS - 1; i++) l_dec->bits[i] = 0;
}

// копирование длинного
void l_copy(long_decimal* resiver, long_decimal* donor) {
  for (int i = 0; i < L_BITS; i++) resiver->bits[i] = donor->bits[i];
}

// поменять длинные местами
void l_swap(long_decimal* l_dec_1, long_decimal* l_dec_2) {
  long_decimal temp = {};
  l_copy(&temp, l_dec_1);
  l_copy(l_dec_1, l_dec_2);
  l_copy(l_dec_2, &temp);
}

// получить i-тый бит
int l_get_bit(long_decimal* l_dec, int i) {
  if (i < 0 || i > L_BITS * 32 - 1) return 0;

  int bit = l_dec->bits[i / 32] & (1 << (i % 32));
  return bit == 0 ? 0 : 1;
}

// установить i-тый бит значением b
void l_set_bit(long_decimal* l_dec, int i, int b) {
  if (i < 0 || i > L_BITS * 32 - 1) return;

  if (b)
    l_dec->bits[i / 32] |= 1 << (i % 32);
  else
    l_dec->bits[i / 32] &= ~(1 << (i % 32));
}

// получить знак '+'/'-'
int l_get_sign(long_decimal* l_dec) {
  //          крайний слева бит
  return (l_dec->bits[L_BITS - 1] & 0x80000000) == 0 ? PLUS : MINUS;
}

// установить знак '+'/'-'
void l_set_sign(long_decimal* l_dec, int sign) {
  if (sign)
    l_dec->bits[L_BITS - 1] |= 0x80000000;
  else
    l_dec->bits[L_BITS - 1] &= ~(0x80000000);
}

// получить экспоненту
int l_get_exp(long_decimal* l_dec) {
  // маской берем 16-23й биты и смещаем их в начало (в право)
  return (l_dec->bits[L_BITS - 1] & 0xFF0000) >> 16;
}

// установить экспоненту
void l_set_exp(long_decimal* l_dec, int exp) {
  // маской обнуляем 16-23й биты
  l_dec->bits[L_BITS - 1] &= 0xFF00FFFF;
  // накладываем маску на первые 8 бит и смещаем
  l_dec->bits[L_BITS - 1] |= ((exp & 0xFF) << 16);
}

int l_is_zero(long_decimal* l_dec) {
  int res = 1;

  for (int i = 0; i < L_BITS - 1; i++)
    if (l_dec->bits[i] != 0) res = 0;

  return res;
}
