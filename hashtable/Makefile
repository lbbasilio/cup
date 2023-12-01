FLAGS = -Wall -Wextra -g

main.exe: main.o strutils.o vector.o hashtable.o
	gcc $(FLAGS) $^ -o $@

main.o: main.c
	gcc $(FLAGS) -c $< -o $@ -I"../../"

strutils.o: ../strutils/strutils.c 
	gcc $(FLAGS) -c $< -o $@ -I"../../"

vector.o: ../vector/vector.c
	gcc $(FLAGS) -c $< -o $@ -I"../../"

hashtable.o: hashtable.c
	gcc $(FLAGS) -c $< -o $@ -I"../../"

clean:
	rm *.o
	rm main.exe

.PHONY: clean
