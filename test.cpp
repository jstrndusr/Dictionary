#include <iostream>
#include <string>

#include "dictionary.cpp"

using namespace std;

int main()
{
  AVL_node<int, char> * my_node = new AVL_node<int, char>();
  AVL_tree<int, char> * my_tree = new AVL_tree<int, char>();
  Dictionary<int, char> * my_dtree = new AVL_tree<int, char>();
  AVL_tree<string, char> * str_ch = new AVL_tree<string, char>();

  try
  {
    //test left-left tree rebalance
    my_tree->Set(10,'a');
    my_tree->print_tree(); cout << "\n";
    my_tree->Set(9,'b');
    my_tree->print_tree(); cout << "\n";
    my_tree->Set(8,'c');
    my_tree->print_tree(); cout << "\n";
    //test left-right tree rebalance
    my_tree->Set(6,'e');
    my_tree->print_tree(); cout << "\n";
    my_tree->Set(7,'a');
    my_tree->print_tree(); cout << "\n";
    //test same key insertion
    my_tree->Set(12,'d');
    my_tree->print_tree(); cout << "\n";
    my_tree->Set(12,'a');
    my_tree->print_tree(); cout << "\n";
    //test same value insertion
    my_tree->Set(15,'d');
    my_tree->print_tree(); cout << "\n";

    //test less string
    str_ch->Set("hello", 'a');
    str_ch->Set("world", 'b');
    str_ch->Set("abcde", 'c');
    str_ch->print_tree(); cout << "\n";
  }
  catch(KeyNotFound<int> &e)
  {
    //cout << "Key not found :" << e.GetKey() << endl;
  }

  delete my_tree;
  return 0;
}
