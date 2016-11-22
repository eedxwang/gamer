#pragma once

#include <cstdint>
#include <map>
#include <set>
#include <iterator>
#include <array>
#include <vector>
#include <iostream>
#include <fstream>
#include <functional>
#include <ostream>
#include <unordered_set>
#include <utility>
#include "util.h"

//using Simplex = unsigned int;

namespace detail {
	template <class T> using map = std::map<size_t,T>;
	/*
	 * asc_Node must be defined outside of simplicial_complex because c++ does not allow
	 * internal templates to be partially specialized. I admit that I do not understand
	 * the reason for this, but this work around seems to work. However, the muddying of
	 * the template parameters is unfortunate.
	 */
	template <class KeyType, size_t k, size_t N, typename DataTypes, class> struct asc_Node;

	/**
	 * @brief      A base class for a Simplicial Complex Node
	 */
	struct asc_NodeBase {
		/**
		 * @brief      Node constructor
		 *
		 * @param[in]  id    The identifier
		 */
		asc_NodeBase(int id) : _node(id) {}
		
		/**
		 * @brief      Destroys the object
		 */
		virtual ~asc_NodeBase() {};

		size_t _node;	/**< Internal Node ID*/
	};

	/**
	 * @brief      Base class for Node with some data
	 *
	 * @tparam     DataType  type of the data to be contained
	 */
	template <class DataType>
	struct asc_NodeData {
		DataType _data; /**< stored data with type DataType */
	};

	/**
	 * @brief      Base class for Node with DataType of void
	 */
	template <>
	struct asc_NodeData<void> {};

	/**
	 * @brief      Base class for Node with edge data of type DataType
	 *
	 * @tparam     KeyType   type for indexing Nodes
	 * @tparam     DataType  type of edge_data stored
	 */
	template <class KeyType, class DataType>
	struct asc_EdgeData {
		std::map<KeyType, DataType> _edge_data;
	};	

	/**
	 * @brief      Base class for Node with no edge data
	 *
	 * @tparam     KeyType  type for indexing Nodes
	 */
	template <class KeyType>
	struct asc_EdgeData<KeyType,void>
	{};

	/**
	 * @brief      Base class for Node with parent Nodes
	 *
	 * @tparam     KeyType        type for indexing Nodes
	 * @tparam     k              the depth of the Node
	 * @tparam     N              the maximum depth of Nodes in the simplicial complex
	 * @tparam     NodeDataTypes  a util::type_holder array of Node types
	 * @tparam     EdgeDataTypes  a util::type_holder array of Edge types
	 */
	template <class KeyType, size_t k, size_t N, class NodeDataTypes, class EdgeDataTypes>
	struct asc_NodeDown : public asc_EdgeData<KeyType,typename util::type_get<k-1,EdgeDataTypes>::type> {
		using DownNodeT = asc_Node<KeyType,k-1,N,NodeDataTypes,EdgeDataTypes>;
		std::map<KeyType, DownNodeT*> _down;	/**< @brief Map of pointers to parents */
	};

	/**
	 * @brief      Base class for Node with children Nodes
	 *
	 * @tparam     KeyType        type for indexing Nodes
	 * @tparam     k              the depth of the Node
	 * @tparam     N              the maximum depth of Nodes in the simplicial complex
	 * @tparam     NodeDataTypes  a util::type_holder array of Node types
	 * @tparam     EdgeDataTypes  a util::type_holder array of Edge types
	 */
	template <class KeyType, size_t k, size_t N, class NodeDataTypes, class EdgeDataTypes>
	struct asc_NodeUp {
		using UpNodeT = asc_Node<KeyType,k+1,N,NodeDataTypes,EdgeDataTypes>;
		std::map<KeyType, UpNodeT*> _up;	/**< @brief Map of pointers to children */
	};

	/**
	 * @brief      Node with both parents and children 
	 *
	 * @tparam     KeyType        index typename
	 * @tparam     k              level of the node
	 * @tparam     N              max level of the simplicial complex
	 * @tparam     NodeDataTypes  util::type_holder of node types
	 * @tparam     EdgeDataTypes  util::type_holder of edge types
	 */
	template <class KeyType, size_t k, size_t N, class NodeDataTypes, class EdgeDataTypes>
	struct asc_Node : public asc_NodeBase,
					  public asc_NodeData<typename util::type_get<k,NodeDataTypes>::type>,
					  public asc_NodeDown<KeyType, k, N, NodeDataTypes, EdgeDataTypes>,
					  public asc_NodeUp<KeyType, k, N, NodeDataTypes, EdgeDataTypes>
	{
		static constexpr size_t level = k;

		asc_Node(int id) : asc_NodeBase(id) {}
        
        friend std::ostream& operator<<(std::ostream& output, const asc_Node& node){
            output  << "Node(level=" << k << ", " << "id=" << node._node;
            if(node._down.size() > 0)
                for(auto it=node._down.cbegin(); it!=node._down.cend(); ++it)
                    output  << ", NodeDownID={'"
                            << it->first << "', "
                            << it->second->_node << "}";
            if(node._up.size() > 0)
               for(auto it=node._up.cbegin(); it!=node._up.cend(); ++it)
                    output  << ", NodeUpID={'"
                            << it->first << "', "
                            << it->second->_node << "}";
            output  << ")";
            return output;
        }
	};

