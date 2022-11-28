#include <iostream>
#include <string>

using namespace std;

int m;

class cvorK {
public:
	cvorK* next = nullptr;
	string* data;
	long long int kljuc;
	cvorK() = default;
	cvorK(long long int a) {
		kljuc = a;
	}
};

class cvorS {
public:
	cvorK* kljucPrvi = nullptr, * kljucPoslednji = nullptr;
	cvorS* sledeci = nullptr, * prvi = nullptr, * poslednji = nullptr, * otac = nullptr;
	bool list = 0;
	int broj = 0; //namestiti brojeve nakon splita

	//cvorS() {
	//	kljucPrvi = new cvorK( INT_MIN );
	//}

	pair<cvorS*, cvorK*> search(long long int key, cvorS* root) { //vraca pokazivac na prethodnika cvora sa trazenim kljucem
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
				if (!srednjiS->list) srednjiS->sledeci = nullptr;
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
		bool flag = 0;
		pair<cvorS*, cvorK*> temp = search(kljuc, *root);
		cvorS* cvorStabla = temp.first, * trenutniS = nullptr, * pokS = nullptr;
		cvorK* trenutniK = temp.second, * pokK = nullptr;
		if (trenutniK && trenutniK->next == cvorStabla->kljucPoslednji) { //ispraviti brisanje cvora sa jednim kljucem
			pokS = cvorStabla->otac;
			while (pokS) {
				pokK = pokS->kljucPrvi;
				while (pokK) {
					if (pokK->kljuc == kljuc) {
						break;
					}
					pokK = pokK->next;
				}
				if (pokK) {
					pokK->kljuc = trenutniK->kljuc;
					break;
				}
				pokS = pokS->otac;
			}
			cvorStabla->kljucPoslednji = trenutniK;
		}
		if (cvorStabla->broj == 1) flag = 1;
		else {
			if (trenutniK) {
				pokK = trenutniK->next;
				trenutniK->next = pokK->next;
			}
			else {
				pokK = cvorStabla->kljucPrvi;
				cvorStabla->kljucPrvi = pokK->next;
				if (cvorStabla->broj == 1) cvorStabla->kljucPoslednji = nullptr;
			}
			delete pokK;
		}
		cvorStabla->broj--;
		while (cvorStabla && cvorStabla != (*root) && cvorStabla->broj < (m / 2)) {
			pokS = cvorStabla->sledeci;
			if (!pokS || pokS->otac != cvorStabla->otac) 
				pokS = nullptr;
			if (pokS && pokS->broj >= (m / 2) + 1) {
				pokK = pokS->otac->kljucPrvi;
				while (pokK->kljuc < cvorStabla->kljucPoslednji->kljuc) {
					pokK = pokK->next;
				}
				cvorStabla->kljucPoslednji->next = pokS->kljucPrvi;
				cvorStabla->kljucPoslednji = pokS->kljucPrvi;
				pokS->kljucPrvi = pokS->kljucPrvi->next;
				cvorStabla->kljucPoslednji->next = nullptr;
				cvorStabla->broj++;
				pokS->broj--;
				int temp = cvorStabla->kljucPoslednji->kljuc;
				if (!cvorStabla->list) {
					cvorStabla->kljucPoslednji->kljuc = pokK->kljuc;
					cvorStabla->poslednji->sledeci = pokS->prvi;
					cvorStabla->poslednji = pokS->prvi;
					pokS->prvi = pokS->prvi->sledeci;
					if (!cvorStabla->poslednji->list) cvorStabla->poslednji->sledeci = nullptr;
					cvorStabla->poslednji->otac = cvorStabla;
				}
				pokK->kljuc = temp;
				break;
			}
			if (cvorStabla == cvorStabla->otac->prvi) pokS = nullptr;
			else pokS = cvorStabla->otac->prvi;
			pokK = cvorStabla->otac->kljucPrvi;
			while (pokS && pokS->sledeci != cvorStabla) {
				pokS = pokS->sledeci;
				pokK = pokK->next;
			}
			if (pokS && pokS->broj >= (m / 2) + 1) {
				cvorK* prethodniK = pokS->kljucPrvi;
				cvorS* prethodniS = nullptr;
				if(!pokS->list) prethodniS = pokS->prvi->sledeci;
				while (prethodniK->next != pokS->kljucPoslednji) {
					prethodniK = prethodniK->next;
					if (!pokS->list) prethodniS = prethodniS->sledeci;
				}
				pokS->kljucPoslednji->next = cvorStabla->kljucPrvi;
				pokS->kljucPoslednji = prethodniK;
				cvorStabla->kljucPrvi = pokS->kljucPoslednji->next;
				pokS->kljucPoslednji->next = nullptr;
				cvorStabla->broj++;
				pokS->broj--;
				int temp = pokS->kljucPoslednji->kljuc;
				if (!cvorStabla->list) {
					cvorStabla->kljucPrvi->kljuc = pokK->kljuc;
					pokS->poslednji->sledeci = cvorStabla->prvi;
					pokS->poslednji = prethodniS;
					cvorStabla->prvi = pokS->poslednji->sledeci;
					if (!cvorStabla->poslednji->list) pokS->poslednji->sledeci = nullptr;
					cvorStabla->prvi->otac = cvorStabla;
				}
				pokK->kljuc = temp;
				break;
			}
			if (cvorStabla->sledeci) {
				cvorStabla->join(root);
				pokS = cvorStabla;
				cvorStabla = cvorStabla->otac;
			}
			else {
				pokS->join(root);
				cvorStabla = pokS->otac;
			}
			if (flag) {
				flag = 0;
				rev(kljuc, root);
			}
		}
		if (flag) {
			flag = 0;
			rev(kljuc, root);
		}
	}

