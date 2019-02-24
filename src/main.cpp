//    ************************************************************
//    *                         PLINKO                           *
//    *                       August 2017                        *
//    ************************************************************
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include <iostream>

#include "../include/Plinko.hh"

const double gDisplayx = 1000;
const double gDisplayy = 1000;

int main() {

  sf::RenderWindow window(sf::VideoMode(gDisplayx,gDisplayy), "Plinko");
  window.setFramerateLimit(60);

  //////////////////////////////////////////////////////
  //                   Initialize                     //
  //////////////////////////////////////////////////////
  int Nsimulations = 350;
  Plinko plinko(gDisplayx, gDisplayy, 10, Nsimulations);

  while( window.isOpen() ) {

    sf::Event event;
    while( window.pollEvent(event) ) {
      if( event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) ) {
	sf::Image output = window.capture();
	output.saveToFile( Form("ssplinko_nsim_%d.jpg",Nsimulations) );
	std::cout << "Saving screenshot " << Form("ssplinko_nsim_%d.jpg",Nsimulations) << std::endl;
	window.close();
      }
    }
    window.clear(sf::Color::White);
    window.draw(plinko);
    window.display();      
  }
  return 0;
}
