
#include "PoCemon.h"
#include "PoCemonData.h"
#include "Enums.h"
#include <cmath>
#include <iostream>
#include <PoCemon.h>
#include <Combat.h>
#include <Enums.h>
#include <PoCemonData.h>
#include <corecrt_math.h>
#include <cstdlib>
#include <fstream>
#include <string>

template <class BasicAttributeReturn>
BasicAttributeReturn Pocemon::getBasicAttribute(BasicAttribute type) const	{
	if (type == BasicAttribute::id) return id;
	if (type == BasicAttribute::level) return level;
	if (type == BasicAttribute::name) return name;
	if (type == BasicAttribute::type1) return type1;

	return type2;
}

int Pocemon::getStat(Stat type) const	{
	if (type == Stat::hp) return hp;
	if (type == Stat::attack) return atk;
	if (type == Stat::speed) return speed;
	if (type == Stat::spAttack) return spAtk;

	return spDef;
}
//////////////////////////////////////////////////////////////
int Pocemon::getCurrentStat(Stat type) const { 
		//int getCurrentStat(Stat type) const; - in declaration
		if (type == Stat::hpCur) return hpCur;
		if (type == Stat::attackCur) return atkCur;
		if (type == Stat::defenseCur) return defCur;
		if (type == Stat::speedCur) return speedCur;
		if (type == Stat::spAttackCur) return spAtkCur;

		return spDefCur;
	}
/////////////////////////////////////////////////////////////
void Pocemon::setCurrentStat(Stat type, int temp) const {
		// void Pocemon::setCurrentStat(Stat type, int) const;
		if (type == Stat::hpCur) {
			hpCur = temp; 
		}
		if (type == Stat::attackCur) {
			attackCur = temp;
		}
		if (type == Stat::defenseCur) {
			defenseCur = temp;
		}
		if (type == Stat::speedCur) {
			speedCur = temp;
		}
		if (type == Stat::spAttackCur) {
			spAttackCur = temp;
		}
		if (type == Stat::spDefCur) { 
			spAttackCur = temp;
		}
	}
////////////////////////////////////////////////////////////////
Pocemon::Pocemon(const PkmnId &selectedId, const int &lvl)
{
    int tempId = static_cast<int>(selectedId);

    id = selectedId;
    level = lvl;
    name = PocemonData::allData[tempId].name;
    type1 = PocemonData::allData[tempId].type1;
    type2 = PocemonData::allData[tempId].type2;

    baseHp = PocemonData::allData[tempId].baseHp;
    baseAtk = PocemonData::allData[tempId].baseAtk;
    baseDef = PocemonData::allData[tempId].baseDef;
    baseSpeed = PocemonData::allData[tempId].baseSpeed;
    baseSpAtk = PocemonData::allData[tempId].baseSpAtk;
    baseSpDef = PocemonData::allData[tempId].baseSpDef;

    // Normally, IV values are a random int from 0 to 15
    // TODO: Need to implement randomization.
    ivHp = 0;
    ivAtk = 0;
    ivDef = 0;
    ivSpeed = 0;
    ivSpAtk = 0;
    ivSpDef = 0;

    // EV values start at 0 and increase with each PoCemon defeated!
    evHp = 0;
    evAtk = 0;
    evDef = 0;
    evSpeed = 0;
    evSpAtk = 0;
    evSpDef = 0;
    //current stats//////////
	int hpCur;
	int attackCur;
	int defenseCur;
	int speedCur;
	int spAttackCur;
	int spDefCur; 
    ////////////////////////
    hp = calculateStat(level, baseHp, ivHp, evHp, true);
    atk = calculateStat(level, baseAtk, ivAtk, evAtk);
    def = calculateStat(level, baseDef, ivDef, evDef);
    speed = calculateStat(level, baseSpeed, ivSpeed, evSpeed);
    spAtk = calculateStat(level, baseSpAtk, ivSpAtk, evSpAtk);
    spDef = calculateStat(level, baseSpDef, ivSpDef, evSpDef);
}


int Pocemon::calculateStat(const int &lvl,
                           const int &baseStat,
                           const int &ivStat /* = 0 */,
                           const int &evStat /* = 0 */,
                           const bool &isHp /* = false*/)
{
    int stat;

    if (isHp)
        stat = lvl + 10;
    else
        stat = 5;

    // Note: Decimal is intentionally truncated.
    // Simplified version of the formula:
    // stat += (lvl * (2 * baseStat)) / 100;
    stat += (lvl * (2 * (baseStat + ivStat) + (sqrt(evStat) / 4))) / 100;
    return stat;
}
//////////////////////////////////////////////////////////////////////////
//global variable count, to count the battle number
	int battleCOUNT = 1; 
