#include <iostream>
#include <limits>
#include <vector> 
#include <list>
#include <string>
#include <queue>

using namespace std;

template <typename TipKljuca, typename TipVrijednosti>
class Mapa {
    public:
    Mapa() {}
    virtual ~Mapa() {}
    virtual int brojElemenata() const { return 0; }
    virtual TipVrijednosti &operator[] (TipKljuca kljuc) = 0;
    virtual TipVrijednosti operator [] (TipKljuca kljuc) const = 0;
    virtual void obrisi() {}
    virtual void obrisi(const TipKljuca &kljuc) {}
};




template <typename TipKljuca, typename TipVrijednosti>
struct Pair {
    TipKljuca kljuc;
    TipVrijednosti vrijednost;
};




template <typename TipKljuca, typename TipVrijednosti>
class HashMapaLan : public Mapa<TipKljuca,TipVrijednosti> {
  private:
    std::vector<std::list<Pair<TipKljuca,TipVrijednosti>>> mapa;
    int broj=0;
    static const int kapacitet=10000;
    unsigned int (*hash)(TipKljuca, unsigned int)=0;
    typename std::list<Pair<TipKljuca,TipVrijednosti>>::iterator trazi(TipKljuca kljuc);
    typename std::list<Pair<TipKljuca,TipVrijednosti>>::const_iterator trazi(TipKljuca kljuc) const;
  public:
    HashMapaLan() : mapa(kapacitet, std::list<Pair<TipKljuca,TipVrijednosti>>()) {}
    int brojElemenata() const { return broj; }
    TipVrijednosti &operator[](TipKljuca kljuc);
    TipVrijednosti operator[](TipKljuca kljuc) const;
    void obrisi() { for(auto &l : mapa) l.clear(); broj=0; }
    void obrisi(const TipKljuca &kljuc);
    void definisiHashFunkciju(unsigned int (*fun)(TipKljuca, unsigned int)) { hash=fun; }
};

template <typename TipKljuca, typename TipVrijednosti>
void HashMapaLan<TipKljuca,TipVrijednosti>::obrisi(const TipKljuca &kljuc) {
    auto iter(trazi(kljuc));
    int indeks=hash(kljuc,mapa.size());
    if(iter==mapa[indeks].end() || iter->kljuc!=kljuc) throw "Error";
    auto iter2(iter++);
    mapa[indeks].erase(iter2,iter);
    broj--;
}

template <typename TipKljuca, typename TipVrijednosti>
TipVrijednosti &HashMapaLan<TipKljuca,TipVrijednosti>::operator[](TipKljuca kljuc) {
    auto iter(trazi(kljuc));
    int indeks=hash(kljuc,mapa.size());
    if(iter==mapa[indeks].end() || iter->kljuc!=kljuc) {
        Pair<TipKljuca,TipVrijednosti> temp;
        temp.kljuc=kljuc;
        temp.vrijednost=TipVrijednosti();
        iter=mapa[indeks].insert(iter,temp);
        broj++;
    }
    return iter->vrijednost;
}

template <typename TipKljuca, typename TipVrijednosti>
TipVrijednosti HashMapaLan<TipKljuca,TipVrijednosti>::operator[](TipKljuca kljuc) const {
    auto iter(trazi(kljuc));
    int indeks=hash(kljuc,mapa.size());
    if(iter==mapa[indeks].end() || iter->kljuc!=kljuc) return TipVrijednosti();
    return iter->vrijednost;
}

template <typename TipKljuca, typename TipVrijednosti>
typename std::list<Pair<TipKljuca,TipVrijednosti>>::const_iterator HashMapaLan<TipKljuca,TipVrijednosti>::trazi(TipKljuca kljuc) const {
    if(hash==0) throw "Error";
    int indeks=hash(kljuc,mapa.size());
    auto iter(mapa[indeks].begin());
    while(iter!=mapa[indeks].end() && kljuc>iter->kljuc) iter++;
    return iter;
}

