src=$(wildcard ./*/*.c)
obj=$(src:.c=.o)
obj_test=$(src:.c=_test.o)

check_src=$(wildcard ./*/*.check)

c_flags=-c -Wall -Werror -Wextra -std=c11
check_flags=$(shell pkg-config --libs check)
gcov_flags=--coverage

all: clean_all s21_decimal_test.a test gcov_report

# цель для вертера
s21_decimal.a: $(obj)
	ar rc s21_decimal.a $(obj)
	ranlib s21_decimal.a

%.o: %.c
	gcc $(c_flags) $< -o $@


# цель для тестов
s21_decimal_test.a: $(obj_test)
	ar rc s21_decimal_test.a $(obj_test)
	ranlib s21_decimal_test.a

%_test.o: %.c
	gcc $(c_flags) $< -o $@ $(gcov_flags) $(check_flags)

test: s21_decimal_test.a
	checkmk clean_mode=1 $(check_src) > test.c
	gcc test.c -L. s21_decimal_test.a -o test $(gcov_flags) $(check_flags)

gcov_report:
	./test
	lcov --capture --directory . --output-file report.info
	genhtml report.info --output-directory report
	make clean
	open report/index.html



# чистка
clean:
	rm -rf ./*/*.[oa] ./*/*.gcda ./*/*.gcno *.[oa] *.gcda *.gcno test.c

clean_all: clean
	rm -rf report test *.info main

format:
	cp ../materials/linters/.clang-format .clang-format
	clang-format */*.c -i
	clang-format *.h -i
	rm -rf .clang-format