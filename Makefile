current_dir = $(shell pwd)
compile:
	echo "If you want to use BearShellC anywhere, make sure you add the directory that contains the BearShellC Binary to the path!"
	cd $(current_dir)
	gcc main.c -o BearShellC -lreadline
	./BearShellC