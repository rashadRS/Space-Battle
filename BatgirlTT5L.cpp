/*
--- Batgirl TT5L ---

1. SYAHMI SAIF BIN SUHAIMI
2. MUHAMMAD ADAM SAFWAN BIN SAFRI
3. AHMAD ALJAMMAL
4. SOFAN RASHADD ALI QAID
*/

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
using namespace std;

// ======================
//     PERSON class
// - SYAHMI SAIF
// ======================
class Person
{
private:
    // basic info
    string id;
    string name;
    string role;

public:
    // constructor
    Person(string id, string name, string role) : id(id), name(name), role(role) {}

    //------------ GETTERS ---------------
    string getRole() {return role;}
    string getName() {return name;}
    string getID() {return id;}
};


// ======================
//     CREW class 
//  - MUHAMMAD ADAM SAFWAN
// ======================
class Crew
{
protected:
    string id;
    string name;
    string type;

public:
    Crew(string id, string name, string type) : id(id), name(name), type(type) {}
    virtual ~Crew() {}

    string getId() const { return id; }
    string getName() const { return name; }
    string getType() const { return type; }
};

class Pilot : public Crew
{
public:
    Pilot(string id, string name) : Crew(id, name, "pilot") {}
};

class Gunner : public Crew
{
public:
    Gunner(string id, string name) : Crew(id, name, "gunner") {}
};

class TorpedoHandler : public Crew
{
public:
    TorpedoHandler(string id, string name) : Crew(id, name, "torpedo handler") {}
};

// ======================
//     WEAPON class
// - SYAHMI SAIF
// ======================
class Weapon
{
protected:
    int power;
    Person* assignedCrew;
    string weaponType;

public:
    // constructor
    Weapon(int power, string type) : power(power), assignedCrew(), weaponType(type) {}

    virtual ~Weapon() {}

    void assignCrew(Person* crew) {assignedCrew = crew;}
    bool hasCrew() {return assignedCrew != nullptr;}

    // --------------------- GETTERS ----------------------------
    int getPower() {return power;}
    string getType() {return weaponType;}
    Person* getCrew() {return assignedCrew;}
    // ----------------------------------------------------------

    virtual void fire() = 0;
};

// ======================
//     Light Cannon
// - SYAHMI SAIF
// ======================
class LightCannon : public Weapon
{
public:
    LightCannon(int power) : Weapon(power, "light_cannon") {}
    
    // battle engine will use getPower() and the target ship's hit chance
    void fire() override{
        // will be called by the battle engine
    }
    
};

// ======================
//      Torpedo
// - SYAHMI SAIF
// ======================
class Torpedo : public Weapon
{
public:
    Torpedo(int power) : Weapon(power, "torpedo") {}
    void fire() override {}
};



// ======================
//      SHIP class
// - SYAHMI SAIF
// ======================
class Ship
{
protected:
    // basic stats
    string id;
    string name;
    int hp;
    int maxHP;

    // crew reqs
    int requiredPilots;
    int currentPilots;
    int requiredGunners;
    int currentGunners;
    int requiredTorpedoHandlers;
    int currentTorpedoHandlers;

    // hit chances (ship gets HIT by enemies)
    double lightHitChance;
    double torpedoHitChance;

    // crew n weapons
    vector<Person*> crew;
    vector<Weapon*> weapons;

public:
    // constructor to initialize the core stats
    Ship(string id, string name, int hp, int reqPilots, int reqGunners, int reqTorpedoHandlers, double lcHitChance, double torpHitChance)
    : id(id), name(name), hp(hp), maxHP(hp), requiredPilots(reqPilots), currentPilots(0), requiredGunners(reqGunners), currentGunners(0), requiredTorpedoHandlers(reqTorpedoHandlers), currentTorpedoHandlers(0),
    lightHitChance(lcHitChance), torpedoHitChance(torpHitChance) {}

    // destructor for cleanup
    virtual ~Ship()
    {
        for (Weapon* w : weapons)
        {
            delete w;
        }
    }

    // pure virual function
    virtual string getShipType() = 0;

    // -------------------- GETTERS -----------------------
    string getID() {return id;}
    string getName() {return name;}
    int getHP() {return hp;}
    int getMaxHP() {return maxHP;}
    bool isDestroyed() {return hp <= 0;}

