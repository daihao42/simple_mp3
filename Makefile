CFLAGS=`pkg-config --cflags gtk+-3.0 gstreamer-0.10`
LIBS=`pkg-config --libs gtk+-3.0 gstreamer-0.10`

mymp3 : gui.o mp3play.o main.o file_op.o config.o 
	gcc -g gui.o mp3play.o file_op.o main.o config.o -o mymp3 $(CFLAGS) $(LIBS)
gui.o : gui.c
	gcc -g -c gui.c -o gui.o $(CFLAGS) $(LIBS)
mp3play.o : mp3play.c
	gcc -g -c mp3play.c -o mp3play.o $(CFLAGS) $(LIBS)
main.o : main.c
	gcc -g -c main.c -o main.o $(CFLAGS) $(LIBS)
file_op.o : file_op.c
	gcc -g -c file_op.c -o file_op.o $(CFLAGS) $(LIBS)
config.o : config.c
	gcc -g -c config.c -o config.o $(CFLAGS) $(LIBS)
clean :
	rm -r -f *.o
