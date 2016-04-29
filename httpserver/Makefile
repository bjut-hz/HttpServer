CFLAGS = -D_REENTRANT -Wall -pedantic -Isrc
LDLIBS = -lpthread

ifdef DEBUG
CFLAGS += -g
LDFLAGS += -g
endif


all: httpserver

httpserver: httpserver.c libthreadpool.so libhttpcore.so 
	gcc -W -Wall -o $@ -g $< ./libthreadpool.so ./libhttpcore.so ${LDLIBS}

shared: libthreadpool.so libhttpcore.so

libthreadpool.so: threadpool/threadpool.c threadpool/threadpool.h
	$(CC) -shared -fPIC ${CFLAGS} -o $@ $< ${LDLIBS}

libhttpcore.so: httpcore/httpcore.c httpcore/httpcore.h
	$(CC) -shared -fPIC ${CFLAGS} -o $@ $< ${LDLIBS}

clean:
	rm httpserver libthreadpool.so libhttpcore.so