template <typename TipKljuca, typename TipVrijednosti>
typename std::list<Pair<TipKljuca,TipVrijednosti>>::iterator HashMapaLan<TipKljuca,TipVrijednosti>::trazi(TipKljuca kljuc) {
    if(hash==0) throw "Error";
    int indeks=hash(kljuc,mapa.size());
    auto iter(mapa[indeks].begin());
    while(iter!=mapa[indeks].end() && kljuc>iter->kljuc) iter++;
    return iter;
}




template <typename Tip>
class Grana;

template <typename Tip>
class Cvor;

template <typename Tip>
class GranaIterator;



  
  template <typename Tip>
  class UsmjereniGraf {
      protected:
       UsmjereniGraf() {}
      public:
       UsmjereniGraf(int n) {}
       virtual ~UsmjereniGraf() {}
       virtual int dajBrojCvorova() const=0;
       virtual void postaviBrojCvorova(int n)=0;
       virtual void dodajGranu(int polazni, int dolazni, float tezina=0)=0;
       virtual void obrisiGranu(int polazni, int dolazni)=0;
       virtual void postaviTezinuGrane(int polazni, int dolazni, float tezina=0)=0;
       virtual float dajTezinuGrane(int polazni, int dolazni) const=0;
       virtual bool postojiGrana(int polazni, int dolazni) const=0;
       virtual void postaviOznakuCvora(int cvor, Tip oznaka)=0;
       virtual Tip dajOznakuCvora(int cvor) const=0;
       virtual void postaviOznakuGrane(int polazni, int dolazni, Tip oznaka)=0;
       virtual Tip dajOznakuGrane(int polazni, int dolazni) const=0;
       virtual Grana<Tip> dajGranu(int polazni, int dolazni)=0;
       virtual Cvor<Tip> dajCvor(int cvor)=0;
       GranaIterator<Tip> dajGranePocetak() { return ++GranaIterator<Tip>(this,0,-1); }
       GranaIterator<Tip> dajGraneKraj() { return GranaIterator<Tip>(this,dajBrojCvorova(),0); }
};




template <typename Tip>
class Grana {
    private:
      UsmjereniGraf<Tip>* g;
      int polazni, dolazni;
    public:
      Grana(UsmjereniGraf<Tip>* graf, int polazni, int dolazni) : g(graf), polazni(polazni), dolazni(dolazni) {}
      float dajTezinu() const { return g->dajTezinuGrane(polazni,dolazni); }
      void postaviTezinu(float tezina) { g->postaviTezinuGrane(polazni,dolazni,tezina); }
      Tip dajOznaku() const { return g->dajOznakuGrane(polazni,dolazni); }
      void postaviOznaku(Tip oznaka) { g->postaviOznakuGrane(polazni,dolazni,oznaka); }
      Cvor<Tip> dajPolazniCvor() const { return g->dajCvor(polazni); }
      Cvor<Tip> dajDolazniCvor() const { return g->dajCvor(dolazni); }
};




template <typename Tip>
class Cvor {
    private:
       UsmjereniGraf<Tip>* g;
       int redni;
    public:
       Cvor(UsmjereniGraf<Tip>* graf, int redniBroj) : g(graf), redni(redniBroj) {}
       Tip dajOznaku() const { return g->dajOznakuCvora(redni); }
       void postaviOznaku(Tip oznaka) { g->postaviOznakuCvora(redni, oznaka); }
       int dajRedniBroj() const { return redni; }
};



template <typename Tip>
class GranaIterator {
    private:
        UsmjereniGraf<Tip>* g;
        int i,j;
    public:
        GranaIterator(UsmjereniGraf<Tip>* graf, int i, int j) : g(graf), i(i), j(j) {}
        Grana<Tip> operator*() { return g->dajGranu(i,j); }
        bool operator==(const GranaIterator &iter) const { return (i==iter.i && j==iter.j && g==iter.g); }
        bool operator!=(const GranaIterator &iter) const { return !(iter==*this); }
        GranaIterator &operator++();
        GranaIterator operator++(int);
};

template <typename Tip>
GranaIterator<Tip> &GranaIterator<Tip>::operator++() {
    do {
        if(j+1>=g->dajBrojCvorova()) j=0,i++;
        else j++;
    }while (i<g->dajBrojCvorova() && !g->postojiGrana(i,j));
    return *this;
}

