#include <iostream>
#include <array>
#include <cstdlib>
#include <ctime>

using namespace std;

const int nmax = 6;
using Labyrinthe = std::array<std::array<char,nmax>,nmax>;

void CreerVide(Labyrinthe & lab) {
 cout<<"(CreerVide)"<<endl<<endl;
 for(int i=0;i<nmax;++i) {
  for(int j=0;j<nmax;++j) {
   if((j == 0) or (i == 0) or (i == (nmax-1)) or (j == (nmax-1))){
    lab[i][j] = '*';
   } else 
   lab[i][j] = ' ';
  }
 }
}

void PlacerObstacles(Labyrinthe & lab, int pobs) {
 cout<<"(PlacerObstacles)"<<endl<<endl;
 int obstacles = ((((nmax - 2) * (nmax - 2))  * pobs) / 100);
 
 cout<<" obstacles: "<<obstacles<<" nmax: "<<nmax<<" pobs: "<<pobs<<" a "<<""<<endl;
 
 int i,j;
 
 while(obstacles>0) {
  i = (rand()%(nmax-1))+1;
  j = (rand()%(nmax-1))+1;
  
  if(lab[i][j] != '*') {
   lab[i][j] = '*';
   --obstacles;
  }
 }
}

void PlacerSourisFromage(Labyrinthe & lab) {
 cout<<"(PlacerSourisFromage)"<<endl<<endl;
 bool tmp = true;
 int i,j;
 
 while(tmp) { //placer souris
  i = (rand()%(nmax-1))+1;
  j = (rand()%(nmax-1))+1;
  
  if(lab[i][j] != '*'){
   lab[i][j] = 'S';
   tmp = false;
   cout<<"(PlacerSourisFromage) souris ligne: "<<i<<" colonne: "<<j<<endl;
  }  
 }
 
 tmp = true;
 while(tmp){ //placer fromage
  i = (rand()%(nmax-1))+1;
  j = (rand()%(nmax-1))+1;
  
  if((lab[i][j] != '*') and (lab[i][j] != 'S')){
   lab[i][j] = 'F';
   tmp = false;
  }  
 }
}

void Afficher(Labyrinthe lab){
 cout<<"(Afficher)"<<endl;
 for(int i=0;i<nmax;++i){
  for(int j=0;j<nmax;++j){
   cout<<lab[i][j];
  }
  cout<<endl;
 }
 cout<<endl<<endl;
}

//f)
struct Position{
 int ligne;
 int colonne;
};

Position Depart(Labyrinthe lab){
 Position res;
 for(int i=1;i<nmax-1;++i){
  for(int j=1;j<nmax-1;++j){
   if(lab[i][j] == 'S'){
    res.ligne = i;
    res.colonne = j;
    return res;
   }
  }
 }
 //erreur
 res.ligne = -1;
 res.colonne = -1;
 return res;
}

//g)
const int nmaxP = 20;
using tabPiles = std::array<Position,nmaxP>;

struct PilePosition{
 int som;
 tabPiles Piles;
};

void Initialiser(PilePosition & pPos){
 pPos.som = 0;
}

bool EstVide(PilePosition pPos){
 return (pPos.som == 0);
}

void Empiler(PilePosition & pPos, Position P){
 if((pPos.som+1) == (nmaxP-1)){
  cout<<"(Empiler) erreur pPos pleinne"<<endl<<endl;
 } else {
  pPos.Piles[pPos.som].ligne = P.ligne;
  pPos.Piles[pPos.som].colonne = P.colonne;
  ++pPos.som;
 }
}

Position Sommet(PilePosition pPos){
 return pPos.Piles[pPos.som];
}

void Depiler(PilePosition & pPos){
 if(!EstVide(pPos)) --pPos.som;
}

void Vider(PilePosition & pPos){
 pPos.som = 0;
}

Position AExplorer(Labyrinthe lab, Position P){
 if((lab[P.ligne+1][P.colonne] == ' ') or (lab[P.ligne+1][P.colonne] == 'F')) ++P.ligne;
 else if((lab[P.ligne-1][P.colonne] == ' ') or (lab[P.ligne-1][P.colonne] == 'F')) --P.ligne;
 else if((lab[P.ligne][P.colonne+1] == ' ') or (lab[P.ligne][P.colonne+1] == 'F')) ++P.colonne;
 else if((lab[P.ligne][P.colonne-1] == ' ') or (lab[P.ligne][P.colonne-1] == 'F')) --P.colonne;
 else {//erreur la souris ne peut pas bouger
  P.ligne = -1;
  P.colonne = -1;
 }
 //cout<<"(AExplorer) P ligne: "<<P.ligne<<" colonne: "<<P.colonne<<endl<<endl;
 return P;
}

void labyrintheResolution(Labyrinthe & lab) {
 Position S = Depart(lab); //S : position de la souris
 PilePosition pPos;
 
 cout<<"(labyrintheResolution) erreur"<< " ligne: "<<S.ligne<<" colonne: "<<S.colonne<<endl<<endl;
 
 Initialiser(pPos);
 lab[S.ligne][S.colonne] = '.';
 
 unsigned int nb = -1;
 
 while(true) {
  ++nb;
  
  if(lab[S.ligne][S.colonne] == 'F') {
   cout<<" la souris a trouvé le fromage"<<endl<<endl;
   return;
  } else if(S.ligne == -1) {
    
    if(EstVide(pPos)){
     cout<<" il n'y a pas de solution pour trouver le fromage"<<endl<<endl;
     return;
    }
    
    cout<<"(labyrintheResolution) Sommet1 erreur pPos.som: "<<pPos.som<< " Sommet(pPos).ligne: "<<Sommet(pPos).ligne<<" Sommet(pPos).colonne: "<<Sommet(pPos).colonne<<endl<<endl;
    
    Depiler(pPos);
    
    cout<<"(labyrintheResolution) Sommet2 erreur pPos.som: "<<pPos.som<< " Sommet(pPos).ligne: "<<Sommet(pPos).ligne<<" Sommet(pPos).colonne: "<<Sommet(pPos).colonne<<endl<<endl;
    
    S.ligne = Sommet(pPos).ligne;
    S.colonne = Sommet(pPos).colonne;
    
    cout<<"(labyrintheResolution) Souris erreur nb: "<<nb<< " ligne: "<<S.ligne<<" colonne: "<<S.colonne<<endl<<endl;
    
  } else{//pas trouve fromage
   Empiler(pPos, S);
   lab[S.ligne][S.colonne] = '.';
   
  }
  
  S = AExplorer(lab,S);
  Afficher(lab);
  
 }
 
}


int main(){
 srand(time(NULL));
 Labyrinthe lab;
 
 CreerVide(lab);
 
 int pobs = 50;
 
 /*if(((((nmax - 2) * (nmax - 2))  * pobs) / 100) >= (((nmax - 2) * (nmax - 2))-1)){
  cout<<" (probs) erreur trop d obstacle"<<endl<<endl;
  return 0;
 }
 */
 
 PlacerObstacles(lab, pobs); //gere mal lor
 
 PlacerSourisFromage(lab); //dans cette version la position de la souris n'est pas celle du fromage
 
 Afficher(lab);
 
 labyrintheResolution(lab);
 
 return 0;
}
