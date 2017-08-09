#ifndef EXPRTREE_H
#define EXPRTREE_H

#include <queue>
#include <stack>
#include <vector>
#include <string>
#include <cstdlib> //This is include for the atoi() function used in the to_number(string) helper function.

#include "TreeNode.h"

using std::queue;
using std::stack;
using std::vector;
using std::string;

class ExprTree{

 private:

  TreeNode * root;
  int _size; 

 public:

  ExprTree();
  ExprTree(TreeNode *);
  ~ExprTree();
  static vector<string> tokenise(string);
  static ExprTree buildTree(vector<string>);
  static int evaluate(TreeNode *);
  int evaluateWholeTree();

  static string prefixOrder(const ExprTree &);
  static string infixOrder(const ExprTree &);
  static string postfixOrder(const ExprTree &);
  int size();
  bool isEmpty();
  TreeNode * getRoot();

};

#endif
