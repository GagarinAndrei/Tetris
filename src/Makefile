CC=gcc
STDFLAGS=-Wall -Werror -Wextra -std=c11 
TST_CFLAGS:=$(STDFLAGS) -g
GCOV_FLAGS?=-fprofile-arcs -ftest-coverage
LINUX_FLAGS=-lrt -lpthread -lm -lsubunit
LIBS=-lcheck
APP = build/Tetris.app 
VALGRIND_FLAGS=--trace-children=yes --track-fds=yes --track-origins=yes --leak-check=full --show-leak-kinds=all --verbose
# SOURCE_SUBDIRS := brick_game/tetris gui/cli
# OBJECT_DIRS := $(add prefix $(SOURCE_DIRS)/, $(SOURCE_SUBDIRS))

TARGET=s21_tetris.a

SRC=$(wildcard *.c)
OBJ=$(patsubst %.c,%.o, ${SRC})

PREF_TEST_SRC=./tests/
TEST_SRC=$(wildcard $(PREF_TEST_SRC)/*.c)
OS := $(shell uname -s)
USERNAME=$(shell whoami)

ifeq ($(OS),Linux)
  OPEN_CMD = firefox
	#  APP_NAME = qt_smart_calc SmartCalc.app
endif
ifeq ($(OS),Darwin)
	OPEN_CMD = open
	# APP_NAME = qt_smart_calc.app SmartCalc.app
endif

.DELETE_ON_ERROR:
all: $(TARGET) install

$(TARGET): ${SRC}
	$(CC) -c $(STDFLAGS) $(SRC)
	ar rc $@ $(OBJ)
	ranlib $@
	cp $@ lib$@
	make clean_obj

build:
	mkdir build
	cd ./build/ && qmake ../ && make  && mv $(APP_NAME)

install: build
	@mkdir $(HOME)/Desktop/SmartCalc_v1.0/
	@cp -rf $(APP) $(HOME)/Desktop/SmartCalc_v1.0/
	make clean

uninstall:
	@rm -rf $(HOME)/Desktop/SmartCalc_v1.0/

dvi:
	open ../README.md

dist:
	mkdir SmartCalc_v1.0/
	mkdir SmartCalc_v1.0/src
	cp Makefile *.c *.h *.pro *.cpp *.ui *.user SmartCalc_v1.0/src/
	tar cvzf SmartCalc_v1.0.tgz SmartCalc_v1.0/
	mv SmartCalc_v1.0.tgz $(HOME)/Desktop/
	rm -rf SmartCalc_v1.0/

test: $(TARGET)
ifeq ($(OS), Darwin)
	${CC} $(STDFLAGS) ${TEST_SRC} $< -o unit_test $(LIBS)
else
	${CC} $(STDFLAGS) ${TEST_SRC} $< -o unit_test $(LIBS) $(LINUX_FLAGS)
endif
	./unit_test

leaks: test
	leaks -atExit -- ./unit_test

gcov_report: clean_test
ifeq ($(OS), Darwin)
	$(CC) $(STDFLAGS) $(GCOV_FLAGS) ${TEST_SRC} ${SRC} -o test $(LIBS)
else
	$(CC) $(STDFLAGS) $(GCOV_FLAGS) ${TEST_SRC} ${SRC} -o test $(LIBS) $(LINUX_FLAGS)
endif
	./test
	lcov -t "stest" -o s21_test.info -c -d .
	genhtml -o report s21_test.info
	$(OPEN_CMD) ./report/index.html

valgrind: $(TARGET)
	$(CC) $(TST_CFLAGS) ${TEST_SRC} $< -o test $(LIBS) $(LINUX_FLAGS)
	CK_FORK=no valgrind $(VALGRIND_FLAGS) --log-file=VALGRIND.txt ./test

clang:
	clang-format -style=google -n *.c headers/*.h *.cpp *.h

brew:
	eval /opt/goinfre/$(USERNAME)/homebrew/bin/brew shellenv; \
	brew update --force --quiet; \
	chmod -R go-w "$(brew --prefix)/share/zsh"

install_brew:
	cd /opt/goinfre/$(USERNAME); \
	mkdir homebrew; \
	git clone https://github.com/Homebrew/brew homebrew; \
	make brew

lcov:
	brew install lcov

clean_obj:
	rm -rf *.o

clean_lib: 
	rm -rf *.a

clean_test:
	rm -rf *.gcda
	rm -rf *.gcno
	rm -rf *.info
	rm -rf test
	rm -rf report

clean: clean_lib clean_lib clean_test clean_obj
	rm -rf unit_test
	rm -rf VALGRIND.txt
	rm -rf *.o *.a
	rm -rf *.gcda *.gcno *.info
	rm -rf s21_test report
	rm -rf s21_calc s21_test
	rm -rf ./build*

.PHONY: all clean check lcov