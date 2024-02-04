#pragma once
#include <map>
#include <set>
#include <memory>
namespace Graph
{
    struct Graph_Node {
        Graph_Node() = default;
        Graph_Node(int id, float widget = 1.0f) : id(id), widget(widget) {}
        int id;
        float widget;
    };
    struct Graph_Edge {
        Graph_Edge() = default;
        Graph_Edge(int from, int to, float weight) : from(from), to(to), weight(weight) {}
        int from;
        int to;
        float weight;
    };

    class Graph_Base {
    protected:
        std::map<int, Graph_Node>m_nodes;
        std::map<int, std::map<int, Graph_Edge>>m_edges;
    public:
        /**
         * @brief 在图中插入节点
         * @param id 节点的id
         * @return 成功返回true，如果节点已存在返回false
        */
        virtual bool add_node(int id) = 0;
        /**
         * @brief 插入节点的列表版本
         * @param ids
         * @return
        */
        virtual bool add_nodes(std::initializer_list<int> ids);

        /**
         * @brief 移除图中的节点，同时断开关联的边
         * @param id 节点的id
         * @return 成功返回true，如果节点不存在则返回false
        */
        virtual bool remove_node(int id) = 0;
        /**
         * @brief 移除节点的列表版本
         * @param ids
         * @return
        */
        virtual bool remove_nodes(std::initializer_list<int> ids);
        /**
         * @brief 在节点之间插入边
         * @param from 起点
         * @param to 终点
         * @param weight 边的权重
         * @return 成功返回true，from，to任意节点不存在，或者边已存在返回false
        */
        virtual bool add_edge(int from, int to, float weight = 1.0) = 0;
        /**
         * @brief 插入边的列表版本
         * @param edges
         * @return
        */
        virtual bool add_edges(std::initializer_list<std::pair<int, int>> edges);
        /**
         * @brief 插入边的带权列表版本
         * @param edges
         * @return
        */
        virtual bool add_edges(std::initializer_list<std::tuple<int, int, float>> edges);
        /**
         * @brief 删除节点之间的边
         * @param from 起点
         * @param to 终点
         * @return 成功返回true，from，to任意节点不存在，或者边不存在返回false
        */
        virtual bool remove_edge(int from, int to) = 0;
        /**
         * @brief 删除节点之间的边的列表版本
         * @param edges
         * @return
        */
        virtual bool remove_edges(std::initializer_list<std::pair<int, int>> edges);
        //获取图中的节点数
        virtual int sizeNode();
        //获取图中的边数
        virtual int sizeEdge();
        //获取图中所有的节点id
        virtual std::set<int> getAllNodes();
        //获取图中相邻的节点id
        virtual std::set<int> getNearNode(int id);
        //获取所有连接边
        virtual std::set<std::pair<int, int>> getAllEdges();
        //获取相邻边
        virtual std::set<std::pair<int, int>> getNearEdges(int id);
        //获取子图
        virtual std::shared_ptr<Graph_Base> getSubGraph(std::set<int> ids) = 0;
        class NodeView;
        class NearNodeView;
        class EdgeView;
        class NearEdgeView;

        /**
         * @brief 获取图中所有节点的迭代器视图
         * @return
        */
        virtual NodeView nodes()
        {
            return NodeView(*this);
        }

        /**
         * @brief 获取图中指定节点相邻节点的迭代器视图
         * @param id 节点id
         * @return
        */
        virtual NearNodeView nearNodes(int id)
        {
            return NearNodeView(*this, id);
        }

        /**
         * @brief 获取图中所有边的迭代器视图
         * @return
        */
        virtual EdgeView edges()
        {
            return EdgeView(*this);
        }

        /**
         * @brief 获取图中指定起点连接边的迭代器视图
         * @param id 节点id
         * @return
        */
        virtual NearEdgeView nearEdges(int id)
        {
            return NearEdgeView(*this, id);
        }

