LIBRARIES = -lcoap-1
INCLUDE = -I /usr/local/include/coap/
CXXFLAGS = -O2 -D_GNU_SOURCE -DWITH_POSIX

all: getdemo postdemo putdemo client server demo-server
	
server:server.c
	gcc server.c $(INCLUDE) $(LIBRARIES) $(CXXFLAGS) -g -o  server
client:client.c
	gcc client.c $(INCLUDE) $(LIBRARIES) $(CXXFLAGS) -g -o  client
getdemo:getdemo.c
	gcc getdemo.c $(INCLUDE) $(LIBRARIES) $(CXXFLAGS) -g -o  getdemo
postdemo:postdemo.c
	gcc postdemo.c $(INCLUDE) $(LIBRARIES) $(CXXFLAGS) -g -o  postdemo
putdemo:putdemo.c
	gcc putdemo.c $(INCLUDE) $(LIBRARIES) $(CXXFLAGS) -g -o  putdemo
demo-server:demo-server.c
	gcc demo-server.c $(INCLUDE) $(LIBRARIES) $(CXXFLAGS) -g -o  demo-server
clean:
	rm -rf *.o getdemo postdemo putdemo server client

