#include <iostream>
#include <list>
#include <random>
#include <memory>

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
    list<unique_ptr<Weapon>> weapons;

public:
    Monster(string name) : name(name) {}

    void addWeapon(unique_ptr<Weapon> weapon) {
        weapons.push_back(move(weapon));
    }

    Weapon* getRandomWeapon() {
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> dis(0, weapons.size() - 1);

        auto it = weapons.begin();
        advance(it, dis(gen));
        return it->get();
    }

    void destroyWeapons() {
        weapons.clear();
    }

    void removeWeapon(Weapon* weapon) {
        weapons.remove_if([&](const unique_ptr<Weapon>& w) { return w.get() == weapon; });
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
    unique_ptr<Blaster> blaster1 = make_unique<Blaster>();
    unique_ptr<Pistol> pistol1 = make_unique<Pistol>();
    unique_ptr<Pistol> pistol2 = make_unique<Pistol>();
    unique_ptr<MachineGun> machineGun1 = make_unique<MachineGun>();

    unique_ptr<Blaster> blaster2 = make_unique<Blaster>();
    unique_ptr<Pistol> pistol3 = make_unique<Pistol>();
    unique_ptr<MachineGun> machineGun2 = make_unique<MachineGun>();

    unique_ptr<Monster> monster1 = make_unique<Monster>("Monster 1");
    monster1->addWeapon(move(blaster1));
    monster1->addWeapon(move(pistol1));
    monster1->addWeapon(move(pistol2));
    monster1->addWeapon(move(machineGun1));

    unique_ptr<Monster> monster2 = make_unique<Monster>("Monster 2");
    monster2->addWeapon(move(blaster2));
    monster2->addWeapon(move(pistol3));
    monster2->addWeapon(move(machineGun2));

    monster1->shoot(monster2.get());

    monster1->destroyWeapons();
    monster2->destroyWeapons();

    return 0;
}
