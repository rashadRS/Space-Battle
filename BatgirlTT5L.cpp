/*

--- Batgirl TT5L ---

1. SYAHMI SAIF BIN SUHAIMI
2. MUHAMMAD ADAM SAFWAN BIN SAFRI
3. AHMAD ALJAMMAL
4. SOFAN RASHADD ALI QAID

*/
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <random>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <algorithm>
#include <cctype>

// ------------------- Person -------------------
class Person {
public:
    Person(std::string name, std::string role)
        : name_(std::move(name)), role_(std::move(role)) {}

    const std::string& name() const { return name_; }
    const std::string& role() const { return role_; }

private:
    std::string name_;
    std::string role_;
};

// ------------------- Ship (abstract) -------------------
class Ship {
public:
    Ship(std::string name, int hp)
        : name_(std::move(name)), hp_(hp) {}

    virtual ~Ship() = default;

    const std::string& name() const { return name_; }
    int hp() const { return hp_; }

    void addCrew(const Person& p) { crew_.push_back(p); }

    bool isDestroyed() const { return hp_ <= 0; }

    // Apply damage
    Ship& operator-=(int dmg) {
        hp_ -= dmg;
        if (hp_ < 0) hp_ = 0;
        return *this;
    }

    // Pure virtual: every ship must define its attack
    virtual int fire() const = 0;

    // Debug helper
    void printStatus() const {
        std::cout << name_ << " HP=" << hp_ << "\n";
    }

protected:
    std::string name_;
    int hp_;
    std::vector<Person> crew_;
};

// ------------------- Ship Subclasses -------------------
class Guerriero : public Ship {
public:
    Guerriero(std::string name) : Ship(std::move(name), 120) {}
    int fire() const override { return 25; }
};

class Medio : public Ship {
public:
    Medio(std::string name) : Ship(std::move(name), 100) {}
    int fire() const override { return 20; }
};

class Corazzata : public Ship {
public:
    Corazzata(std::string name) : Ship(std::move(name), 200) {}
    int fire() const override { return 40; }
};

class Jager : public Ship {
public:
    Jager(std::string name) : Ship(std::move(name), 90) {}
    int fire() const override { return 18; }
};

class Kreuzer : public Ship {
public:
    Kreuzer(std::string name) : Ship(std::move(name), 130) {}
    int fire() const override { return 28; }
};

class Fregatte : public Ship {
public:
    Fregatte(std::string name) : Ship(std::move(name), 80) {}
    int fire() const override { return 15; }
};

// ======================
//     read file 
// - Rashad Sofan 
// ======================
namespace {
    std::string trim(const std::string& value) {
        const auto first = value.find_first_not_of(" \t\r\n");
        if (first == std::string::npos) {
            return "";
        }
        const auto last = value.find_last_not_of(" \t\r\n");
        return value.substr(first, last - first + 1);
    }

    // Parse a single CSV line into fields, handling commas and quotes safely.
    std::vector<std::string> parseCsvLine(const std::string& line) {
        std::vector<std::string> fields;
        std::string field;
        bool inQuotes = false;

        for (size_t i = 0; i < line.size(); ++i) {
            char c = line[i];
            if (c == '"') {
                if (inQuotes && i + 1 < line.size() && line[i + 1] == '"') {
                    field += '"';
                    ++i;
                } else {
                    inQuotes = !inQuotes;
                }
            } else if (c == ',' && !inQuotes) {
                fields.push_back(trim(field));
                field.clear();
            } else {
                field += c;
            }
        }

        fields.push_back(trim(field));
        return fields;
    }

    bool readCsvFile(const std::string& path, std::vector<std::vector<std::string>>& rows) {
        std::ifstream file(path);
        if (!file.is_open()) {
            std::cerr << "Warning: Unable to open CSV file: " << path << "\n";
            return false;
        }

        std::string line;
        while (std::getline(file, line)) {
            if (line.empty()) {
                continue;
            }
            rows.push_back(parseCsvLine(line));
        }

        if (rows.empty()) {
            std::cerr << "Warning: CSV file is empty: " << path << "\n";
            return false;
        }

        return true;
    }

