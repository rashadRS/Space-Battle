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
// ======================
//     read file
// - SOFAN RASHADD ALI QAID
// ======================
#include <cctype>
// ======================
//     game engine
// - Ahmad Aljammal
// ======================
#include <cstdlib>
#include <ctime>
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
//     read file
// - SOFAN RASHADD ALI QAID
// ======================
string trimWhitespace(const string& value)
{
    size_t start = 0;
    while (start < value.size() && isspace(static_cast<unsigned char>(value[start])))
    {
        start++;
    }
    size_t end = value.size();
    while (end > start && isspace(static_cast<unsigned char>(value[end - 1])))
    {
        end--;
    }
    return value.substr(start, end - start);
}

bool parseCsvLine(const string& line, vector<string>& fields)
{
    fields.clear();
    string current;
    bool inQuotes = false;

    for (size_t i = 0; i < line.size(); i++)
    {
        char ch = line[i];
        if (ch == '"')
        {
            if (inQuotes && i + 1 < line.size() && line[i + 1] == '"')
            {
                current += '"';
                i++;
            }
            else
            {
                inQuotes = !inQuotes;
            }
        }
        else if (ch == ',' && !inQuotes)
        {
            fields.push_back(trimWhitespace(current));
            current.clear();
        }
        else
        {
            current += ch;
        }
    }

    if (inQuotes)
    {
        return false;
    }

    fields.push_back(trimWhitespace(current));
    return true;
}

bool isHeaderRow(const vector<string>& fields, const string& firstHeader)
{
    if (fields.empty())
    {
        return false;
    }
    return fields[0] == firstHeader;
}

void readCrewCsv(const string& filename, vector<Person*>& crewList)
{
    ifstream file(filename);
    if (!file.is_open())
    {
        cerr << "Warning: Unable to open crew file: " << filename << endl;
        return;
    }

    string line;
    int lineNumber = 0;
    while (getline(file, line))
    {
        lineNumber++;
        string trimmedLine = trimWhitespace(line);
        if (trimmedLine.empty())
        {
            continue;
        }

        vector<string> fields;
        if (!parseCsvLine(trimmedLine, fields))
        {
            cerr << "Warning: Malformed CSV line in " << filename << " at line " << lineNumber << endl;
            continue;
        }

        if (isHeaderRow(fields, "person_id"))
        {
            continue;
        }

        if (fields.size() != 3)
        {
            cerr << "Warning: Invalid crew row in " << filename << " at line " << lineNumber << endl;
            continue;
        }

        string personId = fields[0];
        string personName = fields[1];
        string role = fields[2];

        if (personId.empty() || personName.empty() || role.empty())
        {
            cerr << "Warning: Missing crew data in " << filename << " at line " << lineNumber << endl;
            continue;
        }

        if (role != "pilot" && role != "gunner" && role != "torpedo_handler")
        {
            cerr << "Warning: Unknown crew role '" << role << "' in " << filename << " at line " << lineNumber << endl;
            continue;
        }

        crewList.push_back(new Person(personId, personName, role));
    }
}

void readShipCsv(const string& filename, vector<Ship*>& ships)
{
    ifstream file(filename);
    if (!file.is_open())
    {
        cerr << "Warning: Unable to open ship file: " << filename << endl;
        return;
    }

    string line;
    int lineNumber = 0;
    while (getline(file, line))
    {
        lineNumber++;
        string trimmedLine = trimWhitespace(line);
        if (trimmedLine.empty())
        {
            continue;
        }

        vector<string> fields;
        if (!parseCsvLine(trimmedLine, fields))
        {
            cerr << "Warning: Malformed CSV line in " << filename << " at line " << lineNumber << endl;
            continue;
        }

        if (isHeaderRow(fields, "ship_id"))
        {
            continue;
        }

        if (fields.size() != 3)
        {
            cerr << "Warning: Invalid ship row in " << filename << " at line " << lineNumber << endl;
            continue;
        }

        string shipId = fields[0];
        string shipType = fields[1];
        string shipName = fields[2];

        if (shipId.empty() || shipType.empty() || shipName.empty())
        {
            cerr << "Warning: Missing ship data in " << filename << " at line " << lineNumber << endl;
            continue;
        }

        Ship* ship = createShip(shipId, shipType, shipName);
        if (ship == nullptr)
        {
            cerr << "Warning: Unknown ship type '" << shipType << "' in " << filename << " at line " << lineNumber << endl;
            continue;
        }

        ships.push_back(ship);
    }
}

