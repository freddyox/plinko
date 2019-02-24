#include "../include/Plinko.hh"

#include <iostream>
#include "TH1D.h"
#include "TF1.h"

Plinko::Plinko(double x, double y, int n, int Nsim)
  : fDisx(x), fDisy(y), fN(n), fNsimulations(Nsim) {

  if( fNsimulations < 1 ) {
    std::cout << "Stupid inputs...setting Nsim=1" << std::endl;
    fNsimulations = 1;
  }
  if( fNsimulations > 1e8 ) {
    fNsimulations = 1e6;
    std::cout << "Nsim too high, setting to 1M" << std::endl;
  }
  
  fOut = new TFile(Form("output_plinko_%d.root",Nsim),"RECREATE");
  
  fCenter = sf::Vector2f( 0.5*fDisx, 0.5*fDisy );
  
  // if( fN > 10 ) fN = 10;
  fRand.SetSeed(0);

  fLine = sf::VertexArray(sf::Lines,2);

  fColor = sf::Color::Green;
  
  fStartCell = fN;
  generateLattice(8.0);
  
  // Map to handle neighbor searching
  generateCellNeighbors();

  // Generate our Plinko
  fPlinko.setRadius( fPlinkoR );
  sf::FloatRect box = fPlinko.getLocalBounds();
  fPlinko.setOrigin( 0.5*box.width, 0.5*box.height );
  fPlinko.setFillColor( sf::Color::White );

  // Nsims, starting position
  randomWalkSimulation(fNsimulations,fN/2-1);

  makePlots();
}

void Plinko::randomWalkSimulation(int Nsimulations, int starting_cell){
  for(int i=0; i<Nsimulations; i++){
    clear();
    int r = fRand.Integer(256);
    int g = fRand.Integer(256);
    int b = fRand.Integer(256);
    // fColor = sf::Color(r,g,b);
    fColor = sf::Color::Black;
    generatePlinko(starting_cell);
    randomWalkDown();
  }
}


void Plinko::draw( sf::RenderTarget& target, sf::RenderStates) const {
  for(unsigned int i=0; i<fLattice.size(); i++) {
    target.draw( fLattice[i] );
  }
  target.draw( fPlinko );
  for(unsigned int i=0; i<fLines.size(); i++){
    target.draw( fLines[i] );
  }
}

void Plinko::generateLattice(float rad){
  // how much space for lattice
  double radius = rad;
  double d_circles = fN * radius * 2.0;
  
  if( d_circles > 0.33 * fDisx ||
      d_circles > 0.33 * fDisy ){
    radius = 0.25 * rad;
  }

  double xstart = fDisx * 0.15;
  double ystart = fDisy * 0.15;
  double d = fDisx - 2.0 * xstart;
  d /= fN;
  fDistance = d;
  
  // We can specify the plinko radius now:
  fPlinkoR = 0.75 * (d/2.0 - radius*2.0);
  if( fPlinkoR <= 0.0 ) {
    std::cout << "Plinko radius is too small..." << std::endl;
    fPlinkoR = 2.0;
  }
  fSigma = fPlinkoR*0.7;
  
  sf::CircleShape circ(radius);
  sf::FloatRect bounds = circ.getLocalBounds();
  circ.setOrigin(0.5*bounds.width, 0.5*bounds.height);
  
  for(int row=0; row<fN; row++){
    for(int col=0; col<fN; col++){
      double xtemp = xstart + col*d - radius;
      if( row%2 != 0 ) xtemp += d/2.0;
      
      double ytemp = ystart + row*d - radius;
      circ.setPosition( xtemp, ytemp );
      circ.setFillColor( sf::Color(189,189,189) );
      fLattice.push_back( circ );
      
      // fLattice and fLatticeMap have same indexing
      int index = row*fN + col;
      fLatticeMap[index] = sf::Vector2f(xtemp,ytemp);

      // Fill Possible starting positions:
      if( row ==0 ) fStartPositions[col] = sf::Vector2f(xtemp,ytemp);
    }
  }

}

void Plinko::generateCellNeighbors(){
  std::map<int,sf::Vector2f>::iterator mit;
  for(mit = fLatticeMap.begin(); mit!=fLatticeMap.end(); mit++){
    int cell = mit->first;
    sf::Vector2f pos = mit->second;
    for(unsigned inner=cell+1; inner<fLatticeMap.size(); inner++){
      sf::Vector2f neigh = fLatticeMap[inner];
      sf::Vector2f diff = neigh - pos;
      double D = sqrt( pow(diff.x,2.0) + pow(diff.y,2.0) );
      
      // 1.5 b/c it's really D*sqrt(2), y>0.0 b/c I don't want anything that is in
      // the same row or above it (plinko goes downwards, not upwards)
      if( D < 1.5*fDistance && diff.y > 0.0 ){
	fCellNeighbors[cell].push_back( inner );
	//printf("%d \t %d \t %.1f \t %.1f \t %.1f \t %.1f \n",cell,inner,D,diff.x,diff.y,fDistance);
      }
    }
  }
}

