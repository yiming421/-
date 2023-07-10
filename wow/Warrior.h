#ifndef WARRIOR_H
#define WARRIOR_H

#include <iostream>
#include <set>
#include <deque>

using std::string; using std::set;

struct Weapen{
    int swordForce = 0;
	int arrowDura = 0;
	int bomb = 0;
	static int arrowForce;
	void blunt();//剑钝化
};

class Warrior{
public:
	Warrior(const string& _name, const int& _strength, const int& force, const int& _num, 
	const set<int>& _ownedWeapens, const string& _color, const int n);
	string getName() const;
	int getStrength() const;
	void setStrength(const int& in);
	int getCity() const;
	int getNum() const;
	string getColor() const;
	Weapen& getWeapen();
	virtual void march(){}
	int attack();//攻击
	void attackReport(const Warrior& other, const int time, const int city) const;
	virtual int reattack();//反击
	int shot() const;//射箭
	void shotReport(const int time);
	void shotReport(const Warrior& other, const int time);
	int useBomb() const;
	void bombReport(const Warrior& other, const int time) const;
	void reattackReport(const Warrior& other, const int time, const int city) const;
	virtual void yell(const int time, const int city) const{}//欢呼
	virtual void run(const int time) const{}//逃跑
	virtual void capture(Warrior& other){}//缴获武器
	void marchReport(const int time, const int n) const;
	void hurt(int damage);//受伤
	void report(const int time) const;//报告
	void reachHeadquarter(const int time) const;//抵达司令部
	void kill(const int time, const int city) const;//杀死敌人
	void setarrayed();//设置为被箭射死
	bool getarrayed() const;//查看被箭射死
	virtual int getLoyalty() const{return 1;}
	virtual void decreasedLoyalty(){}
	virtual void changeMorale(const int in){}

protected:
	Weapen ownedWeapen;//武士拥有的武器
	int force;//武士攻击力
	int strength;//武士生命值
	const string name;//武士名字
	const int num;//武士编号
	const string color;//武士颜色
	bool arrayed = 0;//是否已经被箭射死
};

class Dragon:public Warrior{
public:
	Dragon(const string& _name, const int& _strength, const int& _force, const int& _num, const string& _color, const int n, const int morale);
	void yell(const int time, const int city) const;
	void changeMorale(const int in);
private:
	double morale;
};

class Ninja:public Warrior{
public:
	Ninja(const string& _name, const int& _strength, const int& _force, const int& _num, const string& _color, const int n);
	int reattack();
};

class Iceman:public Warrior{
public:
	Iceman(const string& _name, const int& _strength, const int& _force, const int& _num, const string& _color, const int n);
private:
	int cnt = 0;
	void march();
};

class Lion:public Warrior{
public:
	Lion(const string& _name, const int& _strength, const int& _force, const int& _num, const int& _loyalty,
	const int _loss, const string& _color, const int n);
	void run(const int time) const;
	int getLoyalty() const;
	void decreasedLoyalty();
private:
    int loyalty;
	const int loss;
	int oriElement;
};

class Wolf:public Warrior{
public:
	Wolf(const string& _name, const int& _strength, const int& _force, const int& _num, const string& _color, const int n);
	void capture(Warrior& other);
};

#endif