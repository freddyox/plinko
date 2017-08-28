#ifndef PRISM_HH
#define PRISM_HH

#include <SFML/Graphics.hpp>

#include <vector>

class Prism : public sf::Drawable, public sf::Transformable{

private:
  double fDisx, fDisy;

  sf::VertexArray fLine,fTriangle;
  std::vector<sf::VertexArray> fPrism;
  std::vector<sf::VertexArray> fPhotons;

  sf::Vector2f fIncidencePoint;
  sf::Vector2f fNormal,fParallel;

  sf::Vector2f fIncidencePointR;
  sf::Vector2f fNormalR,fParallelR,fTop;
  
  double fTheta1, fTheta2, fTheta3;
  double fn1, fn2;
  
public:
  Prism(double,double);
  ~Prism() {};

  void draw(sf::RenderTarget&, sf::RenderStates) const;
  void handleFunctions();
  
  void makePrism(double,sf::Vector2f, sf::Color );
  void generateWhiteLight();
  void refract_and_disperse();
  sf::Vector2f intersect(sf::Vector2f, sf::Vector2f, sf::Vector2f, sf::Vector2f);
  
};
#endif