template <typename Tip>
GranaIterator<Tip> GranaIterator<Tip>::operator ++(int) {
    GranaIterator<Tip> temp(*this);
    ++(*this);
    return temp;
}




template <typename Tip>
class MatricaGraf : public UsmjereniGraf<Tip> {
    private:
        std::vector<std::vector<float>> m;
        std::vector<Tip> oznakeCvorova;
        std::vector<std::vector<Tip>> oznakeGrana;
        void ispravanCvor(int redniBroj) const { if(redniBroj<0 || redniBroj>=dajBrojCvorova()) throw "Error"; }
        void ispravnaGrana(int polazni, int dolazni) const { ispravanCvor(polazni); ispravanCvor(dolazni); }
        void postojiLiGrana(int polazni, int dolazni) const { ispravnaGrana(polazni,dolazni); if(!postojiGrana(polazni,dolazni)) throw "Error"; }
    public:
        MatricaGraf(int n);
        ~MatricaGraf() {}
        int dajBrojCvorova() const { return m.size(); }
        void postaviBrojCvorova(int n);
        void dodajGranu(int polazni, int dolazni, float tezina=0);
        void obrisiGranu(int polazni, int dolazni);
        void postaviTezinuGrane(int polazni, int dolazni, float tezina=0);
        float dajTezinuGrane(int polazni, int dolazni) const;
        bool postojiGrana(int polazni, int dolazni) const;
        void postaviOznakuCvora(int cvor, Tip oznaka);
        Tip dajOznakuCvora(int cvor) const;
        void postaviOznakuGrane(int polazni, int dolazni, Tip oznaka);
        Tip dajOznakuGrane(int polazni, int dolazni) const;
        Grana<Tip> dajGranu(int polazni, int dolazni);
        Cvor<Tip> dajCvor(int cvor);
};

template <typename Tip>
Grana<Tip>MatricaGraf<Tip>::dajGranu(int polazni, int dolazni) {
    postojiLiGrana(polazni,dolazni);
    return Grana<Tip>(this,polazni,dolazni);
}
        
template <typename Tip>
Cvor<Tip> MatricaGraf<Tip>::dajCvor(int cvor) {
    ispravanCvor(cvor);
    return Cvor<Tip>(this,cvor);
}

template <typename Tip>
Tip MatricaGraf<Tip>::dajOznakuGrane(int polazni, int dolazni) const {
    postojiLiGrana(polazni,dolazni);
    return oznakeGrana[polazni][dolazni];
}

template <typename Tip>
void MatricaGraf<Tip>::postaviOznakuGrane(int polazni, int dolazni, Tip oznaka) {
    postojiLiGrana(polazni,dolazni);
    oznakeGrana[polazni][dolazni]=oznaka;
}

template <typename Tip>
Tip MatricaGraf<Tip>::dajOznakuCvora(int cvor) const {
    ispravanCvor(cvor);
    return oznakeCvorova[cvor];
}

template <typename Tip>
void MatricaGraf<Tip>::postaviOznakuCvora(int cvor, Tip oznaka) {
    ispravanCvor(cvor);
    oznakeCvorova[cvor]=oznaka;
}

template <typename Tip>
bool MatricaGraf<Tip>::postojiGrana(int polazni, int dolazni) const {
    ispravnaGrana(polazni,dolazni);
    return m[polazni][dolazni]!=-1;
}

template <typename Tip>
float MatricaGraf<Tip>::dajTezinuGrane(int polazni, int dolazni) const {
    postojiLiGrana(polazni,dolazni);
    return m[polazni][dolazni];
}

template <typename Tip>
void MatricaGraf<Tip>::postaviTezinuGrane(int polazni, int dolazni, float tezina) {
    postojiLiGrana(polazni,dolazni);
    m[polazni][dolazni]=tezina;
}

template <typename Tip>
void MatricaGraf<Tip>::obrisiGranu(int polazni, int dolazni) {
    postojiLiGrana(polazni,dolazni);
    m[polazni][dolazni]=-1;
}

template <typename Tip>
void MatricaGraf<Tip>::dodajGranu(int polazni, int dolazni, float tezina) {
    ispravnaGrana(polazni,dolazni);
    m[polazni][dolazni]=tezina;
}

