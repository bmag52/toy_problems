#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <cmath>
#include <limits>
#include <cstdlib>
#include <ctime>

class Node
{
public:
    typedef std::vector<Node*> NeighborVector;
    Node(const int id, const int x, const int y)
      : id_(id)
      , x_(x)
      , y_(y)
      , enabled_(true)
    {
    };

    void set_neighbors(const NeighborVector& neighbors)
    {
        neighbors_ = neighbors;
    }

    float distance(const Node* other) const
    {
        return distance(other->x_, other->y_);
    };

    float distance(const int x, const int y) const
    {
        const int dx = x_ - x;
        const int dy = y_ - y;
        return std::sqrt(dx * dx + dy * dy);
    };

    NeighborVector neighbors_;
    int id_;
    int x_;
    int y_;
    bool enabled_;
};

// Graph represented as a map of nodes
typedef std::map<int, Node*> NodeMap;
// Path represented as an ordered list of nodes
typedef std::vector<Node*> NodeVector;

class Graph
{
public:
    Graph(const size_t max_nodes)
      : max_nodes_(max_nodes)
    {
        side_dimension_ = std::sqrt(max_nodes_);
        int x = 0;
        int y = 0;
        int id_counter = 0;

        // form the intermediate node data
        for (size_t i = 0U; i < max_nodes_; ++i)
        {
            // store intermediate data
            nodes_[id_counter] = new Node(id_counter, x, y);

            // update coords
            if (i % side_dimension_ == side_dimension_ - 1)
            {
                x++;
                y = 0;
            }
            else
            {
                y++;
            }

            // update id counter
            id_counter++;
        }

        // interconnect the nodes
        for (auto it_a = nodes_.begin(); it_a != nodes_.end(); it_a++)
        {
            Node* node_a = it_a->second;

            // get neighbors of node a
            Node::NeighborVector neighbors;
            for (auto it_b = nodes_.begin(); it_b != nodes_.end(); it_b++)
            {
                // make sure that node a is not node b
                Node* node_b = it_b->second;
                if (node_a->id_ == node_b->id_)
                {
                    continue;
                }

                // node neighbors have distance of 1
                if (node_a->distance(node_b) == 1)
                {
                    neighbors.push_back(node_b);
                }
            }
            node_a->set_neighbors(neighbors);
        }
    };

    void disable_nodes(const int x, const int y, const int rad)
    {
        for (auto it = nodes_.begin(); it != nodes_.end(); it++)
        {
            Node* node = it->second;
            node->enabled_ &= it->second->distance(x, y) > rad;
        }
    }

    Node* get_random_enabled_node()
    {
        while (true)
        {
            const int id = std::rand() % max_nodes_;
            auto it = nodes_.find(id);
            if (it->second->enabled_)
            {
                return it->second;
            }
        }
    }

    void print(NodeVector path) const
    {
        for (int i = 0; i < max_nodes_; i++)
        {
            auto it_node = nodes_.find(i);
            const int node_id = it_node->second->id_;

            // print id
            if (node_id < 10)
            {
                std::cout << "00";
            }
            else if (node_id < 100)
            {
                std::cout << "0";
            }
            std::cout << node_id << " ";

            // print whether or not node is in path and if the node is enabled
            auto it_path = std::find_if(path.begin(), path.end(), [node_id](const Node* node) { return node->id_ == node_id; });
            if (!it_node->second->enabled_)
            {
                std::cout << "#";
            }
            else if (it_path != path.end())
            {
                std::cout << ".";
            }
            else
            {
                std::cout << " ";
            }

            // print the delimiter
            std::cout << " ";

            // set new row
            if (i % side_dimension_ == side_dimension_ - 1)
            {
                std::cout << std::endl;
            }
        }
    };

    NodeMap nodes_;
    int side_dimension_;
    size_t max_nodes_;
};

class GraphSolver
{
public:
    GraphSolver() {};

    float heuristic_cost_estimate(Node* current, Node* goal)
    {
        return 0;
    }

    NodeVector reconstruct_path(Node* current)
    {
        NodeVector total_path;
        total_path.push_back(current);

        auto it = came_from_.find(current->id_);
        while (it != came_from_.end())
        {
            current = it->second;
            total_path.push_back(current);
            it = came_from_.find(current->id_);
        }
        return total_path;
    };

