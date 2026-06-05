#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <stack>
#include <typeinfo>
#include <stdexcept>
#include <cstdlib>
#include <ctime>

#include "../src/core/types.h"
#include "../src/core/stats.h"
#include "../src/core/entity.h"
#include "../src/core/player.h"
#include "../src/core/enemy.h"
#include "../src/core/item.h"
#include "../src/core/consumable.h"
#include "../src/core/inventory.h"
#include "../src/core/game_manager.h"
#include "../src/core/object_pool.h"

void sep(const std::string& title) {
    std::cout << "\n========================================\n";
    std::cout << "  " << title << "\n";
    std::cout << "========================================\n";
}

// //////////////////////////////////////////////creare personaj
void creeazaPersonaj() {
    sep("CREEAZA-TI PERSONAJUL");

    std::string nume;
    std::cout << "Cum te numesti? > ";
    std::cin >> nume;

    std::cout << "\nAlege dificultatea:\n";
    std::cout << "  1. Usor  (HP: 120, ATK: 15)\n";
    std::cout << "  2. Normal (HP: 100, ATK: 20)\n";
    std::cout << "  3. Greu  (HP:  80, ATK: 25)\n";
    std::cout << "Alegere: ";
    int dif;
    std::cin >> dif;

    Stats s(100, 20, 4, 8);
    if (dif == 1) s = Stats(120, 15, 6, 7);
    if (dif == 3) s = Stats(80,  25, 2, 10);

    Player player(nume, s);
    std::cout << "\nPersonajul tau:\n";
    std::cout << player << "\n";
    std::cout << "\n" << nume << " a intrat in dungeon. Sa inceapa aventura!\n";

    std::cout << "\n (Entity::entityCount = " << Entity::getEntityCount() << ")\n";
}

// ── combat ────────────────────────────────────────────────────────
void luptaCuInamici() {
    sep("COMBAT");

    Stats ps(100, 20, 4, 8);
    Stats es1(40, 10, 2, 5);
    Stats es2(60, 15, 3, 4);

    Player player("Erou", ps);
    Enemy  goblin(1, "Goblin", es1, DamageType::PHYSICAL, 10);
    Enemy  orc(2,    "Orc",   es2, DamageType::PHYSICAL, 15);

    std::cout << "Au aparut 2 inamici!\n";
    std::cout << "  " << goblin << "\n";
    std::cout << "  " << orc    << "\n";

    std::cout << "\n--- Runda 1 ---\n";
    std::cout << player.getName() << " ataca " << goblin.getName()
              << " pentru " << player.getStats().getAttack() << " damage\n";
    player.attackEnemy(goblin);
    std::cout << goblin.getName() << " HP ramas: " << goblin.getStats().getHP() << "\n";

    std::cout << goblin.getName() << " contraataca pentru " << goblin.getDamage() << " damage\n";
    player.takeDamage(goblin.getDamage());
    std::cout << player.getName() << " HP ramas: " << player.getStats().getHP() << "\n";

    std::cout << "\n--- simulateCombat() \n";
    bool result = player.simulateCombat(orc, 5);
    std::cout << "Rezultat: " << (result ? "Victorie!" : "Meci egal") << "\n";

    std::cout << "\n Enemy::spawnedCount = " << Enemy::getSpawnedCount() << "\n";

    //////////////////////////////////////////////////////////////////////////////////////////// upcast si downcast +++ typeid
    std::cout << "\n--- Upcast & Downcast ---\n";
    Entity* e1 = &player;
    Entity* e2 = &goblin;
    std::cout << "typeid(*e1) = " << typeid(*e1).name() << "\n";
    std::cout << "typeid(*e2) = " << typeid(*e2).name() << "\n";



    Player* dp    = dynamic_cast<Player*>(e1);
    Enemy*  de    = dynamic_cast<Enemy*>(e1); ///////////////// esuat
    Enemy*  de2   = dynamic_cast<Enemy*>(e2); ///////////////// reusit
    std::cout << "dynamic_cast<Player*>(e1): " << (dp  ? "reusit" : "esuat") << "\n";
    std::cout << "dynamic_cast<Enemy*>(e1):  " << (de  ? "reusit": "nullptr — corect") << "\n";
    std::cout << "dynamic_cast<Enemy*>(e2):  " << (de2 ? "reusit": "esuat") << "\n";

    std::cout << "\nVirtual getType():\n";
    std::cout << "  e1->getType() == PLAYER: "
              << (e1->getType() == EntityType::PLAYER ? "true" : "false") << "\n";
    std::cout << "  e2->getType() == ENEMY:  "
              << (e2->getType() == EntityType::ENEMY  ? "true" : "false") << "\n";
}

