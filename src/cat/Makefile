CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11
BIN = cat
SRC = cat.c
OBJ = cat.o
HDR = cat.h
TMP = tmp

all:	$(BIN)

$(BIN):	$(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

$(OBJ):	$(SRC) $(HDR)
	$(CC) $(CFLAGS) -c $(SRC)

clang:
	cp ../../materials/linters/.clang-format .
	clang-format -n *.c *.h
	rm ./.clang-format

txt:
	echo  "Line1\n\nLine2\n\n\nLine3\n" > input_01.txt
	echo  "Tab\tline\tline\n" > input_02.txt
	printf "Ctrl chars:\x01\x02\nLine2\x7F\n" > input_03.txt

test:	all txt
	bash test_cat.sh

clean:
	rm -f $(BIN) *.o
	rm -rf $(TMP)

fclean: clean
	rm -f input_*.txt