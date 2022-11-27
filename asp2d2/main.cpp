#include <iostream>
#include <string>

using namespace std;

int m;

class cvorK {
public:
	cvorK* next = nullptr;
	string* data;
	long long int kljuc;
};

class cvorS {
public:
	cvorK* kljucPrvi = nullptr, * kljucPoslednji = nullptr;
	cvorS* sledeci = nullptr, * prvi = nullptr, * poslednji = nullptr, * otac = nullptr;
	bool list = 0;
	int broj = 0; //namestiti brojeve nakon splita

	pair<cvorS*, cvorK*> search(long long int key, cvorS* root) { //vraca pokazivac na cvor sa kljucem, ako je neuspesno vraca prethodnika cvora
		while (root->list == 0) {
			cvorK* kljucP = root->kljucPrvi;
			root = root->prvi;
			while (kljucP && kljucP->kljuc < key) {
				kljucP = kljucP->next;
				root = root->sledeci;
			}
		}
		cvorK* kljucP = root->kljucPrvi, * prethodni = nullptr;
		while (kljucP && kljucP->kljuc < key) {
			prethodni = kljucP;
			kljucP = kljucP->next;
		}
		if (prethodni->next->kljuc == key) prethodni = prethodni->next;
		return { root, prethodni };
	}

	void insert(cvorK* noviKljuc, cvorS** root) {
		pair<cvorS*, cvorK*> temp = search(noviKljuc->kljuc, *root);
		cvorS* cvorStabla = temp.first, * trenutniS = nullptr, * noviS = nullptr;
		cvorK* trenutniK = temp.second;
		while (1) {
			cvorStabla->broj++;
			if (cvorStabla->kljucPoslednji == trenutniK) {
				cvorS* pokS = cvorStabla->otac;
				while (pokS) {
					cvorK* pokK = pokS->kljucPrvi;
					while (pokK) {
						if (pokK->kljuc == noviKljuc->kljuc) {
							break;
						}
						pokK = pokK->next;
					}
					if (pokK) {
						pokK->kljuc = noviKljuc->kljuc;
						break;
					}
					pokS = pokS->otac;
				}
				cvorStabla->kljucPoslednji = noviKljuc;
				cvorStabla->poslednji = noviS;
			}
			if (trenutniK) {
				noviKljuc->next = trenutniK->next;
				trenutniK->next = noviKljuc;
			}
			else {
				noviKljuc->next = cvorStabla->kljucPrvi; //ulancavanje indeksnih cvorova na istom nivou moze ulancati pogresne
				cvorStabla->kljucPrvi = noviKljuc;
			}
			if (cvorStabla->broj < m ) {
				break;
			}
			cvorK* srednjiK = cvorStabla->kljucPrvi;
			cvorS* srednjiS = cvorStabla->prvi;
			for (int i = 1; i < m / 2 ; i++) {
				srednjiK = srednjiK->next;
				if(!cvorStabla->list) srednjiS = srednjiS->sledeci;
			}
			if (!cvorStabla->list) srednjiS = srednjiS->sledeci;
			else srednjiK = srednjiK->next;
			noviS = new cvorS(*cvorStabla);
			if (cvorStabla->list) noviS->kljucPrvi = srednjiK->next;
			else noviS->kljucPrvi = srednjiK->next->next;
			if(!noviS->list) noviS->prvi = srednjiS->sledeci;
			cvorStabla->sledeci = noviS;
			cvorStabla->kljucPoslednji = srednjiK;
			cvorStabla->poslednji = srednjiS;
			cvorStabla->broj = m / 2 + cvorStabla->list;
			noviS->broj = (m - 1) / 2;
			if (cvorStabla->list) {
				noviKljuc = new cvorK(*srednjiK);
			}
			else {
				noviKljuc = srednjiK->next;
				srednjiS->sledeci = nullptr;
			}

			cvorS* pokS;

			if (!noviS->list) {
				pokS = noviS->prvi; //azuriranje oceva podstabala novog cvora
				while (pokS != noviS->poslednji->sledeci) {
					pokS->otac = noviS;
					pokS = pokS->sledeci;
				} 
			}

			srednjiK->next = nullptr;
			if (!cvorStabla->otac) {
				cvorStabla->otac = new cvorS;
				noviS->otac = cvorStabla->otac;
				*root = cvorStabla->otac;
				(*root)->prvi = cvorStabla;
			}
			cvorStabla = cvorStabla->otac;
			trenutniK = nullptr;
			trenutniS = cvorStabla->prvi;
			cvorK* pokK = cvorStabla->kljucPrvi;
			pokS = cvorStabla->prvi;
			while (pokK && pokK->kljuc < noviKljuc->kljuc) {
				trenutniK = pokK;
				pokK = pokK->next;
				trenutniS = pokS;
				pokS = pokS->sledeci;
			}
		}
	}

	void remove(long long int kljuc, cvorS** root) {
		pair<cvorS*, cvorK*> temp = search(kljuc, *root);
		cvorS* cvorStabla = temp.first, * trenutniS = nullptr, * noviS = nullptr;
		cvorK* trenutniK = temp.second;
	}

	void print(int lvl) {
		cvorK* temp=kljucPrvi;
		cout << broj << ": ";
		for (int i = 0; i < lvl; i++) cout << "-";
		while (temp) {
			cout << temp->kljuc << " ";
			temp = temp->next;
		}
		//cout << endl;
		cout << " | " << kljucPrvi->kljuc << " " << kljucPoslednji->kljuc;
		if (!list) cout << " | " << prvi->kljucPrvi->kljuc << " " << poslednji->kljucPrvi->kljuc;
		cout<< endl;
	}

	void preorder(int lvl) {
		print(lvl);
		cvorS* temp = prvi;
		while (temp) {
			temp->preorder(lvl + 1);
			temp = temp->sledeci;
		}
	}
};

int main() {
	m = 5;
	cvorS* root = new cvorS;
	cvorK* temp = new cvorK;
	temp->kljuc = 3;
	root->kljucPrvi = root->kljucPoslednji = temp;
	root->list = 1;
	root->broj = 1;

	int broj = 3;
	while (broj != -1) {
		cin >> broj;
		temp = new cvorK;
		temp->kljuc = broj;
		root->insert(temp, &root);
		root->preorder(0);
	}
	return 0;
}