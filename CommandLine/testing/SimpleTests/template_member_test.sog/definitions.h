// A class template on a definitions page. The parser must build a real
// instantiation for map<K, V> so the semantic annotator can classify member
// accesses on variables of the instantiated type.
template<typename K, typename V>
struct map {
  int count;
  V slot;
  int size() { return count; }
  V& at(K) { return slot; }
  V& operator[](K) { ++count; return slot; }
};

// Defaulted parameters, including one taking a value: a bare `ring` fills
// only K, and C++ supplies the rest.
template<typename K, typename V = double, int Capacity = 4>
struct ring {
  V slot;
  V& operator[](K) { return slot; }
  int operator()() { return sizeof(V) * 100 + Capacity; }
};
