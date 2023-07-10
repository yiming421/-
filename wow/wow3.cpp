#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <memory>
#include <fstream>
#include "Headquarter.h"

//using std::cin; using std::cout; 
//调试用

std::ifstream cin("test.txt");
std::ofstream cout("result.txt");

using std::vector; using std::unique_ptr;

void oneattack(unique_ptr<Warrior>& attacker, unique_ptr<Warrior>& defender, const int city, const int time){//武士的一次攻击
	defender->hurt(attacker->attack());
	attacker->attackReport(*defender, time, city);
	if(defender->getStrength() > 0){
		int temp = defender->reattack();
		attacker->hurt(temp);
		if(defender->getName() != "ninja") defender->reattackReport(*attacker, time, city);
	}
}

void read(int& m, int& n, int& k, int& t, vector<int>& warriorStrength, vector<int>& warriorForce){//读入数据
	cin >> m >> n >> Weapen::arrowForce >> k >> t;
	warriorStrength.push_back(0);
	warriorForce.push_back(0);
	for(int i = 0; i < 5; ++i){
		int temp = 0;
		cin >> temp;
		warriorStrength.push_back(temp);
	}
	for(int i = 0; i < 5; ++i){
		int temp = 0;
		cin >> temp;
		warriorForce.push_back(temp);
	}
}

template <typename T>
void lionRunhelp(T& city, const int time){//狮子逃跑的辅助函数
	auto& redWarrior = city.getRedWarrior();
	auto& blueWarrior = city.getBlueWarrior();
	if((redWarrior != nullptr && redWarrior->getName() == "lion" && redWarrior->getLoyalty() <= 0)){
		redWarrior->run(time);
		redWarrior.reset();
	}
	if((blueWarrior != nullptr && blueWarrior->getName() == "lion" && blueWarrior->getLoyalty() <= 0)){
		blueWarrior->run(time);
		blueWarrior.reset();
	}
}

void lionRun(Headquarter& red, Headquarter& blue, vector<City>& cities, const int time){//狮子逃跑
	lionRunhelp(red, time);
	for(auto& city:cities) lionRunhelp(city, time);
	lionRunhelp(blue, time);
}

template<typename T1, typename T2>
unique_ptr<Warrior>& WarriorMarchHelp(T1& start, const string& color, T2& end){//武士前进的辅助函数
	auto& start_redWarrior = start.getRedWarrior();
	auto& end_redWarrior = end.getRedWarrior();
	auto& start_blueWarrior = start.getBlueWarrior();
	auto& end_blueWarrior = end.getBlueWarrior();
	if(color == "red"){
		if(start_redWarrior != nullptr){
			end_redWarrior = std::move(start_redWarrior);
			end_redWarrior->march();
		}
		return end_redWarrior;
	}
	else{
		if(start_blueWarrior != nullptr){
			end_blueWarrior = std::move(start_blueWarrior);
			end_blueWarrior->march();
		} 
		return end_blueWarrior;
	}
}

void warriorMarch(vector<City>& cities, const int time, Headquarter& red, Headquarter& blue, bool& flag, const int n){//武士前进
	int blueInRed = (red.getBlueWarrior() != nullptr? red.getBlueWarrior()->getNum() : -1);
	int redInBlue = (blue.getRedWarrior() != nullptr? blue.getRedWarrior()->getNum() : -1);
	auto& blueWarrior = WarriorMarchHelp(cities[1], "blue", red);
	for(int i = 2; i <= n; ++i) WarriorMarchHelp(cities[i], "blue", cities[i - 1]);
	WarriorMarchHelp(blue, "blue", cities[n]);
	auto& redWarrior =  WarriorMarchHelp(cities[n], "red", blue);
	for(int i = n - 1; i >= 1; --i) WarriorMarchHelp(cities[i], "red", cities[i + 1]);
	WarriorMarchHelp(red, "red", cities[1]);
	if(blueWarrior != nullptr){
		if(blueInRed == -1)
			blueWarrior->reachHeadquarter(time);
		if(blueInRed != -1 && blueWarrior->getNum() != blueInRed){
			blueWarrior->reachHeadquarter(time);
			red.taken(time);
			flag = 1;
		}
	}
	for(auto& city: cities){
		auto& redWarriors = city.getRedWarrior();
		auto& blueWarriors = city.getBlueWarrior();
		if(redWarriors != nullptr) redWarriors->marchReport(time, city.getIndex());
		if(blueWarriors != nullptr) blueWarriors->marchReport(time, city.getIndex());
	}
	if(redWarrior != nullptr){
		if(redInBlue == -1)
			redWarrior->reachHeadquarter(time);
		if(redInBlue != -1 && redWarrior->getNum() != redInBlue){
			redWarrior->reachHeadquarter(time);
			blue.taken(time);
			flag = 1;
		}
	}
}