        //全节点迭代器
        struct NodeIterator {
        protected:
            Graph_Base& graph;
            std::map<int, Graph_Node>::iterator m_iter;
            NodeIterator(Graph_Base& graph) : graph(graph) {}
        public:
            static NodeIterator beginIterator(Graph_Base& graph);
            static NodeIterator endIterator(Graph_Base& graph);
            virtual Graph_Node& operator*();
            virtual NodeIterator& operator++();
            virtual bool operator!=(const NodeIterator& other);
        };
        //全边迭代器
        struct EdgeIterator {
        protected:
            Graph_Base& graph;
            std::map<int, std::map<int, Graph_Edge>>::iterator m_iterEdges;
            std::map<int, Graph_Edge>::iterator m_iterNearEdges;
            EdgeIterator(Graph_Base& graph) :graph(graph) {}
        public:
            static EdgeIterator beginIterator(Graph_Base& graph);
            static EdgeIterator endIterator(Graph_Base& graph);
            virtual Graph_Edge& operator*();
            virtual EdgeIterator& operator++();
            virtual bool operator!=(const EdgeIterator& other);
        };
        //相邻节点迭代器
        struct NearNodeIterator {
        protected:
            Graph_Base& graph;
            std::map<int, Graph_Edge>::iterator m_iterNearEdges;
            NearNodeIterator(Graph_Base& graph, int id);
        public:
            static NearNodeIterator beginIterator(Graph_Base& graph, int id);
            static  NearNodeIterator endIterator(Graph_Base& graph, int id);
            virtual Graph_Node& operator*();
            virtual NearNodeIterator& operator++();
            virtual bool operator!=(const NearNodeIterator& other);
        };
        //相邻边迭代器
        struct NearEdgeIterator {
        protected:
            Graph_Base& graph;
            std::map<int, Graph_Edge>::iterator m_iterNearEdges;
            NearEdgeIterator(Graph_Base& graph, int id);
        public:
            static NearEdgeIterator beginIterator(Graph_Base& graph, int id);
            static NearEdgeIterator endIterator(Graph_Base& graph, int id);
            virtual Graph_Edge& operator*();
            virtual NearEdgeIterator& operator++();
            virtual bool operator!=(const NearEdgeIterator& other);
        };

        class NodeView {
            Graph_Base& graph;
        public:
            NodeView(Graph_Base& graph);

            Graph_Base::NodeIterator begin();
            Graph_Base::NodeIterator end();
        };

        class EdgeView {
            Graph_Base& graph;
        public:
            EdgeView(Graph_Base& graph);

            Graph_Base::EdgeIterator begin();
            Graph_Base::EdgeIterator end();
        };
        class NearNodeView {
            Graph_Base& graph;
            int id;
        public:
            NearNodeView(Graph_Base& graph, int id);
            Graph_Base::NearNodeIterator begin();
            Graph_Base::NearNodeIterator end();
        };

        class NearEdgeView {
            Graph_Base& graph;
            int id;
        public:
            NearEdgeView(Graph_Base& graph, int id);
            Graph_Base::NearEdgeIterator begin();
            Graph_Base::NearEdgeIterator end();
        };
    };

    class UnDirected_Graph : public Graph_Base {
    private:
        //反向边
        //因为无向图只能有一条边，所以只在m_edges存储正向边，反向边在m_edges_rev
        std::map<int, std::map<int, Graph_Edge>>m_edges_inv;
    public:
        bool add_node(int id) override;
        bool remove_node(int id) override;
        bool add_edge(int from, int to, float weight = 1.0) override;
        bool remove_edge(int from, int to) override;
        virtual std::shared_ptr<Graph_Base> getSubGraph(std::set<int> ids) override;
        virtual std::set<int> getNearNode(int id) override;
        virtual std::set<std::pair<int, int>> getNearEdges(int id) override;
    };

    class Directed_Graph : public Graph_Base {
    public:
        bool add_node(int id) override;
        bool remove_node(int id) override;
        bool add_edge(int from, int to, float weight = 1.0) override;
        bool remove_edge(int from, int to) override;
        virtual std::shared_ptr<Graph_Base> getSubGraph(std::set<int> ids) override;
    };
}

namespace Graph
{
    inline bool UnDirected_Graph::add_node(int id) {
        if (m_nodes.find(id) != m_nodes.end()) return false;
        m_nodes[id] = Graph_Node(id);
        m_edges.emplace(id, std::map<int, Graph_Edge>());
        m_edges_inv.emplace(id, std::map<int, Graph_Edge>());
        return true;
    }

