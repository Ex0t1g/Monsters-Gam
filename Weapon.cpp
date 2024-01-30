#include <iostream>
#include <list>
#include <random>

using namespace std;

class Weapon {
protected:
    int damage;
    int shootCount;
    int maxAmmo;

public:
    Weapon(int damage, int shootCount, int maxAmmo) : damage(damage), shootCount(shootCount), maxAmmo(maxAmmo) {}

    virtual int shoot() {
        if (shootCount > 0) {
            shootCount--;
            return damage;
        }
        return 0;
    }
};

class Blaster : public Weapon {
public:
    Blaster() : Weapon(10, -1, -1) {}
};

class Pistol : public Weapon {
public:
    Pistol() : Weapon(20, 6, 6) {}
};

class MachineGun : public Weapon {
public:
    MachineGun() : Weapon(5, 100, 100) {}
};

class Monster {
private:
    string name;
    list<Weapon*> weapons;

public:
    Monster(string name) : name(name) {}

    void addWeapon(Weapon* weapon) {
        weapons.push_back(weapon);
    }

    Weapon* getRandomWeapon() {
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> dis(0, weapons.size() - 1);

        auto it = weapons.begin();
        advance(it, dis(gen));
        return *it;
    }

    void destroyWeapons() {
        for (Weapon* weapon : weapons) {
            delete weapon;
        }
        weapons.clear();
    }

    void removeWeapon(Weapon* weapon) {
        weapons.remove(weapon);
        delete weapon;
    }

    bool hasWeapons() {
        return !weapons.empty();
    }

    void shoot(Monster* otherMonster) {
        cout << name << " attacks " << otherMonster->name << " with ";

        while (hasWeapons() && otherMonster->hasWeapons()) {
            Weapon* weapon = getRandomWeapon();
            int damage = weapon->shoot();

            if (damage > 0) {
                cout << name << " Attack " << otherMonster->name << " for " << weapon->shoot() << " damage!" << endl;
                otherMonster->takeDamage(damage);
            }
            else {
                removeWeapon(weapon);
            }
        }

        if (!otherMonster->hasWeapons()) {
            cout << otherMonster->name << " has been defeated!" << endl;
        }
        else if (!hasWeapons()) {
            cout << name << " has been defeated!" << endl;
        }
    }

    void takeDamage(int damage) {
    }
};

int main() {
    Blaster* blaster1 = new Blaster();
    Pistol* pistol1 = new Pistol();
    Pistol* pistol2 = new Pistol();
    MachineGun* machineGun1 = new MachineGun();

    Blaster* blaster2 = new Blaster();
    Pistol* pistol3 = new Pistol();
    MachineGun* machineGun2 = new MachineGun();

    Monster* monster1 = new Monster("Monster 1");
    monster1->addWeapon(blaster1);
    monster1->addWeapon(pistol1);
    monster1->addWeapon(pistol2);
    monster1->addWeapon(machineGun1);

    Monster* monster2 = new Monster("Monster 2");
    monster2->addWeapon(blaster2);
    monster2->addWeapon(pistol3);
    monster2->addWeapon(machineGun2);

    monster1->shoot(monster2);

    monster1->destroyWeapons();
    monster2->destroyWeapons();
    delete monster1;
    delete monster2;

    return 0;
}