void generateElement(vector<City>& cities){//城市生成生命元
	for(auto& city: cities) city.setElement(city.getElement() + 10);
}

void gainReportHelp(Warrior& warrior, const int time, const int city, const int element){//武士获取生命元报告的辅助函数
	cout << std::setfill('0') << std::setw(3) << time / 60 << ":" << std::setfill('0') << std::setw(2) << time % 60;
	cout << " " << warrior.getColor() << " " << warrior.getName() << " " << warrior.getNum() << " earned ";
	cout <<  element << " elements for his headquarter" << std::endl;
}

void gainReport(City& city, const int time){//武士获取生命元的报告
	auto& redWarrior = city.getRedWarrior();
	auto& blueWarrior = city.getBlueWarrior();
	if(redWarrior != nullptr && blueWarrior == nullptr && city.getElement() > 0) 
		gainReportHelp(*redWarrior, time, city.getIndex(), city.getElement());
	if(redWarrior == nullptr && blueWarrior != nullptr && city.getElement() > 0) 
		gainReportHelp(*blueWarrior, time, city.getIndex(), city.getElement());
}

void gainElementAll(vector<City>& cities, Headquarter& red, Headquarter& blue, const int time){//武士获取全部生命元
	for(auto& city: cities){
		auto& redWarrior = city.getRedWarrior();
		auto& blueWarrior = city.getBlueWarrior();
		if(redWarrior != nullptr && blueWarrior == nullptr && city.getElement() > 0){
			gainReport(city, time);
			red.setElement(city.getElement() + red.getElement());
			city.setElement(0);
		}
		if(redWarrior == nullptr && blueWarrior != nullptr && city.getElement() > 0){
			gainReport(city, time);
			blue.setElement(city.getElement() + blue.getElement());
			city.setElement(0);
		}
	}
}

void gainElement(vector<City>& cities, Headquarter& red, Headquarter& blue, const int time){//武士获取生命元
	for(auto& city: cities){
		auto& redWarrior = city.getRedWarrior();
		auto& blueWarrior = city.getBlueWarrior();
		if(redWarrior != nullptr && blueWarrior == nullptr && city.getElement() > 0){
			red.setElement(city.getElement() + red.getElement());
			city.setElement(0);
		}
		if(redWarrior == nullptr && blueWarrior != nullptr && city.getElement() > 0){
			blue.setElement(city.getElement() + blue.getElement());
			city.setElement(0);
		}
	}
}

int testoneFightResult(City& city, Warrior redWarrior, Warrior blueWarrior){//测试一次战斗结果
	bool flag = (city.getFlag() == "red" || (city.getFlag() == "" && city.getIndex() % 2 == 1));
	if(!flag){
		redWarrior.hurt(blueWarrior.attack());
		if(redWarrior.getStrength() > 0 && redWarrior.getName() != "ninja")
			blueWarrior.hurt(redWarrior.reattack());
	}
	else{
		blueWarrior.hurt(redWarrior.attack());
		if(blueWarrior.getStrength() > 0 && blueWarrior.getName() != "ninja") 
			redWarrior.hurt(blueWarrior.reattack());
	}
	if(redWarrior.getStrength() <= 0) return -1;
	if(blueWarrior.getStrength() <= 0) return 1;
	else return 0;
}

