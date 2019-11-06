CPPFLAGS=-std=gnu++98 -pedantic -Wall -Werror -ggdb3
search: search.o test-bsearch.o
	g++ -o search $(CPPFLAGS) search.o test-bsearch.o
search.o: search.cpp
	g++ -c $(CPPFLAGS) search.cpp
test-search.o: test-search.cpp
	g++ -c $(CPPFLAGS) test-search.cpp
