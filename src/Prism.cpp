#include "../include/Prism.hh"

#include <iostream>
#include <cmath>

#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>

Prism::Prism(double x, double y) : fDisx(x), fDisy(y) {

  fLine = sf::VertexArray(sf::LinesStrip,2);
  fTriangle = sf::VertexArray(sf::Triangles,3);

  fTheta1 = fTheta2 = 0.0;
  fn1 = 1.0;
  fn2 = 1.33;
  
  srand (time(NULL));
}

void Prism::draw( sf::RenderTarget& target, sf::RenderStates) const {
  target.draw( fTriangle );

  for(unsigned int i=0; i<fPhotons.size(); i++) target.draw(fPhotons[i]);
}

void Prism::makePrism(double l, sf::Vector2f center, sf::Color color){
  double xc = center.x;
  double yc = center.y;

  // Assumed to be equilateral triangle
  double angle = 60.0 * 3.141592 / 180.0; 
  double blx = xc - l*sin( angle ); // Bottom Left
  double bly = yc + l*cos( angle ); // 
  double brx = xc + l*sin( angle ); // Bottom Right
  double bry = bly;                 // 
  double tx = xc;                   // Top 
  double ty = yc - l;               //

  // Bottem left point:
  fTriangle[0].position = sf::Vector2f(blx,bly);
  // Top point:
  fTriangle[1].position = sf::Vector2f(tx,ty);
  // Bottem right:
  fTriangle[2].position = sf::Vector2f(brx,bry);

  // Everything gets the same color:
  fTriangle[0].color = color;
  fTriangle[1].color = color;
  fTriangle[2].color = color;

  double xmean = (blx + tx)/2.0;
  double ymean = (bly + ty)/2.0;
  fIncidencePoint = sf::Vector2f(xmean,ymean);
  std::cout << "("<< fIncidencePoint.x << ", " << fIncidencePoint.y <<")" << std::endl;

  sf::Vector2f ftemp =  sf::Vector2f(tx,ty) - sf::Vector2f(blx,bly);
  double mag = sqrt( pow(ftemp.x,2) + pow(ftemp.y,2) );
  fParallel = sf::Vector2f(ftemp.x / mag, ftemp.y / mag ); // Parallel to Left Face
  fNormal = sf::Vector2f(-fParallel.y, fParallel.x);       // Points back towards origin

  ftemp = sf::Vector2f(brx,bry) - sf::Vector2f(tx,ty);
  //mag = sqrt( pow(ftemp.x,2) + pow(ftemp.y,2) );
  mag = 1.0;
  fParallelR = sf::Vector2f(ftemp.x / mag, ftemp.y / mag ); // Parallel to Left Face
  fTop = sf::Vector2f(tx,ty);
  
  fLine[0].position = fTop;
  fLine[1].position = fTop + sf::Vector2f(fParallelR.x,fParallelR.y);
  fLine[0].color = sf::Color::Red;
  fLine[1].color = sf::Color::Red;
  //fPhotons.push_back( fLine );
  
  // Temporary:
  fLine[0].position = fIncidencePoint;
  fLine[1].position = fIncidencePoint + sf::Vector2f(500*fNormal.x,500*fNormal.y);
  fLine[0].color = sf::Color::Red;
  fLine[1].color = sf::Color::Red;
  fPhotons.push_back( fLine );
  sf::Vector2f extend = fIncidencePoint - sf::Vector2f(500*fNormal.x,500*fNormal.y);
  fLine[0].position = fIncidencePoint;
  fLine[1].position = extend;
  fLine[0].color = sf::Color::Red;
  fLine[1].color = sf::Color::Red;
  fPhotons.push_back( fLine );
  
  handleFunctions();
}

void Prism::handleFunctions(){
  generateWhiteLight();
  refract_and_disperse();
}

void Prism::generateWhiteLight(){

  double ylow = fTriangle[0].position.y - 20;
  int ytemp = rand() % int(ylow) + 10; // 10 -> xlow-10
  int xtemp = 20;

  double xs = xtemp;
  double ys = ytemp;
  
  fLine[0].position = sf::Vector2f(xs,ys);
  fLine[1].position = fIncidencePoint;
  fLine[0].color = sf::Color::White;
  fLine[1].color = sf::Color::White;
  fPhotons.push_back( fLine );
  
  sf::Vector2f ftemp = fIncidencePoint - sf::Vector2f(xs,ys);
  double mag = sqrt( pow(ftemp.x,2) + pow(ftemp.y,2) );
  sf::Vector2f norm1(ftemp.x/mag, ftemp.y/mag);

  fTheta1 = acos( norm1.x*fNormal.x + norm1.y*fNormal.y );
  fTheta2 = asin( (fn1/fn2)*sin(fTheta1) );

  double rad = 300.0;
  sf::Vector2f n2(rad*norm1.x*cos( fTheta2 ), rad*norm1.y*sin( fTheta2 ) );
  sf::Vector2f ray3 = intersect(fIncidencePoint,n2,fTop,fParallelR);

  // fLine[0].position = fIncidencePoint;
  // fLine[1].position = fIncidencePoint + n2;

  sf::Color col(255,0,0);
  // fLine[0].color = col;
  //  fLine[1].color = col;
  // fPhotons.push_back( fLine );

  fLine[0].position = fIncidencePoint;
  fLine[1].position = fIncidencePoint+n2;
  fLine[0].color = sf::Color::Green;
  fLine[1].color = sf::Color::Green;
  fPhotons.push_back( fLine );
  fLine[0].position = fTop;
  fLine[1].position = fTop+fParallelR;
  fLine[0].color = sf::Color::Black;
  fLine[1].color = sf::Color::Black;
  fPhotons.push_back( fLine );
}

void Prism::refract_and_disperse(){
  ;

}

sf::Vector2f Prism::intersect(sf::Vector2f P1, sf::Vector2f P2,
			      sf::Vector2f P3, sf::Vector2f P4){
  float x1 = P1.x;
  float x2 = P2.x;
  float x3 = P3.x;
  float x4 = P4.x;
  float y1 = P1.y;
  float y2 = P2.y;
  float y3 = P3.y;
  float y4 = P4.y;
  float test[] = {x1,x2,x3,x4,y1,y2,y3,y4};
  for(int i=0; i<8; i++){
    std::cout << i << "  " << test[i] << std::endl;
  }

  float Snum = (x4-x3)*(y1-y3) - (y4-y3)*(x1-x3);
  float Sden = (y4-y3)*(x2-x1) - (x4-x3)*(y2-y1);

  std::cout << Sden << std::endl;
  float S = 0.0;
  if( Sden <= 1e-8 ) {
    std::cout << "Lines are parallel..." << std::endl;
    S=0.0;
  } else {
    S = Snum / Sden;
  }
  std::cout << "Should be on the order of 100 " << S << std::endl;
  float x = x1 + S*(x2-x1);
  float y = y1 + S*(y2-y1);
  return sf::Vector2f(x,y);
}
			   
