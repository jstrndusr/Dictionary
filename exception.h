template<class TKey>
class NotFoundException : public std::exception
{
public:
    virtual const TKey& GetKey() const noexcept = 0;
};

template<class TKey>
class KeyNotFound : public NotFoundException<TKey>
{
public:
  KeyNotFound(const TKey &tk) : k(tk) {}
  ~KeyNotFound() {}

  const TKey& GetKey() const noexcept { return k; }
private:
  TKey k;
};
