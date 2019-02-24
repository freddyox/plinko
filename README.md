# Plinko
* A Monte carlo simulation of the famous plinko game; a full description of my program may be 
seen <a href="https://freddyox.github.io/blog/plinko/">here</a>.
* The simualation makes use of <a href="https://root.cern.ch/download/root_v6.14.04.source.tar.gz">CERN's ROOT libraries</a> 
for plotting and analysis in addition to the <a href="https://www.sfml-dev.org/">SFML multimedia library</a> for visualization. 

## Output
* The game drops a puck which must then traverse through a lattice of pegs, where the pegs are offset by half a lattice spacing
for all odd rows; therefore, the puck naturally random walks to the bottom. 
* The puck's path is built and visualized, and the experiment is repeated many times in order to see which bin the puck falls in 
at the end of it's path. Here is an example of many experiments:
![plinko](https://github.com/freddyox/plinko/blob/master/output/ssplinko_nsim_350.jpg)

* The puck's final position depends on the starting position. If one drops the puck in the center position and runs the experiment
many times, then the puck's final position takes a Gaussian distribution:
![distro](https://github.com/freddyox/plinko/blob/master/output/pinko_results.png)
