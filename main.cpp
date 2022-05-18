#include<iostream>
#include <string>
#include <vector>
#include <exception>
#include <typeinfo>
using namespace std;
class Banca
{
public:
    Banca();
    Banca(string);
    Banca(const Banca&);
    Banca& operator=(const Banca&);
    ~Banca() = default;
    friend istream& operator>>(istream&, Banca&);
    friend ostream& operator<<(ostream&, const Banca&);
    virtual double retragere(int cod,double suma) = 0;
    virtual vector<string> facilitati() = 0;
    virtual void verificare_cont() = 0;
    virtual double credit_bancar(int,double ) = 0;



protected:
    string nume_banca;
private:


};



//constructor de initializare fara parametrii
Banca::Banca() : nume_banca("BCR") {}
Banca::Banca(string nume) : nume_banca(nume) {}
Banca::Banca(const Banca& b) {
    nume_banca = b.nume_banca;
}
Banca& Banca::operator=(const Banca& b) {
    nume_banca = b.nume_banca;
    return *this;
}
istream& operator>>(istream& i, Banca& b) {
    i >> b.nume_banca;
    return i;
}
ostream& operator<<(ostream& o, const Banca& b) {
    o <<"nume banca: "<< b.nume_banca<<'\n';
    return o;
}


class Card :
        public Banca
{
public:
    Card();
    Card(string, int, string, double, string, string);
    Card(const Card&);
    Card& operator=(const Card&);
    ~Card() = default;
    friend istream& operator>>(istream&, Card&);
    friend ostream& operator<<(ostream&, const Card&);
    double retragere(int ,double ) override{cout<<"Doar de pe card standard sau premium se poate extrage";return -1;}
    vector<string> facilitati() override {return {};}
    void verificare_cont() override{cout<<"\nTrebuie sa aveti card standard sau premium pentru a vedea.\n";}
    bool autentificare(int cod);
    double credit_bancar (int cod,double suma) override{return 0;}
    static void set_incercari();
    static void modifica_incercari();

protected:
    int CVV;
    string data_expirare;
    double bani;
    string nr_card;
    string nume_detinator;
private:
    static int incercari;


};

int Card::incercari = 1;
void Card::set_incercari(){incercari = 1;}
void Card::modifica_incercari(){incercari++;}
//constructor de initializare fara parametrii
Card::Card() : nr_card("000000"), nume_detinator("Ionel Vasilescu"), data_expirare("12/02/2025"),
               CVV(000), bani(10000.5) {}
Card::Card(string banca, const int cvv, string nume, double cred, string data, string nr) : Banca(banca), CVV(cvv), nume_detinator(nume), bani(cred),
                                                                                            data_expirare(data), nr_card(nr) {}
Card::Card(const Card& card) : Banca(card) {
    CVV = card.CVV;
    nr_card = card.nr_card;
    nume_detinator = card.nume_detinator;
    data_expirare = card.data_expirare;
    bani = card.bani;

}
Card& Card::operator=(const Card& card) {
    Banca::operator=(card);
    CVV = card.CVV;
    nr_card = card.nr_card;
    nume_detinator = card.nume_detinator;
    data_expirare = card.data_expirare;
    bani = card.bani;
    return *this;
}

istream& operator>>(istream& i, Card& c) {
    i >> dynamic_cast<Banca&>(c) >> c.CVV>>c.bani >> c.data_expirare >> c.nr_card >> c.nume_detinator;
    return i;
}
ostream& operator<<(ostream& o, const Card& c) {
    o << dynamic_cast<const Banca&>(c) << "valoare bani: " << c.bani << "\ncvv: " << c.CVV << "\ndata expirare: " << c.data_expirare << "\n nr_card: "
      << c.nr_card << "\n nume detinator: " << c.nume_detinator<<'\n';
    return o;
}

//functionalitati
bool Card::autentificare(int cod) {
    if (cod == CVV) { cout<<"\nAti introdus codul corect.\n";return true; }
    else {
        if (incercari != 3) {
            cout << "\nMai incercati odata!\n";
            cin >> cod;
            modifica_incercari();
            autentificare(cod);
        } else {
            set_incercari();
            cout << "\n NU mai aveti incercari.\n";
            return false;
        }

    }
}