	/**
	 * @brief      Root node with only children
	 *
	 * @tparam     KeyType        index typename
	 * @tparam     N              max level of the simplicial complex
	 * @tparam     NodeDataTypes  util::type_holder of node types
	 * @tparam     EdgeDataTypes  util::type_holder of edge types
	 */
	template <class KeyType, size_t N, class NodeDataTypes, class EdgeDataTypes>
	struct asc_Node<KeyType,0,N,NodeDataTypes,EdgeDataTypes> : public asc_NodeBase,
											 public asc_NodeData<typename util::type_get<0,NodeDataTypes>::type>,
											 public asc_NodeUp<KeyType, 0, N, NodeDataTypes, EdgeDataTypes>
	{
		static constexpr size_t level = 0;

		asc_Node(int id) : asc_NodeBase(id) {}
       
        friend std::ostream& operator<<(std::ostream& output, const asc_Node& node){
            output  << "Node(level=" << 0
                    << ", id=" << node._node;
            if(node._up.size() > 0)
                for(auto it=node._up.cbegin(); it!=node._up.cend(); ++it)
                    output  << ", NodeUpID={'"
                            << it->first << "', "
                            << it->second->_node << "}";
            output << ")";
            return output;
        }
	};

	/**
	 * @brief      Top level node with only parents
	 *
	 * @tparam     KeyType        index typename
	 * @tparam     N              max level of the simplicial complex
	 * @tparam     NodeDataTypes  util::type_holder of node types
	 * @tparam     EdgeDataTypes  util::type_holder of edge types
	 */
	template <class KeyType, size_t N, class NodeDataTypes, class EdgeDataTypes>
	struct asc_Node<KeyType,N,N,NodeDataTypes,EdgeDataTypes> : public asc_NodeBase,
											 public asc_NodeData<typename util::type_get<N,NodeDataTypes>::type>,
					  						 public asc_NodeDown<KeyType, N, N, NodeDataTypes, EdgeDataTypes>
	{
		static constexpr size_t level = N;

		asc_Node(int id) : asc_NodeBase(id) {}
       
        friend std::ostream& operator<<(std::ostream& output, const asc_Node& node){
            output  << "Node(level=" << N
                    << ", id=" << node._node;
            if(node._down.size() > 0)
                for(auto it=node._down.cbegin(); it!=node._down.cend(); ++it)
                    output  << ", NodeDownID={'"
                            << it->first << "', "
                            << it->second->_node << "}";
            output << ")";
            return output;
        }
	};

	/*
	 * These are iterator adapters. The use of boost libraries is indicated.
	 */
	template <typename Iter, typename Data>
	struct node_id_iterator : public std::iterator<std::bidirectional_iterator_tag, Data> {
	public:
		using super = std::iterator<std::bidirectional_iterator_tag, Data>;
		node_id_iterator() {}
		node_id_iterator(Iter j) : i(j) {}
		node_id_iterator& operator++() { ++i; return *this; }
		node_id_iterator operator++(int) { auto tmp = *this; ++(*this); return tmp; }
		node_id_iterator& operator--() { --i; return *this; }
		node_id_iterator operator--(int) { auto tmp = *this; --(*this); return tmp; }
		bool operator==(node_id_iterator j) const { return i == j.i; }
		bool operator!=(node_id_iterator j) const { return !(*this == j); }
		Data operator*() { return Data(i->second); }
        typename super::pointer operator->() { return Data(i->second); }
	protected:
		Iter i;
	};

	template <typename Iter, typename Data>
	inline node_id_iterator<Iter,Data> make_node_id_iterator(Iter j) { return node_id_iterator<Iter,Data>(j); }

	template <typename Iter, typename Data>
	struct node_data_iterator : public std::iterator<std::bidirectional_iterator_tag, Data> {
	public:
		using super = std::iterator<std::bidirectional_iterator_tag, Data>;
		node_data_iterator() {}
		node_data_iterator(Iter j) : i(j) {}
		node_data_iterator& operator++() { ++i; return *this; }
		node_data_iterator operator++(int) { auto tmp = *this; ++(*this); return tmp; }
		node_data_iterator& operator--() { --i; return *this; }
		node_data_iterator operator--(int) { auto tmp = *this; --(*this); return tmp; }
		bool operator==(node_data_iterator j) const { return i == j.i; }
		bool operator!=(node_data_iterator j) const { return !(*this == j); }
		typename super::reference operator*() { return i->second->_data; }
		typename super::pointer operator->() { return i->second->_data; }
	protected:
		Iter i;
	};

