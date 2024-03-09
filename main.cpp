#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

using ligne = std::vector<char>;
using colonne = std::vector<char>;
using Labyrinthe = std::vector<colonne>;
struct Position{
 int indexLigne;
 int indexColonne;
};
using Positions = std::vector<Position>;

// Fonctions utilitaires
void Afficher(Labyrinthe lab) {
 for(ligne lig : lab){
  for(char valeurCase : lig){
   std::cout << valeurCase;
  }
  std::cout << std::endl;
 }
 std::cout << std::endl;
}
Positions getPosCasesVide(Labyrinthe lab) {
 Positions posCasesVide;
 Position tmpPos;
 unsigned int nbLigne = lab.size();
 unsigned int nbColonne = lab.at(0).size();

 for(unsigned int indexLigne=0; indexLigne<nbLigne; ++indexLigne) {
  for(unsigned int indexColonne=0; indexColonne<nbColonne; ++indexColonne) {
   if(lab.at(indexLigne).at(indexColonne) == ' ') {
    tmpPos.indexLigne = indexLigne;
    tmpPos.indexColonne = indexColonne;
    posCasesVide.push_back(tmpPos);
   }
  }
 }

 return posCasesVide;
}

// Fonctions d'initialisation du programme
struct LigCol {
 unsigned int nbLigne, nbColonne; 
};
void SaisirLigCol(LigCol & lg) {
 std::cout << "Saisir le nombre de ligne (superieur a 1): ";
 std::cin >> lg.nbLigne;
 std::cout << "Saisir le nombre de colonne (superieur a 1): ";
 std::cin >> lg.nbColonne;
}
void SaisirPobs(unsigned int & pobs) {
  std::cout << "Saisir le taux de remplissage des murs interieurs (de 0 à 100): ";
  std::cin >> pobs;
  if (pobs > 100) {
    std::cout << "Erreur. Le taux de remplissage est superieur a 100." << std::endl;
    exit(1);
  }
}
void DefinirLabyrinthe(LigCol & lg, unsigned int & pobs) {
 std::cout << "Definir le labyrinthe:" << std::endl;
 SaisirLigCol(lg);
 SaisirPobs(pobs);
}
void CreerVide(Labyrinthe & lab, LigCol lg) {
 unsigned int nbLigne, nbColonne;
 nbLigne = lg.nbLigne;
 nbColonne = lg.nbColonne;
 ligne tmpLigne;

 for(unsigned int indexLigne=0; indexLigne<nbLigne; ++indexLigne) {
  for(unsigned int indexColonne=0; indexColonne<nbColonne; ++indexColonne) {
   if((indexColonne == 0) or (indexLigne == 0) or (indexLigne == (nbLigne-1)) or (indexColonne == (nbColonne-1)))
    tmpLigne.push_back('*'); // bords du labyrinthe
   else 
    tmpLigne.push_back(' '); // interieur du labyrinthe
  }
  lab.push_back(tmpLigne);
  tmpLigne.clear();
 }
}
void PlacerObstacles(Labyrinthe & lab, int pobs) {
 Positions posCasesVide = getPosCasesVide(lab);
 unsigned int nbObstacle = (posCasesVide.size() * pobs) / 100; // arrondi a l'entier inferieur
 unsigned int randIndexPosCasesVide;
 
 // Prend une case vide aleatoire et le remplace par un obstacle
 for(unsigned int indexNbObstacle=0; indexNbObstacle<nbObstacle; ++indexNbObstacle) {
  randIndexPosCasesVide = (rand() % posCasesVide.size());
  lab.at(posCasesVide.at(randIndexPosCasesVide).indexLigne).at(posCasesVide.at(randIndexPosCasesVide).indexColonne) = '*';
  std::vector<Position>::iterator it = posCasesVide.begin();
  std::advance(it, randIndexPosCasesVide);
  posCasesVide.erase(it);
 }
}
void PlacerSourisFromage(Labyrinthe & lab) {
 Positions posCasesVide = getPosCasesVide(lab);
 unsigned int randIndexPosCasesVide;

 if(posCasesVide.size() < 2) {
   std::cout << "Erreur. Il n'y a pas assez de case vide (2) pour placer le fromage et la souris." << std::endl;
   exit(1);
 }

 // placer souris
 randIndexPosCasesVide = (rand() % posCasesVide.size());
 lab.at(posCasesVide.at(randIndexPosCasesVide).indexLigne).at(posCasesVide.at(randIndexPosCasesVide).indexColonne) = 'S';
 std::vector<Position>::iterator it = posCasesVide.begin();
 std::advance(it, randIndexPosCasesVide);
 posCasesVide.erase(it);
 // placer fromage
 randIndexPosCasesVide = (rand() % posCasesVide.size());
 lab.at(posCasesVide.at(randIndexPosCasesVide).indexLigne).at(posCasesVide.at(randIndexPosCasesVide).indexColonne) = 'F';
 it = posCasesVide.begin();
 std::advance(it, randIndexPosCasesVide);
 posCasesVide.erase(it);
}
Position getPosSouris(Labyrinthe lab){
 Position posSouris;
 unsigned int nbLigne = lab.size();
 unsigned int nbColonne = lab.at(0).size();
 
 for(unsigned int indexLigne=0; indexLigne<nbLigne; ++indexLigne) {
  for(unsigned int indexColonne=0; indexColonne<nbColonne; ++indexColonne) {
   if(lab.at(indexLigne).at(indexColonne) == 'S') {
    posSouris.indexLigne = indexLigne;
    posSouris.indexColonne = indexColonne;
    return posSouris;
   }
  }
 }
 // La souris n'a pas été trouvée
 posSouris.indexLigne = -1;
 posSouris.indexColonne = -1;
 return posSouris;
}

