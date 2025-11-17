# Pokeapi
api for Pokeapi.co pokemon info site
# main
```cpp
#include "Pokeapi.h"
#include <iostream>

int main() {
   Pokeapi api;

    auto berry = api.get_berry_list().then([](json::value result) {
        std::cout << "Search results: " << result.serialize() << std::endl;
    });
    berry.wait();
    
    return 0;
}
```

# Launch (your script)
```
g++ -std=c++11 -o main main.cpp -lcpprest -lssl -lcrypto -lpthread -lboost_system -lboost_chrono -lboost_thread
./main
```