	template <typename Iter, typename Data>
	inline 
	node_data_iterator<Iter,Data> make_node_data_iterator(Iter j) { return node_data_iterator<Iter,Data>(j); }
} // end namespace


template <typename K, typename... Ts>
struct simplicial_complex_traits_default
{
	template <std::size_t k> using all_void = int;
	using KeyType = K;
	using NodeTypes = util::type_holder<Ts...>;
	using EdgeTypes = typename util::int_type_map<std::size_t,
									                util::type_holder,
									                typename std::make_index_sequence<sizeof...(Ts)-1>,
									                all_void>::type;
};


template <typename traits>
class simplicial_complex {
public:
	using KeyType = typename traits::KeyType;
	using NodeDataTypes = typename traits::NodeTypes;
	using EdgeDataTypes = typename traits::EdgeTypes;
	using type_this = simplicial_complex<traits>;
	static constexpr auto numLevels = NodeDataTypes::size;
	static constexpr auto topLevel = numLevels-1;
	using LevelIndex = typename std::make_index_sequence<numLevels>;

	template <std::size_t k> using Node = detail::asc_Node<KeyType,k,topLevel,NodeDataTypes,EdgeDataTypes>;
	template <std::size_t k> using NodeData = typename util::type_get<k,NodeDataTypes>::type;
	template <std::size_t k> using EdgeData = typename util::type_get<k,EdgeDataTypes>::type;
	template <std::size_t k> using NodePtr = Node<k>*;

	template <std::size_t k>
	struct NodeID {
		using complex = simplicial_complex<traits>;
		friend simplicial_complex<traits>;
		static constexpr size_t level = k;

		NodeID() : ptr(nullptr) {}
		NodeID(NodePtr<k> p) : ptr(p) {}
		NodeID(const NodeID& rhs) : ptr(rhs.ptr) {}

		NodeID& operator=(const NodeID& rhs) { ptr = rhs.ptr; return *this;}
		friend bool operator==(NodeID lhs, NodeID rhs) { return lhs.ptr == rhs.ptr; }
		friend bool operator!=(NodeID lhs, NodeID rhs) { return lhs.ptr != rhs.ptr; }
		friend bool operator<=(NodeID lhs, NodeID rhs) { return lhs.ptr <= rhs.ptr; }
		friend bool operator>=(NodeID lhs, NodeID rhs) { return lhs.ptr >= rhs.ptr; }
		friend bool operator<(NodeID lhs, NodeID rhs)  { return lhs.ptr < rhs.ptr; }
		friend bool operator>(NodeID lhs, NodeID rhs)  { return lhs.ptr > rhs.ptr; }

		explicit operator std::uintptr_t () const { return reinterpret_cast<std::uintptr_t>(ptr); }

		auto const&& operator*() const { return ptr->_data; }
		auto&& operator*() { return ptr->_data; }

		auto const&& data() const { return ptr->_data; }
		auto&& data() { return ptr->_data; }

//		friend std::ostream& operator<<(std::ostream& out, const NodeID& nid) { out << *nid.ptr; return out; }
		friend std::ostream& operator<<(std::ostream& out, const NodeID& nid) { out << nid.ptr; return out; }

	private:
		NodePtr<k> ptr;
	};


	template <std::size_t k>
	struct EdgeID {
		using complex = simplicial_complex<traits>;
		friend simplicial_complex<traits>;
		static constexpr size_t level = k;

		EdgeID() : ptr(nullptr), edge(0) {}
		EdgeID(NodePtr<k> p, KeyType e) : ptr(p), edge(e) {}
		EdgeID(const EdgeID& rhs) : ptr(rhs.ptr), edge(rhs.edge) {}

		EdgeID& operator=(const EdgeID& rhs) { ptr = rhs.ptr; edge = rhs.edge; return *this;}
		friend bool operator==(EdgeID lhs, EdgeID rhs) { return lhs.ptr == rhs.ptr && lhs.edge == rhs.edge; }
		friend bool operator!=(EdgeID lhs, EdgeID rhs) { return !(lhs == rhs); }
		friend bool operator<=(EdgeID lhs, EdgeID rhs) { return lhs < rhs || lhs == rhs; }
		friend bool operator>=(EdgeID lhs, EdgeID rhs) { return lhs > rhs || lhs == rhs; }
		friend bool operator<(EdgeID lhs, EdgeID rhs)
		{
			return (lhs.ptr < rhs.ptr) || (lhs.ptr == rhs.ptr && lhs.edge < rhs.edge);
		}
		friend bool operator>(EdgeID lhs, EdgeID rhs)  { return rhs < lhs; }

//		explicit operator std::size_t () const { return static_cast<std::size_t>(ptr); }

