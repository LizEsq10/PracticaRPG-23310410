//
// Created by Victor Navarro on 15/02/24.
//

#include "Player.h"
#include <iostream>
#include <string.h>

#include "../Files/FileHandler.h"

using namespace std;

Player::Player(char* _name, int _health, int _attack, int _defense, int _speed) : Character(_name, _health, _attack, _defense, _speed, true) {
    level = 1;
    experience = 0;
}
Player::Player(char* _name, int _health, int _attack, int _defense, int _speed, bool _isPlayer, int _level, int _experience) : Character(_name, _health, _attack, _defense, _speed, _isPlayer)
{
    level = _level;
    experience = _experience;
}

void Player::doAttack(Character *target) {
    target->takeDamage(attack);
}

void Player::takeDamage(int damage) {

    damage = damage * level;

    int trueDamage = damage - getDefense();

    if (trueDamage < 0) {
        trueDamage = 0;
    }

    health-= trueDamage >= 0 ? trueDamage: 0;
    cout << name << " took " << (trueDamage >= 0 ? trueDamage: 0) << " damage!" << endl;
    cout << "Remainig health: " + to_string(this->getHealth())<<endl;

    if(health <= 0) {
        cout << name << " has been defeated!" << endl;
    }
}

void Player::levelUp() {
    level++;
    health += 10;
    attack += 10;
    defense += 10;
    speed += 10;

    cout << name << " ha subido al nivel " << level << endl;
    cout << "Estadisticas: " <<  endl;
    cout << "Salud: " << health << ", Ataque: " << attack << ", Defensa: " << defense << ", Velocidad: " << speed << endl;


}

void Player::gainExperience(int exp) {
    experience += exp;
    if (experience >= 100) {
        experience = experience - 100;
        levelUp();
        cout << "Experiencia restante: " << experience << endl;
    }

}
void Player::saveProgress()
{


    char* buffer = this ->serialize();
    FileHandler file_handler = FileHandler();
    file_handler.writeToFile("Jugador.data", buffer, Player::BUFFER_SIZE);

    cout << "Se ha guardado tus datos !!" << endl;
}
Character* Player::selectTarget(vector<Enemy*> possibleTargets) {
    int selectedTarget = 0;
    bool invalid = true;
    do {
        cout << "-------------------------------" << endl;
        cout << "Select a target: " << endl;
        for (int i = 0; i < possibleTargets.size(); i++) {
            cout << i << ". " << possibleTargets[i]->getName() << endl;
        }
        cin >> selectedTarget;
        for (int i = 0; i < possibleTargets.size(); i++) {
            if (selectedTarget == i) {
                invalid = false;
                break;
            }
        }
        if (invalid) {
            cout << "Invalid option" << endl;
        }
    } while (invalid);
    return possibleTargets[selectedTarget];
}

Action Player::takeAction(vector<Enemy*> enemies) {
    int action;
    cout << "Select an action: " << endl
         << "1. Attack" << endl
         << "2. Defend" << endl
         <<" 3. Guardar partida " << endl;

    cin >> action;
    Action currentAction;
    Character* target = nullptr;
    switch(action) {
        case 1:
            target = selectTarget(enemies);
        currentAction.target = target;
        currentAction.action = [this, target](){
            doAttack(target);
            if (target -> getHealth() <= 0)
            {
                this->gainExperience(((Enemy *) target)->getExperience());

            }
        };
        currentAction.speed = getSpeed();
        break;
        case 2:
            currentAction.target = target;
        currentAction.action = [this]()
        {
            Defense();
        };
        currentAction.speed = 999999;
        break;
    case 3:
        saveProgress();
        return takeAction(enemies);
        break;
        default:
            cout << "Invalid action" << endl;
            currentAction.action = nullptr;
            break;
    }


    return currentAction;
}
char* Player::serialize(){
    char* iterator = buffer;

    memcpy(iterator,&name, sizeof(name));
    iterator += sizeof(name);

    memcpy(iterator, &health, sizeof(health));
    iterator += sizeof(health);

    memcpy(iterator,&attack,sizeof(attack));
    iterator += sizeof(attack);

    memcpy(iterator,&defense,sizeof(defense));
    iterator += sizeof(defense);

    memcpy(iterator,&speed,sizeof(speed));
    iterator += sizeof(speed);

    memcpy(iterator,&isPlayer,sizeof(isPlayer));
    iterator += sizeof(isPlayer);

    memcpy(iterator,&level,sizeof(level));
    iterator += sizeof(level);
    memcpy(iterator,&experience,sizeof(experience));
    iterator += sizeof(experience);

    return buffer;

}

Player* Player::unserialize(char* buffer) {
    char *iterator = buffer;

    char name[40];
    int health, attack, defense, speed, level, experience;
    bool isPlayer;

    memcpy(&name, iterator, sizeof(name));
    iterator += sizeof(name);

    memcpy(&health, iterator, sizeof(health));
    iterator += sizeof(health);

    memcpy(&attack, iterator, sizeof(attack));
    iterator += sizeof(attack);

    memcpy(&defense, iterator, sizeof(defense));
    iterator += sizeof(defense);

    memcpy(&speed, iterator, sizeof(speed));
    iterator += sizeof(speed);

    memcpy(&isPlayer, iterator, sizeof(isPlayer));
    iterator += sizeof(isPlayer);

    memcpy(&level, iterator, sizeof(level));
    iterator += sizeof(level);

    memcpy(&experience, iterator, sizeof(experience));
    iterator += sizeof(experience);

    return new Player(name, health, attack, defense, speed, isPlayer, level, experience);

}


