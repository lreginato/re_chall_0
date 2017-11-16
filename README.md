This is a simple reverse engineering challenge.
The idea is not mine, i actually encountered this challenge myself, and rewrote it from the ASM.
It works with a server listening on port 4242 so you can host it on your local network.
Comment out printfs and debug info before giving the binary to your challengers.


Instructions :
You can easily compile this with GCC :
$ gcc -o vm.c
or
$ gcc -s -o vm vm.c

Place a file in the same directory, for example named flag, and write a flag in it.
$ echo "Congr4tzY0uSolvedIt!" > flag
The objective of the challenge is to read the file content.
Then, start listening :
$ ./vm