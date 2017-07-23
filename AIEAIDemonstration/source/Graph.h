#pragma once

#include <vector>

#define TEMPLATE template<typename T>
#define TEMPLATE2 template<typename T, typename U>

//forward declaration for the edge
TEMPLATE2
class Vertex;

#pragma region Edge
/*
* class Edge
* template with two types
*
* represents a connection going from one vertex to another
*
* author: Bradley Booth, Academy of Interactive Entertainment, 2017
*/
TEMPLATE2
class Edge
{

public:

	//the ends of the edge
	Vertex<T,U>* start = nullptr;
	Vertex<T,U>* end = nullptr;

	//value associated with the edge, used for costs and heurstics
	U value;


	/*
	* Edge()
	* default constructor
	*/
	Edge() {}


	/*
	* ~Edge()
	* default destructor
	*/
	~Edge() {}

};
#pragma endregion



#pragma region Vertex
/*
* class Vertex
* template
*
* a data point in the graph
*
* author: Bradley Booth, Academy of Interactive Entertainment, 2017
*/
TEMPLATE2
class Vertex
{

public:

	T data; //value stored at the vertex
	std::vector<Edge<T,U>> edges; //connections that start at this vertex

	//temp values
	int serial = 0; //used when copying the graph
	bool visited = false; //flag indicating if the vertex has been visited in the current transversal


	/*
	* Vertex()
	* default constructor
	*/
	Vertex() {}


	/*
	* ~Vertex()
	* default destructor
	*/
	~Vertex() {}

};
#pragma endregion



#pragma region Graph
/*
* class Graph
* template with two types
*
* tracks a list of vertices that are connected with edges
* vertices hold values, edges link two vertices together
* but can also hold values if needed
*
* author: Bradley Booth, Academy of Interactive Entertainment, 2017
*/
TEMPLATE2
class Graph
{

public:
	
	std::vector<Vertex<T, U>*> vertices;

	/*
	* Graph()
	* default constructor
	*/
	Graph() {}


	/*
	* ~Graph()
	* destructor, removes all nodes
	*/
	~Graph()
	{
		//iterate through all vertices, delete each vertex
		for (size_t i = 0; i < vertices.size(); i++)
		{
			delete vertices[i];
		}
	}


	/*
	* Graph(const Graph& other)
	* copy constructor
	*
	* @param const Graph& other - the graph to copy from
	*/
	Graph(const Graph& other)
	{
		//iterate through all vertices, serialise each vertex with the order it came in the vertices list
		for (size_t i = 0; i < other.vertices.size(); i++)
		{
			//speed improvement, no array access needed
			Vertex<T, U>* original = other.vertices[i];
			original->serial = (int)i;

			//create a duplicate vertex with the same data and serial, but ignore connections (for now)
			Vertex<T, U>* copy = new Vertex<T, U>();
			copy->data = original->data;
			copy->serial = original->serial;

			//add the duplicate vertex to the new graph
			vertices.push_back(copy);
		}

		//iterate through all vertices, copying all connections to the new graph
		for (size_t i = 0; i < other.vertices.size(); i++)
		{
			//speed improvement, no array access needed
			Vertex<T, U>* original = other.vertices[i];

			for (size_t j = 0; j < original->edges.size(); j++)
			{
				Edge<T, U> originalEdge = original->edges[j];

				Vertex<T, U>* startVert = vertices[originalEdge.start->serial];
				Vertex<T, U>* endVert = vertices[originalEdge.end->serial];

				//create a duplicate connection between the vertices
				createConnection(startVert, endVert);

				//copy the value held at the original edge to the new edge
				vertices[startVert->serial]->edges[j].value = originalEdge.value;
			}
		}


	}


