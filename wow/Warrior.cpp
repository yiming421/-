#include "Warrior.h"
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <set>
#include <fstream>
using std::string; 
//using std::cout;
extern std::ofstream cout;

void Weapen::blunt(){
	swordForce  = swordForce * 8 / 10;
}

int Weapen::arrowForce = 0;//

Warrior::Warrior(const string& _name, const int& _strength, const int& _force, const int& _num, 
const std::set<int>& _ownedWeapen, const string& _color, const int n):
	name(_name), strength(_strength), force(_force), num(_num), color(_color){
		if(_ownedWeapen.find(0) != _ownedWeapen.end()) ownedWeapen.swordForce = _force * 2 / 10;
		if(_ownedWeapen.find(2) != _ownedWeapen.end()) ownedWeapen.arrowDura = 3;
		if(_ownedWeapen.find(1) != _ownedWeapen.end()) ownedWeapen.bomb = 1;
	}

string Warrior::getColor() const{
	return color;
}

string Warrior::getName() const{
	return name;
}

int Warrior::getStrength() const{
	return strength;
}

void Warrior::setStrength(const int& in){
	strength = in;
}

int Warrior::getNum() const{
	return num;
}

Weapen& Warrior::getWeapen(){
	return ownedWeapen;
}

int Warrior::attack(){
	int ret = force + ownedWeapen.swordForce;
	ownedWeapen.blunt();
	return ret;
}

void Warrior::attackReport(const Warrior& other, const int time, const int city) const{
	cout << std::setfill('0') << std::setw(3) << time / 60 << ":" << std::setfill('0') << std::setw(2) << time % 60;
	cout << " " << color << " " << name <<" " << num << " attacked " << other.color<< " " << other.name << " " << other.num;
	cout << " in city " << city << " with " << strength << " elements and force " << force << std::endl;
}

int Warrior::reattack(){
	int ret = force / 2 + ownedWeapen.swordForce;
	ownedWeapen.blunt();
	return ret;
}

void Warrior::reattackReport(const Warrior& other, const int time, const int city) const{
	cout << std::setfill('0') << std::setw(3) << time / 60 << ":" << std::setfill('0') << std::setw(2) << time % 60;
	cout << " " << color << " " << name <<" " << num << " fought back against " << other.color<< " " << other.name << " " << other.num;
	cout << " in city " << city << std::endl;
}

int Warrior::shot() const{
	if(ownedWeapen.arrowDura) return ownedWeapen.arrowForce;
	else return 0;
}

void Warrior::shotReport(const int time){
	if(ownedWeapen.arrowDura){
		ownedWeapen.arrowDura--;
		cout << std::setfill('0') << std::setw(3) << time / 60 << ":" << std::setfill('0') << std::setw(2) << time % 60;
		cout << " " << color << " " << name <<" " << num << " shot" << std::endl;
	}
}

void Warrior::shotReport(const Warrior& other, const int time){
	if(ownedWeapen.arrowDura){
		ownedWeapen.arrowDura--;
		cout << std::setfill('0') << std::setw(3) << time / 60 << ":" << std::setfill('0') << std::setw(2) << time % 60;
		cout << " " << color << " " << name <<" " << num << " shot and killed " << other.color << " " << other.name << " " << other.num << std::endl;
	}
}

int Warrior::useBomb() const{
	return ownedWeapen.bomb;
}

void Warrior::bombReport(const Warrior& other, const int time) const{
	cout << std::setfill('0') << std::setw(3) << time / 60 << ":" << std::setfill('0') << std::setw(2) << time % 60;
	cout << " " << color << " " << name <<" " << num << " used a bomb and killed " << other.color << " " << other.name << " " << other.num << std::endl;
}

void Warrior::marchReport(const int time, const int city) const{
	cout << std::setfill('0') << std::setw(3) << time / 60 << ":" << std::setfill('0') << std::setw(2) << time % 60;
	cout << " " << color << " " << name <<" " << num << " marched to city " << city << " with ";
	cout << strength << " elements and force " << force << std::endl;
}

void Warrior::hurt(int damage){
	strength -= damage;
}

