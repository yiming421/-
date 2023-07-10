#include "City.h"
#include "Warrior.h"
#include <iostream>
#include <iomanip>
#include <memory>
#include <vector>
#include <fstream>
using std::unique_ptr; using std::vector; 
//using std::cout;
extern std::ofstream cout;

void City::addRedWarrior(unique_ptr<Warrior>& in){
    redWarrior = std::move(in);
}

void City::addBlueWarrior(unique_ptr<Warrior>& in){
    blueWarrior = std::move(in);
}

int City::getElement() const{
    return lifeVal;
}

const string& City::getFlag() const{
    return flag;
}

int City::getRedCnt() const{
    return redCnt;
}

int City::getBlueCnt() const{
    return blueCnt;
}

void City::setCnt(const string& color, const int in){
    if(color == "red") redCnt = in;
    else blueCnt = in;
}

void City::flagRaise(const string& color, const int city, const int time){
    flag = color;
    cout << std::setfill('0') << std::setw(3) << time / 60 << ":" << std::setfill('0') << std::setw(2) << time % 60;
	cout << " " << color << " flag raised in city " << city << std::endl;
}

unique_ptr<Warrior>& City::getRedWarrior(){
    return redWarrior;
}

unique_ptr<Warrior>& City::getBlueWarrior(){
    return blueWarrior;
}

void City::setElement(const int& in){
    lifeVal = in;
}

int City::getIndex() const{
    return index;
}

void City::setWinFlag(const int in){
    winFlag = in;
}

int City::getWinFlag() const{
    return winFlag;
}