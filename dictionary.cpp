#include <stack>
#include <iostream>
#include "dictionary.h"
#include "exception.h"

using namespace std;

//AVL_node class
//empty
template<class TKey, class TValue>
AVL_node<TKey, TValue>::AVL_node()
{
  key = TKey();
  value = TValue();
  left = nullptr;
  right = nullptr;
}

//key value node
template <class TKey, class TValue>
AVL_node<TKey, TValue>::AVL_node(const TKey &tkey, const TValue &tvalue)
{
  key = tkey;
  value = tvalue;
  left = nullptr;
  right = nullptr;
}

//copy
template <class TKey, class TValue>
AVL_node<TKey, TValue>::AVL_node(const AVL_node &node)
{
  key = node.key;
  value = node.value;
  left = nullptr;
  right = nullptr;
}

//destructor
template< class TKey, class TValue>
AVL_node<TKey, TValue>::~AVL_node()
{
  if(left) delete left;
  if(right) delete right;
}

//AVL_tree class
//empty
template<class TKey, class TValue>
AVL_tree<TKey, TValue>::AVL_tree()
{
  this->head = nullptr;
}

//copying tree from a node
template <class TKey, class TValue>
AVL_tree<TKey, TValue>::AVL_tree(const AVL_node<TKey, TValue> &node)
{
  stack<AVL_node<TKey, TValue> *> st_node;
  st_node.push(&node);

  AVL_node<TKey, TValue> * nhead = new AVL_node<TKey, TValue>(node);

  while(!st_node.empty())
  {
    if(st_node.top()->left != nullptr)
    {
      AVL_node<TKey, TValue> * left_node = new AVL_node<TKey, TValue>(st_node.top()->left);
      nhead->left = left_node;
      st_node.push(st_node.top()->left);
      nhead = st_node.top();
    }
    else if(st_node.top()->right != nullptr)
    {
      AVL_node<TKey, TValue> * right_node = new AVL_node<TKey, TValue>(st_node.top()->right);
      nhead->right = right_node;
      st_node.push(st_node.top()->right);
      nhead = st_node.top();
    }
    else
    {
      st_node.pop();
      while(st_node.top()->right == nhead && !st_node.empty())
      {
        nhead = st_node.top();
        st_node.pop();
      }

      while(st_node.top()->left == nhead && st_node.top()->right == nullptr && !st_node.empty())
      {
        nhead = st_node.top();
        st_node.pop();
      }
      if(st_node.top()->right != nullptr)
      {
        AVL_node<TKey, TValue> * right_node = new AVL_node<TKey, TValue>(st_node.top()->right);
        nhead->right = right_node;
        st_node.push(st_node.top()->right);
        nhead = st_node.top();
      }
    }
  }
  this->head = &nhead;
}

//construct tree from node
template <class TKey, class TValue>
AVL_tree<TKey, TValue>::AVL_tree(const AVL_tree &tnode)
{
  AVL_tree<TKey, TValue> * tree_ptr = new AVL_tree<TKey, TValue>(tnode->head);
  this->head = tree_ptr->head;
}


template <class TKey, class TValue>
bool AVL_tree<TKey, TValue>::IsSet(const TKey &tkey) const
{
  if(this->empty())
    throw(KeyNotFound<TKey>(tkey));
  AVL_node<TKey, TValue> * cur = head;
  while(cur != nullptr)
  {
    if(cur->key != tkey)
    {
      if(tkey < cur->key)
        cur = cur->left;
      else
        cur = cur->right;
    }
    else
      break;
  }
  if(cur != nullptr)
    return true;
  else
    throw(KeyNotFound<TKey>(tkey));
}


