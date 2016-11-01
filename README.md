Instructions:
run g++ -std=c++14 -o run_masudio_server MasudioServer.cpp MainRouterHandler.cpp StorageImpl.cpp HomePageHTML.cpp -lproxygenhttpserver -lfolly -lglog -lgflags -pthread

and then run sudo ./run_masudio_server

sudo is needed if you're running on port 80.

and then from a different terminal on the same host, run curl -X POST -d "heyeh" -v localhost:80
