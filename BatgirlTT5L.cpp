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

    BattleEngine engine;
    engine.addToFleetA(&g1);
    engine.addToFleetA(&m1);
    engine.addToFleetA(&c1);

    engine.addToFleetB(&j1);
    engine.addToFleetB(&k1);
    engine.addToFleetB(&f1);

    engine.runSimulation();

    return 0;
}
