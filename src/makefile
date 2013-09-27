main : main.o options.o network.o response.o threadpool.o
	gcc main.o options.o network.o response.o threadpool.o -o main -pthread
main.o : main.c main.h options.h network.h response.h threadpool.h
	gcc -c  main.c
options.o : options.c options.h main.h
	gcc -c  options.c
network.o : network.c network.h main.h
	gcc -c  network.c
response.o : response.c response.h main.h
	gcc -c  response.c
threadpool.o : threadpool.c threadpool.h main.h
	gcc -c  threadpool.c
.PHONY : clean
clean:
	rm -f main.o options.o network.o response.o threadpool.o