template <typename Tip>
MatricaGraf<Tip>::MatricaGraf(int n) {
    if(n<0) throw "Error";
    m=std::vector<std::vector<float>>(n, std::vector<float>(n,-1));
    oznakeCvorova=std::vector<Tip>(n);
    oznakeGrana=std::vector<std::vector<Tip>>(n, std::vector<Tip>(n));
}

template <typename Tip>
void MatricaGraf<Tip>::postaviBrojCvorova(int n) {
    if(n<dajBrojCvorova()) throw "Error";
    m.resize(n);
    for(int i(0);i<m.size();i++)
    m[i].resize(n,-1);
    oznakeCvorova.resize(n);
    oznakeCvorova.resize(n);
    for(int i(0);i<oznakeGrana.size();i++)
       oznakeGrana[i].resize(n);
}




template <typename Tip>
struct Element {
    int cvor;
    float tezina;
    Tip oznaka;
};



template <typename Tip>
class ListaGraf : public UsmjereniGraf<Tip> {
    private:
       std::vector<std::list<Element<Tip>>> l;
       std::vector<Tip> oznakeCvorova;
       void ispravanCvor(int redniBroj) const { if(redniBroj<0 || redniBroj>=dajBrojCvorova()) throw "Error"; }
       void ispravnaGrana(int polazni, int dolazni) const { ispravanCvor(polazni); ispravanCvor(dolazni); }
       void postojiLiGrana(int polazni, int dolazni) const { ispravnaGrana(polazni,dolazni); if(!postojiGrana(polazni,dolazni)) throw "Error"; }
       typename std::list<Element<Tip>>::iterator traziElement(int i, int j);
       typename std::list<Element<Tip>>::const_iterator traziElement(int i, int j) const;
    public:
       ListaGraf(int n);
       int dajBrojCvorova() const { return l.size(); }
       void postaviBrojCvorova(int n);
       void dodajGranu(int polazni, int dolazni, float tezina=0);
       void obrisiGranu(int polazni, int dolazni);
       void postaviTezinuGrane(int polazni, int dolazni, float tezina=0);
       float dajTezinuGrane(int polazni, int dolazni) const;
       bool postojiGrana(int polazni, int dolazni) const;
       void postaviOznakuCvora(int cvor, Tip oznaka);
       Tip dajOznakuCvora(int cvor) const;
       void postaviOznakuGrane(int polazni, int dolazni, Tip oznaka);
       Tip dajOznakuGrane(int polazni,int dolazni) const;
       Grana<Tip> dajGranu(int polazni, int dolazni);
       Cvor<Tip> dajCvor(int cvor);
};

template <typename Tip>
Cvor<Tip> ListaGraf<Tip>::dajCvor(int cvor) {
    ispravanCvor(cvor);
    return Cvor<Tip>(this,cvor);
}

template <typename Tip>
Grana<Tip> ListaGraf<Tip>::dajGranu(int polazni, int dolazni) {
    postojiLiGrana(polazni,dolazni);
    return Grana<Tip>(this,polazni,dolazni);
}

template <typename Tip>
Tip ListaGraf<Tip>::dajOznakuGrane(int polazni, int dolazni) const {
    postojiLiGrana(polazni,dolazni);
    return traziElement(polazni,dolazni)->oznaka;
}

template <typename Tip>
void ListaGraf<Tip>::postaviOznakuGrane(int polazni, int dolazni, Tip oznaka) {
    postojiLiGrana(polazni,dolazni);
    traziElement(polazni,dolazni)->oznaka=oznaka;
}

template <typename Tip>
Tip ListaGraf<Tip>::dajOznakuCvora(int cvor) const {
    ispravanCvor(cvor);
    return oznakeCvorova[cvor];
}

template <typename Tip>
void ListaGraf<Tip>::postaviOznakuCvora(int cvor, Tip oznaka) {
    ispravanCvor(cvor);
    oznakeCvorova[cvor]=oznaka;
}