void warriorBoom(vector<City>& cities, const int time){//武士放炸弹
	for(auto& city: cities){
		auto& redWarrior = city.getRedWarrior();
		auto& blueWarrior = city.getBlueWarrior();
		if(redWarrior != nullptr && blueWarrior != nullptr){
			if(redWarrior->getStrength() <= 0 || blueWarrior->getStrength() <= 0) continue;
			if(testoneFightResult(city, *redWarrior, *blueWarrior) == 1 && blueWarrior->useBomb()){
				blueWarrior->bombReport(*redWarrior, time);
				blueWarrior.reset();
				redWarrior.reset();
			}
			else if(testoneFightResult(city, *redWarrior, *blueWarrior) == -1 && redWarrior->useBomb()){
				redWarrior->bombReport(*blueWarrior, time);
				redWarrior.reset();
				blueWarrior.reset();
			}  
		}
	}
}

void warriorShotHelp(unique_ptr<Warrior>& attacker, unique_ptr<Warrior>& defender, const int time){//武士射箭的辅助函数
	if(attacker != nullptr && defender != nullptr){
		defender->hurt(attacker->shot());
		if(defender->getStrength() <= 0){
			defender->setStrength(0);
			defender->setarrayed();
			attacker->shotReport(*defender, time);
		}
		else attacker->shotReport(time);
	}
}

void warriorShot(vector<City>& cities, const int time){//武士射箭
	if(cities.size() >= 3 && cities[1].getRedWarrior() != nullptr && cities[2].getBlueWarrior() != nullptr){
		auto& redWarrior = cities[1].getRedWarrior();
		auto& blueWarrior = cities[2].getBlueWarrior();
		warriorShotHelp(redWarrior, blueWarrior, time);
	}
	for(int i = 2; i < cities.size() - 1; ++i){
		auto& redWarrior = cities[i].getRedWarrior();
		auto& blueWarrior = cities[i].getBlueWarrior();
		auto& nextRedWarrior = cities[i - 1].getRedWarrior();
		auto& nextBlueWarrior = cities[i + 1].getBlueWarrior();
		warriorShotHelp(redWarrior, nextBlueWarrior, time);
		warriorShotHelp(blueWarrior, nextRedWarrior, time);
	}
	if(cities.size() >= 3 && cities[cities.size() - 1].getBlueWarrior() != nullptr && cities[cities.size() - 2].getRedWarrior() != nullptr){
		auto& blueWarrior = cities[cities.size() - 1].getBlueWarrior();
		auto& redWarrior = cities[cities.size() - 2].getRedWarrior();
		warriorShotHelp(blueWarrior, redWarrior, time);
	}
}