// Pile
struct PilePosition{
 unsigned int som;
 Positions Piles;
};
void Initialiser(PilePosition & pPos) {
 pPos.som = 0;
}
bool EstVide(PilePosition pPos) {
 return (pPos.som == 0);
}
void Empiler(PilePosition & pPos, Position P) {
 if(pPos.Piles.size() == pPos.som)
  pPos.Piles.push_back(P);
 else
  pPos.Piles.at(pPos.som-1) = P;
 ++pPos.som;
}
Position Sommet(PilePosition pPos) {
 return pPos.Piles.at(pPos.som-1);
}
void Depiler(PilePosition & pPos) {
 if(!EstVide(pPos)) --pPos.som;
}
void Vider(PilePosition & pPos) {
 pPos.som = 0;
}

// Fonctions de simulation
Position AExplorer(Labyrinthe lab, Position P) {
 // Cherche les cases non explore ou le fromage du nord, sud, est puis ouest
 if(lab.at(P.indexLigne+1).at(P.indexColonne) == ' ' or lab.at(P.indexLigne+1).at(P.indexColonne) == 'F') ++P.indexLigne;
 else if(lab.at(P.indexLigne-1).at(P.indexColonne) == ' ' or lab.at(P.indexLigne-1).at(P.indexColonne) == 'F') --P.indexLigne;
 else if(lab.at(P.indexLigne).at(P.indexColonne+1) == ' ' or lab.at(P.indexLigne).at(P.indexColonne+1) == 'F') ++P.indexColonne;
 else if(lab.at(P.indexLigne).at(P.indexColonne-1) == ' ' or lab.at(P.indexLigne).at(P.indexColonne-1) == 'F') --P.indexColonne;
 else {
  // la souris ne peut pas bouger
  P.indexLigne = -1;
  P.indexColonne = -1;
 }
 return P;
}

void labyrintheResolution(Labyrinthe & lab) {
 Position posS = getPosSouris(lab); // position de depart
 PilePosition pPos;
 Initialiser(pPos);

 unsigned int indexMouvementSouris = 0;
 while(true) {
  ++indexMouvementSouris;
  lab.at(posS.indexLigne).at(posS.indexColonne) = '.';

  posS = AExplorer(lab,posS);
  if(posS.indexLigne == -1) {
   // revient en arrière
   Depiler(pPos);
   if(EstVide(pPos)) {
    std::cout << "La souris ne peut pas trouver le fromage." << std::endl << std::endl;
    return;
   }
   posS.indexLigne = Sommet(pPos).indexLigne;
   posS.indexColonne = Sommet(pPos).indexColonne;
  } else if(lab.at(posS.indexLigne).at(posS.indexColonne) == 'F') {
   std::cout << "La souris a trouve le fromage." << std::endl << std::endl;
   return;
  } else {
   // decouvre une nouvelle case
   Empiler(pPos, posS);
  }
  
  std::cout << "Mouvement n°" << indexMouvementSouris << ": " << std::endl;
  lab.at(posS.indexLigne).at(posS.indexColonne) = 'S';
  Afficher(lab);
 }
}

int main(){ 
 srand(time(NULL));
 Labyrinthe lab;
 LigCol lg;
 unsigned int pobs;
 DefinirLabyrinthe(lg, pobs);
 
 std::cout << std::endl << "Stades de la simulation:" << std::endl;
 CreerVide(lab, lg);
 std::cout << "Afficher le labyrinthe vide" << std::endl;
 Afficher(lab);
 std::cout << "Placer les obstacles" << std::endl;
 PlacerObstacles(lab, pobs);
 Afficher(lab);
 std::cout << "Placer la souris et le fromage" << std::endl;
 PlacerSourisFromage(lab);
 Afficher(lab);
 labyrintheResolution(lab);
 
 return 0;
}