template <typename Tip>
bool ListaGraf<Tip>::postojiGrana(int polazni, int dolazni) const {
    ispravnaGrana(polazni,dolazni);
    return traziElement(polazni,dolazni)!=l[polazni].end();
}

template <typename Tip>
float ListaGraf<Tip>::dajTezinuGrane(int polazni, int dolazni) const {
    postojiLiGrana(polazni,dolazni);
    return traziElement(polazni,dolazni)->tezina;
}

template <typename Tip>
void ListaGraf<Tip>::postaviTezinuGrane(int polazni, int dolazni, float tezina) {
    postojiLiGrana(polazni,dolazni);
    traziElement(polazni,dolazni)->tezina=tezina;
}

template <typename Tip>
typename std::list<Element<Tip>>::const_iterator ListaGraf<Tip>::traziElement(int i, int j) const {
    auto it=l[i].begin();
    while (it!=l[i].end()) {
        if(j==it->cvor) return it;
        else if(j<it->cvor) return l[i].end();
        it++;
    }
    return it;
}

template <typename Tip>
typename std::list<Element<Tip>>::iterator ListaGraf<Tip>::traziElement(int i, int j) {
    auto it=l[i].begin();
    while (it!=l[i].end()) {
        if(j==it->cvor) return it;
        else if(j<it->cvor) return l[i].end();
        it++;
    } 
    return it;
}
 
template <typename Tip>
void ListaGraf<Tip>::obrisiGranu(int polazni, int dolazni) {
    postojiLiGrana(polazni,dolazni);
    l[polazni].erase(traziElement(polazni,dolazni));
}

template <typename Tip>
void ListaGraf<Tip>::dodajGranu(int polazni, int dolazni, float tezina) {
    ispravnaGrana(polazni,dolazni);
    auto it=l[polazni].begin();
    while(it!=l[polazni].end() && dolazni>it->cvor) it++;
    Element<Tip> temp;
    temp.cvor=dolazni; 
    temp.tezina= tezina; 
    temp.oznaka=Tip(); 
    l[polazni].insert(it, temp); 
}

template <typename Tip>
void ListaGraf<Tip>::postaviBrojCvorova(int n) {
    if(n<dajBrojCvorova()) throw "Error";
    l.resize(n);
    oznakeCvorova.resize(n);
}

template <typename Tip>
ListaGraf<Tip>::ListaGraf(int n) {
    if(n<0) throw "Error";
    l=std::vector<std::list<Element<Tip>>>(n, std::list<Element<Tip>>());
    oznakeCvorova=std::vector<Tip>(n);
}




template <typename Tip>
void dfs(UsmjereniGraf<Tip>* graf, std::vector<Cvor<Tip>> &dfsobilazak, Cvor<Tip> cvor) {
    cvor.postaviOznaku(1);
    dfsobilazak.push_back(cvor);
    for(GranaIterator<Tip> it=graf->dajGranePocetak();it!=graf->dajGraneKraj();++it) {
        Cvor<Tip> susjed=(*it).dajDolazniCvor();
        if((*it).dajPolazniCvor().dajRedniBroj()==cvor.dajRedniBroj() && susjed.dajOznaku()!=1)
        dfs(graf,dfsobilazak,susjed);
    }
}

template <typename Tip>
void bfs(UsmjereniGraf<Tip>* graf, std::vector<Cvor<Tip>> &bfsobilazak, Cvor<Tip> cvor) {
    cvor.postaviOznaku(1);
    bfsobilazak.push_back(cvor);
    std::queue<Cvor<Tip>> q;
    q.push(cvor);
    while(!q.empty()) {
        Cvor<Tip> c=q.front();
        q.pop();
        for(GranaIterator<Tip> it=graf->dajGranePocetak();it!=graf->dajGraneKraj();it++) {
            Cvor<Tip> susjed=(*it).dajDolazniCvor();
            if((*it).dajPolazniCvor().dajRedniBroj()==c.dajRedniBroj() && susjed.dajOznaku()!=1) {
                susjed.postaviOznaku(1);
                bfsobilazak.push_back(susjed);
                q.push(susjed);
            }
        }
    }
}

  
int main() {
    std::cout << "Zadaća 5, Zadatak 1";
    return 0;
}
