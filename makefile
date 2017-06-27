sources = main.cc
builddir = /tmp/mzk

objects = ${sources:%.cc=${builddir}/%.o}
depfiles = ${objects:%.o=%.d}

all: mzk

run: mzk
	./mzk

clean:
	rm -f mzk ${builddir} -r

mzk: ${objects}
	g++ ${objects} -o $@

${builddir}/%.o: %.cc
	@mkdir -p ${dir $@}
	g++ -c $< -o $@ -std=c++14 -Wall -Og -MMD

-include ${depfiles}

