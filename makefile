#Builds the executables
falsh:
	gcc -o falsh falsh.c

#Remove the executables
clean:
	rm -f falsh