	/*
	* operator=(const Graph& other)
	* assignmnet operator
	*
	* @param const Graph& other - the graph to copy from
	* @returns Graph& - reference to the graph that is created from the duplicate
	*/
	Graph& operator=(const Graph& other)
	{
		//iterate through all vertices, serialise each vertex with the order it came in the vertices list
		for (size_t i = 0; i < other.vertices.size(); i++)
		{
			//speed improvement, no array access needed
			Vertex<T, U>* original = other.vertices[i];
			original->serial = (int)i;

			//create a duplicate vertex with the same data and serial, but ignore connections (for now)
			Vertex<T, U>* copy = new Vertex<T, U>();
			copy->data = original->data;
			copy->serial = original->serial;

			//add the duplicate vertex to the new graph
			vertices.push_back(copy);
		}

		//iterate through all vertices, copying all connections to the new graph
		for (size_t i = 0; i < other.vertices.size(); i++)
		{
			//speed improvement, no array access needed
			Vertex<T, U>* original = other.vertices[i];

			for (size_t j = 0; j < original->edges.size(); j++)
			{
				Edge<T, U> originalEdge = original->edges[j];

				Vertex<T, U>* startVert = vertices[originalEdge.start->serial];
				Vertex<T, U>* endVert = vertices[originalEdge.end->serial];

				//create a duplicate connection between the vertices
				createConnection(startVert, endVert);

				//copy the value held at the original edge to the new edge
				vertices[startVert->serial]->edges[j].value = originalEdge.value;
			}
		}

		return *this;
	}


	/*
	* addVertex
	*
	* creates a new vertex and places it into the vertices list
	*
	* @param T value - the data to give the new vertex
	* @returns Vertex<T, U>* - pointer to the vertex that was just added
	*/
	Vertex<T, U>* addVertex(T value)
	{
		Vertex<T,U>* vert = new Vertex<T,U>();
		vert->data = value;

		vertices.push_back(vert);

		return vert;
	}
	

	/*
	* removeVertex
	*
	* iterates through all vertices, searching for the vertice specified
	* the vertex to delete (target) is deleted after iterating all vertices, while
	* connections to the target are deleted from every other vertex
	*
	* @param Vertex<T>* target - the vertex to delete
	* @returns void
	*/
	void removeVertex(Vertex<T,U>* target)
	{
		Vertex<T,U>* search = nullptr;

		//iterate through all of the vertices, searching for the node to erase
		for (size_t i = 0; i < vertices.size(); i++)
		{
			//has the vertex been found
			if (search == nullptr)
			{
				//remember the vertex
				if (search == vertices[i])
				{
					search = vertices[i];
				}
			}
			else
			{
				Vertex<T,U>* current = vertices[i];

				std::vector<Edge<T, U>>::iterator edgeIter = current->edges.begin();

				//iterate through all edges in the vertices, looking 
				for (; edgeIter != current->edges.end(); edgeIter++)
				{
					//if the connection ends at the vertex that is being deleted, delete it
					if (target == edgeIter.end)
					{
						current->edges.remove(edgeIter);
					}
				}
			}
		}
	}


	/*
	* createConnection
	*
	* connects two vertices together by creating a edge from vert1 to vert2
	*
	* @param Vertex<T, U>* vert1 - the first vertex to connect
	* @param Vertex<T, U>* vert2 - the second vertex to connect
	* @returns void
	*/
	void createConnection(Vertex<T, U>* vert1, Vertex<T, U>* vert2)
	{
		//make the first edge
		Edge<T, U> edge1 = Edge<T, U>();
		edge1.start = vert1;
		edge1.end = vert2;

		//add the edges to each vertices
		vert1->edges.push_back(edge1);
	}


	/*
	* connectVertices
	*
	* connects two vertices together by creating two edges between them
	*
	* @param Vertex<T, U>* vert1 - the first vertex to connect
	* @param Vertex<T, U>* vert2 - the second vertex to connect
	* @returns void
	*/
	void connectVertices(Vertex<T, U>* vert1, Vertex<T, U>* vert2) 
	{
		//make the first edge
		Edge<T, U> edge1 = Edge<T, U>();
		edge1.start = vert1;
		edge1.end = vert2;

		//make the second edge
		Edge<T, U> edge2 = Edge<T, U>();
		edge2.start = vert2;
		edge2.end = vert1;

		//add the edges to each vertices
		vert1->edges.push_back(edge1);
		vert2->edges.push_back(edge2);
	}

};
#pragma endregion