class Card_Standard :
        public Card
{
public:
    Card_Standard();
    Card_Standard(string banca, int cvv, string nume, double cred, string data, string nr, int limita, double comision);
    Card_Standard(const Card_Standard&);
    Card_Standard& operator=(const Card_Standard&);
    virtual ~Card_Standard() {limita_extragere = 0;}
    friend istream& operator>>(istream&, Card_Standard&);
    friend ostream& operator<<(ostream&, const Card_Standard&);
    double retragere(int, double )override;
    double credit_bancar(int,double)override;
    void verificare_cont()override;
    vector<string> facilitati()override;
    inline double get_chashback_standard()const{return -1;}
    virtual void set_nume(string s){nume_detinator = s;}
    virtual void adauga_bani(int nr){bani += nr;}
    virtual double get_balance()const{return bani;};
protected:
private:
    int limita_extragere;
    double comision_depasire_limita;
};


//constructor de initializare fara parametrii
Card_Standard::Card_Standard() :limita_extragere(3000), comision_depasire_limita(0.02) {}

Card_Standard::Card_Standard(string banca, const int cvv, string nume, double cred, string data, string nr, int limita, double comision) :
        Card(banca, cvv, nume, cred, data, nr), limita_extragere(limita), comision_depasire_limita(comision) {}
Card_Standard::Card_Standard(const Card_Standard& cs) : Card(cs) {
    limita_extragere = cs.limita_extragere;
    comision_depasire_limita = cs.comision_depasire_limita;
}
Card_Standard& Card_Standard::operator=(const Card_Standard& cs) {
    Card::operator=(cs);
    limita_extragere = cs.limita_extragere;
    comision_depasire_limita = cs.comision_depasire_limita;
    return *this;

}

istream& operator>>(istream& i, Card_Standard& cs) {
    i >> dynamic_cast<Card&>(cs) >> cs.limita_extragere >> cs.comision_depasire_limita;
    return i;
}
ostream& operator<<(ostream& o, const Card_Standard& cs) {
    o << dynamic_cast<const Card&>(cs) <<"\nlimita extragere: "<<cs.limita_extragere<<"\ncomision depasire limita extragere:" << cs.comision_depasire_limita<<'\n';
    return o;
}

//functionalitati
double Card_Standard::credit_bancar(int cod, double suma) {

    if(autentificare(cod) == true) {
        if(bani>0) {
            if (suma < 10000) {
                cout << '\n';
                cout << suma << " ron imprumutati de la banca.\n";
                return suma;
            } else {
                cout << "\nSuma dorita pentru credit bancar a depasit 10000 de ron.\n";
                cout << "\nAti imprumatat doar 9999 ron.\n";

                return 9999;
            }
        }
        else
            cout<<"\nNu sunteti eligibil pentru imprumut.\n";
    }
    else
        return 0;

}
double Card_Standard::retragere(int cod,double suma) {
    if (autentificare(cod) == true) {
        if (suma < bani) {
            if (suma < limita_extragere) {
                bani -= suma;
                cout<<"\nAti retras: "<<suma<<" ron.\n";
                return suma;
            } else {
                bani -= (suma + suma * comision_depasire_limita);
                suma -= suma * comision_depasire_limita;
                cout<<"\nAti retras: "<<suma<<" ron. Deorarece s-a aplicaat comisionul de depasire de "<<comision_depasire_limita<<" din suma ceruta.\n";
                return suma;
            }
        } else {
            cout << "\nFonduri insuficiente doriti sa efectuati un credit Bancar?\n";
            string raspuns;
            cin >> raspuns;
            if (raspuns == "da") {
                double imprumut = credit_bancar(cod, suma - bani);
                if (imprumut == suma - bani) {
                    bani = 0;
                    bani -= imprumut;
                    cout << "\nAti primit banii.\n";
                    return suma;
                }
                else {
                    suma = bani+imprumut;
                    bani = 0;bani -= imprumut;
                    cout << "\nImprumut prea mare!\n";
                    return suma;}
            } else if (raspuns == "nu") {
                if(bani>0) {
                    double y = bani;
                    bani = 0;
                    cout << "Ati retras doar: " << y << " ron\n";
                    return y;
                } else {
                    cout << "\nAti retras 0 lei.\n";
                    return 0;
                }
            }

        }
        return 0;

    }
}
void Card_Standard::verificare_cont() {
    cout<<"\nIntroduceti codul:\n";
    int cod;cin>>cod;
    if(autentificare(cod) == true)
        cout<<"\nBani: "<<bani<<endl;
    else
        cout<<"\nCod gresit. Card blocat pentru 24 de ore.\n";
}

