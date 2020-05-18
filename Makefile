CPPFLAGS= -Wall -Werror -std=gnu++11 -pedantic -ggdb3

all: ffosh test failedtest
ffosh: ffosh.cpp parse.cpp CheckBuiltInFunc.cpp
	g++ $(CPPFLAGS) -o ffosh ffosh.cpp parse.cpp CheckBuiltInFunc.cpp
test: test.cpp
	g++ $(CPPFLAGS) -o test test.cpp
failedtest: failedtest.cpp
	g++ $(CPPFLAGS) -o failedtest failedtest.cpp

clean:
	rm -f *~ *.o ffosh test failedtest

