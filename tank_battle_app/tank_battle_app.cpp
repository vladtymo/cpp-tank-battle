#include <iostream>
#include <string>
#include "ConsoleColors.h"
using namespace std;

class Machinery
{
private:
    string type;
    string model;
protected:
    int health; // 0-100%
public:
    Machinery(string model, string type) : model(model), type(type), health(100)
    { }

    bool IsDestroyed() const
    {
        return health == 0;
    }
    int GetHealth() const { return health; }
    string GetModel() const { return model; }
    void ShowInfo() const
    {
        cout << "Vehicle: " << model << " - " << type << endl;
        cout << "Health: " << health << " / 100%" << endl;
    }
    virtual float Attack() const = 0;
    virtual void Defense(float damage) = 0;
};

class Tank : public Machinery
{
private: 
    float reloadTime;       // (R)
    float shotAccuracy;     // (A)
    float armorThickness;   // (T)
public:
    Tank(string model, float reloadT, float shotA, float armorT)
        : Machinery(model, "Tank"), 
          reloadTime(reloadT), shotAccuracy(shotA), armorThickness(armorT)
    { }

    float Attack() const override
    {
        // damage = 100 * A / R
        return 100 * shotAccuracy / reloadTime;
    }
    void Defense(float damage) override
    {
        // health damage = damage - T
        float healthDamage = damage - armorThickness;

        if (healthDamage < 0) return;

        if (this->health - healthDamage < 0)
            this->health = 0;
        else
            this->health -= healthDamage;
    }
};

void Move(Machinery& attacking, Machinery& defensing)
{
    float damage = attacking.Attack();
    SetConsoleColor(ConsoleColors::YELLOW);
    cout << attacking.GetModel() << " attacked... with damage: " << damage << "\t";

    int healthBefore = defensing.GetHealth();
    defensing.Defense(damage);
    SetConsoleColor(ConsoleColors::RED);
    cout << defensing.GetModel() << " defensed... health: " << healthBefore << " -> " << defensing.GetHealth() << endl;

    ResetConsoleColor();
}
void ShowWinner(Machinery& bm1, Machinery& bm2)
{
    SetConsoleColor(ConsoleColors::GREEN);
    if (!bm1.IsDestroyed())
        cout << bm1.GetModel() << " is WINNER !!!\n";
    if (!bm2.IsDestroyed())
        cout << bm2.GetModel() << " is WINNER !!!\n";

    ResetConsoleColor();
}

void Battle(Machinery& bm1, Machinery& bm2)
{
    do
    {
        Move(bm1, bm2);
        Move(bm2, bm1);

    } while (!bm1.IsDestroyed() && !bm2.IsDestroyed());

    ShowWinner(bm1, bm2);
}


int main()
{
    Tank t1("T-44", 30, 12.5, 28);
    Tank t2("T-70", 20, 18.5, 30);

    Battle(t1, t2);
}