vector<string> Card_Standard::facilitati() {
    return {"\nposibilitate de imprumut\n","\nplata cu cardul\n"};
}
class Card_Premium :
        public Card_Standard
{
public:
    Card_Premium();
    Card_Premium(string banca, int cvv, string nume, double cred, string data, string nr, int limita, double comision, double, int, double);
    Card_Premium(const Card_Premium&);
    Card_Premium& operator=(const Card_Premium&);
    ~Card_Premium() = default;
    friend istream& operator>>(istream&, Card_Premium&);
    friend ostream& operator<<(ostream&, const Card_Premium&);
    double retragere(int,double) override;
    vector<string> facilitati() override;
    double credit_bancar(int cod,double suma) override;
    void verificare_cont() override;
    void set_nume(string s) override{nume_detinator = s;}
    inline double get_cashback_premium()const{return cashback;}
    void adauga_bani(int nr) override{bani += nr;}
    double get_balance()const{return bani;}
private:
    double cashback;
    int limita_extragere_premium;
    double comision_depasire_premium;
};


Card_Premium::Card_Premium() :cashback(0.005), limita_extragere_premium(6000), comision_depasire_premium(0.1) {}
Card_Premium::Card_Premium(string banca, const int cvv, string nume, double cred, string data, string nr, int limita, double comision, double cash, int limp, double comp) :
        Card_Standard(banca, cvv, nume, cred, data, nr, limita, comision), cashback(cash), limita_extragere_premium(limp), comision_depasire_premium(comp) {}
Card_Premium::Card_Premium(const Card_Premium& cp) : Card_Standard(cp) {
    cashback = cp.cashback;
    limita_extragere_premium = cp.limita_extragere_premium;
    comision_depasire_premium = cp.comision_depasire_premium;
}

Card_Premium& Card_Premium::operator=(const Card_Premium& cp) {
    Card_Standard::operator=(cp);
    cashback = cp.cashback;
    limita_extragere_premium = cp.limita_extragere_premium;
    comision_depasire_premium = cp.comision_depasire_premium;
    return *this;
}

istream& operator>>(istream& i, Card_Premium& cp) {
    i >> dynamic_cast<Card_Standard&>(cp) >> cp.cashback >> cp.limita_extragere_premium >> cp.comision_depasire_premium;
    return i;
}
ostream& operator<<(ostream& o, const Card_Premium& cp) {
    o << dynamic_cast<const Card&>(cp) <<"cashback: "<< cp.cashback <<"\nlimita extragere card premium: "<< cp.limita_extragere_premium<<
      "\ncomision depasire card premium: "<< cp.comision_depasire_premium<<'\n';
    return o;
}
//functionalitati
double Card_Premium::credit_bancar(int cod,double suma){


    if(autentificare(cod) == true) {
        if (suma < 40000) {
            cout<<'\n';
            cout<<suma<<" ron imprumutati de la banca.\n";
            return suma;
        } else
        {
            cout<<"\nSuma dorita pentru credit bancar a depasit 40000 de ron.\n";
            cout<<"\nAti imprumatat doar 39999 ron.\n";

            return 39999;}
    }
    else
        return 0;


}
void Card_Premium::verificare_cont() {
    cout<<"--------------";
    cout<<"\n    PREMIUM\n";
    cout<<"---------------";
    cout<<"\nIntroduceti codul:\n";
    int cod;cin>>cod;
    if(autentificare(cod) == true)
        cout<<"\nBani: "<<bani<<endl;
    else
        cout<<"\nCod gresit. Card blocat pentru 24 de ore.\n";
}