    inline bool UnDirected_Graph::remove_node(int id) {
        if (m_nodes.find(id) == m_nodes.end()) return false;
        m_nodes.erase(id);
        m_edges.erase(id);
        m_edges_inv.erase(id);
        for (auto& edge : m_edges) {
            if (edge.second.find(id) != edge.second.end()) {
                remove_edge(edge.first, id);
            }
        }
        return true;
    }

    inline bool UnDirected_Graph::add_edge(int from, int to, float weight) {
        if (from == to) return false;//不允许自环
        if (from > to)
            std::swap(from, to);
        if (m_nodes.find(from) == m_nodes.end() || m_nodes.find(to) == m_nodes.end()) return false;
        m_edges[from][to] = Graph_Edge(from, to, weight);
        m_edges_inv[to][from] = Graph_Edge(to, from, weight);
        return true;
    }

    inline bool UnDirected_Graph::remove_edge(int from, int to) {
        if (from > to)
            std::swap(from, to);
        if (m_nodes.find(from) == m_nodes.end() || m_nodes.find(to) == m_nodes.end()) return false;
        if (m_edges[from].find(to) == m_edges[from].end() || m_edges[from].find(to) == m_edges[from].end()) return false;
        m_edges[from].erase(to);
        m_edges_inv[to].erase(from);
        return true;
    }

    inline std::shared_ptr<Graph_Base> UnDirected_Graph::getSubGraph(std::set<int> ids)
    {
        std::shared_ptr<Graph_Base> subGraph(new UnDirected_Graph(*this));
        for (auto& node : m_nodes)
        {
            if (ids.find(node.first) == ids.end())
            {
                remove_node(node.first);
            }
        }
        return subGraph;
    }

    inline std::set<int> UnDirected_Graph::getNearNode(int id)
    {
        auto nodes = Graph_Base::getNearNode(id);
        for (auto& edge : m_edges_inv[id])
        {
            nodes.emplace(edge.second.to);
        }
        return nodes;
    }

    inline std::set<std::pair<int, int>> UnDirected_Graph::getNearEdges(int id)
    {
        auto edges = Graph_Base::getNearEdges(id);
        for (auto& edge : m_edges_inv[id])
        {
            edges.emplace(edge.second.from, edge.second.to);
        }
        return edges;
    }

    inline bool Directed_Graph::add_node(int id) {
        if (m_nodes.find(id) != m_nodes.end()) return false;
        m_nodes[id] = Graph_Node(id);
        m_edges.emplace(id, std::map<int, Graph_Edge>());
        return true;
    }

    inline bool Directed_Graph::remove_node(int id) {
        if (m_nodes.find(id) == m_nodes.end()) return false;
        m_nodes.erase(id);
        m_edges.erase(id);
        for (auto& edge : m_edges) {
            if (edge.second.find(id) != edge.second.end()) {
                edge.second.erase(id);
            }
        }
        return true;
    }

    inline bool Directed_Graph::add_edge(int from, int to, float weight) {
        if (from == to) return false;//不允许自环
        if (m_nodes.find(from) == m_nodes.end() || m_nodes.find(to) == m_nodes.end()) return false;
        m_edges[from][to] = Graph_Edge(from, to, weight);
        return true;
    }

    inline bool Directed_Graph::remove_edge(int from, int to) {
        if (m_nodes.find(from) == m_nodes.end() || m_nodes.find(to) == m_nodes.end()) return false;
        if (m_edges[from].find(to) == m_edges[from].end() || m_edges[from].find(to) == m_edges[from].end()) return false;
        m_edges[from].erase(to);
        return true;
    }

    inline std::shared_ptr<Graph_Base> Directed_Graph::getSubGraph(std::set<int> ids)
    {
        std::shared_ptr<Graph_Base> subGraph(new Directed_Graph(*this));
        for (auto& node : m_nodes)
        {
            if (ids.find(node.first) == ids.end())
            {
                remove_node(node.first);
            }
        }
        return subGraph;
    }

    inline bool Graph_Base::add_nodes(std::initializer_list<int> ids)
    {
        for (auto& id : ids)
        {
            add_node(id);
        }
        return true;
    }

    inline bool Graph_Base::remove_nodes(std::initializer_list<int> ids)
    {
        for (auto& id : ids)
        {
            remove_node(id);
        }
        return true;
    }

    inline bool Graph_Base::add_edges(std::initializer_list<std::pair<int, int>> edges)
    {
        for (auto& edge : edges)
        {
            add_edge(edge.first, edge.second);
        }
        return true;
    }