		auto const& operator*() const { return data(); }
		auto& operator*() { return data(); }

		KeyType key() const { return edge; }

		auto const& data() const { return ptr->_edge_data[edge]; }
		auto& data() { return ptr->_edge_data[edge]; }

		NodeID<k>   up() const { return ptr; }
		NodeID<k-1> down() const { return NodeID<k-1>(ptr->_down[edge]); }

	private:
		NodePtr<k> ptr;
		KeyType edge;
	};

	simplicial_complex()
		: node_count(0)
	{
		_root = create_node<0>();
		for(auto& x : level_count)
		{
			x = 0;
		}
	}

	~simplicial_complex()
	{
		size_t count;
		remove_recurse<0,0>::apply(this, &_root, &_root + 1, count);
	}

	template <size_t n>
	void insert(const KeyType (&s)[n])
	{
		insert_for<0,n,false>::apply(this, _root, s);
	}

	template <size_t n>
	void insert(const KeyType (&s)[n], const NodeData<n>& data)
	{
		Node<n>* rval = insert_for<0,n,true>::apply(this, _root, s);
		rval->_data = data;
	}

	template <size_t n>
	void insert(const std::array<KeyType,n>& s)
	{
		insert_for<0,n,false>::apply(this, _root, s.data());
	}

	template <size_t n>
	void insert(const std::array<KeyType,n>& s, const NodeData<n>& data)
	{
		Node<n>* rval = insert_for<0,n,true>::apply(this, _root, s.data());
		rval->_data = data;
	}

	template <size_t n>
	std::array<KeyType,n> get_name(NodeID<n> id) const
	{
		std::array<KeyType,n> s;

		int i = 0;
		for(auto curr : id.ptr->_down)
		{
			s[i++] = curr.first;
		}
		
		return s;
	}

	std::array<KeyType,0> get_name(NodeID<0> id) const
	{
		std::array<KeyType,0> name;
		return name;
	}

	// Node
	template <size_t n>
	NodeID<n> get_node_up(const KeyType (&s)[n]) const
	{
		return get_recurse<0,n>::apply(this, s, _root);
	}

	template <size_t i, size_t j>
	NodeID<i+j> get_node_up(const NodeID<i> nid, const KeyType (&s)[j]) const
	{
		return get_recurse<i,j>::apply(this, s, nid);
	}

	template <size_t i>
	NodeID<i+1> get_node_up(const NodeID<i> nid, const KeyType s) const
	{
		return get_recurse<i,1>::apply(this, &s, nid.ptr);
	}

	template <size_t i, size_t j>
	NodeID<i-j> get_node_down(const NodeID<i> nid, const KeyType (&s)[j]) const
	{
		return get_down_recurse<i,j>::apply(this, s, nid.ptr);
	}

	template <size_t i>
	NodeID<i-1> get_node_down(const NodeID<i> nid, const KeyType s) const
	{
		return get_down_recurse<i,1>::apply(this, &s, nid.ptr);
	}

	NodeID<0> get_node_up() const
	{
		return _root;
	}

	NodeID<0> get_node_down() const
	{
		return _root;
	}

	template <size_t k, class Inserter>
	void get_cover(const NodeID<k> id, Inserter pos) const
	{
		for(auto curr : id.ptr->_up)
		{
			pos++ = curr.first;
		}
	}

	template <size_t k>
	auto get_cover(const NodeID<k> id) const
	{
		std::vector<KeyType> rval;
		get_cover(id, std::back_inserter(rval));
		return std::move(rval);
	}

	template <size_t k>
	std::set<NodeID<k+1>> up(const std::set<NodeID<k>>& nodes) const
	{
		std::set<NodeID<k+1>> rval;
		for(auto nid : nodes)
		{
			for(auto p : nid.ptr->_up)
			{
				rval.insert(NodeID<k+1>(p.second));
			}
		}
		return rval;
	}

	template <size_t k>
	std::set<NodeID<k+1>> up(const NodeID<k> nid) const
	{
		std::set<NodeID<k+1>> rval;
		for(auto p : nid.ptr->_up)
		{
			rval.insert(NodeID<k+1>(p.second));
		}
		return rval;
	}

	template <size_t k>
	std::set<NodeID<k-1>> down(const std::set<NodeID<k>>& nodes) const
	{
		std::set<NodeID<k-1>> rval;
		for(auto nid : nodes)
		{
			for(auto p : nid.ptr->_down)
			{
				rval.insert(NodeID<k-1>(p.second));
			}
		}
		return rval;
	}

