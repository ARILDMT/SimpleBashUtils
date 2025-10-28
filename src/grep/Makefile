CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11
BIN = grep
SRC = grep.c
OBJ = grep.o
HDR = grep.h
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
	echo  "Hello grep\nGrep is great\nPattern match\n" > input_01.txt
	echo  "This is a Test\nAnother test Line\nLine again" > input_02.txt
	echo  "bar\nfoobar\nbar line\n" > input_03.txt
	echo  "123\nABC\nxyz\nPattern123\n" > input_04.txt
	echo  "test\ntest again\nbar\nxyz\npattern\n" > patterns.txt

test: all txt
	bash test_grep.sh

clean:
	rm -f $(BIN) *.o
	rm -rf $(TMP)

fclean:	clean
	rm -f input_*.txt patterns.txt