build:
	echo "no need to build just copy paste the sources and it will work"

format:
	clang-format -i *.h *.c

clean:
	rm -rf *.out *.o *.s *.gch
