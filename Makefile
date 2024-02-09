build:
	gcc src/*.c -o out/bin

run:
	gcc src/*.c -o out/bin
	./out/bin

clean:
	rm out/bin
