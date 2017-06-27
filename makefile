sources = main.cc src/timer.cc src/timer-thread.cc
builddir = /tmp/mzk

objects = ${sources:%.cc=${builddir}/%.o}
depfiles = ${objects:%.o=%.d}

all: test

run: test
	./test

clean:
	rm -f test ${builddir} -r

test: ${objects}
	g++ ${objects} -o $@

${builddir}/%.o: %.cc
	@mkdir -p ${dir $@}
	g++ -c $< -o $@ -std=c++14 -Wall -Og -MMD -Iinclude/

-include ${depfiles}

