#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include "kreator.h"
using namespace std;

void kreator::zapiszDoPliku(){
	fstream plik;
	plik.open("../Release/Images/save.txt", ios::out);
	plik<<nick<<" "<<hp<<" "<<atak<<" "<<obrona;
}

void kreator::wczytajZPliku(){
	fstream plik;
	plik.open("../Release/Images/save.txt", ios::in);
	plik>>nick;
	plik>>hp;
	plik>>atak;
	plik>>obrona;
}

kreator::kreator(){
	cout<<endl<<"Podaj swoj nick: ";
	cin>>nick;

	cout<<endl<<"Witaj "<<nick<<" masz 30pkt do wydania na atak, obrone i zycie, rozdziel je wedlug uznania.\n";
	int pkt=30;

	cout<<"Podaj swoja ilosc zycia (max "<<pkt<<"pkt): ";
	cin>>hp;
	pkt-=hp;

	cout<<"Podaj swoja ilosc ataku (max "<<pkt<<"pkt): ";
	cin>>atak;
	pkt-=atak;
	

	cout<<"Pozostale punkty zostaly przydzielone na obrone czyli "<<pkt<<endl<<endl;
	obrona=pkt;
	
}

kreator::kreator(string imie="NoName", int zycie=10, int pktAtaku=10, int pktObrony=10){
	nick=imie;
	hp=zycie;
	atak=pktAtaku;
	obrona=pktObrony;
}