// ///////////////////////////////////////////////////// ridica obiecte & inventar
void ridicaObiect() {
    sep("RIDICA OBIECTE & INVENTAR");

    Stats ps(100, 20, 4, 8);
    Player player("Erou", ps);

    std::cout << "Eroul gaseste obiecte in dungeon...\n\n";

    Item* potion1 = new Consumable("Health Potion", 20);
    Item* potion2 = new Consumable("Mega Potion",   50);
    Item* potion3 = new Consumable("Elixir",       100);

    std::cout << "Obiect gasit: " << *potion1 << " — " << potion1->describe() << "\n";
    std::cout << "Obiect gasit: " << *potion2 << " — " << potion2->describe() << "\n";
    std::cout << "Obiect gasit: " << *potion3 << " — " << potion3->describe() << "\n";

    std::cout << "\nEroul ia 40 damage...\n";
    player.takeDamage(40);
    std::cout << "HP: " << player.getStats().getHP() << "\n";

    std::cout << "\nRidica Health Potion \n";
    player.pickupItem(potion1);
    std::cout << "HP dupa potiune: " << player.getStats().getHP() << "\n";

    // std::list prin Inventory
    std::cout << "\n--- Inventar ---\n";
    Inventory inv(5);
    inv.addItem(new Consumable("Health Potion", 20));
    inv.addItem(new Consumable("Mega Potion",   50));
    std::cout << inv << "\n";
    inv.printAll();

    std::cout << "\nScoate primul item din inventar:\n";
    Item* scos = inv.popFirst();
    if (scos) {
        std::cout << "  Scos: " << *scos << "\n";
        delete scos;
    }
    std::cout << "Inventar dupa: " << inv << "\n";
    while (!inv.isEmpty()) delete inv.popFirst();

    delete potion2;
    delete potion3;
}

// ///////////////////////////////////////////////////// sistem wave
void sistemWave() {
    sep("WAVE SYSTEM ");

    GameManager* gm  = GameManager::getInstance();
    GameManager* gm2 = GameManager::getInstance();

    std::cout << "gm == gm2 (aceeasi instanta): "
              << (gm == gm2 ? "true — SINGLETON" : "false") << "\n";

    gm->startGame();
    std::cout << "\n" << *gm << "\n";

    std::cout << "\n--- Stiva de stari ---\n";
    gm->startWave();
    std::cout << "startWave: " << *gm << "\n";

    gm->onEnemyDied();
    gm->onEnemyDied();
    gm->addScore(200);
    std::cout << "2 inamici morti + 200 scor: " << *gm << "\n";

    gm->pushState(GameState::WAVE_COMPLETE);
    std::cout << "push WAVE_COMPLETE\n";
    gm->pushState(GameState::GAME_OVER);
    std::cout << "push GAME_OVER\n";
    gm->popState();
    std::cout << "pop -> inapoi la WAVE_COMPLETE\n";
    gm->popState();
    std::cout << "pop -> inapoi la PLAYING\n";
    std::cout << *gm << "\n";
}

// ///////////////////////////////////////////////////// level up
void levelUp() {
    sep("LEVEL UP [functie complexa]");

    Stats ps(100, 20, 4, 8);
    Player player("Erou", ps);

    std::cout << "Inainte de level up:\n" << player << "\n";
    player.levelUp();
    std::cout << "\nDupa level up:\n" << player << "\n";
    player.levelUp();
    std::cout << "\nDupa al doilea level up:\n" << player << "\n";
}

