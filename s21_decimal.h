#ifndef SRC_S21_DECIMAL_H_
#define SRC_S21_DECIMAL_H_

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
  unsigned int bits[4];
} s21_decimal;

enum { OK, TOO_BIG, TOO_SMALL, DIV_BY_ZERO };
enum { PLUS, MINUS };
enum { FALSE, TRUE };

// basic functions
int s21_from_int_to_decimal(int, s21_decimal *);
int s21_from_float_to_decimal(float, s21_decimal *);
int s21_from_decimal_to_int(s21_decimal, int *);
int s21_from_decimal_to_float(s21_decimal, float *);

int s21_negate(s21_decimal, s21_decimal *);
int s21_floor(s21_decimal, s21_decimal *);
int s21_round(s21_decimal, s21_decimal *);
int s21_truncate(s21_decimal, s21_decimal *);

int s21_is_equal(s21_decimal, s21_decimal);
int s21_is_not_equal(s21_decimal, s21_decimal);
int s21_is_less(s21_decimal, s21_decimal);
int s21_is_less_or_equal(s21_decimal, s21_decimal);
int s21_is_greater(s21_decimal, s21_decimal);
int s21_is_greater_or_equal(s21_decimal, s21_decimal);

// getters and setters
int get_bit(int, s21_decimal *);
int get_exp(s21_decimal *);
int get_sign(s21_decimal *);
int dec_div_10(s21_decimal *);
int dec_mul_10(s21_decimal *);

void set_null(s21_decimal *);
void set_bit(int, int, s21_decimal *);
void set_sign(int, s21_decimal *);
void set_exp(int, s21_decimal *);

int sum_mantissa(s21_decimal *, s21_decimal *, s21_decimal *);
void decimal_alignment(s21_decimal *, s21_decimal *);
void copy_decimal(s21_decimal *, s21_decimal *);
void shift_left(s21_decimal *);
void shift_right(s21_decimal *);
void output_decimal_binary(s21_decimal *);

int null_mantissa(s21_decimal *);
int compare_sign(s21_decimal *dec1, s21_decimal *dec2);
int compare_exp(s21_decimal *dec1, s21_decimal *dec2);
int compare_mantissa(s21_decimal *dec1, s21_decimal *dec2);

// BASIC functions
void expanent_alligment(s21_decimal *a, s21_decimal *b);
int multiply_by_10(s21_decimal *dec);
int is_correct_decimal(s21_decimal *dec);

// BASIC getters and setters
void decimal_copy(s21_decimal *reciver, s21_decimal *donor);
void decimal_swap(s21_decimal *dec_1, s21_decimal *dec_2);
void set_zero(s21_decimal *dec);
int is_zero(s21_decimal *dec);
int get_sign(s21_decimal *dec);
int get_exp(s21_decimal *dec);

// debug functions
void decimal_set(s21_decimal *dec, int exp, const char *num_str);
void decimal_print_bin(s21_decimal *dec);
void decimal_to_str(s21_decimal *dec, char *str);
void set_code_to_str(int code, char *str);

// ARIPMETIC
int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);

#endif  // SRC_S21_DECIMAL_H_