    NodeVector a_star(Node* start, Node* goal)
    {
        typedef std::map<int, float> ScoreMap;
        NodeMap open_set, closed_set;
        open_set[start->id_] = start;

        ScoreMap g_score, f_score;
        g_score[start->id_] = 0;
        f_score[start->id_] = heuristic_cost_estimate(start, goal);

        came_from_.clear();
        while (!open_set.empty())
        {
            float min_f_score = std::numeric_limits<float>::max();
            int min_f_score_node_id = -1;
            for (auto it_open_set = open_set.begin(); it_open_set != open_set.end(); it_open_set++)
            {
                auto it_f_score = f_score.find(it_open_set->first);
                if (it_f_score == f_score.end())
                {
                    continue;
                }

                if (it_f_score->second < min_f_score)
                {
                    min_f_score_node_id = it_f_score->first;
                    min_f_score = it_f_score->second;
                }
            }
            auto it_open_set = open_set.find(min_f_score_node_id);
            Node* current = it_open_set->second;
            
            if (current->id_ == goal->id_)
            {
                return reconstruct_path(current);
            }

            open_set.erase(current->id_);
            closed_set[current->id_] = current;

            for (Node* neighbor : current->neighbors_)
            {
                // Ignore the neighbor which is already evaluated.
                if (closed_set.find(neighbor->id_) != closed_set.end())
                {
                    continue;       
                }

                // Ignore disabled neighbors
                if (!neighbor->enabled_)
                {
                    continue;
                }

                // The distance from start to a neighbor
                float tentative_g_score = std::numeric_limits<float>::max();
                auto it_g_score = g_score.find(current->id_);
                if (it_g_score != g_score.end())
                {
                    tentative_g_score = it_g_score->second + current->distance(neighbor);
                }

                if (open_set.find(neighbor->id_) == open_set.end())
                {
                    // Discover a new node
                    open_set[neighbor->id_] = neighbor;
                }
                else if (tentative_g_score >= g_score[neighbor->id_])
                {
                    continue;
                }

                // This path is the best until now. Record it!
                came_from_[neighbor->id_] = current;
                g_score[neighbor->id_] = tentative_g_score;
                f_score[neighbor->id_] = tentative_g_score + heuristic_cost_estimate(neighbor, goal);
            }
        }

        return NodeVector();
    };

    NodeVector dijkstra(Graph graph, Node* start, Node* goal)
    {
        typedef std::vector<float> DistanceVector;

        DistanceVector distance;
        NodeMap remaining_nodes;
        for (auto it = graph.nodes_.begin(); it != graph.nodes_.end(); it++)
        {
            distance.push_back(std::numeric_limits<float>::max());
            remaining_nodes[it->first] = it->second;
        }
        distance[start->id_] = 0;

        came_from_.clear();
        while (!remaining_nodes.empty())
        {
            // get the min distance from of non-evaluated nodes and mark as evaluated
            float min_distance = std::numeric_limits<float>::max();
            int min_distance_node_id = -1;
            for (auto it = remaining_nodes.begin(); it != remaining_nodes.end(); it++)
            {
                Node* node = it->second;
                if (distance[node->id_] < min_distance)
                {
                    min_distance_node_id = node->id_;
                    min_distance = distance[node->id_];
                }
            }

            if (min_distance_node_id == -1 || min_distance_node_id == goal->id_)
            {
                break;
            }

            // update the remaining nodes
            auto it = remaining_nodes.find(min_distance_node_id);
            Node* current = it->second;
            remaining_nodes.erase(it);

            // update the distances
            for (Node* neighbor : current->neighbors_)  
            {
                if (!neighbor->enabled_)
                {
                    continue;
                }

                float neighbor_distance = distance[current->id_] + current->distance(neighbor);
                if (neighbor_distance < distance[neighbor->id_])
                {
                    distance[neighbor->id_] = neighbor_distance;
                    came_from_[neighbor->id_] = current;
                }
            }
        }

        return reconstruct_path(goal);
    };

    NodeMap came_from_;
};

int main()
{
    Graph graph(625);
    graph.disable_nodes(graph.side_dimension_ * 1 / 3, graph.side_dimension_ * 1 / 3, 4);
    graph.disable_nodes(graph.side_dimension_ * 2 / 3, graph.side_dimension_ * 2 / 3, 4);
    std::srand(std::time(nullptr));
    Node* start_node = graph.get_random_enabled_node();
    Node* goal_node = graph.get_random_enabled_node();
    std::cout << "start node id: " << start_node->id_ << std::endl;
    std::cout << "goal node id: " << goal_node->id_ << std::endl;

    std::clock_t start;
    auto print_duration = [&](const std::clock_t& start) { 
        std::cout << "run time: "<< (std::clock() - start) / (double) CLOCKS_PER_SEC << std::endl;
    };

    GraphSolver graph_solver;
    std::cout << "====== A* ======" << std::endl;
    start = std::clock();
    graph.print(graph_solver.a_star(start_node, goal_node));
    print_duration(start);

    std::cout << "=== Dijkstra ===" << std::endl;
    start = std::clock();
    graph.print(graph_solver.dijkstra(graph, start_node, goal_node));
    print_duration(start);
}
