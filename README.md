Instructions:
run g++ -std=c++14 -o my_echo EchoServer.cpp EchoHandler.cpp -lproxygenhttpserver -lfolly -lglog -lgflags -pthread

and then run ./my_echo

and then from a different terminal on the same host, run curl -X POST -d "heyeh" -v localhost:11000

