# segment_tree
simple segment tree C++ implementation for range sum queries

Heap-allocated template segment tree for all your insert/range sum query needs.  
Currently implements
```c++
segment_tree<T>::segment_tree(T min, T max)
void segment_tree<T>::insert(T key, T value)
T segment_tree<T>::query(T key, T value) const
void segment_tree<T>::clear()
```

# Example
```c++
segment_tree<int> tree(0, 100); // Construct a tree for an interval [0-100]

tree.insert(20, 2);
assert(tree.query(15, 20) == 2);
assert(tree.query(15, 25) == 2);
tree.insert(21, 3);
assert(tree.query(15, 20) == 2);
assert(tree.query(21, 25) == 3);
assert(tree.query(15, 25) == 5);
```