double Card_Premium::retragere(int cod,double suma){
    if(autentificare(cod) == true) {
        if(suma<bani) {
            if (suma < limita_extragere_premium) {
                cout << "\nAti retras " << suma << " ron.\n";
                cout<<"\nvaloare cashback: "<<(suma*cashback)<<" ron.\n"<<endl;
                return (suma+ (suma*cashback/100));
            } else {
                cout << "\nAti depasit limita de extragere.\n";
                bani -= (suma + suma * comision_depasire_premium);
                cout<<"\nAti retras: "<<((suma - suma * comision_depasire_premium  ) + (suma - suma * comision_depasire_premium  )*cashback)<<" ron.\n";
                return ((suma - suma * comision_depasire_premium  ) + (suma - suma * comision_depasire_premium  )*cashback);
            }
        }
        else
        {
            cout<<"\nDoriti sa efectuati un credit bancar?\n";
            string raspuns;
            cin>>raspuns;
            if(raspuns == "da") {
                double imprumut = credit_bancar(cod, suma - bani);
                if (imprumut == suma - bani)
                {
                    bani = 0;
                    bani -= imprumut;
                    cout<<"\nAti primit banii";
                    cout<<"\nAti primit cashbach: "<<suma*cashback<<" ron.\n";
                    return (suma+ suma*cashback);
                }
                else {
                    suma = bani + 39999;
                    bani = 0;bani -= imprumut;
                    cout << "\nImprumut prea mare!\n";
                    cout<<"\nAti primit cashbach: "<<suma*cashback<<" ron.\n";
                    return (suma+ suma*cashback);}
            } else if (raspuns == "nu") {
                if(bani>0)
                { double y = bani;
                    bani = 0;
                    cout<<"\nAti retras doar:"<<(y+y*cashback)<<" ron\n";
                    return (y+y*cashback);}
                else {
                    cout << "\nAti retras 0 lei.\n";
                    return 0;
                }
            }}

    }
    return 0;

}
vector<string> Card_Premium::facilitati() {
    return {"\ncashback generos la retragere\n","\nsuma de imprumut mai mare\n","\nlimita mai mare de extragere\n","\ncomision de depasire a limitei de extragere mai mic\n","\nplus toate avantajele cardului standard\n"};
}


/////////////////////tema3///////////////////////////////////////////

///abstract factory ///
class Contract{
public:
    virtual double calcul_rate(int ani) = 0;
    virtual double calcul_dobanda(int ani) = 0;
    virtual ~Contract(){};
protected:
private:
};
class Operatii_active: public Contract{
public:
    double calcul_rate(int ani)override;
    double calcul_dobanda(int ani) override;
    Operatii_active(){valoare_credit = 200000;}
    virtual ~Operatii_active() {}
    Operatii_active(double valoareCredit);
    friend ostream &operator<<(ostream &os, const Operatii_active &active);
    friend istream &operator>>(istream &is, Operatii_active &pasive);
    virtual double get_valoare()const{return valoare_credit;}

private:
    double valoare_credit;
};
class Operatii_pasive: public Contract{
public:
    double calcul_rate(int ani)override;
    double calcul_dobanda(int ani) override;
    Operatii_pasive(){valoare_depunere = 10000;}
    Operatii_pasive(double valoareDepunere);
    virtual ~Operatii_pasive() {}
    friend ostream &operator<<(ostream &os, const Operatii_pasive &pasive);
    friend istream &operator>>(istream &is, Operatii_pasive &pasive);
    double get_valoare()const {return valoare_depunere;}

private:
    double valoare_depunere;
};

class ContractFactory{
public:
    static Contract * NewContract(const string &alegere);

};
double Operatii_active::calcul_rate(int ani) {
    double rata;
    rata = valoare_credit/ani;
    return rata;
}
double Operatii_active::calcul_dobanda(int ani) {
    double rata = calcul_rate(ani);
    double dobanda = (double ) 1/ rata;
    return dobanda;
}

Operatii_active::Operatii_active(double valoareCredit) : valoare_credit(valoareCredit) {}

ostream &operator<<(ostream &os, const Operatii_active &active) {
    os << " valoare_credit: " << active.valoare_credit;
    return os;
}
istream &operator>>(istream &is,Operatii_active &active) {
    is>> active.valoare_credit;
    return is;
}

double Operatii_pasive::calcul_dobanda(int ani) {
    double dobanda = (ani*0.001)*0.001*valoare_depunere;
    return dobanda;
}

double Operatii_pasive::calcul_rate(int ani) {
    double dobanda = calcul_dobanda(ani);
    double rata = valoare_depunere/ani+dobanda;
    return rata;
}

Operatii_pasive::Operatii_pasive(double valoareDepunere) : valoare_depunere(valoareDepunere) {}

ostream &operator<<(ostream &os, const Operatii_pasive &pasive) {
    os << " valoare_depunere: " << pasive.valoare_depunere;
    return os;
}
istream &operator>>(istream &is, Operatii_pasive &pasive) {
    is >> pasive.valoare_depunere;
    return is;
}

