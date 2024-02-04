#include "graph.h"
#include <iostream>

int main()
{
    Graph::UnDirected_Graph directed_graph;
    directed_graph.add_nodes({ 1,2,3,4,5,6,7 });
    directed_graph.add_edges({
        {1,2},{1,6},{1,7},
        {2,1},{2,3},{2,6},
        {3,2},{3,6},{3,5},{3,4},
        {4,3},{4,5},
        {5,3},{5,4},{5,6},
        {6,1},{6,2},{6,3},{6,5},{6,7},
        {7,1},{7,5},{7,6}
        });
    //测试node迭代器
    for (auto& node : directed_graph.nodes())
    {
        std::cout << node.id << std::endl;
    }
    //测试edge迭代器
    for (auto& edge : directed_graph.edges())
    {
        std::cout << edge.from << "," << edge.to << std::endl;
    }
    //测试获取所有节点
    auto nodes = directed_graph.getAllNodes();
    //测试获取相邻节点
    auto node = directed_graph.getNearNode(3);
    //测试获取所有edge
    auto edges = directed_graph.getAllEdges();
    //测试获取邻边
    auto edge = directed_graph.getNearEdges(3);
    //测试删除节点
    directed_graph.remove_node(3);
    //测试删除边
    directed_graph.remove_edges({ { 1,2 }, { 6,1 } });
}