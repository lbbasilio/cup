FLAGS = -Wall -Wextra -g

main.exe: main.o list.o
	gcc $(FLAGS) $^ -o $@

main.o: main.c 
	gcc $(FLAGS) -c $< -o $@ 

list.o: list.c 
	gcc $(FLAGS) -c $< -o $@ 

clean:
	rm *.o
	rm main.exe

.PHONY: clean
