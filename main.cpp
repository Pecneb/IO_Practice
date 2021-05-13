#include <iostream>
#include <new>
#include <sstream>
#include <cstring>
#include <string>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>

using namespace std;

struct Megallo {
	string nev;
	string erkezes;
	string indulas;
	Megallo* kov;
};

struct Jarat {
	string nev;
	Megallo* start;
	Megallo* end;
};

struct Menetrend {
	Jarat *jaratok;
	int db;
	int maxdb;
};

Menetrend betolt(string fajl) {
	Menetrend tmp_menet;
	tmp_menet.maxdb = 4;
	tmp_menet.jaratok = new Jarat[tmp_menet.maxdb];
	ifstream f(fajl);
	if(f.is_open()) {
		string data;
		for(tmp_menet.db=0; f >> data, data != "vege"; tmp_menet.db++) {
			if(tmp_menet.db==tmp_menet.maxdb) {
				tmp_menet.maxdb += 4;
				Jarat *swap = new Jarat[tmp_menet.maxdb];
				for(int i=0; i<tmp_menet.db; i++) {
					swap[i] = tmp_menet.jaratok[i];
				}
				delete[] tmp_menet.jaratok;
				tmp_menet.jaratok = swap;
			}
			tmp_menet.jaratok[tmp_menet.db].nev = data;
			tmp_menet.jaratok[tmp_menet.db].start = NULL;
			tmp_menet.jaratok[tmp_menet.db].end = NULL;
			while(f >> data, data != "---") {
				Megallo *uj = new Megallo;
				uj->nev = data;
				f >> uj->erkezes;
				f >> uj->indulas;
				if(tmp_menet.jaratok[tmp_menet.db].start == NULL) tmp_menet.jaratok[tmp_menet.db].start=uj;
				if(tmp_menet.jaratok[tmp_menet.db].end != NULL) tmp_menet.jaratok[tmp_menet.db].end->kov=uj; 
				tmp_menet.jaratok[tmp_menet.db].end = uj;
			}
		}
		f.close();
	}
	return tmp_menet;
}

void PrintSchedule(Menetrend &m) {
	for(int i=0; i<m.db; i++) {
		cout << "Jarat neve: " << m.jaratok[i].nev << '\n';
		Megallo *aktm = m.jaratok[i].start;
		while(aktm!=NULL) {
			cout << aktm->nev << '\t' << aktm->erkezes << '\t' << aktm->indulas << '\n';
			aktm = aktm->kov;
		}
		cout << '\n';
	}
};

void felszabadit(Menetrend &m) {
	for(int i=0; i<m.db; i++) {
		Megallo *akt = m.jaratok[i].start;
		while(akt != NULL) {
			Megallo *kov = akt->kov;
			delete akt;
			akt = kov;
		}
	}
}

int main(int argc, char** argv) {
	Menetrend m = betolt("vonat.txt");
	// PrintSchedule(m);
	string inputStart;
	string inputEnd;
	cout << "Ajda meg az Indulas es Vegallomas nevet\n"
			"es a program kiirja, melyik jarat mikor indul es erkezik.\n";
	do {
		cout << "Indulasi allomas: "; cin >> inputStart;
		if(inputStart!="vege") {
			bool van = false;
			cout << "Vegallomas: "; cin >> inputEnd;
			for(int i=0; i<m.db; i++) {
				if(m.jaratok[i].start->nev == inputStart && m.jaratok[i].end->nev == inputEnd) {
					cout << "Jarat neve: " << m.jaratok[i].nev << '\n';
					cout << "Inulas idopontja: " << m.jaratok[i].start->indulas << '\n';
					cout << "Erkezes a vegallomasra: " << m.jaratok[i].end->erkezes << '\n';
					van = true;
				}
			}
			if(!van) cout << "Nincs ilyen jarat.\n";
		}
		cout << endl;
	} while(inputStart!="vege");
	felszabadit(m);
	return 0;
}
