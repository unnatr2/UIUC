#include "Graph.h"
#include "Edge.h"
#include "Vertex.h"

#include <string>
#include <iostream>

/**
* @return The number of vertices in the Graph
*/
template <class V, class E>
unsigned int Graph<V, E>::size() const
{
	// TODO: Part 2
	return vertexMap.size();
	// return 0;
}

/**
* @return Returns the degree of a given vertex.
* @param v Given vertex to return degree.
*/
template <class V, class E>
unsigned int Graph<V, E>::degree(const V &v) const
{
	// TODO: Part 2
	return incidentEdges(v).size();
	// return 0;
}

/**
* Inserts a Vertex into the Graph by adding it to the Vertex map and adjacency list
* @param key The key of the Vertex to insert
* @return The inserted Vertex
*/
template <class V, class E>
V &Graph<V, E>::insertVertex(std::string key)
{
	// TODO: Part 2
	V &v = *(new V(key));
	vertexMap.insert(std::pair<std::string, V &>(key, v));
	adjList.insert(std::pair<std::string, std::list<edgeListIter>>(key, std::list<edgeListIter>()));
	return v;
}

/**
* Removes a given Vertex
* @param v The Vertex to remove
*/
template <class V, class E>
void Graph<V, E>::removeVertex(const std::string &key)
{
	// TODO: Part 2
	if (adjList.find(key) != adjList.end())
	{
		for (E &edge : incidentEdges(key))
		{
			removeEdge(edge.source(), edge.dest());
		}
		adjList.erase(key);
		vertexMap.erase(key);
	}
	return;
}

/**
* Inserts an Edge into the adjacency list
* @param v1 The source Vertex
* @param v2 The destination Vertex
* @return The inserted Edge
*/
template <class V, class E>
E &Graph<V, E>::insertEdge(const V &v1, const V &v2)
{
	// TODO: Part 2
	E &e = *(new E(v1, v2));
	edgeList.push_front(e);
	adjList[v1.key()].push_back(edgeList.begin());
	adjList[v2.key()].push_back(edgeList.begin());
	return e;
}

/**
* Removes an Edge from the Graph
* @param key1 The key of the source Vertex
* @param key2 The key of the destination Vertex
*/
template <class V, class E>
void Graph<V, E>::removeEdge(const std::string key1, const std::string key2)
{
	// TODO: Part 2
	if (adjList.find(key1) != adjList.end())
	{
		for (typename std::list<edgeListIter>::iterator AdjEdgeItr = adjList[key1].begin(); AdjEdgeItr != adjList[key1].end(); AdjEdgeItr++)
		{
			E &edge = **AdjEdgeItr;
			if (edge.dest().key() == key2 || edge.source().key() == key2)
			{
				for (typename std::list<edgeListIter>::iterator AdjEdgeItr2 = adjList[key2].begin(); AdjEdgeItr2 != adjList[key2].end(); AdjEdgeItr2++)
				{
					E &edge2 = **AdjEdgeItr2;
					if (edge2.dest().key() == key1 || edge2.source().key() == key1)
					{
						adjList[key2].erase(AdjEdgeItr2);
						break;
					}
				}
				adjList[key1].erase(AdjEdgeItr);
				edgeList.erase(*AdjEdgeItr);
				break;
			}
		}
	}
	return;
}

/**
* Removes an Edge from the adjacency list at the location of the given iterator
* @param it An iterator at the location of the Edge that
* you would like to remove
*/
template <class V, class E>
void Graph<V, E>::removeEdge(const edgeListIter &it)
{
	// TODO: Part 2
	removeEdge(it->source().key(), it->dest().key());
	return;
}

/**
* @param key The key of an arbitrary Vertex "v"
* @return The list edges (by reference) that are adjacent to "v"
*/
template <class V, class E>
const std::list<std::reference_wrapper<E>> Graph<V, E>::incidentEdges(const std::string key) const
{
	// TODO: Part 2
	std::list<std::reference_wrapper<E>> edges;
	for (const std::pair<std::string, std::list<edgeListIter>> &pair : adjList)
	{
		if (pair.first == key)
		{
			const std::list<edgeListIter> &edgeList = pair.second;
			for (const edgeListIter &it : edgeList)
			{
				E_byRef e = *it;
				if (e.get().directed() == true)
				{
					if (e.get().source() == key)
					{
						edges.push_back(e);
					}
				}
				else
				{
					edges.push_back(e);
				}
			}
		}
	}
	return edges;
}

/**
* Return whether the two vertices are adjacent to one another
* @param key1 The key of the source Vertex
* @param key2 The key of the destination Vertex
* @return True if v1 is adjacent to v2, False otherwise
*/
template <class V, class E>
bool Graph<V, E>::isAdjacent(const std::string key1, const std::string key2) const
{
	// TODO: Part 2
	std::list<std::reference_wrapper<E>> edges = this->incidentEdges(key1);
	for (std::reference_wrapper<E> edgeWrap : edges)
	{
		E &edge = edgeWrap.get();
		if (edge.source().key() == key2 || edge.dest().key() == key2)
		{
			return true;
		}
	}
	return false;
}
