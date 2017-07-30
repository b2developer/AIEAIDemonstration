#pragma once
#include "Graph.h"
#include "MathsLibrary.h"
#include "NavMeshStructures.h"
#include "Heuristics.h"
#include <vector>


//forward declaration
class Application2D;

/*
* class NavMesh
*
* holds a pathfinding graph structure, dynamically manages it
* can calculate the shortest path between two nodes on the map
*
* uses a completely customisable heurstic function
*
* responsible for cleaning up the pathfinding structure after
* A* pathfinding calculations are run on it
*
* smooths paths by calculating which nodes in the path are 
* irrelevant and can be pruned from the final list of vectors
*
* author: Bradley Booth, Academy of Interactive Entertainment, 2017
*/
class NavMesh
{
public:

	//pointer to the game
	Application2D* appPtr = nullptr;

	//heurstic function pointer
	float(*heursticFunction)(Vector2 current, Vector2 end);

	//the graph structure to transverse with A*
	Graph<NavNode*, NavConnection*> data;

	//list of vertices, every three vertices form a triangle
	std::vector<Vertex<NavNode*, NavConnection*>*> triangleData;

	/*
	* NavMesh()
	* default constructor 
	*/
	NavMesh() {}


	/*
	* ~NavMesh()
	* destructor, deletes all path nodes and path connections
	*/
	~NavMesh();


	/*
	* createGrid
	*
	* generates a graph that resembles 2D grid
	* to specific dimensions and spacing
	*
	* @param unsigned int sizeX - the amount of nodes in a row
	* @param unsigned int sizeY - the amount of nodes in a column
	* @param Vector2 origin - the position of the bottom left most node
	* @param Vector2 spacing - the horizontal and vertical gaps between nodes
	* @returns void
	*/
	void createGrid(unsigned int sizeX, unsigned int sizeY, Vector2 origin, Vector2 spacing);


	/*
	* createMesh
	*
	* generates a mesh from a list of vertices
	* and a list of triangles
	*
	* @param std::vector<Vector2> vertices - the location of all vertices
	* @param std::vector<int> triangles - a list of numbers, every three numbers are vertex indices that form a triangle
	* @returns void
	*/
	void createMesh(std::vector<Vector2> vertices, std::vector<int> triangles);


	/*
	* calculateSharedEdges
	* 
	* examines all triangles and determines
	* all edges in the entire mesh that are shared
	* by more than one triangle
	*
	* also removes duplicate edges from the mesh
	*
	* @returns void;
	*/
	void calculateSharedEdges();


	/*
	* load
	*
	* loads a navmesh from a .txt file path
	*
	* @param filename[FILENAME_MAX] - c-string of the file path
	* @returns void
	*/
	void load(char fileName[FILENAME_MAX]);


	/*
	* resetSearchedNodes
	*
	* resets all nodes that were manipulated by the A* search
	* back to their default values to be used again
	*
	* @param std::vector<Vertex<NavNode*, NavConnection*>*> open - list of nodes that were open when the A* search terminated
	* @param std::vector<Vertex<NavNode*, NavConnection*>*> closed - list of nodes that were closed when the A* search terminated
	* @returns void
	*/
	void resetSearchedNodes(std::vector<Vertex<NavNode*, NavConnection*>*> open, std::vector<Vertex<NavNode*, NavConnection*>*> closed);


	/*
	* findRawPath
	*
	* runs the A* pathfinding algorithm on the graph
	* to find the shortest path between the start and end nodes
	*
	* the path returned is raw data off the graph, optimisations
	* could be made to the path to reduce the amount of vectors in it
	* or make it smoother
	*
	* @param Vertex<NavNode*, NavConnection*>* start - the starting node
	* @param Vertex<NavNode*, NavConnection*>* end - the target node
	* @param float E = 1.0f - heurstic multiplier, has varying effects on performance and the correctness of paths calculated
	* @returns std::vector<Vector2> - a list of vectors that make up the path
	*/
	std::vector<Vector2> findRawPath(Vertex<NavNode*, NavConnection*>* start, Vertex<NavNode*, NavConnection*>* end, float E = 1.0f);

	
	/*
	* smoothPath
	*
	* prunes a raw list of points returned from an A* search
	* by testing the LOS between various points on the path with the NavMesh
	*
	* @param std::vector<Vector2> rawPath - the list of points in the original path
	* @returns std::vector<Vector2> - a list of points with the unneccessary points pruned
	*/
	std::vector<Vector2> smoothPath(std::vector<Vector2> rawPath);


	/*
	* drawMesh
	*
	* draws all the nodes and connections
	* that form the mesh
	*
	* @param float nodeRadius - the radius of circles that represent nodes
	* @param float connectionThickness - the thickness of lines that represent connections
	* @param float r = 1.0f - the portion of red to draw the graph with
	* @param float g = 1.0f - the portion of green to draw the graph with
	* @param float b = 1.0f - the portion of blue to draw the graph with
	* @param float depth = 0.0f - the z coordinate, determines the order that the mesh is drawn in relation to other textures
	* @returns void
	a
	*/
	void drawMesh(float nodeRadius, float connectionThickness, float r = 1.0f, float g = 1.0f, float b = 1.0f, float depth = 0.0f);

};