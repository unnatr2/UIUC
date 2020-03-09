/**
 * @file NimLearner.cpp
 * CS 225: Data Structures
 */

#include "NimLearner.h"
#include <ctime>

/**
 * Constructor to create a game of Nim with `startingTokens` starting tokens.
 *
 * This function creates a graph, `g_` representing all of the states of a
 * game of Nim with vertex labels "p#-X", where:
 * - # is the current player's turn; p1 for Player 1, p2 for Player2
 * - X is the tokens remaining at the start of a player's turn
 *
 * For example:
 *   "p1-4" is Player 1's turn with four (4) tokens remaining
 *   "p2-8" is Player 2's turn with eight (8) tokens remaining
 *
 * All legal moves between states are created as edges with initial weights
 * of 0.
 *
 * @param startingTokens The number of starting tokens in the game of Nim.
 */
NimLearner::NimLearner(unsigned startingTokens) : g_(true, true)
{
	if (startingTokens >= 1)
	{
		startingVertex_ = "p1-" + to_string(startingTokens);
		for (int i = startingTokens; i >= 0; i--)
		{

			g_.insertVertex("p1-" + to_string(i));
			g_.insertVertex("p2-" + to_string(i));
		}
		for (int i = startingTokens; i > 0; i--)
		{
			Vertex current = "p1-" + to_string(i);
			g_.insertEdge(current, "p2-" + to_string(i - 1));
			g_.setEdgeWeight(current, "p2-" + to_string(i - 1), 0);
			current = "p2-" + to_string(i);
			g_.insertEdge(current, "p1-" + to_string(i - 1));
			g_.setEdgeWeight(current, "p1-" + to_string(i - 1), 0);
		}
		for (int i = startingTokens; i > 1; i--)
		{
			Vertex current = "p1-" + to_string(i);
			g_.insertEdge(current, "p2-" + to_string(i - 2));
			g_.setEdgeWeight(current, "p2-" + to_string(i - 2), 0);
			current = "p2-" + to_string(i);
			g_.insertEdge(current, "p1-" + to_string(i - 2));
			g_.setEdgeWeight(current, "p1-" + to_string(i - 2), 0);
		}
	}
}

/**
 * Plays a random game of Nim, returning the path through the state graph
 * as a vector of `Edge` classes.  The `origin` of the first `Edge` must be
 * the vertex with the label "p1-#", where # is the number of starting
 * tokens.  (For example, in a 10 token game, result[0].origin must be the
 * vertex "p1-10".)
 *
 * @returns A random path through the state space graph.
 */
std::vector<Edge> NimLearner::playRandomGame() const
{
	vector<Edge> path;
	int index = 0;
	Vertex current = startingVertex_;
	Vertex next = startingVertex_;
	while (current != "p1-0" && current != "p2-0")
	{
		int random = std::rand();
		int random_size = g_.getAdjacent(current).size();
		int index = random % random_size;
		next = g_.getAdjacent(current)[index];
		path.push_back(g_.getEdge(current, next));
		current = next;
	}
	return path;
}

/*
 * Updates the edge weights on the graph based on a path through the state
 * tree.
 *
 * If the `path` has Player 1 winning (eg: the last vertex in the path goes
 * to Player 2 with no tokens remaining, or "p2-0", meaning that Player 1
 * took the last token), then all choices made by Player 1 (edges where
 * Player 1 is the source vertex) are rewarded by increasing the edge weight
 * by 1 and all choices made by Player 2 are punished by changing the edge
 * weight by -1.
 *
 * Likewise, if the `path` has Player 2 winning, Player 2 choices are
 * rewarded and Player 1 choices are punished.
 *
 * @param path A path through the a game of Nim to learn.
 */
void NimLearner::updateEdgeWeights(const std::vector<Edge> &path)
{
	if (path[path.size() - 1].dest[1] == '1')
	{
		for (size_t i = 0; i < path.size(); i++)
		{
			if (path[i].source[1] == '1')
			{
				g_.setEdgeWeight(path[i].source, path[i].dest, g_.getEdgeWeight(path[i].source, path[i].dest) - 1);
			}
			else if (path[i].source[1] == '2')
			{
				g_.setEdgeWeight(path[i].source, path[i].dest, g_.getEdgeWeight(path[i].source, path[i].dest) + 1);
			}
		}
	}
	else if (path[path.size() - 1].dest[1] == '2')
	{
		for (size_t i = 0; i < path.size(); i++)
		{
			if (path[i].source[1] == '1')
			{
				g_.setEdgeWeight(path[i].source, path[i].dest, g_.getEdgeWeight(path[i].source, path[i].dest) + 1);
			}
			else if (path[i].source[1] == '2')
			{
				g_.setEdgeWeight(path[i].source, path[i].dest, g_.getEdgeWeight(path[i].source, path[i].dest) - 1);
			}
		}
	}
}

/**
 * Label the edges as "WIN" or "LOSE" based on a threshold.
 */
void NimLearner::labelEdgesFromThreshold(int threshold)
{
	for (const Vertex &v : g_.getVertices())
	{
		for (const Vertex &w : g_.getAdjacent(v))
		{
			int weight = g_.getEdgeWeight(v, w);

			// Label all edges with positve weights as "WINPATH"
			if (weight > threshold)
			{
				g_.setEdgeLabel(v, w, "WIN");
			}
			else if (weight < -1 * threshold)
			{
				g_.setEdgeLabel(v, w, "LOSE");
			}
		}
	}
}

/**
 * Returns a constant reference to the state space graph.
 *
 * @returns A constant reference to the state space graph.
 */
const Graph &NimLearner::getGraph() const
{
	return g_;
}