    inline bool Graph_Base::add_edges(std::initializer_list<std::tuple<int, int, float>> edges)
    {
        for (auto& edge : edges)
        {
            add_edge(std::get<0>(edge), std::get<1>(edge), std::get<2>(edge));
        }
        return true;
    }

    inline bool Graph_Base::remove_edges(std::initializer_list<std::pair<int, int>> edges)
    {
        for (auto& edge : edges)
        {
            remove_edge(edge.first, edge.second);
        }
        return true;
    }

    inline int Graph_Base::sizeNode()
    {
        return static_cast<int>(m_nodes.size());
    }


    inline int Graph_Base::sizeEdge()
    {
        int i = 0;
        for (auto& near_edges : m_edges)
        {
            i += static_cast<int>(near_edges.second.size());
        }
        return i;
    }


    inline std::set<int> Graph_Base::getAllNodes()
    {
        std::set<int> nodes;
        for (auto& node : m_nodes)
        {
            nodes.insert(node.first);
        }
        return nodes;
    }


    inline std::set<int> Graph_Base::getNearNode(int id)
    {
        std::set<int> nodes;
        for (auto& edges : m_edges[id])
        {
            nodes.insert(edges.second.to);
        }
        return nodes;
    }


    inline std::set<std::pair<int, int>> Graph_Base::getAllEdges()
    {
        std::set<std::pair<int, int>> edges;
        for (auto& node : m_edges)
        {
            for (auto& edge : node.second)
            {
                edges.insert(std::make_pair(node.first, edge.second.to));
            }
        }
        return edges;
    }


    inline std::set<std::pair<int, int>> Graph_Base::getNearEdges(int id)
    {
        std::set<std::pair<int, int>> edges;
        for (auto& edge : m_edges[id])
        {
            edges.insert(std::make_pair(id, edge.second.to));
        }
        return edges;
    }


    inline Graph_Base::NodeIterator Graph_Base::NodeIterator::beginIterator(Graph_Base& graph)
    {
        NodeIterator iter(graph);
        iter.m_iter = graph.m_nodes.begin();
        return iter;
    }

    inline Graph_Base::NodeIterator Graph_Base::NodeIterator::endIterator(Graph_Base& graph)
    {
        NodeIterator iter(graph);
        iter.m_iter = graph.m_nodes.end();
        return iter;
    }

    inline Graph_Node& Graph_Base::NodeIterator::operator*() {
        return m_iter->second;
    }

    inline Graph_Base::NodeIterator& Graph_Base::NodeIterator::operator++() {
        ++m_iter;
        return *this;
    }

    inline bool Graph_Base::NodeIterator::operator!=(const NodeIterator& other) {
        return m_iter != other.m_iter;
    }

    inline Graph_Base::EdgeIterator Graph_Base::EdgeIterator::beginIterator(Graph_Base& graph)
    {
        EdgeIterator iter(graph);
        if (graph.m_edges.size() != 0)
        {
            iter.m_iterEdges = graph.m_edges.begin();
            iter.m_iterNearEdges = iter.m_iterEdges->second.begin();
        }
        else
        {
            iter.m_iterEdges = graph.m_edges.end();
        }
        return iter;
    }

    inline Graph_Base::EdgeIterator Graph_Base::EdgeIterator::endIterator(Graph_Base& graph)
    {
        EdgeIterator iter(graph);
        if (graph.m_edges.size() != 0)
        {
            iter.m_iterEdges = graph.m_edges.end();
        }
        return iter;
    }

    inline Graph_Edge& Graph_Base::EdgeIterator::operator*() {
        return m_iterNearEdges->second;
    }

    inline Graph_Base::EdgeIterator& Graph_Base::EdgeIterator::operator++() {
        m_iterNearEdges++;
        while (m_iterNearEdges == m_iterEdges->second.end())
        {
            m_iterEdges++;
            if (m_iterEdges == graph.m_edges.end())
                break;
            m_iterNearEdges = m_iterEdges->second.begin();
        }
        return *this;
    }

    inline bool Graph_Base::EdgeIterator::operator!=(const EdgeIterator& other) {
        if (m_iterEdges == graph.m_edges.end())
            return m_iterEdges != other.m_iterEdges;
        return m_iterEdges != other.m_iterEdges || m_iterNearEdges != other.m_iterNearEdges;
    }

