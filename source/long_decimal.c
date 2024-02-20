#include "long_decimal.h"

#include "../s21_decimal.h"

// вычисление целой части
void l_calculate_integer_piece(long_decimal* l_dec_1, long_decimal* l_dec_2,
                               long_decimal* l_res) {
  int bits_count_1 = (L_BITS - 1) * 32;  // сколько бит занимает запись числа
  for (int i = (L_BITS - 1) * 32 - 1; i >= 0 && l_get_bit(l_dec_1, i) != 1; i--)
    bits_count_1--;

  int bits_count_2 = (L_BITS - 1) * 32;
  for (int i = (L_BITS - 1) * 32 - 1; i >= 0 && l_get_bit(l_dec_2, i) != 1; i--)
    bits_count_2--;

  int iterations = bits_count_1 - bits_count_2;

  // выравниваем (теперь не по экспоненте, а по крайним левым битам)
  for (int i = 0; i < iterations; i++) l_shift_left(l_dec_2);

  // сам процесс вычисления целой части
  for (int i = 0; i < iterations; i++) {
    l_shift_left(l_res);

    if (l_compire_by_module(l_dec_1, l_dec_2) >= 0) {
      l_set_bit(l_res, 1, 1);
      l_sub_bit_by_bit(l_dec_1, l_dec_2, l_dec_1);
    }
    l_shift_right(l_dec_2);
  }
}

// выравниваем записи по экспоненте (для легкого побитового сложения/вычитания)
void l_decimal_equalize(long_decimal* l_dec_1, long_decimal* l_dec_2) {
  if (l_get_exp(l_dec_1) == l_get_exp(l_dec_2)) return;

  // фиксируем указатели на меньую/большую экспаненту
  long_decimal* less;
  int more_exp, less_exp;
  //
  if (l_get_exp(l_dec_1) > l_get_exp(l_dec_2)) {
    more_exp = l_get_exp(l_dec_1);
    less = l_dec_2, less_exp = l_get_exp(l_dec_2);
  } else {
    more_exp = l_get_exp(l_dec_2);
    less = l_dec_1, less_exp = l_get_exp(l_dec_1);
  }
  // подгоняем меньшую экспоненту к большей
  while (less_exp < more_exp) {
    l_multiply_by_10(less);
    l_set_exp(less, ++less_exp);
  }
}

// (10 * a)              = (8 * a)  + (2 * a) =
// (2^3 * a) + (2^1 * a) = (2 << 3 * a) + (2 << 1 * a)
// (a << 3) + (a << 1)   = (a << 3)     + (b << 1), где a = b
void l_multiply_by_10(long_decimal* l_dec) {
  // a << 3
  long_decimal a = *l_dec;
  for (int i = 0; i < 3; i++) l_shift_left(&a);

  // b << 1 (= a << 1)
  long_decimal b = *l_dec;
  l_shift_left(&b);

  // a << 3 + b << 1 = 10 * a
  int bit_overflow = 0;
  for (int i = 0; i <= (L_BITS - 1) * 32; i++) {
    int sum = l_get_bit(&a, i) + l_get_bit(&b, i) + bit_overflow;

    if (sum == 1 || sum == 3)
      l_set_bit(l_dec, i, 1);
    else
      l_set_bit(l_dec, i, 0);

    bit_overflow = sum / 2;
  }
}

// деление на 10
int l_devide_by_10(long_decimal* src) {
  int tmp = 0;
  long_decimal res = {0};
  int exp = l_get_exp(src) - 1, sign = l_get_sign(src);

  for (int i = 191; i >= 0; i--) {
    tmp *= 2;
    l_shift_left(&res);
    tmp += l_get_bit(src, i);
    if (tmp > 9) {
      res.bits[0] += 1;
      tmp -= 10;
    }
  }
  l_set_zero(src);
  l_copy(src, &res);
  l_set_sign(src, sign);
  l_set_exp(src, exp);
  return tmp;  // возвращается остаток от деления на 10
}