template <class TKey, class TValue>
void AVL_tree<TKey, TValue>::Set(const TKey &tkey, const TValue &tvalue)
{
  //if empty
  if(this->empty())
  {
    AVL_node<TKey, TValue> * nhead = new AVL_node<TKey, TValue>(tkey, tvalue);
    this->head = nhead;
    return;
  }

  //if tkey element already exists
  try
  {
    if(this->IsSet(tkey))
      return;
  }
  catch(...){};

  //else
  AVL_node<TKey, TValue> * cur = this->head;
  AVL_node<TKey, TValue> * next;

  if(tkey < cur->key )
  {
    if(cur->left != nullptr)
      next = cur->left;
    else
    {
      AVL_node<TKey, TValue> * left_n = new AVL_node<TKey, TValue>(tkey, tvalue);
      cur->left = left_n;
      return;
    }
  }
  else
  {
    if(cur->right != nullptr)
      next = cur->right;
    else
    {
      AVL_node<TKey, TValue> * right_n = new AVL_node<TKey, TValue>(tkey, tvalue);
      cur->right = right_n;
      return;
    }
  }

  while(true)
  {
    //flags
    bool simple_insert = false,
         shift_left = false,
         shift_right = false;
    //type of tree calibration left-left = 0, left-right ...
    enum Calib_type { ll, lr, rr, rl };
    Calib_type calibration;


    if(cur->left != nullptr && cur->right != nullptr)
      simple_insert = true;

    if(tkey < next->key)
      if(next->left != nullptr)
        shift_left = true;
    if(tkey > next->key)
      if(next->right != nullptr)
        shift_right = true;

    //shift to next node without inserting
    //or simple insert
    //or calibrated insertion
    if(shift_left || shift_right)
    {
      cur = next;
      if(shift_left)
        next = next->left;
      if(shift_right)
        next = next->right;
      continue;
    }
    else
    {
      AVL_node<TKey, TValue> * nnode = new AVL_node<TKey, TValue>(tkey, tvalue);
      if(simple_insert)
      {
        if(tkey < next->key)
          next->left = nnode;
        else
          next->right = nnode;
        return;
      }
      else
      {
        if(cur->left == next)
        {
          if(tkey < next->key)
            calibration = ll;
          else
            calibration = lr;
        }
        else
        {
          if(tkey < next->key)
            calibration = rl;
          else
            calibration = rr;
        }

        TValue tmp_value;
        TKey tmp_key;
        switch(calibration)
        {
          case ll:
          {
            cur->right = nnode;

            tmp_key = cur->right->key;
            tmp_value = cur->right->value;

            cur->right->key = cur->key;
            cur->right->value = cur->value;

            cur->key = cur->left->key;
            cur->value = cur->left->value;

            cur->left->key = tmp_key;
            cur->left->value = tmp_value;
            return;
          }
          case lr:
          {
            cur->right = nnode;

            tmp_key = cur->right->key;
            tmp_value = cur->right->value;

            cur->right->key = cur->key;
            cur->right->value = cur->value;

            cur->key = tmp_key;
            cur->value = tmp_value;
            return;
          }
          case rl:
          {
            cur->left = nnode;

            tmp_key = cur->left->key;
            tmp_value = cur->left->value;

            cur->left->key = cur->key;
            cur->left->value = cur->value;

            cur->key = tmp_key;
            cur->value = tmp_value;
            return;
          }
          case rr:
          {
            cur->left = nnode;

            tmp_key = cur->left->key;
            tmp_value = cur->left->value;

            cur->left->key = cur->key;
            cur->left->value = cur->value;

            cur->key = cur->right->key;
            cur->value = cur->right->value;

            cur->right->key = tmp_key;
            cur->right->value = tmp_value;
            return;
          }
        }
      }
    }
  }
}


template <class TKey, class TValue>
const TValue& AVL_tree<TKey, TValue>::Get(const TKey &tkey) const
{
  AVL_node<TKey, TValue> * cur = head;
  while(cur != nullptr)
  {
    if(cur->key != tkey)
    {
      if(tkey < cur->key)
        cur = cur->left;
      else
        cur = cur->right;
    }
    else
      break;
  }
  if(cur != nullptr)
    return cur->value;
  else
    throw(KeyNotFound<TKey>(tkey));
}

//empty
template<class TKey, class TValue>
bool AVL_tree<TKey, TValue>::empty() const
{
  if(this->head == nullptr)
    return true;
  else
    return false;
}

//destructor
template< class TKey, class TValue>
AVL_tree<TKey, TValue>::~AVL_tree()
{
  if(head)
    delete head;
}


//std output overload
template <class TKey, class TValue>
void AVL_node<TKey, TValue>::print_from_node() const
{
  if(this)
  {
    if(this->left != nullptr)
      this->left->print_from_node();
    else
      std::cout << "<";
    std::cout << "[" << this->key << ";" << this->value << "]";
    if(this->right != nullptr)
      this->right->print_from_node();
    else
      std::cout << ">";
  }
}


template <class TKey, class TValue>
void AVL_tree<TKey, TValue>::print_tree() const
{
  if(head)
    this->head->print_from_node();
}
