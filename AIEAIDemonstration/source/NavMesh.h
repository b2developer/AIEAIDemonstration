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
	Graph<NavMeshTriangle*, NavMeshEdge*> data;

	//vertices in the emsh
	std::vector<NavMeshVertex*> triangleVerts;

	//edges in the mesh
	std::vector<NavMeshTriangleEdge*> triangleEdges;

	/*
	* NavMesh()
	* default constructor 
	*/
	NavMesh() {}


	/*
	* ~NavMesh()
	* destructor, deletes all vertices, edges and triangle data
	*/
	~NavMesh();


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
	* @param std::vector<Vertex<NavMeshTriangle*, NavMeshEdge*>*> open - list of nodes that were open when the A* search terminated
	* @param std::vector<Vertex<NavMeshTriangle*, NavMeshEdge*>*> closed - list of nodes that were closed when the A* search terminated
	* @returns void
	*/
	void resetSearchedNodes(std::vector<Vertex<NavMeshTriangle*, NavMeshEdge*>*> open, std::vector<Vertex<NavMeshTriangle*, NavMeshEdge*>*> closed);


	/*
	* findRawPath
	*
	* runs the A* pathfinding algorithm on the graph
	* to find the shortest path between the start and end nodes
	*
	* the path returned is raw data off the graph, optimisations
	* could be made to the path to reduce the amount of nodes in it
	* or make it smoother
	*
	* @param Vertex<NavMeshTriangle*, NavMeshEdge*>* start - the starting node
	* @param Vertex<NavMeshTriangle*, NavMeshEdge*>* end - the target node
	* @param float E = 1.0f - heurstic multiplier, has varying effects on performance and the correctness of paths calculated
	* @returns std::vector<NavMeshTriangle*> - a list of triangle nodes that make up the path
	*/
	std::vector<NavMeshTriangle*> findRawPath(Vertex<NavMeshTriangle*, NavMeshEdge*>* start, Vertex<NavMeshTriangle*, NavMeshEdge*>* end, float E = 1.0f);

	
	/*
	* funnelPath
	*
	* prunes a raw list of points returned from an A* search
	* using the funnelling algorithm
	*
	* @param std::vector<NavMeshTriangle*> rawPath - the list of points in the original path
	* @returns std::vector<Vector2> - a list of points with the unneccessary points pruned
	*/
	std::vector<Vector2> funnelPath(std::vector<NavMeshTriangle*> rawPath, Vector2 start, Vector2 end);


	/*
	* findPath
	*
	* uses A* and the funneling algorithm to find
	* the shortest path between two points in the NavMesh
	* returns an empty list if the points do not belong
	* to the NavMesh
	*
	* @param Vector2 start - the desired beginning of the path
	* @param Vector2 end - the desired target of the path
	* @returns std::vector<Vector2> - a list of vectors that make up the path when followed in order
	*/
	std::vector<Vector2> findPath(Vector2 start, Vector2 end);


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
	*/
	void drawMesh(float nodeRadius, float connectionThickness, float r = 1.0f, float g = 1.0f, float b = 1.0f, float depth = 0.0f);

};