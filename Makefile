all:
	gcc -Wall conn_check.c -lcurl -lb64 -o shell