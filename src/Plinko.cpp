#include "../include/Plinko.hh"

#include <iostream>


Plinko::Plinko(double x, double y, int n) : fDisx(x), fDisy(y), fN(n) {

  generateLattice(10.0);
}


void Plinko::draw( sf::RenderTarget& target, sf::RenderStates) const {
  target.draw( fLattice[0] );
}

void Plinko::generateLattice(float rad){

  sf::CircleShape circ(rad);
  sf::FloatRect bounds = circ.getLocalBounds();
  circ.setOrigin(0.5*bounds.width, 0.5*bounds.height);
  circ.setPosition(fDisx/2.0, fDisy/2.0);
  fLattice.push_back( circ );

}
