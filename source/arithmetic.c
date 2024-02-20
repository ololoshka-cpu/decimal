#include "../s21_decimal.h"
#include "long_decimal.h"

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal* result) {
  int status = OK;
  set_zero(result);

  long_decimal l_dec_1, l_dec_2, l_res;
  l_initialize(&l_dec_1, &value_1);
  l_initialize(&l_dec_2, &value_2);
  l_set_zero(&l_res);

  // сложение чисел с одинаковыми знаками
  if (l_get_sign(&l_dec_1) == l_get_sign(&l_dec_2)) {
    int res_sign = l_get_sign(&l_dec_1);

    l_decimal_equalize(&l_dec_1, &l_dec_2);
    l_sum_bit_by_bit(&l_dec_1, &l_dec_2, &l_res);

    l_set_sign(&l_res, res_sign);  // установка знака

    // нужно проверить, что число не слишком большое / маленькое
    status = l_check_min_max(&l_res);

    // если в сумме корректное число
    if (status == OK) {
      l_squeeze_into_the_mantissa(
          &l_res);  // обрезаем/округляем не влезающую дробную часть
      l_convert_long_to_decimal(&l_res, result);  // приводим к децимал
    }
  }
  // слождение чисел с разными знаками -> сложение заменяем вычитанием
  else {
    int res_sign = l_get_sign(
        &l_dec_1);  // знак резульата = знаку операнда, большего по модулю
    if (l_compire_by_module(&l_dec_2, &l_dec_1) > 0)
      res_sign = l_get_sign(&l_dec_2);

    if (get_sign(&value_1) == MINUS)
      decimal_swap(&value_1, &value_2);  // отрицатльный опреанд ставим справа
    set_sign(PLUS, &value_2);  // убираем у него минус

    status = s21_sub(value_1, value_2, result);
    set_sign(res_sign, result);
  }

  return status;
}

int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal* result) {
  int status = OK;
  set_zero(result);

  long_decimal l_dec_1, l_dec_2, l_res;
  l_initialize(&l_dec_1, &value_1);
  l_initialize(&l_dec_2, &value_2);
  l_set_zero(&l_res);

  // вычитание чисел с одинаковыми знаками
  if (l_get_sign(&l_dec_1) == l_get_sign(&l_dec_2)) {
    int res_sign = PLUS;
    // 1 - 3 = - 2 / -3 - -1 = -2
    if (l_compire_by_module(&l_dec_2, &l_dec_1) > 0 &&
        l_get_sign(&l_dec_2) == PLUS)
      res_sign = MINUS;
    if (l_compire_by_module(&l_dec_1, &l_dec_2) > 0 &&
        l_get_sign(&l_dec_1) == MINUS)
      res_sign = MINUS;

    // поменяем местами если второе больше первого (чтобы вычитать l_dec_1 -
    // l_dec_2)
    if (l_compire_by_module(&l_dec_1, &l_dec_2) < 0) l_swap(&l_dec_1, &l_dec_2);

    l_decimal_equalize(&l_dec_1, &l_dec_2);
    l_sub_bit_by_bit(&l_dec_1, &l_dec_2, &l_res);

    l_set_sign(&l_res, res_sign);  // установка знака

    // нужно проверить, что число не слишком большое / маленькое
    status = l_check_min_max(&l_res);

    // если в сумме корректное число
    if (status == OK) {
      l_squeeze_into_the_mantissa(
          &l_res);  // обрезаем/округляем не влезающую дробную часть
      l_convert_long_to_decimal(&l_res, result);  // приводим к децимал
    }
  }
  // вычитание чисел с разными знаками (заменяем сложением)
  else {
    int res_sign = l_get_sign(&l_dec_1);  // совпадает со знаком 1 операнда
    // сбрасываем знаки
    set_sign(PLUS, &value_1);
    set_sign(PLUS, &value_2);

    status = s21_add(value_1, value_2, result);

    set_sign(res_sign, result);

    if (status != OK) status = (res_sign == PLUS) ? TOO_BIG : TOO_SMALL;
  }

  return status;
}

int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal* result) {
  if (is_zero(&value_1) || is_zero(&value_2)) {
    set_zero(result);
    return OK;
  }

  int status = OK;
  set_zero(result);

  long_decimal l_dec_1, l_dec_2, l_res;
  l_initialize(&l_dec_1, &value_1);
  l_initialize(&l_dec_2, &value_2);
  l_set_zero(&l_res);

  // знаки и экспонента произведения
  int sign1 = l_get_sign(&l_dec_1), sign2 = l_get_sign(&l_dec_2);
  int res_sign = (sign1 == sign2) ? PLUS : MINUS;
  int res_exp = l_get_exp(&l_dec_1) + l_get_exp(&l_dec_2);
  l_set_exp(&l_dec_1, 0), l_set_exp(&l_dec_2, 0);
  // выравнивание
  l_decimal_equalize(&l_dec_1, &l_dec_2);

  // побитовое умножение
  for (int i_1 = 0; i_1 <= L_LAST_BIT; i_1++) {
    // сумма умножения 1 бита 1-го числа на все биты 2-го числа
    long_decimal temp;
    l_set_zero(&temp);

    for (int i_2 = 0; i_2 <= L_LAST_BIT; i_2++)
      l_set_bit(&temp, i_1 + i_2,
                l_get_bit(&l_dec_1, i_1) * l_get_bit(&l_dec_2, i_2));

    l_sum_bit_by_bit(&l_res, &temp, &l_res);
  }

  l_set_exp(&l_res, res_exp);  // экспонента с учетом умножения
  l_set_sign(&l_res, res_sign);  // установка знака

  // проверим, что число не слишком большое / маленькое
  status = l_check_min_max(&l_res);
  if (status == OK) {
    l_squeeze_into_the_mantissa(
        &l_res);  // обрезаем/округляем не влезающую дробную часть
    l_convert_long_to_decimal(&l_res, result);  // приводим к децимал
  }

  return status;
}

