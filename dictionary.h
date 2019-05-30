//task
template<class TKey, class TValue>
class Dictionary
{
public:
    virtual ~Dictionary() = default;

    virtual const TValue& Get(const TKey& key) const = 0;
    virtual void Set(const TKey& key, const TValue& value) = 0;
    virtual bool IsSet(const TKey& key) const = 0;
};

//AVL_node
template<class TKey, class TValue>
class AVL_node
{
public:
  AVL_node();
  AVL_node(const TKey&, const TValue&);
  AVL_node(const AVL_node&);

  void print_from_node() const;
  ~AVL_node();

  TKey key;
  TValue value;
  AVL_node * left, * right;
};

//AVL tree class
template<class TKey, class TValue>
class AVL_tree : public Dictionary<TKey, TValue>
{
public:
  AVL_tree();
  AVL_tree(const AVL_node<TKey, TValue>&);
  AVL_tree(const AVL_tree&);

  const TValue& Get(const TKey&) const;
  void Set(const TKey&, const TValue&);
  bool IsSet(const TKey&) const;

  void print_tree() const;
  bool empty() const;

  ~AVL_tree();
private:
  AVL_node<TKey, TValue> * head;
};