// урезаем / округляем непомещающуюся дробную часть
void l_squeeze_into_the_mantissa(long_decimal* l_dec) {
  if (l_fit_into_mantissa(l_dec) && l_get_exp(l_dec) <= 28) return;

  int sign = l_get_sign(l_dec);

  int cutting_num = -1, rounding_num = l_get_last_decimal_digit(l_dec);

  // отслеживаем 2 крайние цифры (обрезаемую и округляемую)
  while (!l_fit_into_mantissa(l_dec) || l_get_exp(l_dec) > 28) {
    cutting_num = rounding_num;
    rounding_num = l_devide_by_10(l_dec);
  }
  cutting_num = rounding_num;
  rounding_num = l_get_last_decimal_digit(l_dec);

  // если надо, округляем в большую сторону
  if ((cutting_num == 5 && rounding_num % 2 == 1) || cutting_num > 5) {
    // long decimal = 1 и лог для суммировани с 1
    long_decimal l_1;
    l_decimal_set(&l_1, 0, "+ 1");

    l_sum_bit_by_bit(l_dec, &l_1, l_dec);
  }

  l_set_sign(l_dec, sign);
}

// впихивание в мантиссу с обычным округлением (а не банковским)
void l_squeeze_into_the_mantissa_div(long_decimal* l_dec) {
  if (l_fit_into_mantissa(l_dec) && l_get_exp(l_dec) <= 28) return;
  int sign = l_get_sign(l_dec);

  int cutting_num = -1, rounding_num = l_get_last_decimal_digit(l_dec);

  // отслеживаем 2 крайние цифры (обрезаемую и округляемую)
  while (!l_fit_into_mantissa(l_dec) || l_get_exp(l_dec) > 28) {
    cutting_num = rounding_num;
    rounding_num = l_devide_by_10(l_dec);
  }
  cutting_num = rounding_num;
  rounding_num = l_get_last_decimal_digit(l_dec);

  // если надо, округляем в большую сторону
  if (cutting_num >= 5) {
    // long decimal = 1 и лог для суммировани с 1
    long_decimal l_1;
    l_decimal_set(&l_1, 0, "+ 1");

    l_sum_bit_by_bit(l_dec, &l_1, l_dec);
  }

  l_set_sign(l_dec, sign);

  // чистка незначащих нулей (1.000 -> 1)
  while (1) {
    int junior_digit = l_get_last_decimal_digit(l_dec);
    int exp = l_get_exp(l_dec);

    if (junior_digit == 0 && exp > 0) {
      l_devide_by_10(l_dec);
      // l_multiply_by_10(l_dec);
    } else
      break;
  }
}

void l_shift_left(long_decimal* l_dec) {
  // запоминаем бит, усекающийся бит (перескакивающий на след. инт)
  int transfer[L_BITS - 2] = {};
  for (int i = 0; i < L_BITS - 2; i++) transfer[i] = l_dec->bits[i] >> 31;

  // делаем сдвиг
  for (int i = 0; i <= L_BITS - 2; i++) l_dec->bits[i] <<= 1;

  // восстанавливваем (переносим) крайний бит
  for (int i = 1; i <= L_BITS - 2; i++)
    l_set_bit(l_dec, i * 32, transfer[i - 1]);
}

void l_shift_right(long_decimal* l_dec) {
  // запоминаем бит, усекающийся бит (перескакивающий на след. инт)
  int transfer[L_BITS - 1] = {};
  for (int i = 0; i < L_BITS - 2; i++) transfer[i] = l_dec->bits[i + 1] & 1;

  // делаем сдвиг
  for (int i = 0; i <= L_BITS - 2; i++) l_dec->bits[i] >>= 1;

  // восстанавливваем (переносим) крайний бит
  for (int i = 1; i <= L_BITS - 1; i++)
    // l_set_bit(l_dec, i * 32 - 1, 0);
    l_set_bit(l_dec, i * 32 - 1, transfer[i - 1]);
}