	void rev(long long int kljuc, cvorS** root) {
		pair<cvorS*, cvorK*> temp = search(kljuc, *root);
		cvorS* cvorStabla = temp.first, * trenutniS = nullptr, * pokS = nullptr;
		cvorK* trenutniK = temp.second, * pokK = nullptr;
		if (trenutniK && trenutniK->next == cvorStabla->kljucPoslednji) { //ispraviti brisanje cvora sa jednim kljucem
			pokS = cvorStabla->otac;
			while (pokS) {
				pokK = pokS->kljucPrvi;
				while (pokK) {
					if (pokK->kljuc == kljuc) {
						break;
					}
					pokK = pokK->next;
				}
				if (pokK) {
					pokK->kljuc = trenutniK->kljuc;
					break;
				}
				pokS = pokS->otac;
			}
			cvorStabla->kljucPoslednji = trenutniK;
		}
		if (trenutniK) {
			pokK = trenutniK->next;
			trenutniK->next = pokK->next;
		}
		else {
			pokK = cvorStabla->kljucPrvi;
			cvorStabla->kljucPrvi = pokK->next;
		}
		delete pokK;
	}

	void join(cvorS** root) {
		cvorS* trenutniS = nullptr, * cvorR = sledeci;
		cvorK* trenutniK = otac->kljucPrvi, * prethodni = nullptr;
		while (trenutniK->kljuc < kljucPoslednji->kljuc) {
			prethodni = trenutniK;
			trenutniK = trenutniK->next;
		}
		if (!prethodni) {
			otac->kljucPrvi = trenutniK->next;
		}
		else {
			prethodni->next = trenutniK->next;
		}
		if (!trenutniK->next) {
			otac->kljucPoslednji = prethodni;
		}
		otac->broj--;
		if (!list) {
			kljucPoslednji->next = trenutniK;
			kljucPoslednji = trenutniK;
		}
		else {
			delete trenutniK;
		}
		kljucPoslednji->next = cvorR->kljucPrvi;
		kljucPoslednji = cvorR->kljucPoslednji;
		if (!list) poslednji->sledeci = cvorR->prvi;
		poslednji = cvorR->poslednji;
		broj += cvorR->broj + !list;
		sledeci = cvorR->sledeci;
		if (!sledeci) otac->poslednji = this;
		trenutniS = cvorR->prvi;
		while (trenutniS) {
			trenutniS->otac = this;
			trenutniS = trenutniS->sledeci;
		}
		delete cvorR;
		if (otac == (*root) && (*root)->broj == 0) {
			delete otac;
			otac = nullptr;
			*root = this;
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
		//cout << endl;
		cout << " | " << kljucPrvi->kljuc << " " << kljucPoslednji->kljuc;
		if (!list) cout << " | " << prvi->kljucPrvi->kljuc << " " << poslednji->kljucPrvi->kljuc;
		cout<< endl;
	}

	void preorder(int lvl) {
		print(lvl);
		cvorS* temp = prvi;
		while (temp && !list && temp != poslednji->sledeci) {
			temp->preorder(lvl + 1);
			temp = temp->sledeci;
		}
	}
};

int main() {
	m = 3;
	cvorS* root = new cvorS, * tempS = nullptr;
	cvorK* temp = new cvorK;
	temp->kljuc = 3;
	root->kljucPrvi = root->kljucPoslednji = nullptr;
	root->list = 1;
	root->broj = 0;

	int broj = 3;
	while (broj != 24) {
		cin >> broj;
		temp = new cvorK;
		temp->kljuc = broj;
		root->insert(temp, &root);
		root->preorder(0);
	}

	tempS = root->search(2, root).first;

	broj = 3;
	while (broj != -1) {
		cin >> broj;
		root->remove(broj, &root);
		root->preorder(0);
	}
	return 0;
}