// ///////////////////////////////////////////////////// exceptii
void exceptii() {
    sep("EXCEPTII");

    std::cout << "Test 1: ID inamic invalid\n";
    try {
        int id = -1;
        if (id < 0)
            throw InvalidTargetException("ID inamic invalid: " + std::to_string(id));
    } catch (const InvalidTargetException& e) {
        std::cout << "  Prins: " << e.what() << "\n";
    }

    std::cout << "\nTest 2: inventar gol\n";
    try {
        Inventory inv(3);
        if (inv.isEmpty())
            throw ItemNotFoundException("Inventarul este gol");
    } catch (const ItemNotFoundException& e) {
        std::cout << "  Prins: " << e.what() << "\n";
    }

    std::cout << "\nTest 3: pool de obiecte epuizat\n";
    try {
        throw std::runtime_error("ObjectPool: niciun obiect disponibil");
    } catch (const std::runtime_error& e) {
        std::cout << "  Prins: " << e.what() << "\n";
    }
}

// ///////////////////////////////////////////////////// n inamici
void nInamici() {
    sep("SPAWNEAZA SI AFISEAZA N INAMICI");

    int n;
    std::cout << "Cati inamici vrei sa spawezi? ";
    std::cin >> n;

    std::vector<Enemy*> inamici;
    for (int i = 0; i < n; i++) {
        std::cout << "\nInamic " << (i+1) << ":\n";
        std::string nume;
        std::cout << "  Nume: ";
        std::cin >> nume;
        Stats s(1, 1, 1, 1);
        std::cin >> s;
        inamici.push_back(new Enemy(i+1, nume, s, DamageType::PHYSICAL, 10));
    }

    std::cout << "\n=== Inamicii spawezati ===\n";
    for (const auto* e : inamici)
        std::cout << *e << "\n";

    std::cout << "\nTotal inamici: " << Enemy::getSpawnedCount() << "\n";

    for (auto* e : inamici) delete e;
}

// ///////////////////////////////////////////////////// template
void templateDemo() {
    sep("TEMPLATE ObjectPool<T>");

    std::cout << "ObjectPool<T> pre-aloca obiecte si le reutilizeaza.\n";
    std::cout << "T poate fi orice tip — Enemy, Stats, Consumable, etc.\n\n";

    // instantiem real cu Stats (are constructor Stats(hp,atk,def,spd))
    ObjectPool<Stats> pool(3);
    std::cout << "Pool creat cu 3 obiecte Stats:\n";
    std::cout << "  " << pool << "\n";

    Stats* s1 = pool.acquire();
    Stats* s2 = pool.acquire();
    std::cout << "\nDupa 2 acquire():\n";
    std::cout << "  " << pool << "\n";

    pool.release(s1);
    std::cout << "\nDupa release(s1):\n";
    std::cout << "  " << pool << "\n";

    pool.release(s2);
    std::cout << "\nDupa release(s2) — toate inapoi in pool:\n";
    std::cout << "  " << pool << "\n";

    std::cout << "\nAceasi clasa functioneaza cu orice tip:\n";
    std::cout << "  ObjectPool<Stats>  pool(3);\n";
    std::cout << "  ObjectPool<Enemy>  pool(10);\n";
    std::cout << "  Acelasi cod, tipuri diferite — asta e template.\n";
}

// ///////////////////////////////////////////////////// meniu
void menu() {
    int choice = -1;
    while (choice != 0) {
        std::cout << "\n========================================\n";
        std::cout << "  ROGUELITE DUNGEON -- POO DEMO\n";
        std::cout << "========================================\n";
        std::cout << "  1. Creeaza personajul\n";
        std::cout << "  2. Lupta cu inamicii\n";
        std::cout << "  3. Ridica obiecte & inventar\n";
        std::cout << "  4. Wave system\n";
        std::cout << "  5. Level up\n";
        std::cout << "  6. Exceptii proprii\n";
        std::cout << "  7. Spawneaza N inamici\n";
        std::cout << "  8. Template ObjectPool<T>\n";
        std::cout << "  0. Iesire\n";
        std::cout << "========================================\n";
        std::cout << "Alegere: ";
        std::cin >> choice;

        switch (choice) {
            case 1: creeazaPersonaj(); break;
            case 2: luptaCuInamici();  break;
            case 3: ridicaObiect();    break;
            case 4: sistemWave();      break;
            case 5: levelUp();         break;
            case 6: exceptii();        break;
            case 7: nInamici();        break;
            case 8: templateDemo();    break;
            case 0: std::cout << "La revedere, erou!\n"; break;
            default: std::cout << "Alegere invalida.\n";  break;
        }
    }
}

int main() {
    srand(static_cast<unsigned int>(time(nullptr)));
    menu();
    GameManager::destroyInstance();
    return 0;
}