int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal* result) {
  if (is_zero(&value_2))
    return DIV_BY_ZERO;
  else if (is_zero(&value_1)) {
    set_zero(result);
    return OK;
  }

  int status = OK;
  set_zero(result);

  long_decimal l_dec_1, l_dec_2, l_res;
  l_initialize(&l_dec_1, &value_1), l_initialize(&l_dec_2, &value_2);
  l_set_zero(&l_res);

  // знак произведения
  int sign1 = l_get_sign(&l_dec_1), sign2 = l_get_sign(&l_dec_2);
  int res_sign = (sign1 == sign2) ? PLUS : MINUS;
  int res_exp = 0;

  // выравнивание по экспоненте
  l_decimal_equalize(&l_dec_1, &l_dec_2);

  // вычисление целой части
  l_calculate_integer_piece(&l_dec_1, &l_dec_2, &l_res);

  // вычисление дробной части
  long_decimal l_one = {};
  l_decimal_set(&l_one, 0, "1");

  while (!l_is_zero(&l_dec_1) && l_fit_into_mantissa(&l_res) && res_exp < 29) {
    res_exp++;
    l_multiply_by_10(&l_res);
    l_multiply_by_10(&l_dec_1);

    while (l_compire_by_module(&l_dec_1, &l_dec_2) >= 0) {
      l_sub_bit_by_bit(&l_dec_1, &l_dec_2, &l_dec_1);
      l_sum_bit_by_bit(&l_res, &l_one, &l_res);
    }
  }

  // установка экспоненты и знака
  l_set_exp(&l_res, res_exp);
  l_set_sign(&l_res, res_sign);

  // проверка на МАКС МИН
  status = l_check_min_max(&l_res);

  if (status == OK) {
    l_squeeze_into_the_mantissa_div(&l_res);
    l_convert_long_to_decimal(&l_res, result);
  }

  return status;
}

void decimal_copy(s21_decimal* reciver, s21_decimal* donor) {
  reciver->bits[0] = donor->bits[0];
  reciver->bits[1] = donor->bits[1];
  reciver->bits[2] = donor->bits[2];
  reciver->bits[3] = donor->bits[3];
}

void decimal_swap(s21_decimal* dec_1, s21_decimal* dec_2) {
  s21_decimal temp = {};

  decimal_copy(&temp, dec_1);
  decimal_copy(dec_1, dec_2);
  decimal_copy(dec_2, &temp);
}

// обнулить число
void set_zero(s21_decimal* dec) {
  dec->bits[0] = dec->bits[1] = dec->bits[2] = dec->bits[3] = 0;
}

int is_zero(s21_decimal* dec) {
  return dec->bits[0] == 0 && dec->bits[1] == 0 && dec->bits[2] == 0;
}

// установить биты по заданной строке типа
// "- 00000001000000000000000000000000 00000000000000000011100000000000
// 00000000000100000000000000000000"
void decimal_set(s21_decimal* dec, int exp, const char* num_str) {
  set_zero(dec);
  set_exp(exp, dec);

  if (*num_str && (*num_str == '+' || *num_str == '-')) {
    if (*num_str == '-') set_sign(MINUS, (dec));
    while (*num_str && !(*num_str == '0' || *num_str == '1')) num_str++;
  }

  const char* str_end = num_str;
  while (*(str_end++))
    ;
  str_end -= 2;

  for (int i = 0; i < 96 && str_end >= num_str; i++, str_end--) {
    if (*str_end == ' ') str_end--;
    if (str_end >= num_str && *str_end - '0' == 1) set_bit(i, 1, dec);
  }
}

// для проверки через check.h
void decimal_to_str(s21_decimal* dec, char* str) {
  for (int i = 4 * 32 - 1, i_str = 0; i >= 0; i--, i_str++) {
    if (get_bit(i, dec))
      str[i_str] = '1';
    else
      str[i_str] = '0';
  }

  str[4 * 32] = '\0';
}

void set_code_to_str(int code, char* str) {
  if (code == OK) return;

  if (code == TOO_BIG) {
    str[0] = 'T', str[1] = 'O', str[2] = 'O', str[3] = '_';
    str[4] = 'B', str[5] = 'I', str[6] = 'G', str[7] = '\0';
  } else if (code == TOO_SMALL) {
    str[0] = 'T', str[1] = 'O', str[2] = 'O', str[3] = '_';
    str[4] = 'S', str[5] = 'M', str[6] = 'A', str[7] = 'L', str[8] = 'L',
    str[9] = '\0';
  } else if (code == DIV_BY_ZERO) {
    str[0] = 'D', str[1] = 'I', str[2] = 'V', str[3] = '_';
    str[4] = 'B', str[5] = 'Y', str[6] = '_';
    str[7] = 'Z', str[8] = 'E', str[9] = 'R', str[10] = 'O', str[11] = '\0';
  }
}