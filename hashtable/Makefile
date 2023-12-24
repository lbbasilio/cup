FLAGS = -Wall -Wextra -g

main.exe: main.o
	gcc $(FLAGS) $^ -o $@

main.o: main.c
	gcc $(FLAGS) -c $< -o $@ -I"../../"

clean:
	rm *.o
	rm main.exe

.PHONY: clean
