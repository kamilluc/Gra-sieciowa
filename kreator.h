#pragma once
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

class kreator{
	public:
	string nick;
	int hp;
	int atak;
	int obrona;

	void zapiszDoPliku();
	void wczytajZPliku();
	kreator();
	kreator(string imie, int zycie, int pktAtaku, int pktObrony);
};
