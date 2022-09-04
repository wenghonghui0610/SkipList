#ifndef SKIPLIST_SKIPLIST_H
#define SKIPLIST_SKIPLIST_H

#include <mutex>
#include "Node.h"
#include "Random.h"

static std::mutex mtx;

template <typename K, typename V>
class SkipList {
public:
    SkipList();
    SkipList(int maxLevel);
    ~SkipList();

    void insert(K key, V val);
    Node<K, V>* search(K key) const;
    bool remove(K key);
    void displayList();

private:
    int maxLevel;
    int curLevel;
    Node<K, V>* header;
    int elementCount;

    int getRandomLevel();


};

template <typename K, typename V>
SkipList<K, V>::SkipList() {
    maxLevel = 32;
    curLevel = 0;
    elementCount = 0;

    K k;
    V v;
    header = new Node<K, V>(k, v, maxLevel);
}

template <typename K, typename V>
SkipList<K, V>::SkipList(int maxLevel) {
    maxLevel = maxLevel;
    curLevel = 0;
    elementCount = 0;

    K k;
    V v;
    header = new Node<K, V>(k, v, maxLevel);
}

template <typename K, typename V>
SkipList<K, V>::~SkipList() {
    delete header;
}

template <typename K, typename V>
Node<K, V>* SkipList<K, V>::search(K key) const{
    Node<K, V>* cur = header;

    // Start from the top left corner of the SkipList
    for(int i = curLevel; i >= 0; --i) {
        while(cur->forward[i] && cur->forward[i]->getKey() < key) {
            cur = cur->forward[i];
        }
    }

    // there are two cases for the next node in level 0
    // The first case is the key we need to find
    // The second case is larger than the key we find
    cur = cur->forward[0];
    if(cur != nullptr && cur->getKey() == key)
        return cur;
    else
        return nullptr;
}

template <typename K, typename V>
void SkipList<K, V>::insert(K key, V val){
    mtx.lock();

    Node<K, V>* cur = header;
    // create and initialize update array
    Node<K, V>** update = new Node<K, V>* [maxLevel + 1]();

    // search
    for(int i = curLevel; i >= 0; --i) {
        while(cur->forward[i] != nullptr && cur->forward[i]->getKey() < key) {
            cur = cur->forward[i];
        }
        update[i] = cur;
    }

    cur = cur->forward[0];

    // If the key value of the current node is the same as that of the node to be inserted,
    // it indicates that the node already exists, we can modify the value of the node
    if(cur != nullptr && cur->getKey() == key) {
        cur->setValue(val);
    }

    // If the current node is null means that the element need to be inserted behind the last node.
    // If the current key value is not the same as the key to be inserted,
    // it means that we should insert the node between update [0] and current
    else {
        int randomLevel = getRandomLevel();

        if(randomLevel > curLevel) {
            for(int i = curLevel + 1; i <= randomLevel; ++i) {
                update[i] = header;
            }
            curLevel = randomLevel;
        }

        // create new node
        Node<K, V>* insertNode = new Node<K, V>(key, val, randomLevel);

        // insert node
        for(int i = 0; i <= randomLevel; ++i) {
            insertNode->forward[i] = update[i]->forward[i];
            update[i]->forward[i] = insertNode;
        }
        ++elementCount;
    }
    mtx.unlock();
    return;
}

template <typename K, typename V>
bool SkipList<K, V>::remove(K key){
    mtx.lock();
    Node<K, V>** update = new Node<K, V>* [maxLevel + 1]();
    Node<K, V>* cur = header;
    for(int i = curLevel; i >= 0; --i) {
        while(cur->forward[i] != nullptr && cur->forward[i]->getKey() < key) {
            cur = cur->forward[i];
        }
        update[i] = cur;
    }
    cur = cur->forward[0];
    if(cur != nullptr && cur->getKey() == key) {
        for(int i = 0; i <= curLevel; ++i) {
            if(update[i]->forward[i] != cur) {
                break;
            }
            update[i]->forward[i] = cur->forward[i];
        }
        delete cur;

        while(curLevel > 0 && header->forward[curLevel] == nullptr) {
            --curLevel;
        }
        --elementCount;
        mtx.unlock();
        return true;
    }
    else {
        mtx.unlock();
        return false;
    }

}



// Insert given key and value in skip list
// return true means insert successfully
// return false means element exists



#endif //SKIPLIST_SKIPLIST_H