	template <size_t k>
	std::set<NodeID<k-1>> down(const NodeID<k> nid) const
	{
		std::set<NodeID<k-1>> rval;
		for(auto p : nid.ptr->_down)
		{
			rval.insert(NodeID<k-1>(p.second));
		}
		return rval;
	}

	// Edge
	template <size_t k>
	auto get_edge_up(NodeID<k> nid, KeyType a)
	{
		return EdgeID<k+1>(nid.ptr->_up[a], a);
	}

	template <size_t k>
	auto get_edge_down(NodeID<k> nid, KeyType a)
	{
		return EdgeID<k>(nid.ptr, a);
	}

	template <size_t k>
	auto get_edge_up(NodeID<k> nid, KeyType a) const
	{
		return EdgeID<k+1>(nid.ptr->_up[a], a);
	}

	template <size_t k>
	auto get_edge_down(NodeID<k> nid, KeyType a) const
	{
		return EdgeID<k>(nid.ptr, a);
	}

	template <size_t k>
	bool exists(const KeyType (&s)[k]) const
	{
		
		return get_recurse<0,k>::apply(this, s, _root) != 0;
	}

	template <std::size_t k>
	auto size() const
	{
		return std::get<k>(levels).size();
	}

	template <std::size_t k>
	auto get_level_id()
	{
		auto begin = std::get<k>(levels).begin();
		auto end = std::get<k>(levels).end();
		auto data_begin = detail::make_node_id_iterator<decltype(begin),NodeID<k>>(begin);
		auto data_end = detail::make_node_id_iterator<decltype(end),NodeID<k>>(end);
		return util::make_range(data_begin, data_end);
	}

	template <std::size_t k>
	auto get_level_id() const
	{
		auto begin = std::get<k>(levels).cbegin();
		auto end = std::get<k>(levels).cend();
		auto data_begin = detail::make_node_id_iterator<decltype(begin), const NodeID<k>>(begin);
		auto data_end = detail::make_node_id_iterator<decltype(end), const NodeID<k>>(end);
		return util::make_range(data_begin, data_end);
	}

	template <std::size_t k>
	auto get_level()
	{
		auto begin = std::get<k>(levels).begin();
		auto end = std::get<k>(levels).end();
		auto data_begin = detail::make_node_data_iterator<decltype(begin),NodeData<k>>(begin);
		auto data_end = detail::make_node_data_iterator<decltype(end),NodeData<k>>(end);
		return util::make_range(data_begin, data_end);
	}

	template <std::size_t k>
	auto get_level() const
	{
		auto begin = std::get<k>(levels).cbegin();
		auto end = std::get<k>(levels).cend();
		auto data_begin = detail::make_node_data_iterator<decltype(begin), const NodeData<k>>(begin);
		auto data_end = detail::make_node_data_iterator<decltype(end), const NodeData<k>>(end);
		return util::make_range(data_begin, data_end);
	}

	template <std::size_t k>
	size_t remove(const KeyType (&s)[k])
	{
		Node<k>* root = get_recurse<0,k>::apply(this, s, _root);
		size_t count = 0;
		return remove_recurse<k,0>::apply(this, &root, &root + 1, count);
	}

	template <std::size_t k>
	size_t remove(const std::array<KeyType,k>& s)
	{
		Node<k>* root = get_recurse<0,k>::apply(this, s.data(), _root);
		size_t count = 0;
		return remove_recurse<k,0>::apply(this, &root, &root + 1, count);
	}

	template <std::size_t k>
	std::size_t remove(NodeID<k> s)
	{
		size_t count = 0;
		return remove_recurse<k,0>::apply(this, &s.ptr, &s.ptr + 1, count);
	}

	template <std::size_t L, std::size_t R>
	bool leq(NodeID<L> lhs, NodeID<R> rhs) const
	{
		auto name_lhs = get_name(lhs);
		auto name_rhs = get_name(rhs);

		std::size_t i = 0;
		for(std::size_t j = 0; i < L && j < R; ++j)
		{
			if(name_lhs[i] == name_rhs[j])
			{
				++i;
			}
		}

		return (i == L);
	}

	template <std::size_t L, std::size_t R>
	bool eq(NodeID<L> lhs, NodeID<R> rhs) const
	{
		return false;
	}

	template <std::size_t k>
	bool eq(NodeID<k> lhs, NodeID<k> rhs) const
	{
		auto name_lhs = get_name(lhs);
		auto name_rhs = get_name(rhs);

		for(std::size_t i = 0; i < k; ++i)
		{
			if(name_lhs[i] != name_rhs[i])
			{
				return false;
			}
		}

		return true;
	}

	template <std::size_t L, std::size_t R>
	bool lt(NodeID<L> lhs, NodeID<R> rhs) const
	{
		return L < R && leq(lhs,rhs);
	}



