#ifndef SRC_SOURCE_LONG_DECIMAL_H_
#define SRC_SOURCE_LONG_DECIMAL_H_
#include "../s21_decimal.h"


#define L_BITS 7

// LONG DECIMAL
typedef struct {
  int bits[L_BITS]; // 3*2 + 1
} long_decimal;

#define L_LAST_BIT (L_BITS - 1) * 32 - 1
void l_calculate_integer_piece (long_decimal *l_dec_1, long_decimal *l_dec_2, long_decimal *l_res);
void l_decimal_equalize(long_decimal* l_dec_1, long_decimal* l_dec_2);
void l_multiply_by_10(long_decimal* l_dec);
int  l_devide_by_10(long_decimal* l_dec);
int  l_get_num_before_dot(long_decimal *l_dec);
int  l_get_num_after_dot(long_decimal *l_dec);
void l_shift_left(long_decimal* l_dec);
void l_shift_right(long_decimal* l_dec);
void l_sum_bit_by_bit(long_decimal* l_dec_1, long_decimal* l_dec_2, long_decimal* l_res);
void l_sub_bit_by_bit(long_decimal* l_dec_1, long_decimal* l_dec_2, long_decimal* l_res);
int  l_number_of_nums (long_decimal* l_dec_origin);
int  l_check_min_max(long_decimal* l_dec);
int  l_compire(long_decimal* l_dec_1, long_decimal* l_dec_2); 
int  l_compire_by_module (long_decimal* l_dec_1, long_decimal* l_dec_2);
void l_squeeze_into_the_mantissa(long_decimal* l_dec);
void l_squeeze_into_the_mantissa_div(long_decimal* l_dec);
int  l_fit_into_mantissa(long_decimal* l_dec);
int  l_get_last_decimal_digit (long_decimal* l_dec);

// long getters and setters
void  l_print_bin(long_decimal* l_dec);
void  l_print_bin_full(long_decimal* l_dec);
void  l_set_zero(long_decimal* l_dec);
void  l_initialize(long_decimal* l_dec, s21_decimal* dec);
void  l_copy(long_decimal* resiver, long_decimal* donor);
void  l_swap(long_decimal* l_dec_1, long_decimal* l_dec_2);
int   l_get_bit(long_decimal* l_dec, int i);
void  l_set_bit(long_decimal* l_dec, int i, int b);
int   l_get_sign(long_decimal* l_dec);
void  l_set_sign(long_decimal* l_dec, int sign);
int   l_get_exp(long_decimal* l_dec);
void  l_set_exp(long_decimal* l_dec, int exp);
void  l_convert_long_to_decimal(long_decimal* l_dec, s21_decimal* dec);
int   l_is_zero(long_decimal* l_dec);
void  l_decimal_set(long_decimal* l_dec, int exp, const char* num_str);

#endif  // SRC_SOURCE_LONG_DECIMAL_H_