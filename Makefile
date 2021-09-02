all:
	gcc main.c utils/myqueue.c utils/http.c -o out -lpthread 