    double getLightHitChance()
    {
        if (requiredPilots == 2 && currentPilots == 1)
        {
            return lightHitChance * 1.25;
        }
        return lightHitChance;
    }
    
    double getTorpedoHitChance()
    {
        if (requiredPilots == 2 && currentPilots == 1)
        {
            return torpedoHitChance * 1.25;
        }
        return torpedoHitChance;
    }

    vector<Weapon*>& getWeapons() {return weapons;}
    const vector<Weapon*>& getWeapons() const {return weapons;}

    bool canFly() {return currentPilots>0;}

    // ------------------ CREW MANAGEMENT -----------------------
    bool addCrew(Person* person) 
    {
        string role = person->getRole();
        if (role == "pilot")
        {
            if (currentPilots < requiredPilots)
            {
                crew.push_back(person);
                currentPilots++;
                return true;
            }
        }
        else if (role == "torpedo_handler")
        {
            if (currentTorpedoHandlers < requiredTorpedoHandlers)
            {
                for (Weapon* w : weapons)
                {
                    if (w->getType() == "torpedo" && !w->hasCrew())
                    {
                        w->assignCrew(person);
                        crew.push_back(person);
                        currentTorpedoHandlers++;
                        return true;
                    }
                }
            }
        }
        return false;
    }

    // ------------------- OPERATOR OVERLOADING ----------------------
    // Operator -= : apply damage to ship

    Ship& operator-=(int damage)
    {
        hp -= damage;
        if (hp < 0) hp = 0;
        return *this;
    }

    friend ostream& operator<<(ostream& os, Ship& ship)
    {
        os << ship.getShipType() << " " << ship.name << " (HP: " << ship.hp << "/" << ship.maxHP << ")";
        return os;
    }

    // --------------- COUNT HOW MANY WEAPONS HAVE CREW -------------------
    int getActiveWeaponCount()
    {
        int count = 0;
        for (Weapon* w : weapons)
        {
            if (w->hasCrew()) count++;
        }
        return count;
    }
};

// ======================
// Zapezoid ships
// - SYAHMI SAIF
// ======================
class Guerriero : public Ship
{
public:
    Guerriero(string id, string name) : Ship(id, name, 123, 1, 1, 0, 0.26, 0.06) // 1 pilot 1 gunner 0 torp
    {
        // adds 1 LC with POWER: 96
        weapons.push_back(new LightCannon(96));
    }

    // virtual function override, ts will return the name of each ships (polymorphism)
    string getShipType() override {return "Guerriero";}
};

class Medio : public Ship
{
public:
    Medio(string id, string name) : Ship(id, name, 214, 1, 2, 0, 0.31, 0.11) // 1 pilot 2 gunners 0 torp
    {
        // adds 2 LC with POWER: 134
        weapons.push_back(new LightCannon(134));
        weapons.push_back(new LightCannon(134));
    }

    string getShipType() override {return "Medio";}
};

class Corazzata : public Ship
{
public:
    Corazzata(string id, string name) : Ship(id, name, 1031, 2, 10, 4, 0.50, 0.25) // 2 pilot 10 gunners 4 torp
    {
        // adds 10 LC with POWER: 164
        for (int i=0; i<10; i++)
        {
            weapons.push_back(new LightCannon(164));
        }

        // adds 4 torpedoes with POWER: 293
        for (int i=0; i<4; i++)
        {
            weapons.push_back(new Torpedo(293));
        }
    }

    string getShipType() override{return "Corazzata";}
};

// ======================
// Rogoatuskan ships
// - SYAHMI SAIF
// ======================
class Jager : public Ship
{
public:
    Jager(string id, string name) : Ship(id, name, 112, 1, 1, 0, 0.24, 0.05) // 1 pilot 1 gunner 0 torp
    {
        // adds 1 LC wit POWER: 101
        weapons.push_back(new LightCannon(101));
    }

    // virtual function override, ts will return the name of each ships (polymorphism)
    string getShipType() override{return "Jager";}
};

