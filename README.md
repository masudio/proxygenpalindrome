Instructions:
run g++ -std=c++14 -o run_masudio_server MasudioServer.cpp MainRouterHandler.cpp -lproxygenhttpserver -lfolly -lglog -lgflags -pthread

and then run ./run_masudio_server

and then from a different terminal on the same host, run curl -X POST -d "heyeh" -v localhost:11000
