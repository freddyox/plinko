#ifndef PLINKO_HH
#define PLINKO_HH

#include <SFML/Graphics.hpp>

#include <vector>
#include <map>
#include <set>

#include "TRandom.h"
#include "TFile.h"

class Plinko : public sf::Drawable, public sf::Transformable{

private:
  TFile *fOut;
  
  double fDisx, fDisy;
  sf::Vector2f fCenter;

  TRandom fRand;
  int fNsimulations;
  
  ////////////////////////////////////////////////////////////////////////////////
  // The Lattice
  //
  int fN;
  double fDistance;
  std::vector<sf::CircleShape> fLattice;
  // key is cell no, starts at 0 top left, value is the coordinates:
  std::map<int, sf::Vector2f> fLatticeMap;
  std::map<int, std::vector<int> > fCellNeighbors;
  int fStartCell;
  double fSigma;
  
  ////////////////////////////////////////////////////////////////////////////////
  // The Plinko
  //
  sf::CircleShape fPlinko;
  double fPlinkoR;
  std::map<int, sf::Vector2f> fStartPositions;

  ////////////////////////////////////////////////////////////////////////////////
  // The Path
  //
  sf::VertexArray fLine;
  std::vector<sf::VertexArray> fLines;
  sf::Vector2f fLastSpot;
  sf::Color fColor;
  std::vector<double> fEndingLocation;
  
public:
  Plinko(double,double,int,int);
  ~Plinko() {};

  void draw(sf::RenderTarget&, sf::RenderStates) const;
  void generateLattice(float); //  radius of an obstacle
  void generatePlinko(int);
  void generateCellNeighbors();
  void randomWalkSimulation(int,int); // n sims, starting cell no (-1 is random)

  
  void randomWalkDown();

  void clear();
  
  int cell_to_row(int cell){return cell/fN;}
  int cell_to_col(int cell){return cell%fN;}

  // ROOT:
  void makePlots();
};
#endif
 