Contract* ContractFactory::NewContract(const string &alegere) {

        if(alegere == "active")
            return new Operatii_active;
        else if(alegere == "pasive")
            return new Operatii_pasive;
        else
            return nullptr;

}
///template///
template<class T> class client{
private:
    string nume;
    string telefon;
    T card;
    ContractFactory contract;
    static int id_calculat;
    int id_client_card_premium;
    static int k;
public:
    client(){nume = "maria ion";telefon = "0292393";T c; c.set_nume("maria ion"); card = c;
        Card_Premium cp;
        if(typeid(card) == typeid(cp)) {
            id_client_card_premium = id_calculat;
            increment_id();
        }
        ContractFactory co; contract = co;
    }
    client(const string &nume, const string &telefon) : nume(nume), telefon(telefon) { T c;c.set_nume(nume);this->card = c;
        Card_Premium cp;
        if(typeid(card) == typeid(cp)) {
            id_client_card_premium = id_calculat;
            increment_id();
        }
        ContractFactory co; contract = co;
    }

    ~client();

    void plata(){
        int cod;
        double suma;
        cin>>cod>>suma;
        cout<<card.retragere(cod,suma);
    }
    void log_in()
    {
        card.verificare_cont();
    }
    void premiu_castigat(int nr)
    {
        if(id_client_card_premium == 5 && k == 0) {
            card.adauga_bani(nr);
            mod_k();
            cout << "\nVerificati-va contul pentru a vedea premiul.\n";
        }
        log_in();
    }
    int get_id()const{Card_Premium cp;
        if(typeid(cp) == typeid(card))
        return id_client_card_premium;
        else
        {
            cout<<"\nNu puteti participa la tombola cardurilor deoarece nu aveti card premium!.\n";
            return -1;
        }}
    static void increment_id(){id_calculat += 1;}
    static void mod_k(){k = 1;}
    T get_card(){return card;}
    void operatie_financiara(int ani);
};
template<class T>
void client<T>::operatie_financiara(int ani)
{
    string alegere;cout<<"\nAlegeti ce fel de operatiune financiara doriti sa faci (active - primiti bani de la banca si ii dati inapoi in x ani/ pasive- dati bani la banca si ii primiti inapoi in x ani).\n";
    cin>>alegere;
    if(alegere != "pasive" && alegere != "active") {
        cout<<"\nNu ati ales o varianta valida!\n";return;
    }        double dobanda = contract.NewContract(alegere)->calcul_dobanda(ani);
    double rata = contract.NewContract(alegere)->calcul_rate(ani);
    if(alegere == "pasive" && card.get_balance()>=10000) {
        cout << "\nrata pe care o veti primi de la banca este de: " << rata;
        cout << "\ndobanda pe care o veti primi de la banca este de: "<< dobanda;
        card.adauga_bani(-(rata*ani + dobanda*ani));
    } else if(alegere == "pasive" && !(card.get_balance()>=10000)) {
        cout<<"\nNu aveti destule fonduri pentru depunere.\n";
    } else if(alegere == "active")
    {
        cout << "\nrata pe care o veti plati de la banca este de: " << rata;
        cout << "\ndobanda pe care o veti plati de la banca este de: "<< dobanda;
        card.adauga_bani(rata*ani + dobanda*ani);
    } else if(alegere != "pasive" && alegere != "active")
        cout<<"\nNu ati ales o varianta valida!\n";
}

template <class T>
int client<T>::id_calculat = 1;
template <class T>
int client<T>::k = 0;

template<class T>
client<T>::~client() {
    nume = "";telefon = "";
}

///Singleton///
class Premiu{
    static Premiu* obiect;
    int valoare_premiu;
    Premiu(){
        valoare_premiu = 100000;
    }

public:
    ~Premiu(){}

public:
    static Premiu *getInstance() {
            if (!obiect)
                obiect = new Premiu;
            return obiect;
    }
    int get_premiu(int nr){
        if(nr == 5) {
            cout << "\nFelicitari! Ati luat premiul de 100000 de ron. (Chiar daca a-ti luat premiul acesta nu se poate lua decat o data asa ca doar prima oara ve-ti castiga 100000 de ron, altfel ve-ti castiga imensa suma de 0 ron.)\n";
            int x = valoare_premiu;
            valoare_premiu = 0;
            return x;
        }
        else if(nr <5)
        {
            cout<<"\nNu sunteti posesorul de card Premium norocos\n";
            return 0;
        }
        else {
            cout << "\nS-a dat premiul.\n";
            return 0;
        }
    }
};
Premiu *Premiu::obiect = 0;


