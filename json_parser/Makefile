FLAGS = -Wall -Wextra -g

main.exe: main.o strutils.o vector.o hashtable.o list.o json_parser.o
	gcc $(FLAGS) $^ -o $@

main.o: main.c
	gcc $(FLAGS) -c $< -o $@ -I"../"

strutils.o: ../strutils/strutils.c 
	gcc $(FLAGS) -c $< -o $@ -I"../"

vector.o: ../vector/vector.c
	gcc $(FLAGS) -c $< -o $@ -I"../"

hashtable.o: ../hashtable/hashtable.c
	gcc $(FLAGS) -c $< -o $@ -I"../"

list.o: ../list/list.c
	gcc $(FLAGS) -c $< -o $@ -I"../"

json_parser.o: ../json_parser/json_parser.c
	gcc $(FLAGS) -c $< -o $@ -I"../"

clean:
	rm *.o
	rm main.exe

.PHONY: clean