void Plinko::generatePlinko(int cell){
  if( cell < 0 ) cell = 0;
  
  if( cell+1 > fN ) {
    std::cout << "Starting position is out of range...defaulting to center" << std::endl;
    if( fN%2==0 ) cell = fN/2 - 1;
    else          cell = (fN-1)/2;
  }
  
  int random = fRand.Integer(fN);       // 0 to 9
  int plusorminus = fRand.Integer(2); // 0 and 1

  if( cell >= 0 ) random = cell;
  
  sf::Vector2f start = fStartPositions[random];
  double ymodify = -3.0*fPlinkoR;  
  double xmodify = 0.0;

  // Randomly select a direction (left or right)
  if( plusorminus ){
    xmodify = fDistance / 2.0;
  } else {
    xmodify = -fDistance / 2.0;
  }
  
  if( cell >= 0 ) xmodify = fDistance/2.0;
		    
  // Fix Boundaries, cannot leave the plinko board!
  if( random==0 )   xmodify = fDistance / 2.0;
  if( random==fN-1) xmodify = -fDistance / 2.0;

  sf::Vector2f modify( start.x + xmodify, start.y + ymodify );

  fPlinko.setPosition( modify );

  // What cell is below?
  int counter = 0;
  for(unsigned int i=fN; i<2*fN-1; i++){
    sf::Vector2f temp( start.x + xmodify, start.y );
    sf::Vector2f closest = fLatticeMap[i];
    sf::Vector2f diff = temp - closest;
    double D = sqrt( pow(diff.x,2.0) + pow(diff.y,2.0) );
    if( D < 1.1*fDistance ){
      fStartCell = i;
      counter++;
    }
  }
  if(counter == 1){
    // std::cout << "Starting Plinko random calculation!" << std::endl;
  } else {
    std::cout << "There is something WRONG here, counter>1 which should not happen!" << std::endl;
  }

  fLastSpot = sf::Vector2f(fLatticeMap[fStartCell].x,
			   fLatticeMap[fStartCell].y - fDistance);
  fLine[0].position = modify;
  fLine[1].position = fLastSpot;
  fLine[0].color = fColor;
  fLine[1].color = fColor;
  fLines.push_back( fLine );
}

void Plinko::clear(){
  fStartCell = 0;
}

void Plinko::randomWalkDown(){
  for(int lvl=1; lvl<fN; lvl++){
    int cell = fStartCell;
    sf::Vector2f pos = fLatticeMap[cell];
    // need random number to decide if I go left of right
    int left_or_right = fRand.Integer(2); // 0 or 1
    double xnew = 0.0;
    if( left_or_right ){
      xnew = -fDistance/2.0;
    } else {
      xnew = fDistance/2.0;
    }

    if( (cell-lvl)%(fN-1)==0 && lvl>1 ) xnew = -fDistance/2.0;
    if( cell%fN==0 )       xnew = fDistance/2.0;
    

    int low = fN*(lvl+1);
    int hi = low + (fN-1);
    sf::Vector2f newpos( pos.x + xnew, pos.y );
    double smear = fRand.Gaus(0.0,fSigma);
    sf::Vector2f newpos_smear( pos.x + xnew + smear, pos.y );
   
    fLine[0].position = fLastSpot;
    fLine[1].position = newpos_smear;
    fLastSpot = newpos_smear;
    fLine[0].color = fColor;
    fLine[1].color = fColor;
    fLines.push_back( fLine );
    
    for(int search = low; search <= hi; search++){
      sf::Vector2f closest = fLatticeMap[search];
      sf::Vector2f diff = newpos - closest;
      double D = sqrt( pow(diff.x,2.0) + pow(diff.y,2.0) );
      //printf("%d \t %d \t %d \t %d \t %d \t %.1f \t %.1f", cell, search,
      //      left_or_right, cell%fN, (cell-lvl)%(fN-1), D, fDistance);
      
      if( D < 1.2*fDistance ){
	fStartCell = search;
	//printf("\t IN \n");
      } else {
	//printf("\n");
      }
    }  
  }

  sf::Vector2f final(fLastSpot.x, fLastSpot.y + fDistance);
  fLine[0].position = fLastSpot;
  fLine[1].position = final;
  fLine[0].color = fColor;
  fLine[1].color = fColor;
  fLines.push_back( fLine );

  // Save this info and we can plot it later:
  fEndingLocation.push_back( fLastSpot.x );
}

void Plinko::makePlots(){

  TH1D *hFinalDist = new TH1D("hFinalDis","",fN, fDisx*0.15, fDisx*0.85);
  for(unsigned int i=0; i<fEndingLocation.size(); i++){
    hFinalDist->Fill( fEndingLocation[i] );
  }
  hFinalDist->SetXTitle("Final bin");
  hFinalDist->SetLineColor(kBlue+2);
  hFinalDist->SetFillColor(kBlue+2);
  hFinalDist->SetFillStyle(3001);
  hFinalDist->GetXaxis()->CenterTitle();

  TF1 *hfit = new TF1("hfit","gaus");
  hFinalDist->Fit(hfit);

  fOut->Write();

}