template<class T>
vector<client<T>>baza_de_date;
int main()
{

    int tema;
    cout<<"\nAlegeti 1 (pentru tema 2) sau 2 (pentru tema 3).\n";
    cin>>tema;
    if(tema == 1) {
        int comanda;
        bool meniu = true;
        while (meniu) {
            citire_comanda:
            cout << "Apasati 0 pentru a iesi din program.\n";
            cout << "Apasati 1 pentru a va crea cardul standard.\n";
            cout << "Apasati 2 pentru constructori si operatori.\n";
            cout << "Apasati 3 pentru crea o colectie de carduri.\n";


            cin >> comanda;
            try {
                if (comanda > 4)
                    throw comanda;
            }
            catch (...) {
                cout << "Comanda invalida.\n";
                goto citire_comanda;
            }
            if (comanda == 0)
                meniu = false;
            else if (comanda == 1) {
                cout
                        << "Cititi proprietatiile cardului standard: banca(string),cvv(int),(double) banii din card, (string) data expirarii, (string) nr cardului, (string) nume detinator,(int) limita extragere, (double) comision depasire limita\n";
                Card_Standard csc;
                cin >> csc;
                Card_Standard *cs = &csc;
                crds_sub:
                cout << "Apasati 0 pentru a nu mai folosi cardul standard\n";
                cout << "Apasati 1 pentru a face o retragere.\n";
                cout << "Apasati 2 pentru a face un credit bancar.\n";
                cout << "Apasati 3 pentru a vedea cat bani aveti pe card.\n";
                cout << "Apasati 4 pentru a face upgrade la card premium.\n";
                cout << "Apasati 5 pentru a vedea facilitatiile.\n";
                cout << "Apasati 6 pentru a mai da comenzi.\n";
                int subcomanda_css;
                int cod;
                double suma;
                cin >> subcomanda_css;
                try {
                    if (subcomanda_css > 6)
                        throw subcomanda_css;

                }
                catch (...) {
                    cout << "subcomanda pentru card standard invalida.\n";
                    goto crds_sub;

                }
                if (subcomanda_css == 0)
                    goto citire_comanda;
                else if (subcomanda_css == 1) {
                    cout << "Introduceti codul de deblocare a cardului si suma pe care vreti sa o retrageti.\n";
                    cin >> cod >> suma;
                    cs->retragere(cod, suma);
                    goto crds_sub;
                } else if (subcomanda_css == 2) {
                    cout << "Introduceti codul de deblocare a cardului si suma pe care vreti sa o imprumutati.\n";
                    cin >> cod >> suma;
                    cs->credit_bancar(cod, suma);
                    goto crds_sub;
                } else if (subcomanda_css == 3) {
                    cs->verificare_cont();
                    goto crds_sub;
                } else if (subcomanda_css == 4) {

                    ///upcasting
                    cs = new Card_Premium;
                    cout << "Acum explorati noile facilitati ale cardului premium. Codul de deblocare este 0.\n";
                    vector<string> vp = cs->facilitati();
                    for (long long i = 0; i < vp.size(); i++)
                        cout << vp[i];
                    csp_et:
                    cout << "Apasati 0 pentru a nu mai folosi cardul premium\n";
                    cout << "Apasati 1 pentru a face o retragere.\n";
                    cout << "Apasati 2 pentru a face un credit bancar.\n";
                    cout << "Apasati 3 pentru a vedea cat bani aveti pe card.\n";
                    cout << "Apasati 4 pentru a face downgrade la card standard.\n";
                    cout << "Apasati 5 pentru a mai da comenzi.\n";

                    int csp_sub;
                    cin >> csp_sub;
                    try {
                        if (csp_sub > 5)
                            throw csp_sub;
                    }
                    catch (...) {
                        cout << "comanda invalida.\n";
                        goto csp_et;
                    }
                    switch (csp_sub) {
                        case 0:
                            goto citire_comanda;
                            break;
                        case 1: {
                            cout << "Introduceti codul de deblocare a cardului si suma pe care vreti sa o retrageti.\n";
                            cin >> cod >> suma;
                            cs->retragere(cod, suma);
                            goto csp_et;
                            break;
                        }
                        case 2: {
                            cout
                                    << "Introduceti codul de deblocare a cardului si suma pe care vreti sa o imprumutati.\n";
                            cin >> cod >> suma;
                            cs->credit_bancar(cod, suma);
                            goto csp_et;
                            break;
                        }
                        case 3: {
                            cs->verificare_cont();
                            goto csp_et;
                            break;
                        }
                        case 4: {
                            cout << "\nAti facut downgrade la cardul standard. Codul de deblocare este tot 0.\n";
                            cs = new Card_Standard;
                            cs = dynamic_cast<Card_Standard *>(cs);
                            cs->verificare_cont();
                            goto crds_sub;
                            break;

                        }
                    }
                } else if (subcomanda_css == 5) {
                    vector<string> v = cs->facilitati();
                    for (long long i = 0; i < v.size(); i++)
                        cout << v[i];
                    goto crds_sub;
                    break;
                } else if (subcomanda_css == 6) {
                    goto crds_sub;
                }


                break;
            } else if (comanda == 2) {
                constructori:
                cout << "Apasati 0 pentru a iesi.\n";
                cout << "Apasati 1 pentru clasa Card.\n";
                cout << "Apasati 2 pentru clasa Card Standard.\n";
                cout << "Apasati 3 pentru clasa Card Premium.\n";
                short cons_sub;
                cin >> cons_sub;
                switch (cons_sub) {
                    case 0: {
                        goto citire_comanda;
                        break;
                    }
                    case 1: {
                        cout << "\nconstructor fara parametrii si operatorul <<.\n";
                        Card co;
                        cout << co;
                        cout << "\nconstructor cu paramentrii.\n";
                        Card co1("BRD", 100, "Ionescu Vasile", 12000.25, "12/26", "023222922");
                        cout << co1;
                        cout << "\nconstructor de copiere si operator de atribuire.\n";
                        Card co2(co1);
                        cout << "\nco2:\n" << co2;
                        co = co1;
                        cout << "\nco2:\n" << co;
                        cout << "\noperatorul >> si <<";
                        cout
                                << "Cititi proprietatiile cardului standard: banca(string),cvv(int),(double) banii din card, (string) data expirarii, (string) nr cardului, (string) nume detinator.\n";
                        cin >> co;
                        cout << co;
                        goto constructori;
                        break;
                    }
                    case 2: {
                        cout << "\nconstructor fara parametrii si operatorul <<.\n";
                        Card_Standard cso;
                        cout << cso;
                        cout << "\nconstructor cu paramentrii.\n";
                        Card_Standard cso1("BRD", 100, "Ionescu Vasile", 12000.25, "12/26", "023222922", 2000, 0.03);
                        cout << cso1;
                        cout << "\nconstructor de copiere si operator de atribuire.\n";
                        Card_Standard cso2(cso1);
                        cout << "\ncso2:\n" << cso2;
                        cso = cso1;
                        cout << "\ncso2:\n" << cso;
                        cout << "\noperatorul >> si <<";
                        cout
                                << "Cititi proprietatiile cardului standard: banca(string),cvv(int),(double) banii din card, (string) data expirarii, (string) nr cardului, (string) nume detinator,(int) limita extragere, (double) comision depasire limita\n";
                        cin >> cso;
                        cout << cso;
                        goto constructori;
                        break;

                    }
                    case 3: {
                        cout << "\nconstructor fara parametrii si operatorul <<.\n";
                        Card_Premium cpo;
                        cout << cpo;
                        cout << "\nconstructor cu paramentrii.\n";
                        Card_Premium cpo1("BRD", 100, "Ionescu Vasile", 12000.25, "12/26", "023222922", 2000, 0.03,
                                          0.005, 5500, 0.01);
                        cout << cpo1;
                        cout << "\nconstructor de copiere si operator de atribuire.\n";
                        Card_Premium cpo2(cpo1);
                        cout << "\ncpo2:\n" << cpo2;
                        cpo = cpo1;
                        cout << "\ncpo2:\n" << cpo;
                        cout << "\noperatorul >> si <<";
                        cout
                                << "Cititi proprietatiile cardului standard: banca(string),cvv(int),(double) banii din card, (string) data expirarii, (string) nr cardului, (string) nume detinator, (double) cashback, (int) limita extragere premium, (double) comision depasire limita premium\n";
                        cin >> cpo;
                        cout << cpo;
                        goto constructori;
                        break;

                    }


                }
            } else if (comanda == 3) {
                long long n;
                cout << "Introduceti numarul de carduri:\n";
                cin >> n;
                try {
                    if (n < 1) throw runtime_error("Vectorul nu poate avea un mai mic de 1 de elemente!");
                }
                catch (...) {
                    cout << "Ati introdus un numar invalid de elemente ale vectorului.";
                    return 0;
                }
                Card *carduri[n + 1];
                for (long long i = 0; i < n; i++)
                    if (i % 2)
                        carduri[i] = new Card_Standard;
                    else
                        carduri[i] = new Card_Premium;
                int count = 1;
                for (long long i = 0; i < n; i++) {
                    cout << "\nCardul: " << count << endl;
                    if (Card_Premium *cpp = dynamic_cast<Card_Premium *>(carduri[i])) {
                        cout << "\n-----------------------------------------------";
                        cout << "Cardul: " << count << '\n';
                        cout << "\nVerifica contul codul este 0:\n";
                        if (cpp->get_cashback_premium() != -1) {
                            cpp->verificare_cont();
                            cout << "\nCardul dvs. are cashback: " << cpp->get_cashback_premium() << '\n';
                        }
                    } else if (Card_Standard *csp = dynamic_cast<Card_Standard *>(carduri[i])) {
                        if (csp->get_chashback_standard() == -1) {
                            csp->verificare_cont();
                            cout << "\nCardul standard nu are optiunea de cashback.\n";
                        }
                    }
                    count++;
                    cout << "---------------------------------------------------\n";
                }
                for (long long i = 0; i < n; i++)
                    delete carduri[i];
            }
        }
    }
    //////////////////////tema3///////////////////////////////////////////
    else {
        client<Card_Premium> c("Mircea Petcu", "07332929323");
        client<Card_Premium> c2("Simona", "0738383838");
        client<Card_Standard> c3("Ionut Negru", "0333112");
        baza_de_date<Card_Standard>.push_back(c3);
        Premiu *premiu = premiu->getInstance();
        client<Card_Premium> c20("Mircea Astept", "07332323");
        client<Card_Premium> c4("Mircea Tettcu", "073329323");
        client<Card_Premium> c5("Tuna", "07923239323");
        client<Card_Premium> c6("Mircea coctcu", "0733223233929323");
        baza_de_date<Card_Premium>.push_back(c);
        baza_de_date<Card_Premium>.push_back(c2);
        baza_de_date<Card_Premium>.push_back(c4);
        baza_de_date<Card_Premium>.push_back(c5);
        baza_de_date<Card_Premium>.push_back(c6);
        baza_de_date<Card_Premium>.push_back(c20);
        c2.premiu_castigat(premiu->get_premiu(c2.get_id()));
        c5.premiu_castigat(premiu->get_premiu(c5.get_id()));
        c6.premiu_castigat(premiu->get_premiu(c6.get_id()));
        Premiu *premiu2 = premiu2->getInstance();
        client<Card_Premium> c7("Mircesdja Petcu", "07332922339323");
        client<Card_Premium> c8("Simdsona", "073838233838");
        client<Card_Premium> c9("Mircea Pewewwewtcu", "07332929323");
        client<Card_Premium> c10("Mircea sdsdPetcu", "0733292955323");
        client<Card_Premium> c11("Tuna", "07332929366523");
        client<Card_Premium> c12("Mirceweea Petcu", "0736532929323");
        baza_de_date<Card_Premium>.push_back(c7);
        baza_de_date<Card_Premium>.push_back(c8);
        baza_de_date<Card_Premium>.push_back(c9);
        baza_de_date<Card_Premium>.push_back(c10);
        baza_de_date<Card_Premium>.push_back(c11);
        baza_de_date<Card_Premium>.push_back(c12);
        c5.premiu_castigat(premiu2->get_premiu(c5.get_id()));
        //////////////////////////////////////////////////////////
        cout<<"\nImplementate Factory Method:\n";
        c5.log_in();
        c5.operatie_financiara(20);
        c5.log_in();

    }

    return 0;
}