	void genGraph(const std::string& filename){
	    std::ofstream fout(filename);
	    if(!fout.is_open())
	    {
	        std::cerr   << "File '" << filename 
	                    << "' could not be writen to." << std::endl;
	        exit(1); 
	    }
	    fout 	<< "digraph {\n"
	    		<< "dpi=300;\n";
		std::set<Node<0>*> root{_root};
		writeGraph<0,0>::apply(fout, root);
	    
	    fout << "}\n";
		fout.close();    	
	}
/*
	void renumber(){
		auto it = std::get<1>(levels).end();

		for(int i=0; i < this->size<1>(); ++i)
		{
			auto nodeID = this->get_node_up({i});
			if(nodeID == nullptr)
			{
				--it;
				auto name = (*it->second->_down.begin()).first;
				if (name <= i){
					std::cout << "Failure!" << std::endl;
				}
				std::set<Node<1>*> node{it->second};
				renumber_recurse<1,0>::apply(node, name, i);
			}
		}
	}
*/
private:
/*
	template <size_t k, size_t foo>	
	struct renumber_recurse
	{
	    template <typename T>
	    static void apply(T nodes, KeyType from, KeyType to)
	    {
			std::set<Node<k+1>*> next;
			// for each node of interest...
			for(auto node : nodes)
			{
				auto& down = node->_down;
				auto parent = down[from];
				down.erase(from);
				down[to] = parent;

				auto& pUP = parent->_up;
				pUP.erase(from);
				pUP[to] = node;

				auto up = node->_up;
				for(auto j = up.begin(); j != up.end(); ++j)
				{
					next.insert(j->second);
				}
			}
			renumber_recurse<k+1,foo>::apply(next, from, to);
		}	
	};
	
	template <size_t foo>
	struct renumber_recurse<numLevels-1,foo>
	{
		template <typename T>
		static void apply(T nodes, KeyType from, KeyType to)
		{
			for(auto node : nodes)	
			{		
				auto& down = node->_down;
				auto parent = down[from];
				down.erase(from);
				down[to] = parent;
			}
		}
	};
*/

	template <size_t k, size_t foo>	
	struct writeGraph{
	    template <typename T>
	    static void apply(std::ofstream& fout, T nodes){
			std::set<Node<k+1>*> next;
			// for each node of interest...
			for(auto node : nodes)
			{
				auto up = node->_up;
				for(auto j = up.begin(); j != up.end(); ++j)
				{
					auto orient = j->second->_edge_data[j->first].orientation;
					if (orient == 1)
						fout <<	"	" << node->_node << " -> " << j->second->_node 
							 << "[label=\"" << j->first << "\"]\n";
					else 
						fout <<	"	" << j->second->_node << " -> " << node->_node
							 << "[label=\"" << j->first << "\"]\n";
					next.insert(j->second);
				}
			}
			
			fout 	<< "subgraph cluster_" << k << " {\n"
					<< "label=\"Level " << k << "\"\n";
			for (auto j : nodes){
				fout << j->_node << ";";
			}
			fout 	<< "\n}\n";
			writeGraph<k+1,foo>::apply(fout, next);
		}	
	};
	
	template <size_t foo>
	struct writeGraph<numLevels-1,foo>
	{
		template <typename T>
		static void apply(std::ofstream & fout, T nodes)
		{
			fout 	<< "subgraph cluster_" << numLevels-1 << " {\n"
					<< "label=\"Level " << numLevels-1 << "\"\n";
			for (auto node : nodes){
				fout << node->_node << ";";
			}
			fout 	<< "\n}\n";
		}
	};

	/**
	 * Recursively deletes dependent nodes.
	 *
	 * @tparam     level  { description }
	 * @tparam     foo    { description }
	 */
	// The foo argument is necessary to get the compiler to shut up...
	template <size_t level, size_t foo>
	struct remove_recurse
	{
		template <typename T>
		static size_t apply(type_this* that, T begin, T end, size_t& count)
		{
			std::set<Node<level+1>*> next;
			// for each node of interest...
			for(auto i = begin; i != end; ++i)
			{
				auto up = (*i)->_up;
				for(auto j = up.begin(); j != up.end(); ++j)
				{
					next.insert(j->second);
				}
				that->remove_node(*i);
				++count;
			}
			return remove_recurse<level+1,foo>::apply(that, next.begin(), next.end(), count);
		}
	};

    // Terminal condition for remove_recurse
	template <size_t foo>
	struct remove_recurse<numLevels-1,foo>
	{
		template <typename T>
		static size_t apply(type_this* that, T begin, T end, size_t& count)
		{
			for(auto i = begin; i != end; ++i)
			{
				that->remove_node(*i);
				++count;
			}
			return count;
		}
	};

