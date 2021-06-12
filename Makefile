compile1:
	gcc -o a1.exe Blocking2019228.c -lpthread
run1: compile1
	./a1.exe
compile2:
	gcc -o a2.exe NonBlocking2019228.c -lpthread
run2: compile2
	./a2.exe