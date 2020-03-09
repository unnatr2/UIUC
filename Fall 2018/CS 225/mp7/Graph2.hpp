#include <queue>
#include <algorithm>
#include <string>
#include <list>

template <class V = Vertex>
class Comp
{
  public:
	bool operator()(V &key1, V &key2)
	{
		if (std::stod(key1["dist"]) < std::stod(key2["dist"]))
		{
			return true;
		}
		return false;
	}
};

/**
 * Returns an std::list of vertex keys that creates some shortest path between `start` and `end`.
 *
 * This list MUST include the key of the `start` vertex as the first vertex in the list, the key of
 * the `end` vertex as the last element in the list, and an ordered list of all vertices that must
 * be traveled along the shortest path.
 * 
 * For example, the path a -> c -> e returns a list with three elements: "a", "c", "e".
 * 
 * @param start The key for the starting vertex.
 * @param end   The key for the ending vertex.
 */
template <class V, class E>
std::list<std::string> Graph<V, E>::shortestPath(const std::string start, const std::string end)
{
	std::list<std::string> path;
	for (std::pair<std::string, V &> vertexPair : vertexMap)
	{
		V &v = vertexPair.second;
		v["dist"] = std::to_string(__INT_MAX__);
		v["pred"] = "";
	}
	V &startVertex = vertexMap.find(start)->second;
	startVertex["dist"] = std::to_string(0);
	std::priority_queue<V_byRef, std::vector<V_byRef>, Comp<V>> Q;
	Q.push(startVertex);

	while (!Q.empty())
	{
		V &currentVertex = Q.top();
		Q.pop();
		for (E &edge : incidentEdges(currentVertex))
		{
			const V &oppVertex = (edge.source() == currentVertex) ? edge.dest() : edge.source();
			V &oppositeV = vertexMap.find(oppVertex.key())->second;
			double weight = edge.weight();
			if (weight + std::stod(currentVertex["dist"]) < std::stod(oppositeV["dist"]))
			{
				oppositeV["dist"] = std::to_string(weight + std::stod(currentVertex["dist"]));
				oppositeV["pred"] = currentVertex.key();
				Q.push(oppositeV);
			}
		}
	}

	V &endVertex = vertexMap.find(end)->second;
	while (endVertex["pred"] != "")
	{
		path.push_front(endVertex.key());
		endVertex = vertexMap.find(endVertex["pred"])->second;
	}
	path.push_front(start);

	return path;
}