    inline Graph_Base::NearNodeIterator::NearNodeIterator(Graph_Base& graph, int id) :graph(graph) {}

    inline Graph_Base::NearNodeIterator Graph_Base::NearNodeIterator::beginIterator(Graph_Base& graph, int id)
    {
        auto iter = NearNodeIterator(graph, id);
        iter.m_iterNearEdges = graph.m_edges[id].begin();
        return iter;
    }

    inline Graph_Base::NearNodeIterator Graph_Base::NearNodeIterator::endIterator(Graph_Base& graph, int id)
    {
        auto iter = NearNodeIterator(graph, id);
        iter.m_iterNearEdges = graph.m_edges[id].end();
        return iter;
    }

    inline Graph_Node& Graph_Base::NearNodeIterator::operator*() {
        return graph.m_nodes[m_iterNearEdges->second.from];
    }

    inline Graph_Base::NearNodeIterator& Graph_Base::NearNodeIterator::operator++() {
        ++m_iterNearEdges;
    }

    inline bool Graph_Base::NearNodeIterator::operator!=(const NearNodeIterator& other) {
        return m_iterNearEdges != other.m_iterNearEdges;
    }

    inline Graph_Base::NearEdgeIterator::NearEdgeIterator(Graph_Base& graph, int id) :graph(graph) {
        m_iterNearEdges = graph.m_edges[id].begin();
    }

    inline Graph_Base::NearEdgeIterator Graph_Base::NearEdgeIterator::beginIterator(Graph_Base& graph, int id)
    {
        auto iter = NearEdgeIterator(graph, id);
        iter.m_iterNearEdges = graph.m_edges[id].begin();
        return iter;
    }

    inline Graph_Base::NearEdgeIterator Graph_Base::NearEdgeIterator::endIterator(Graph_Base& graph, int id)
    {
        auto iter = NearEdgeIterator(graph, id);
        iter.m_iterNearEdges = graph.m_edges[id].end();
        return iter;
    }

    inline Graph_Edge& Graph_Base::NearEdgeIterator::operator*() {
        return m_iterNearEdges->second;
    }

    inline Graph_Base::NearEdgeIterator& Graph_Base::NearEdgeIterator::operator++() {
        ++m_iterNearEdges;
    }

    inline bool Graph_Base::NearEdgeIterator::operator!=(const NearEdgeIterator& other) {
        return m_iterNearEdges != other.m_iterNearEdges;
    }

    inline Graph_Base::NodeView::NodeView(Graph_Base& graph) : graph(graph) {}

    inline Graph_Base::NodeIterator Graph_Base::NodeView::begin() {
        return Graph_Base::NodeIterator::beginIterator(graph);
    }

    inline Graph_Base::NodeIterator Graph_Base::NodeView::end() {
        return Graph_Base::NodeIterator::endIterator(graph);
    }

    inline Graph_Base::EdgeView::EdgeView(Graph_Base& graph) : graph(graph) {}

    inline Graph_Base::EdgeIterator Graph_Base::EdgeView::begin() {
        return Graph_Base::EdgeIterator::beginIterator(graph);
    }

    inline Graph_Base::EdgeIterator Graph_Base::EdgeView::end() {
        return Graph_Base::EdgeIterator::endIterator(graph);
    }

    inline Graph_Base::NearNodeView::NearNodeView(Graph_Base& graph, int id) : graph(graph), id(id) {}

    inline Graph_Base::NearNodeIterator Graph_Base::NearNodeView::begin() {
        return Graph_Base::NearNodeIterator::beginIterator(graph, id);
    }

    inline Graph_Base::NearNodeIterator Graph_Base::NearNodeView::end() {
        return Graph_Base::NearNodeIterator::endIterator(graph, id);
    }

    inline Graph_Base::NearEdgeView::NearEdgeView(Graph_Base& graph, int id) : graph(graph), id(id) {}

    inline Graph_Base::NearEdgeIterator Graph_Base::NearEdgeView::begin() {
        return Graph_Base::NearEdgeIterator::beginIterator(graph, id);
    }

    inline Graph_Base::NearEdgeIterator Graph_Base::NearEdgeView::end() {
        return Graph_Base::NearEdgeIterator::endIterator(graph, id);
    }
}