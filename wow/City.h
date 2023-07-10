#ifndef CITY_H
#define CITY_H
#include <memory>
#include "Warrior.h"
#include <vector>
using std::unique_ptr; using std::vector;

class City{
public:
    City(const int _index, const int _lifeVal = 0): index(_index), lifeVal(_lifeVal){};
    const string& getFlag() const;
    int getElement() const;
    void setElement(const int& in);
    int getRedCnt() const;
    int getBlueCnt() const;
    void setCnt(const string& color, const int in);
    void flagRaise(const string& color, const int city, const int time);
    unique_ptr<Warrior>& getRedWarrior();
    unique_ptr<Warrior>& getBlueWarrior();
    void addRedWarrior(unique_ptr<Warrior>& in);
    void addBlueWarrior(unique_ptr<Warrior>& in);
    int getIndex() const;
    void setWinFlag(const int in);
    int getWinFlag() const;
protected:
    int winFlag = 0;
    int index = 0;
    int lifeVal = 0;
    string flag;
    int redCnt = 0;
    int blueCnt = 0;
    unique_ptr<Warrior> blueWarrior = nullptr;
    unique_ptr<Warrior> redWarrior = nullptr; 
};

#endif