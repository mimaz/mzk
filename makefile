sources = main.cc src/threaded.cc src/thread-data.cc
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

