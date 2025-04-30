This project is my final project for Data Structures. In this project I will find the shortest flight paths based on distance from between US capital cities (airports).
The overall Data Structure for this project will be a fully-connected, undirected Graph. The Graph's vertices will store data such as the city's name, its latitudinal and longitudinal coordinates, 
its distance in the graph from the source node, and the parent node. The Graph's edges will store data such as the nodes at each end, and its weight. 

In order to determine the shortest flight paths, I will use an adaptation of Dijkstra's algorithm which dynamically calculates the weight between nodes accoring to the haversine formula. 
The haversine formula is given by, ![equation](https://latex.codecogs.com/svg.image?\dpi{110}\mathrm{hav}(\theta)=\mathrm{hav}(\Delta\phi)+\cos(\phi_{1})\cos(\phi_{2})\mathrm{hav}(\Delta\lambda))