// побитовое сложение мантисс
void l_sum_bit_by_bit(long_decimal* l_dec_1, long_decimal* l_dec_2,
                      long_decimal* l_res) {
  long_decimal l_sum;
  l_set_zero(&l_sum);

  // суммируем мантиссы
  int owerflow = 0;
  for (int i = 0; i <= (L_BITS - 1) * 32 - 1; i++) {
    int sum = l_get_bit(l_dec_1, i) + l_get_bit(l_dec_2, i) + owerflow;

    if (sum == 1 || sum == 3)
      l_set_bit(&l_sum, i, 1);
    else
      l_set_bit(&l_sum, i, 0);

    owerflow = sum / 2;
  }
  // копируем эспаненту
  l_set_exp(&l_sum, l_get_exp(l_dec_1));

  l_copy(l_res, &l_sum);
}

// побитовое вычитание мантисс (l_dec_1 >= l_dec_2)
void l_sub_bit_by_bit(long_decimal* l_dec_1, long_decimal* l_dec_2,
                      long_decimal* l_res) {
  long_decimal l_res_tepm = {};
  l_set_zero(&l_res_tepm);

  for (int i = 0; i <= L_LAST_BIT; i++) {
    // 0-0 / 1-1 / 1-0 / 0-1
    int sub = l_get_bit(l_dec_1, i) - l_get_bit(l_dec_2, i);
    // 0 - 1 -> производим заем из старшего бита
    if (sub == -1) {
      int j = i;  // откуда начать поиск старшего бита = 1
      while (j <= L_LAST_BIT && l_get_bit(l_dec_1, j) != 1)
        j++;  // поиск бита = 1

      l_set_bit(l_dec_1, j, 0);  // заимствуем из старшего бита
      j--;
      while (j >= i) l_set_bit(l_dec_1, j--, 1);

      sub = 1;
    }

    l_set_bit(&l_res_tepm, i, sub);
  }

  // копируем знак и эспаненту
  l_res_tepm.bits[L_BITS - 1] = l_dec_1->bits[L_BITS - 1];

  l_copy(l_res, &l_res_tepm);
}

// проверка на выход за пределы максимально возможного числа
int l_check_min_max(long_decimal* l_dec) {
  int res = OK;

  // проверка на максимум по модулю
  long_decimal MAX_MODULE = {};
  l_decimal_set(
      &MAX_MODULE, 0,
      "+ 11111111111111111111111111111111 11111111111111111111111111111111 "
      "11111111111111111111111111111111");
  l_decimal_equalize(&MAX_MODULE, l_dec);

  if (l_compire_by_module(l_dec, &MAX_MODULE) > 0) {
    if (l_get_sign(l_dec) == PLUS)
      res = TOO_BIG;
    else
      res = TOO_SMALL;
  }

  return res;
}

// проверка - число вмещается в мантиссу обычного децимала
int l_fit_into_mantissa(long_decimal* l_dec) {
  long_decimal l_temp = {};
  l_copy(&l_temp, l_dec);

  int digits = 0;
  while (!l_is_zero(&l_temp)) {
    digits++;
    l_devide_by_10(&l_temp);
  }

  return digits < 30 ? 1 : 0;
}

// возвращает последнюю цифру 10-чной записи (3 для 666.23)
int l_get_last_decimal_digit(long_decimal* l_dec) {
  long_decimal l_temp = {};
  l_copy(&l_temp, l_dec);

  return l_devide_by_10(&l_temp);
}

// сравнение по модулю
int l_compire_by_module(long_decimal* l_dec_1, long_decimal* l_dec_2) {
  int res = 0;

  l_decimal_equalize(l_dec_1, l_dec_2);

  int i = (L_BITS - 1) * 32 - 1;
  while (i >= 0 && res == 0) {
    res = l_get_bit(l_dec_1, i) - l_get_bit(l_dec_2, i);
    i--;
  }

  return res;
}