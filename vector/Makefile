FLAGS = -Wall -Wextra -g

main.exe: main.o vector.o
	gcc $(FLAGS) $^ -o $@

main.o: main.c 
	gcc $(FLAGS) -c $< -o $@ 

vector.o: vector.c 
	gcc $(FLAGS) -c $< -o $@ 

clean:
	rm *.o
	rm main.exe

.PHONY: clean
