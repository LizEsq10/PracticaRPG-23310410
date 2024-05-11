#include <iostream>
#include "Player/Player.h"
#include "Enemy/Enemy.h"
#include "Combat/Combat.h"
#include "Files/FileHandler.h"

Player* loadPlayerInfo() {
    try {
        char buffer[Player::BUFFER_SIZE];
        FileHandler fileHandler = FileHandler();
        fileHandler.readFromFile("Jugador.data", buffer, Player::BUFFER_SIZE);
        return Player::unserialize(buffer);
    } catch(int error) {
        //Preguntar al usuario el nombre de jugador
        //return new Player("Otro Jugador", 10, 5, 4, 3);
        return new Player("Lizy ", 40, 10, 5, 3);
    }
}

int main() {
    Player *player = loadPlayerInfo();
    cout << player -> toString() << endl;
    // Player *player = new Player("Lizy", 40, 10, 5, 3);
    Enemy *enemy = new Enemy("Economia", 15, 6, 2, 5, 100);
    Enemy *enemy2 = new Enemy("HCI", 15, 6, 2, 5, 100);

    vector<Character*> participants;

    participants.push_back(player);
    participants.push_back(enemy);
    participants.push_back(enemy2);

    Combat *combat = new Combat(participants);
    combat->doCombat();

    delete player;
    delete enemy;
    delete combat;
    return 0;
}

