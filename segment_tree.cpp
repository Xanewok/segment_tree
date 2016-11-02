#include <iostream>
#include <cassert>

void log_debug()
{
#ifndef NDEBUG
		std::cerr << "\n";
#endif
}
template<typename T, typename ... Args>
void log_debug(T t, Args&&... args)
{
#ifndef NDEBUG
		std::cerr << t;
		log_debug(args...);
#endif
}

#define SAFE_DELETE(p) if (p) delete p; p = nullptr;

template <typename T>
class segment_tree
{
	struct node
	{
		T value;
		node* left;
		node* right;

		node() : value(T()), left(nullptr), right(nullptr) {}
		node(T _value) : value(_value), left(nullptr), right(nullptr) {}
		~node()
		{
			SAFE_DELETE(left);
			SAFE_DELETE(right);
		}
	};


	node* root;
	T min_value;
	T max_value;

private:
	struct node_context
	{
		struct node* node;
		T begin;
		T end;
		node_context(struct node* _node, T _begin, T _end) : node(_node), begin(_begin), end(_end) {}
	};

	void insert_internal(T key, T value, const node_context& ctx)
	{
		log_debug("insert_internal(", key, ", ", value, ", ", (unsigned long long)ctx.node, ", ", ctx.begin, ", ", ctx.end, ")");

		if (ctx.begin <= key && ctx.end >= key)
			ctx.node->value = ctx.node->value + value;

		// leaf
		if (ctx.begin == ctx.end)
			return;

		auto fetch_node = [](node*& p) { return p != nullptr? p : p = new node(); };

		T middle = (ctx.begin + ctx.end) / 2;
		if (key >= ctx.begin && key <= middle)
			insert_internal(key, value, node_context(fetch_node(ctx.node->left), ctx.begin, middle));

		if (key >= middle + 1 && key <= ctx.end)
			insert_internal(key, value, node_context(fetch_node(ctx.node->right), middle + 1, ctx.end));
	}

	T query_internal(T begin, T end, const node_context& ctx) const
	{
		if (ctx.node == nullptr || ctx.begin > end || ctx.end < begin)
			return 0;

		log_debug("query_internal(", begin, ", ", end, ", ", (unsigned long long)ctx.node, ", ", ctx.begin, ", ", ctx.end, ")");

		if (ctx.begin >= begin && ctx.end <= end)
		{
			return ctx.node->value;
		}
		else
		{
			T middle = (ctx.begin + ctx.end) / 2;
			return query_internal(begin, end, node_context(ctx.node->left, ctx.begin, middle)) +
					query_internal(begin, end, node_context(ctx.node->right, middle + 1, ctx.end));
		}
	}

public:
	segment_tree(T min, T max) : root(new node()), min_value(min), max_value(max) {}
	~segment_tree()
	{
		SAFE_DELETE(root);
	}

	void insert(T key, T value)
	{
		assert(key >= min_value);
		assert(key <= max_value);

		insert_internal(key, value, node_context(root, min_value, max_value));

		log_tree();
	}

	T query(T begin, T end) const
	{
		assert(begin >= min_value);
		assert(end <= max_value);

		return query_internal(begin, end, node_context(root, min_value, max_value));
	}

#ifndef NDEBUG
private:
	// Prints a tree in a Newick format
	void log_tree(struct node* node, T begin, T end) const
	{
		if (node)
		{
			if (node->left || node->right)
			{
				T middle = (begin + end) / 2;
				std::cerr << "(";
				log_tree(node->left, begin, middle);
				std::cerr << ",";
				log_tree(node->right, middle + 1, end);
				std::cerr << ")";
			}
		}
		std::cerr << begin << "~" << end;
	}
#endif

public:
	void log_tree() const
	{
#ifndef NDEBUG
		log_tree(root, min_value, max_value);
		std::cerr << "\n";
#endif
	}
};

int main()
{
	segment_tree<int> tree(0, 100);
	tree.insert(20, +2);
	assert(tree.query(0, 10) == 0);
	assert(tree.query(15, 20) == 2);
	assert(tree.query(20, 25) == 2);
	tree.insert(21, 3);
	assert(tree.query(15, 20) == 2);
	assert(tree.query(21, 25) == 3);
	assert(tree.query(15, 25) == 5);
	return 0;
}
