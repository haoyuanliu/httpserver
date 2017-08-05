object=server.o Socket.o InetAddress.o httpResponse.o TimeStamp.o Util.o httpRequest.o handle.o
target=server
CXXFLAGS=-std=c++11 -g
CPP=g++ -std=c++11 -g
$(target):$(object)
	$(CPP) -o $@ $^
clean:
	rm -rf $(target) $(object)
