src = main.cc mzk.cc

all: mzk

run: mzk
	./mzk

clean:
	rm -f mzk mzk.d

mzk: ${src}
	g++ main.cc mzk.cc -o $@ -std=c++14 -Wall -Og -MMD

-include mzk.d