void readAllCsvFiles(vector<Person*>& rCrew, vector<Ship*>& rShips, vector<Person*>& zCrew, vector<Ship*>& zShips)
{
    readCrewCsv("rCrew1.csv", rCrew);
    readShipCsv("rShips1.csv", rShips);
    readCrewCsv("zCrew1.csv", zCrew);
    readShipCsv("zShips1.csv", zShips);
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

bool hasActiveShips(const vector<Ship*>& ships)
{
    for (const Ship* ship : ships)
    {
        if (!ship->isDestroyed())
        {
            return true;
        }
    }
    return false;
}

Ship* selectRandomTarget(vector<Ship*>& ships)
{
    vector<Ship*> validTargets;
    for (Ship* ship : ships)
    {
        if (!ship->isDestroyed())
        {
            validTargets.push_back(ship);
        }
    }

    if (validTargets.empty())
    {
        return nullptr;
    }

    int index = rand() % validTargets.size();
    return validTargets[index];
}

bool resolveWeaponHit(Weapon* weapon, Ship* target)
{
    if (weapon == nullptr || target == nullptr)
    {
        return false;
    }

    double hitChance = 0.0;
    if (weapon->getType() == "light_cannon")
    {
        hitChance = target->getLightHitChance();
    }
    else if (weapon->getType() == "torpedo")
    {
        hitChance = target->getTorpedoHitChance();
    }

    double roll = static_cast<double>(rand()) / static_cast<double>(RAND_MAX);
    return roll <= hitChance;
}

bool performAttackRound(vector<Ship*>& attackers, vector<Ship*>& defenders)
{
    bool attacked = false;

    for (Ship* attacker : attackers)
    {
        if (attacker->isDestroyed() || !attacker->canFly())
        {
            continue;
        }

        for (Weapon* weapon : attacker->getWeapons())
        {
            if (!weapon->hasCrew())
            {
                continue;
            }

            Ship* target = selectRandomTarget(defenders);
            if (target == nullptr)
            {
                return attacked;
            }

            attacked = true;

            // roll to see if the attack hits based on target evasion
            if (resolveWeaponHit(weapon, target))
            {
                int damage = weapon->getPower();
                *target -= damage;
                cout << attacker->getName() << " hits " << target->getName()
                     << " for " << damage << " damage." << endl;
            }
            else
            {
                cout << attacker->getName() << " misses " << target->getName() << "." << endl;
            }
        }
    }

    return attacked;
}

void runBattle(vector<Ship*>& rShips, vector<Ship*>& zShips)
{
    int round = 1;

    while (hasActiveShips(rShips) && hasActiveShips(zShips))
    {
        cout << "\n--- Battle Round " << round << " ---" << endl;

        bool rAttack = performAttackRound(rShips, zShips);
        bool zAttack = performAttackRound(zShips, rShips);

        if (!rAttack && !zAttack)
        {
            cout << "Battle ended due to no valid attacks." << endl;
            return;
        }

        round++;
    }

    if (hasActiveShips(rShips))
    {
        cout << "Rogoatuskan wins the battle." << endl;
    }
    else if (hasActiveShips(zShips))
    {
        cout << "Zapezoid wins the battle." << endl;
    }
    else
    {
        cout << "Battle ended with no ships remaining." << endl;
    }
}


// testing 
int main(int argc, char* argv[]) {
    srand(time(0));

    cout << "Space Battle Simulation" << endl;
    
    vector<Person*> rCrew;
    vector<Person*> zCrew;
    vector<Ship*> rShips;
    vector<Ship*> zShips;

    readAllCsvFiles(rCrew, rShips, zCrew, zShips);

    assignCrewToShips(rShips, rCrew);
    assignCrewToShips(zShips, zCrew);

    cout << "Rogoatuskan ships loaded: " << rShips.size() << endl;
    cout << "Zapezoid ships loaded: " << zShips.size() << endl;

    for (Ship* ship : rShips)
    {
        cout << *ship << endl;
    }

    for (Ship* ship : zShips)
    {
        cout << *ship << endl;
    }

    runBattle(rShips, zShips);

    for (Ship* ship : rShips)
    {
        delete ship;
    }

    for (Ship* ship : zShips)
    {
        delete ship;
    }

    for (Person* person : rCrew)
    {
        delete person;
    }

    for (Person* person : zCrew)
    {
        delete person;
    }
    
    return 0;
}
