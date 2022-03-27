#include <iostream>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <unistd.h>
#include <algorithm>
#include<string>
#include <vector>

using namespace std;

int main(){
    char b[64] = {0};

    // create socket descriptor in INET address family (domen id), pre-installation of the connection using TCP protocol
    int listeningSocket = socket(AF_INET, SOCK_STREAM, 0);

    //create address in INET address family, port number 8080 and tied it to all interfaces using the INADDR_ANY
    struct sockaddr_in address{AF_INET, htons(8083), htons(INADDR_ANY)};

    //bind socket to adders
    if (bind(listeningSocket, (struct sockaddr*)&address, sizeof (address)) < 0){
        cout << "bind < 0\nERROR!!!\n";
        return -1;
    }

    socklen_t size = sizeof address;
    vector <int> clients;

    listen(listeningSocket, 3);
    while (true){
        fd_set readSet; //файловые дескрипторы, в которые будут проверяться с помощью select
        FD_ZERO(&readSet); //чистим массив с дескрипторами
        FD_SET(listeningSocket, &readSet);  // добавляем слушающий сокет
        for (int it : clients){  //добавляем дескрипторы сокетов, на которые приходят сообшения от клиентов
            FD_SET(it, &readSet);
        }
        int mx; //первый аргумент для srlrct
        if (!clients.empty()){
            mx = max(listeningSocket, *max_element(clients.begin(), clients.end())) + 1;
        } else{
            mx = listeningSocket + 1;
        }
        if (select(mx, &readSet, nullptr, nullptr, nullptr) <= 0){ //ждем изменений в readSet
            cout << "Problem: select <= 0\n";
        }
        if (FD_ISSET(listeningSocket, &readSet)){   // если изменился слушающий сокет, то добавляем клиента
            int sock = accept(listeningSocket, (struct sockaddr *)&address, &size);
            clients.push_back(sock);
        }
        for(int i = 0; i < clients.size(); i += 1){ // если изменился один из клиентских сокетов, то читаем содержимое буффера
            if (FD_ISSET(clients[i], &readSet)){
                recv(clients[i], b, sizeof b, 0);
                cout << clients[i] << ": " << string(b) << '\n';
                if (string(b) == "-q"){ // условный флаг выхода клиента
                    close(clients[i]);
                    clients.erase(clients.begin() + i);
                }
            }
        }
    }
    return 0;
}