//////////////////////////////////////////////////////////////////////////
void Pocemon::doDamage(const Type &typeATK, const int &power, Pocemon *pocemon1, Pocemon *pocemon2) {
		
		//saving information to the file 
		cout << "Saving data..." << endl;
		ofstream input;
		myfile.open("DATA.txt");
		input << "DATA FOR" << pocemon1->Pocemon::getBasicAttribute(id) << endl;
		input << "Statistics before the fight#" << battleCOUNT << ":::" << endl << endl;
		input << "current HP:" << endl;
		input << to_string(pocemon1->Pocemon::getCurrentStat(hpCur));
		input << "current attack:" << endl;
		input << to_string(pocemon1->Pocemon::getCurrentStat(attackCur));
		input << "current defense:" << endl;
		input << to_string(pocemon1->Pocemon::getCurrentStat(defenseCur));
		input << "current speed:" << endl;
		input << to_string(pocemon1->Pocemon::getCurrentStat(speedCur));
		input << "current special attack:" << endl;
		input << to_string(pocemon1->Pocemon::getCurrentStat(spAttackCur));
		input << "current special defense:" << endl;
		input << to_string(pocemon1->Pocemon::getCurrentStat(spDefCur));
	
		
		//random factor: if it is odd, the value is rounded down; if it is even, the value is rounded up
		int randomint = rand() % 10000;

		//calculating damage for pocemon1
		modifier = Combat::getDmgMultiplier(typeATK, (pocemon2->Pocemon::getBasicAttribute(type1)), (pocemon2->Pocemon::getBasicAttribute()));  //there is no argument for the second getter, right? 
		if (randomint % 2 == 0) {
			int damage = floor(((((2 + (2 * (pocemon2->Pocemon::getBasicAttribute(level)) / 5))* power * ((pocemon2->Pocemon::getCurrentStat(attackCur)) / pocemon2->Pocemon::getCurrentStat())) / 50) + 2)*modifier); // no argument for the last fucntion as well, right? 
		}
		else {
			int damage = ceil(((((2 + (2 * (pocemon2->Pocemon::getBasicAttribute(level)) / 5))* power * ((pocemon2->Pocemon::getCurrentStat(attackCur)) / pocemon2->Pocemon::getCurrentStat())) / 50) + 2)*modifier); // no argument for the last fucntion as well, right? 
		}
	        //doing damage to pocemon1
		/*first - figure out what type of the attack it is, them do damage to a specefic zone */
		/*
		PHYSICAL (for the attack and defense)
         Normal
         Fighting
         Flying
	 Ground
         Rock
         Bug
         Ghost
         Poison
         Steel
		SPECIAL (for special attack and defence)
         Water
         Grass
         Fire
         Ice
         Electric
         Psychic
	 Dragon
         Dark
		 HP drain, could not find the moves on the list
	Absorb
	Bouncy Bubble
	Drain Punch
	Draining Kiss
	Dream Eater
	Giga Drain
	Horn Leech
	Leech Life
	Leech Seed
	Mega Drain
	Oblivion Wing
	Parabolic Charge
		*/
		//could not find the moves/attacks for speed, so I wrote in two ways: first one - does demage to all the current 
		// stats of the pokemon, except health and speed; another one - does demage ONLY to health of the pokemon.  
		/*
		if (typeATK == Normal || 
            typeATK == Fighting || 
            typeATK == Flying ||
            typeATK == Ground ||
            typeATK == Rock ||
            typeATK == Bug ||
            typeATK == Ghost ||
            typeATK == Poison ||
            typeATK == Steel) {
	               int spDefCur = pocemon1->Pocemon::getCurrentStat(attackCur) - damage;
			pocemon1->Pocemon::setCurrentStat(attackCur, newattackCur);
			int spDefCur = pocemon1->Pocemon::getCurrentStat(defenseCur) - damage;
			pocemon1->Pocemon::setCurrentStat(defenseCur, newdefenseCur);
			}
			if (typeATK == Water ||
                typeATK == Grass ||
                typeATK == Fire ||
                typeATK == Ice ||
	        typeATK == Electric ||
                typeATK == Psychic ||
		typeATK == Dragon ||
                typeATK == Dark) {
		                int newspAttackCur = pocemon1->Pocemon::getCurrentStat(spAttackCur) - damage;
				pocemon1->Pocemon::setCurrentStat(spAttackCur, newspAttackCur);
				int spDefCur = pocemon1->Pocemon::getCurrentStat(spDefCur) - damage;
				pocemon1->Pocemon::setCurrentStat(spDefCur, newspDefCur);
				}
		*/
	        int newhpCur = pocemon1->Pocemon::getCurrentStat(hpCur) - damage;
		pocemon1->Pocemon::setCurrentStat(hpCur, newhpCur);
	
	
                cout << "Saving data..." << endl;
		input << "DATA FOR" << pocemon1->Pocemon::getBasicAttribute(id) << endl;
		input << "Statistics after the fight#" << battleCOUNT << ":::" << endl << endl;
		input << "current HP:" << endl;
		input << to_string(pocemon1->Pocemon::getCurrentStat(hpCur));
		input << "current attack:" << endl;
		input << to_string(pocemon1->Pocemon::getCurrentStat(attackCur));
		input << "current defense:" << endl;
		input << to_string(pocemon1->Pocemon::getCurrentStat(defenseCur));
		input << "current speed:" << endl;
		input << to_string(pocemon1->Pocemon::getCurrentStat(speedCur));
		input << "current special attack:" << endl;
		input << to_string(pocemon1->Pocemon::getCurrentStat(spAttackCur));
		input << "current special defense:" << endl;
		input << to_string(pocemon1->Pocemon::getCurrentStat(spDefCur));

		battleCOUNT++; 
	}
////////////////////////////////////////////////////////////////////////////////////////////////////////