void Warrior::report(const int time) const{
	cout << std::setfill('0') << std::setw(3) << time / 60 << ":" << std::setfill('0') << std::setw(2) << time % 60;
	cout << " " << color << " " << name <<" " << num << " has ";
	if(ownedWeapen.arrowDura) cout << "arrow(" << ownedWeapen.arrowDura << ")";
	if(ownedWeapen.bomb){
		if(ownedWeapen.arrowDura) cout << ",";
		cout << "bomb";
	}
	if(ownedWeapen.swordForce){
		if(ownedWeapen.arrowDura || ownedWeapen.bomb) cout << ",";
		cout << "sword(" << ownedWeapen.swordForce << ")";
	}
	if(!ownedWeapen.arrowDura && !ownedWeapen.bomb && !ownedWeapen.swordForce) cout << "no weapon";
	cout << std::endl;
}

void Warrior::kill(const int time, const int city) const{
	cout << std::setfill('0') << std::setw(3) << time / 60 << ":" << std::setfill('0') << std::setw(2) << time % 60;
	cout << " " << color << " " << name << " " << num << " was killed in city " << city << std::endl;
}

void Warrior::reachHeadquarter(const int time) const{
	string reverseColor;
	if(color == "red") reverseColor = "blue";
	else reverseColor = "red";
	cout << std::setfill('0') << std::setw(3) << time / 60 << ":" << std::setfill('0') << std::setw(2) << time % 60;
	cout << " " << color << " " << name <<" " << num << " reached " << reverseColor << " headquarter with ";
	cout << strength << " elements and force " << force << std::endl;
}

void Warrior::setarrayed(){
	arrayed = 1;
}

bool Warrior::getarrayed() const{
	return arrayed;
}

void Dragon::yell(const int time, const int city) const{
	if(morale > 0.8){
		cout << std::setfill('0') << std::setw(3) << time / 60 << ":" << std::setfill('0') << std::setw(2) << time % 60;
		cout << " " << color << " " << name <<" " << num << " yelled in city " << city << std::endl;
	}
}

void Dragon::changeMorale(const int in){
	morale += in;
}

int Ninja::reattack(){
	return 0;
}

void Iceman::march(){
	if(cnt == 0) cnt += 1;
	else{
		cnt = 0;
		strength -= 9;
		force += 20;
		if(strength <= 0) strength = 1;
	}
}

void Wolf::capture(Warrior& other){
	auto& otherWeapen = other.getWeapen();
	if(ownedWeapen.swordForce == 0) ownedWeapen.swordForce = otherWeapen.swordForce;
	if(ownedWeapen.arrowDura == 0) ownedWeapen.arrowDura = otherWeapen.arrowDura;
	if(ownedWeapen.bomb == 0) ownedWeapen.bomb = otherWeapen.bomb;
}

void Lion::run(const int time) const{
	cout << std::setfill('0') << std::setw(3) << time / 60 << ":" << std::setfill('0') << std::setw(2) << time % 60;
	cout << " " << color << " " << name <<" " << num << " ran away" << std::endl;
}

int Lion::getLoyalty() const{
	return loyalty;
}

void Lion::decreasedLoyalty(){
	loyalty -= loss;
}

Dragon::Dragon(const string& _name, const int& _strength, const int& _force, const int& _num, const string& _color, const int n,
	const int _morale):
	Warrior(_name, _strength, _force, _num, {_num % 3}, _color, n), morale(_morale){}

Ninja::Ninja(const string& _name, const int& _strength, const int& _force, const int& _num, const string& _color, const int n):
	Warrior(_name, _strength, _force, _num, {(_num) % 3, (_num + 1) % 3}, _color, n){}

Iceman::Iceman(const string& _name, const int& _strength, const int& _force, const int& _num, const string& _color, const int n):
	Warrior(_name, _strength, _force, _num, {_num % 3}, _color, n){}

Lion::Lion(const string& _name, const int& _strength, const int& _force, const int& _num, 
const int& _loyalty, const int _loss, const string& _color, const int n):
    Warrior(_name, _strength, _force, _num, {}, _color, n), loyalty(_loyalty), loss(_loss), oriElement(_strength) {}

Wolf::Wolf(const string& _name, const int& _strength, const int& _force, const int& _num, const string& _color, const int n):
	Warrior(_name, _strength, _force, _num, {}, _color, n) {}