

COMPILER:=gcc#tcc, gcc, clang
# Optimization increase code speed by 5X-6X for gcc and clang ONLY

# OS AND Processor detection 
ifeq ($(OS),Windows_NT)
    OS_FLAG := WIN32
    ifeq ($(PROCESSOR_ARCHITEW6432),AMD64)
        PROCESSOR_FLAG = AMD64
    else
        ifeq ($(PROCESSOR_ARCHITECTURE),AMD64)
            PROCESSOR_FLAG := AMD64
        endif
        ifeq ($(PROCESSOR_ARCHITECTURE),x86)
            PROCESSOR_FLAG := IA32
        endif
    endif
else
    UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S),Linux)
        OS_FLAG := LINUX
    endif
    ifeq ($(UNAME_S),Darwin)
        PROCESSOR_FLAG := OSX
    endif
    UNAME_P := $(shell uname -p)
    ifeq ($(UNAME_P),x86_64)
        PROCESSOR_FLAG := AMD64
    endif
    ifneq ($(filter %86,$(UNAME_P)),)
        PROCESSOR_FLAG := IA32
    endif
    ifneq ($(filter arm%,$(UNAME_P)),)
        PROCESSOR_FLAG := ARM
    endif
endif

$(info $$OS_FLAG is [${OS_FLAG}])
$(info $$PROCESSOR_FLAG is [${PROCESSOR_FLAG}])
$(info $$COMPILER [$(COMPILER)])

LINUX_EXT := .bin
WIN_EXT := .exe
LINUX_PRE := ./
WIN_PRE := 

ifeq ($(COMPILER),gcc)
    FLAGS_COV = -fprofile-arcs -ftest-coverage #Debug
else
    FLAGS_COV = #Debug
endif

# FLAGS_BUILD_TYPE = -O3 -DNDEBUG #Release
FLAGS_BUILD_TYPE = -O0 -g #Debug

# FLAGS_ERROR := -Wall -pedantic-errors
FLAGS_ERROR := -w
INCLUDE_ALL := -I. 

# astyle detection: isASTYLE is empty unless astyle exists
ifeq ($(OS_FLAG),WIN32)
	EXTENSION := $(WIN_EXT)
    PREFIX := $(WIN_PRE)
	isASTYLE := $(shell where astyle)
    CFLAGS := ${INCLUDE_ALL} ${FLAGS_BUILD_TYPE} ${FLAGS_ERROR}
else
	EXTENSION := $(LINUX_EXT)
    PREFIX := $(LINUX_PRE)
	isASTYLE := $(shell type astyle)
    CFLAGS := ${INCLUDE_ALL} ${FLAGS_BUILD_TYPE} ${FLAGS_ERROR} -lm
endif

# $(info $$isASTYLE is [$(isASTYLE)])
$(info $$EXTENSION is [$(EXTENSION)])

ifeq ($(isASTYLE),)
	ASTYLE :=
else
	ASTYLE := astyle 
endif

EXEC := $(PREFIX)test$(EXTENSION)
EXEC_ALL := ${EXEC} ${EXEC_TCC} ${EXEC_GCC} ${EXEC_CLANG}

.PHONY: all 
all: ${ASTYLE} $(EXEC) run 
SOURCES_TEST := test.c
SOURCES_NOURSMATH := nmath.c
HEADERS := $(wildcard *.h)
SOURCES_ALL := $(SOURCES_TEST)
TARGETS_NOURSMATH := $(SOURCES_NOURSMATH:.c=.o)
TARGETS_NOURSMATH_TCC := $(SOURCES_NOURSMATH:.c=_tcc.o)
TARGETS_NOURSMATH_GCC := $(SOURCES_NOURSMATH:.c=_gcc.o)
TARGETS_NOURSMATH_CLANG := $(SOURCES_NOURSMATH:.c=_clang.o)
EXEC_GCC := $(PREFIX)test_gcc$(EXTENSION)
EXEC_TCC := $(PREFIX)test_tcc$(EXTENSION)
EXEC_CLANG := $(PREFIX)test_clang$(EXTENSION)
TARGETS_ALL := ${TARGETS_NOURSMATH} ${EXEC_GCC} ${EXEC_TCC} ${EXEC_CLANG}

.PHONY: compile_test
compile_test: ${ASTYLE} ${EXEC_TCC} ${EXEC_GCC} ${EXEC_CLANG} tcc gcc clang

.PHONY : cov
cov:  $(TARGETS_NOURSMATH) $(EXEC) run ; lcov -c --no-external -d . -o main_coverage.info ; genhtml main_coverage.info -o out

.PHONY : run
run: $(EXEC); $(EXEC)
.PHONY : tcc
tcc: $(EXEC_TCC) ; $(EXEC_TCC)
.PHONY : gcc
gcc: $(EXEC_GCC) ; $(EXEC_GCC)
.PHONY : clang
clang: $(EXEC_CLANG) ; $(EXEC_CLANG)
.PHONY : astyle
astyle: $(HEADERS) $(SOURCES_ALL); astyle --style=java --indent=spaces=4 --indent-switches --pad-oper --pad-comma --pad-header --unpad-paren  --align-pointer=middle --align-reference=middle --add-braces --add-one-line-braces --attach-return-type --convert-tabs --suffix=none *.h *.c

$(EXEC): $(SOURCES_TEST) $(TARGETS_NOURSMATH); ${COMPILER} $< $(TARGETS_NOURSMATH) -o $@ $(CFLAGS) $(FLAGS_COV)

$(TARGETS_NOURSMATH) : $(SOURCES_NOURSMATH) ; $(COMPILER) $< -c -o $@ $(FLAGS_COV)
$(TARGETS_NOURSMATH_TCC) : $(SOURCES_NOURSMATH) ; tcc $< -c -o $@ 
$(TARGETS_NOURSMATH_GCC) : $(SOURCES_NOURSMATH) ; gcc $< -c -o $@ 
$(TARGETS_NOURSMATH_CLANG) : $(SOURCES_NOURSMATH) ; clang $< -c -o $@ 

$(EXEC_TCC): $(SOURCES_TEST) $(TARGETS_NOURSMATH_TCC); tcc $< $(TARGETS_NOURSMATH_TCC) -o $@ $(CFLAGS)
$(EXEC_GCC): $(SOURCES_TEST) $(TARGETS_NOURSMATH_GCC); gcc $< $(TARGETS_NOURSMATH_GCC) -o $@ $(CFLAGS)
$(EXEC_CLANG): $(SOURCES_TEST) $(TARGETS_NOURSMATH_CLANG); clang $< $(TARGETS_NOURSMATH_CLANG) -o $@ $(CFLAGS)

.PHONY: clean
clean: ; @echo "Cleaning noursmath" & rm -frv $(TARGETS_ALL) $(EXEC_ALL) *.gcda *.gcno *.gcov *.info *.bin *.exe *.o 