void fight(vector<City>& cities, const int time, Headquarter& red, Headquarter& blue){//开战！！！
	for(auto& city: cities){
		auto& redWarrior = city.getRedWarrior();
		auto& blueWarrior = city.getBlueWarrior();
		int redOriElement = 0, blueOriElement = 0;
		if(redWarrior != nullptr) redOriElement = redWarrior->getStrength();
		if(blueWarrior != nullptr) blueOriElement = blueWarrior->getStrength();
		bool flag = (city.getFlag() == "red" || (city.getFlag() == "" && city.getIndex() % 2 == 1));
		if(redWarrior != nullptr && redWarrior->getarrayed() == 1 && blueWarrior == nullptr) redWarrior.reset();
		if(blueWarrior != nullptr && blueWarrior->getarrayed() == 1 && redWarrior == nullptr) blueWarrior.reset();
		if(redWarrior != nullptr && blueWarrior != nullptr){
			if(redWarrior->getStrength() <= 0 && blueWarrior->getStrength() <= 0){
				redWarrior.reset();
				blueWarrior.reset();
				continue;
			}
			if(flag && redWarrior->getStrength() > 0 && blueWarrior->getStrength() > 0)
				oneattack(redWarrior, blueWarrior, city.getIndex(), time);
			if((!flag) && blueWarrior->getStrength() > 0 && redWarrior->getStrength() > 0)
				oneattack(blueWarrior, redWarrior, city.getIndex(), time);
			if(redWarrior->getStrength() <= 0){
				if(redWarrior->getarrayed() == 0) redWarrior->kill(time, city.getIndex());
				if(blueWarrior->getStrength() > 0){
					if(redWarrior->getName() == "lion") blueWarrior->setStrength(blueWarrior->getStrength() + redOriElement);
					blueWarrior->changeMorale(0.2);
					if(!flag) blueWarrior->yell(time, city.getIndex());
					blueWarrior->capture(*redWarrior);
					city.setCnt("red", 0);
					city.setCnt("blue", city.getBlueCnt() + 1);
					blue.sendElement(*blueWarrior);
					redWarrior.reset();
					gainReport(city, time);
					if(city.getBlueCnt() >= 2 && city.getFlag() != "blue")
						city.flagRaise("blue", city.getIndex(), time);
				}
				else redWarrior.reset();
			}
			if(blueWarrior->getStrength() <= 0){
				if(blueWarrior->getarrayed() == 0) blueWarrior->kill(time, city.getIndex());
				if(redWarrior->getStrength() > 0){
					if(blueWarrior->getName() == "lion") redWarrior->setStrength(redWarrior->getStrength() + blueOriElement);
					redWarrior->changeMorale(0.2);
					if(flag) redWarrior->yell(time, city.getIndex());
					redWarrior->capture(*blueWarrior);
					city.setCnt("blue", 0);
					city.setCnt("red", city.getRedCnt() + 1);
					red.sendElement(*redWarrior);
					blueWarrior.reset();
					gainReport(city, time);
					if(city.getRedCnt() >= 2 && city.getFlag() != "red")
						city.flagRaise("red", city.getIndex(), time);
				}
				else blueWarrior.reset();
			}
			if((redWarrior != nullptr && blueWarrior != nullptr) && (blueWarrior->getStrength() > 0) && (redWarrior->getStrength() > 0)){
				redWarrior->changeMorale(-0.2);
				blueWarrior->changeMorale(-0.2);
				redWarrior->decreasedLoyalty();
				blueWarrior->decreasedLoyalty();
				if(!flag) blueWarrior->yell(time, city.getIndex());
				if(flag) redWarrior->yell(time, city.getIndex());
				city.setCnt("red", 0);
				city.setCnt("blue", 0);
			}
		}
	}
	gainElement(cities, red, blue, time);
}

void reportWeapen(vector<City>& cities, const int time, Headquarter& red, Headquarter& blue){//武器报告
	for(auto& city: cities){
		auto& redWarrior = city.getRedWarrior();
		if(redWarrior != nullptr) redWarrior->report(time);
	}
	if(blue.getRedWarrior() != nullptr) blue.getRedWarrior()->report(time);
	if(red.getBlueWarrior() != nullptr) red.getBlueWarrior()->report(time);
	for(auto& city: cities){
		auto& blueWarrior = city.getBlueWarrior();
		if(blueWarrior != nullptr) blueWarrior->report(time);
	}
}

int main(){
	int T = 0;
	cin >> T;
	for(int i = 1; i <= T; ++i){
		int m = 0, k = 0, t = 0, n = 0;
		vector<int> warriorStrength;
		vector<int> warriorForce;
		read(m, n, k, t, warriorStrength, warriorForce);
		cout << "Case " << i << ":" << std::endl;
		int time = 0;
		vector<City> cities;
		for(int i = 0; i <= n; ++i) cities.push_back(City(i));
		Headquarter red(m, "red"), blue(m, "blue");
		bool flag = 0;
		while(true){
			red.train(warriorStrength, warriorForce, k, time, n);
			blue.train(warriorStrength, warriorForce, k, time, n);
			time += 5;
			if(time > t) break;
			if(flag) break;
			lionRun(red, blue, cities, time);
			time += 5;
			if(time > t) break;
			warriorMarch(cities, time, red, blue, flag, n);
			if(flag) break;
			time += 10;
			if(time > t) break;
			generateElement(cities);
			time += 10;
			if(time > t) break;
			gainElementAll(cities, red, blue, time);
			time += 5;
			if(time > t) break;
			warriorShot(cities, time);
			time += 3;
			if(time > t) break;
			warriorBoom(cities, time);
			time += 2;
			if(time > t) break;
			fight(cities, time, red, blue);
			time += 10;
			if(time > t) break;
			red.report(time);
			blue.report(time);
			time += 5;
			if(time > t) break;
			reportWeapen(cities, time, red, blue);
			time += 5;
			if(time > t) break;
		}
	}
	return 0;
}