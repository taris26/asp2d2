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

	pair<cvorS*, cvorK*> search(long long int key, cvorS* root) {
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
		return { root, prethodni };
	}

	void insert(cvorK* noviKljuc, cvorS** root) {
		pair<cvorS*, cvorK*> temp = search(noviKljuc->kljuc, *root);
		cvorS* cvorStabla = temp.first, * trenutniS = nullptr, * noviSt = nullptr;
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
				cvorStabla->poslednji = noviSt;
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
			cvorS* noviS = new cvorS(*cvorStabla);
			noviS->kljucPrvi = srednjiK->next;
			if(!noviS->list) noviS->prvi = srednjiS->sledeci;
			cvorStabla->sledeci = noviS;
			cvorStabla->kljucPoslednji = srednjiK;
			cvorStabla->poslednji = srednjiS;
			cvorStabla->broj = m / 2 + cvorStabla->list;
			noviS->broj = m - cvorStabla->broj;
			if (cvorStabla->list) {
				noviKljuc = new cvorK(*srednjiK);
			}
			else {
				noviKljuc = srednjiK->next;
				srednjiS->sledeci = nullptr;
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
			cvorS* pokS = cvorStabla->prvi;
			while (pokK && pokK->kljuc < noviKljuc->kljuc) {
				trenutniK = pokK;
				pokK = pokK->next;
				trenutniS = pokS;
				pokS = pokS->sledeci;
			}
		}
	}

	void print(int lvl) {
		cvorK* temp=kljucPrvi;
		cout << broj << ": ";
		for (int i = 0; i < lvl; i++) cout << "-";
		while (temp) {
			cout << temp->kljuc << " ";
			temp = temp->next;
		}
		cout << endl;
	}

	void preorder(int lvl) {
		print(lvl);
		cvorS* temp = prvi;
		while (temp) {
			temp->print(lvl + 1);
			temp = temp->sledeci;
		}
	}
};

int main() {
	m = 4;
	cvorS* root = new cvorS;
	cvorK* temp = new cvorK;
	temp->kljuc = 3;
	root->kljucPrvi = root->kljucPoslednji = temp;
	root->list = 1;
	root->broj = 1;

	temp = new cvorK;
	temp->kljuc = 4;
	root->insert(temp, &root);
	root->preorder(0);
	temp = new cvorK;
	temp->kljuc = 5;
	root->insert(temp, &root);
	root->preorder(0);
	temp = new cvorK;
	temp->kljuc = 2;
	root->insert(temp, &root);
	root->preorder(0);
	temp = new cvorK;
	temp->kljuc = 6;
	root->insert(temp, &root);
	root->preorder(0);
	temp = new cvorK;
	temp->kljuc = 1;
	root->insert(temp, &root);
	root->preorder(0);
	temp = new cvorK;
	temp->kljuc = 11;
	root->insert(temp, &root);
	root->preorder(0);
	temp = new cvorK;
	temp->kljuc = 0;
	root->insert(temp, &root);
	root->preorder(0);
	temp = new cvorK;
	temp->kljuc = 10;
	root->insert(temp, &root);
	root->preorder(0);
	temp = new cvorK;
	temp->kljuc = 12;
	root->insert(temp, &root);
	root->preorder(0);
	temp = new cvorK;
	temp->kljuc = 8;
	root->insert(temp, &root);
	root->preorder(0);
	temp = new cvorK;
	temp->kljuc = 9;
	root->insert(temp, &root);
	root->preorder(0);
	return 0;
}