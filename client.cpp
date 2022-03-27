#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstdlib>

using namespace std;

int main(){
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in address{AF_INET, htons(8083), htons(INADDR_ANY)};

    if (connect(serverSocket, (struct sockaddr *)&address, sizeof (address)) < 0){
        return -1;
    }
    char b[64] = {0};
    while (true){
        cin >> b;
        send(serverSocket, b, sizeof (b), 0);
        if (string(b) == "-q"){
            break;
        }
    }
    close(serverSocket);

    return 0;
}
