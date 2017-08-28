#ifndef PLINKO_HH
#define PLINKO_HH

#include <SFML/Graphics.hpp>

#include <vector>

class Plinko : public sf::Drawable, public sf::Transformable{

private:
  double fDisx, fDisy;

  // the lattice:
  int fN;
  std::vector<sf::CircleShape> fLattice;

  
public:
  Plinko(double,double,int);
  ~Plinko() {};

  void draw(sf::RenderTarget&, sf::RenderStates) const;
  void generateLattice(float); //  radius of an obstacle
  
};
#endif