class Kreuzer : public Ship
{
public:
    Kreuzer(string id, string name) : Ship(id, name, 212, 1, 2, 0, 0.29, 0.10) // 1 pilot 2 gunners 0 torp
    {
        // adds 2 LC with POWER: 132
        weapons.push_back(new LightCannon(132));
        weapons.push_back(new LightCannon(132));
    }

    string getShipType() override{return "Kreuzer";}
};

class Fregatte : public Ship
{
public:
    Fregatte(string id, string name) : Ship(id, name, 1143, 2, 11, 5, 0.60, 0.30) // 2 pilots 11 guners 5 torp
    {
        //adds 11 LC, POWER: 159
        for (int i=0; i<11; i++)
        {
            weapons.push_back(new LightCannon(159));
        }
        // add 5 torps, POWER: 282
        for (int i=0; i<5; i++)
        {
            weapons.push_back(new Torpedo(282));
        }
    }

    string getShipType() override{return "Fregatte";}
};

// ======================
//   For CSV to object
// - SYAHMI SAIF
// ======================
Ship* createShip(string id, string type, string name) {
    if (type == "Guerriero") return new Guerriero(id, name);
    if (type == "Medio") return new Medio(id, name);
    if (type == "Corazzata") return new Corazzata(id, name);
    if (type == "Jager") return new Jager(id, name);
    if (type == "Kreuzer") return new Kreuzer(id, name);
    if (type == "Fregatte") return new Fregatte(id, name);
    return nullptr;
}


// ======================
//   Assign crew to ships
// - MUAMMAD ADAM SAFWAN
// ======================
void assignCrewToShips(vector<Ship*>& ships, vector<Person*>& crewList)
{
    // separate crew by role
    vector<Person*> pilots;
    vector<Person*> gunners;
    vector<Person*> torpedoHandlers;
    
    for (Person* p : crewList)
    {
        if (p->getRole() == "pilot")
            pilots.push_back(p);
        else if (p->getRole() == "gunner")
            gunners.push_back(p);
        else if (p->getRole() == "torpedo_handler")
            torpedoHandlers.push_back(p);
    }
    
    // assign one pilot to each ship first
    int pilotIdx = 0;
    for (Ship* ship : ships)
    {
        if (pilotIdx < pilots.size())
        {
            ship->addCrew(pilots[pilotIdx++]);
        }
    }
    
    // assign second pilots to ships that need them (Corazzata and Fregatte need 2 pilots)
    for (Ship* ship : ships)
    {
        if (pilotIdx < pilots.size())
        {
            ship->addCrew(pilots[pilotIdx++]);
        }
    }
    
    // distribute gunners across ships (round-robin style)
    int gunnerIdx = 0;
    bool assigned = true;
    while (assigned && gunnerIdx < gunners.size())
    {
        assigned = false;
        for (Ship* ship : ships)
        {
            if (gunnerIdx < gunners.size())
            {
                // try to assign gunner to light cannon
                for (Weapon* w : ship->getWeapons())
                {
                    if (w->getType() == "light_cannon" && !w->hasCrew())
                    {
                        w->assignCrew(gunners[gunnerIdx]);
                        gunnerIdx++;
                        assigned = true;
                        break;
                    }
                }
            }
        }
    }
    
    // distribute torpedo handlers across ships (round-robin style)
    int handlerIdx = 0;
    assigned = true;
    while (assigned && handlerIdx < torpedoHandlers.size())
    {
        assigned = false;
        for (Ship* ship : ships)
        {
            if (handlerIdx < torpedoHandlers.size())
            {
                if (ship->addCrew(torpedoHandlers[handlerIdx]))
                {
                    handlerIdx++;
                    assigned = true;
                }
            }
        }
    }
}


// testing 
int main(int argc, char* argv[]) {
    srand(time(0));

    cout << "Space Battle Simulation" << endl;
    
    Ship* testShip1 = new Guerriero("Z001", "Star Striker");
    Ship* testShip2 = new Jager("R001", "Shadow Hunter");
    
    cout << *testShip1 << endl;
    cout << *testShip2 << endl;
    
    *testShip1 -= 50;
    cout << "After taking 50 damage: " << *testShip1 << endl;
    
    delete testShip1;
    delete testShip2;
    
    return 0;
}
