OS := $(shell uname -s)
SRC = s21_3DViewer.c s21_MoveModel.c
TEST = testest.c
CC = gcc -std=c11

CFLAGS	= -Wall -Wextra -Werror
FLAGS_PLATFORM = -lcheck

GCOVFLAGS = -fprofile-arcs -ftest-coverage
REPORT = GcovReport

ifeq ($(OS), Darwin)
	FLAGS_PLATFORM = -lcheck
else
	FLAGS_PLATFORM = -lcheck -lsubunit -lrt -lm -lpthread -g
endif

all: install

.PHONY: all, install, uninstall, clean, dvi, dist, gcov_report

dvi:
	open dvi.html

gcov_report:
	gcc $(GCOVFLAGS) $(TEST) $(SRC) $(FLAGS_PLATFORM) -o $(REPORT)
	chmod +x *
	./GcovReport
	lcov -t "$(REPORT)" -o $(REPORT).info -c -d .
	genhtml -o report $(REPORT).info
	rm -rf ./*.gcno ./*.gcda ./$(REPORT).*
	open ./report/index.html
	rm $(REPORT)
	rm -rf *.dSYM

install:
	make clean
	mkdir build
	cd ../3DViewerApp && qmake && make && make clean && rm Makefile && cd ../ && mv 3DViewerApp/3DViewerApp.app build
	cd ../build/Contents/MacOS && open 3DViewerApp

open:
	cd ../build/Contents/MacOS && open 3DViewerApp

uninstall:
	@rm -rf ../build*

dist:
	rm -rf ../Archive_Viewer_v1.0/
	mkdir ../Archive_Viewer_v1.0/
	mkdir ../Archive_Viewer_v1.0/src
	cp ../build/Contents/MacOS/3DViewerApp ../Archive_Viewer_v1.0/src/
	cd .. && tar cvzf Archive_Viewer_v1.0.tgz ../Archive_Viewer_v1.0 && mv Archive_Viewer_v1.0 src/

cl:
	rm -rf ../Archive_Viewer_v1.0

bist:
	rm -rf ../Archive_Viewer_v1.0/
	mkdir ../Archive_Viewer_v1.0/
	mkdir ../Archive_Viewer_v1.0/src
	cp ../build/Contents/MacOS/SmartCalc ../Archive_Viewer_v1.0/src/
	mv ../Archive_Viewer_v1.0/ .
	tar cvzf Archive_Viewer_v1.0.tgz Archive_Viewer_v1.0
	open .

test:
	@$(CC) $(CFLAGS) $(TEST) $(SRC) $(FLAGS_PLATFORM) -o tests.out
	@./tests.out

check: $(NAME)
	@echo -------------------CLANG_FORMAT------------------------
	@cppcheck *.h $(SRC)
	@echo -------------------------------------------------------
	@cp ../materials/linters/.clang-format .clang-format
	@clang-format -style=file --dry-run --Werror $(SRC) $(TEST) *.c *.h
	@rm .clang-format
	@make leaks

leaks:
	@echo -------------------MEMORY_LEAKS-------------------
	$(CC) -g $(TEST) $(SRC) $(FLAGS_PLATFORM) -o unit.out
	CK_FORK=no leaks --atExit -- ./unit.out

clean:
	@rm -rf *.{a,o,dSYM,out}
	@rm -rf $(EXECUTABLE)
	@rm -rf gcov_report
	@rm -rf report
	@rm -rf CPPLINT.cfg
	@rm -rf *.gcda
	@rm -rf *.gcno
	@rm -rf *.info
	@rm -rf test
	@rm -rf Archive_SmartCalc_v1.0
	@rm -rf *tgz
	@rm -rf build
	@rm -rf gcov_test