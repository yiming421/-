#ifndef HEADQUARTER_H
#define HEADQUARTER_H

#include <iostream>
#include <vector>
#include <memory>
#include "City.h"
#include "Warrior.h"

using std::string; using std::vector;

class Headquarter:public City{
public:
	Headquarter(const int m, const string incolor);
	Headquarter(const Headquarter& other) = delete;
	Headquarter& operator=(const Headquarter& other) = delete;
	bool train(const vector<int>& WarriorStrength, const vector<int>& WarriorForce, const int k, const int time, const int n);//训练武士
	void report(const int time);//报告
	void taken(const int time);//被占领
	bool sendElement(Warrior& winner);//送出生命值
private:
	string color;//大本营颜色
	vector<int> warriorOrder;//武士训练顺序
	int curWarriorNum = 0;//武士编号
	void printInfo(const vector<int>& warriorStrength, const int curNum, const int time, const double morale);//train的帮助函数
	void trainWarrior(const string& toGenerate, const vector<int>& WarriorStrength, 
	const vector<int>& WarriorForce, const int k, const int n, const double morale);//train的帮助函数
};

#endif