	template <size_t i, size_t n>
	struct get_recurse
	{
		static Node<i+n>* apply(const type_this* that, const KeyType* s, Node<i>* root)
		{
			if(root)
			{
				auto p = root->_up.find(*s);
				if(p != root->_up.end())
				{
					return get_recurse<i+1,n-1>::apply(that, s+1, root->_up[*s]);
				}
				else
				{
					return nullptr;
				}
			}
			else
			{
				return nullptr;
			}
		}
	};

	template <size_t i>
	struct  get_recurse<i,0>
	{
		static Node<i>* apply(const type_this* that, const KeyType* s, Node<i>* root)
		{
			return root;
		}
	};

	template <size_t i, size_t n>
	struct get_down_recurse
	{
		static Node<i-n>* apply(const type_this* that, const KeyType* s, Node<i>* root)
		{
			if(root)
			{
				auto p = root->_down.find(*s);
				if(p != root->_down.end())
				{
					return get_recurse<i-1,n-1>::apply(that, s+1, root->_down[*s]);
				}
				else
				{
					return nullptr;
				}
			}
			else
			{
				return nullptr;
			}
		}
	};

	template <size_t i>
	struct  get_down_recurse<i,0>
	{
		static Node<i>* apply(const type_this* that, const KeyType* s, Node<i>* root)
		{
			return root;
		}
	};


	template <size_t i, size_t n, bool do_insert>
	struct insert_for
	{
		static Node<i+n>* apply(type_this* that, Node<0>* root, const KeyType* begin)
		{
			that->extend<0,i,do_insert>(root, begin, *(begin+i));
			return insert_for<i+1,n-1,do_insert>::apply(that, root, begin);
		}
	};

	template <size_t i, bool do_insert>
	struct insert_for<i, 1, do_insert>
	{
		static Node<i+1>* apply(type_this* that, Node<0>* root, const KeyType* begin)
		{
			return that->extend<0,i,do_insert>(root, begin, *(begin+i));
		}
	};

	template <size_t level, size_t i, size_t n, bool do_insert>
	struct extend_for
	{
		static Node<level+i+n+1>* apply(type_this* that, Node<level>* root, const KeyType* begin, int value)
		{
			that->extend<level+1,i,do_insert>(root->_up[*(begin+i)], begin, value);
			return extend_for<level,i+1,n-1,do_insert>::apply(that, root, begin, value);
		}
	};

	template <size_t level, size_t i, bool do_insert>
	struct extend_for<level,i,1,do_insert>
	{
		static Node<level+i+1+1>* apply(type_this* that, Node<level>* root, const KeyType* begin, int value)
		{
			return that->extend<level+1,i,do_insert>(root->_up[*(begin+i)], begin, value);
		}
	};

	template <size_t level, size_t i, bool do_insert>
	struct extend_for<level,i,0,do_insert>
	{
		static Node<level+i+1>* apply(type_this* that, Node<level>* root, const KeyType* begin, int value)
		{
			return root->_up[value];
		}
	};

    /**
     *  @brief Backfill in the pointers from prior nodes to the new node
     *  @param root is a parent node
     *  @param nn is the new child node
     *  @param value is the exposed id of nn
     *  @return void
     */
	template <size_t level>
	void backfill(Node<level>* root, Node<level+1>* nn, int value)
	{
		for(auto curr = root->_down.begin(); curr != root->_down.end(); ++curr)
		{
			int v = curr->first;

			Node<level-1>* parent = curr->second;
			Node<level>* child = parent->_up[value]; 

			nn->_down[v] = child;
			child->_up[v] = nn;
		}
	}

    /**
     *  @brief Fill in the pointers from level 1 to 0.
     *  @param root is a level 0 node
     *  @param nn is a level 1 node
     *  @param value is the exposed id of nn
     *  @return void
     */
	void backfill(Node<0>* root, Node<1>* nn, int value)
	{
		return;
	}

	template <size_t level, size_t n, bool do_insert>
	Node<level+n+1>* extend(Node<level>* root, const KeyType* begin, int value)
	{
		if(root->_up.find(value) == root->_up.end())
		{
			Node<level+1>* nn;
			if(n == 0) // we are inserting the node the user requested.
			{
				nn = create_node<level+1>();
			}
			else // we are backfilling. The user may or may not want this.
			{
				nn = create_node<level+1>();
			}
			nn->_down[value] = root;
			root->_up[value] = nn;

			backfill(root, nn, value);
		}

		return extend_for<level,0,n,do_insert>::apply(this, root, begin, value);
	}

