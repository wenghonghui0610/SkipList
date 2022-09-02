#ifndef SKIPLIST_NODE_H
#define SKIPLIST_NODE_H

template<typename K, typename V>
class SkipList;

template<typename K, typename V>
class Node {
    friend class SkipList<K, V>;

public:

    Node() {}
    Node(K k, V v, int level);
    ~Node();

    K getKey() const;
    V getValue() const;
    void setValue(V v);

    // next nodes in different levels
    Node<K, V> **forward;
    int nodeLevel;

private:
    K key;
    V value;

};

template <typename K, typename V>
Node <K, V>::Node(const K k, const V v, int level) {
    key = k;
    value = v;
    nodeLevel = level;
    forward = new Node<K, V> *[nodeLevel + 1] ();
}

template <typename K, typename V>
Node<K, V>::~Node() {
    delete[] forward;
}

template <typename K, typename V>
K Node<K, V>::getKey() const {
    return key;
}

template <typename K, typename V>
V Node<K, V>::getValue() const {
    return value;
}

template <typename K, typename V>
void Node<K, V>::setValue(V v) {
    value = v;
}


#endif //SKIPLIST_NODE_H
