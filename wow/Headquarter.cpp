#include <iostream>
#include <iomanip>
#include <vector>
#include <memory>
#include <fstream>
#include "City.h"
#include "Headquarter.h"
#include "Warrior.h"

using std::string; using std::vector; 
//using std::cout;
extern std::ofstream cout;

Headquarter::Headquarter(const int m, const string incolor):color(incolor), City(0, m){
	if(incolor == "red") warriorOrder = {0, 3, 4, 5, 2, 1};
	else warriorOrder = {0, 4, 1, 2, 3, 5};
}

void Headquarter::trainWarrior(const string& toGenerate, const vector<int>& WarriorStrength, 
const vector<int>& WarriorForce, const int k, const int n, const double morale){
	const vector<string> weapens = {"sword", "bomb", "arrow"};
	if(toGenerate == "dragon")
		if(color == "red") redWarrior = (std::make_unique<Dragon>("dragon", WarriorStrength[1], WarriorForce[1], 
		curWarriorNum, color, n, morale));
		else blueWarrior = (std::make_unique<Dragon>("dragon", WarriorStrength[1], WarriorForce[1],
		curWarriorNum, color, n, morale));
	if(toGenerate == "ninja")
		if(color == "red") redWarrior = (std::make_unique<Ninja>("ninja", WarriorStrength[2], WarriorForce[2],
		curWarriorNum, color, n));
		else blueWarrior = (std::make_unique<Ninja>("ninja", WarriorStrength[2], WarriorForce[2],
		curWarriorNum, color, n));
	if(toGenerate == "iceman")
		if(color == "red") redWarrior = (std::make_unique<Iceman>("iceman", WarriorStrength[3], WarriorForce[3],
		curWarriorNum, color, n));
		else blueWarrior = (std::make_unique<Iceman>("iceman", WarriorStrength[3], WarriorForce[3],
		curWarriorNum, color, n));
	if(toGenerate == "lion")
		if(color == "red") redWarrior = (std::make_unique<Lion>("lion", WarriorStrength[4], WarriorForce[4],
		curWarriorNum, lifeVal, k, color, n));
		else{blueWarrior = (std::make_unique<Lion>("lion", WarriorStrength[4], WarriorForce[4],
		curWarriorNum, lifeVal, k, color, n));}
	if(toGenerate == "wolf")
		if(color == "red") redWarrior = (std::make_unique<Wolf>("wolf", WarriorStrength[5], WarriorForce[5],
		curWarriorNum, color, n));
		else blueWarrior = (std::make_unique<Wolf>("wolf", WarriorStrength[5], WarriorForce[5],
		curWarriorNum, color, n));
}

bool Headquarter::train(const vector<int>& WarriorStrength, const vector<int>& WarriorForce, const int k, 
const int time, const int n){
	const vector<string> name{" ", "dragon", "ninja", "iceman", "lion", "wolf"};
	int curNum = warriorOrder[(curWarriorNum) % 5 + 1];
	string toGenerate = name[curNum];
	int cost = WarriorStrength[curNum];
	if(cost <= lifeVal){
		curWarriorNum++;
		lifeVal -= cost;
		double morale = static_cast<double>(lifeVal) / cost;
		trainWarrior(toGenerate, WarriorStrength, WarriorForce, k, n, morale);
		printInfo(WarriorStrength, curNum, time, morale);
		return 1;
	}
	return 0;
}

void Headquarter::printInfo(const vector<int>& WarriorStrength, const int curNum, const int time, const double morale){
	const vector<string> name{" ", "dragon", "ninja", "iceman", "lion", "wolf"};
	cout << std::setfill('0') << std::setw(3) << time / 60 << ":" << std::setfill('0') << std::setw(2) << time % 60;
	cout << " " << color << " " << name[curNum] <<" " << curWarriorNum << " born" << std::endl;
	if(name[curNum] == "lion")
		cout << "Its loyalty is " << lifeVal << std::endl;
	if(name[curNum] == "dragon")
		cout << "Its morale is " << std::fixed << std::setprecision(2) <<morale << std::endl;
}

void Headquarter::report(const int time){
	cout << std::setfill('0') << std::setw(3) << time / 60 << ":" << std::setfill('0') << std::setw(2) << time % 60;
	cout << " " << lifeVal << " elements in " << color << " headquarter" << std::endl;
}

void Headquarter::taken(const int time){
	cout << std::setfill('0') << std::setw(3) << time / 60 << ":" << std::setfill('0') << std::setw(2) << time % 60;
	cout << " " <<  color << " headquarter was taken" << std::endl;
}

bool Headquarter::sendElement(Warrior& winner){
	if(lifeVal <= 8) return 0;
	else{
		winner.setStrength(winner.getStrength() + 8);
		lifeVal -= 8;
		return 1;
	}
}