    std::string toLower(std::string value) {
        std::transform(value.begin(), value.end(), value.begin(),
                       [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
        return value;
    }

    std::unique_ptr<Ship> createShipFromType(const std::string& type, const std::string& name) {
        const std::string normalized = toLower(type);
        if (normalized == "guerriero") {
            return std::make_unique<Guerriero>(name);
        }
        if (normalized == "medio") {
            return std::make_unique<Medio>(name);
        }
        if (normalized == "corazzata") {
            return std::make_unique<Corazzata>(name);
        }
        if (normalized == "jager") {
            return std::make_unique<Jager>(name);
        }
        if (normalized == "kreuzer") {
            return std::make_unique<Kreuzer>(name);
        }
        if (normalized == "fregatte") {
            return std::make_unique<Fregatte>(name);
        }

        std::cerr << "Warning: Unknown ship type '" << type << "' for ship '" << name << "'.\n";
        return nullptr;
    }
}

// ------------------- BattleEngine -------------------
class BattleEngine {
public:
    void addToFleetA(Ship* ship) { fleetA_.push_back(ship); }
    void addToFleetB(Ship* ship) { fleetB_.push_back(ship); }

    void runSimulation() {
        std::cout << "=== Battle Simulation Start ===\n";

        size_t i = 0;
        while (!fleetA_.empty() && !fleetB_.empty()) {
            Ship* attackerA = fleetA_[i % fleetA_.size()];
            Ship* attackerB = fleetB_[i % fleetB_.size()];

            // A attacks B
            int dmgA = attackerA->fire();
            std::cout << attackerA->name() << " fires for " << dmgA << " damage!\n";
            (*attackerB) -= dmgA;
            if (attackerB->isDestroyed()) {
                std::cout << attackerB->name() << " destroyed!\n";
                removeDestroyed(fleetB_);
            }

            if (fleetB_.empty()) break;

            // B attacks A
            int dmgB = attackerB->fire();
            std::cout << attackerB->name() << " fires for " << dmgB << " damage!\n";
            (*attackerA) -= dmgB;
            if (attackerA->isDestroyed()) {
                std::cout << attackerA->name() << " destroyed!\n";
                removeDestroyed(fleetA_);
            }

            ++i;
        }

        std::cout << "=== Battle Finished ===\n";
        if (fleetA_.empty()) {
            std::cout << "Fleet B wins!\n";
        } else if (fleetB_.empty()) {
            std::cout << "Fleet A wins!\n";
        }
    }

private:
    std::vector<Ship*> fleetA_;
    std::vector<Ship*> fleetB_;

    void removeDestroyed(std::vector<Ship*>& fleet) {
        fleet.erase(
            std::remove_if(fleet.begin(), fleet.end(),
                           [](Ship* s) { return s->isDestroyed(); }),
            fleet.end());
    }
};

// ------------------- Testing Main -------------------
int main() {
    std::vector<std::vector<std::string>> shipRows;
    std::vector<std::vector<std::string>> crewRows;
    std::unordered_map<std::string, std::vector<Person>> crewByShip;
    std::vector<std::unique_ptr<Ship>> fleetAStorage;
    std::vector<std::unique_ptr<Ship>> fleetBStorage;
    BattleEngine engine;

    const bool shipsLoaded = readCsvFile("ships.csv", shipRows);
    const bool crewLoaded = readCsvFile("crew.csv", crewRows);

    if (crewLoaded) {
        for (const auto& row : crewRows) {
            if (row.size() < 3) {
                std::cerr << "Warning: Skipping malformed crew row (expected 3 columns).\n";
                continue;
            }
            const std::string shipName = row[0];
            const std::string crewName = row[1];
            const std::string role = row[2];
            if (shipName.empty() || crewName.empty()) {
                std::cerr << "Warning: Skipping crew row with empty ship or crew name.\n";
                continue;
            }
            crewByShip[shipName].emplace_back(crewName, role);
        }
    }

    if (shipsLoaded) {
        bool assignToFleetA = true;
        for (const auto& row : shipRows) {
            if (row.size() < 2) {
                std::cerr << "Warning: Skipping malformed ship row (expected 2 columns).\n";
                continue;
            }
            const std::string shipType = row[0];
            const std::string shipName = row[1];
            if (shipType.empty() || shipName.empty()) {
                std::cerr << "Warning: Skipping ship row with empty type or name.\n";
                continue;
            }

            std::unique_ptr<Ship> ship = createShipFromType(shipType, shipName);
            if (!ship) {
                continue;
            }

            auto crewIt = crewByShip.find(shipName);
            if (crewIt != crewByShip.end()) {
                for (const auto& member : crewIt->second) {
                    ship->addCrew(member);
                }
            }

            if (assignToFleetA) {
                engine.addToFleetA(ship.get());
                fleetAStorage.push_back(std::move(ship));
            } else {
                engine.addToFleetB(ship.get());
                fleetBStorage.push_back(std::move(ship));
            }
            assignToFleetA = !assignToFleetA;
        }
    } else {
        Guerriero g1("Guerriero-1");
        Medio m1("Medio-1");
        Corazzata c1("Corazzata-1");
        Jager j1("Jager-1");
        Kreuzer k1("Kreuzer-1");
        Fregatte f1("Fregatte-1");

        g1.addCrew(Person("Alice", "Pilot"));
        g1.addCrew(Person("Bob", "Gunner"));

        m1.addCrew(Person("Celine", "Engineer"));
        c1.addCrew(Person("Dante", "Commander"));

        engine.addToFleetA(&g1);
        engine.addToFleetA(&m1);
        engine.addToFleetA(&c1);

        engine.addToFleetB(&j1);
        engine.addToFleetB(&k1);
        engine.addToFleetB(&f1);

        engine.runSimulation();
        return 0;
    }

    engine.runSimulation();

    return 0;
}
