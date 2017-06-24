all: mzk

run: mzk
	./mzk

clean:
	rm -f mzk mzk.d

mzk: ${src}
	g++ main.cc -o $@ -std=c++14 -Wall -Og -MMD -I../../

-include mzk.d