	template <size_t level>
	Node<level>* create_node()
	{
		auto p = new Node<level>(node_count++);
		++(level_count[level]);
	    
	    bool ret = std::get<level>(levels).insert(
	    		std::pair<size_t,NodePtr<level>>(node_count-1, p)).second; // node_count-1 to match the id's correctly
        // sanity check to make sure there aren't duplicate keys... 
        if (ret==false) {
            std::cout << "Error: Node '" << node_count << "' already existed with value " << *p << std::endl;
        }
		return p;
	}

	template <size_t level>
	void remove_node(Node<level>* p)
	{
		for(auto curr = p->_down.begin(); curr != p->_down.end(); ++curr)
		{
			curr->second->_up.erase(curr->first);
		}
		for(auto curr = p->_up.begin(); curr != p->_up.end(); ++curr)
		{
			curr->second->_down.erase(curr->first);
		}
		--(level_count[level]);
		std::get<level>(levels).erase(p->_node);
		delete p;
	}

	void remove_node(Node<0>* p)
	{
		for(auto curr = p->_up.begin(); curr != p->_up.end(); ++curr)
		{
			curr->second->_down.erase(curr->first);
		}
		--(level_count[0]);
		std::get<0>(levels).erase(p->_node);
		delete p;
	}

	void remove_node(Node<topLevel>* p)
	{
		for(auto curr = p->_down.begin(); curr != p->_down.end(); ++curr)
		{
			curr->second->_up.erase(curr->first);
		}
		--(level_count[topLevel]);
		std::get<topLevel>(levels).erase(p->_node);
		delete p;
	}

	NodePtr<0> _root;
	size_t node_count;
	std::array<size_t,numLevels> level_count;
	using NodePtrLevel = typename util::int_type_map<std::size_t, std::tuple, LevelIndex, NodePtr>::type;
	typename util::type_map<NodePtrLevel, detail::map>::type levels;
};

template <typename KeyType, typename... Ts>
using AbstractSimplicialComplex = simplicial_complex<simplicial_complex_traits_default<KeyType,Ts...>>;


template <class Complex, std::size_t level, class InsertIter>
void neighbors(Complex &F, typename Complex::template NodeID<level> nid, InsertIter iter)
{
	for (auto a : F.get_name(nid))
	{
		auto id = F.get_node_down(nid,a);
		for(auto b : F.get_cover(id))
		{
			auto nbor = F.get_node_up(id,b);
			if(nbor != nid)
			{
				*iter++ = nbor;
			}
		}
	}
}

template <class Complex, class NodeID, class InsertIter>
void neighbors(Complex& F, NodeID nid, InsertIter iter)
{
	neighbors<Complex, NodeID::level, InsertIter>(F,nid,iter);
}


template <class Complex, std::size_t level, class InsertIter>
void neighbors_up(Complex &F, typename Complex::template NodeID<level> nid, InsertIter iter)
{
	for (auto a : F.get_cover(nid))
	{
		auto id = F.get_node_up(nid,a);
		for(auto b : F.get_name(id))
		{
			auto nbor = F.get_node_down(id,b);
			if(nbor != nid)
			{
				*iter++ = nbor;
			}
		}
	}
}

template <class Complex, class NodeID, class InsertIter>
void neighbors_up(Complex& F, NodeID nid, InsertIter iter)
{
	neighbors_up<Complex, NodeID::level, InsertIter>(F,nid,iter);
}

/**
 * Code for returning a set of k-ring neighbors. Currently obseleted by neighbors_up visitor pattern
 */
// template <class Complex, std::size_t level>
// std::set<typename Complex::template NodeID<level>> neighbors_up(Complex &F, 
// 			std::set<typename Complex::template NodeID<level>>& nodes,
// 			std::set<typename Complex::template NodeID<level>> next,
// 			int ring)
// {
// 	if (ring == 0)
// 		return nodes;
// 	std::set<typename Complex::template NodeID<level>> tmp;
// 	for (auto nid : next){
// 		for (auto a : F.get_cover(nid))
// 		{
// 			auto id = F.get_node_up(nid,a);
// 			for(auto b : F.get_name(id))
// 			{
// 				auto nbor = F.get_node_down(id,b);
// 				if(nodes.insert(nbor).second){
// 					tmp.insert(nbor);
// 				}
// 			}
// 		}
// 	}
// 	return neighbors_up<Complex, level>(F, nodes, tmp, ring-1);
// }

// template <class Complex, class NodeID>
// std::set<NodeID> neighbors_up(Complex& F, NodeID nid, int ring)
// {
// 	std::set<NodeID> nodes{nid};
// 	return neighbors_up<Complex, NodeID::level>(F,nodes,nodes,ring);
// }

template <typename NodeID>
struct hashNodeID{
   size_t operator()(const NodeID nid) const
   {
       return std::hash<std::uintptr_t>()(static_cast<uintptr_t>(nid));
   }
};

template <typename T> using NodeSet = std::unordered_set<T,hashNodeID<T>>;
