#include "ExprTree.h"
#include <sstream>

/*
 * Helper function that tests whether a char is a number.
 */
bool isdigit(const char & c){

  switch (c) {
  case '0' :
  case '1' :
  case '2' :
  case '3' :
  case '4' :
  case '5' :
  case '6' :
  case '7' :
  case '8' :
  case '9' : return true;
  }

  return false;
   
}

/*
 * Helper function that tests whether a string is a non-negative integer.
 */
bool is_number(const std::string & s)
{
    std::string::const_iterator it = s.begin();
    while (it != s.end() && isdigit(*it)) ++it;
    return !s.empty() && it == s.end();
}

/*
 * Helper function that converts a string to an int.
 */
int to_number(const std::string & s){
  return atoi(s.c_str());
}

/*
 * Helper function that converts a number to a string.
 */
string to_string(const int & n){
  std::stringstream stream;
  stream << n;
  return stream.str();
}

/*
 * Helper function that creates a TreeNode with the appropriate operator
 * when given a string that's "+", "-", "*" or "/". If the string is wrong
 * it gives a NoOp value.
 */
TreeNode * createOperatorNode(const string & op){

  if (op == "+") return new TreeNode(Plus);
  if (op == "-") return new TreeNode(Minus);
  if (op == "*") return new TreeNode(Times);
  if (op == "/") return new TreeNode(Divide);
  return new TreeNode(NoOp);

}

/*
 * Helper function that recursively clean up a tree from given TreeNode.
 */
void deleteTree(TreeNode * treeNode){
    
    	if(treeNode != NULL){
		
		deleteTree(treeNode->getLeftChild());
		deleteTree(treeNode->getRightChild());
		delete treeNode;
		
	}
}

/*
 * Helper function that return the size of a tree.
 */
int calTreeSize(TreeNode * treeNode){

	if(treeNode == NULL)
		return 0;
	return (calTreeSize(treeNode->getLeftChild()) + calTreeSize(treeNode->getRightChild()) + 1);
}

/*
 * Helper function that takes given TreeNode node, returns a string that represents 
 * that same expression as the tree in prefix notation.
 */
string prefix(TreeNode * node){
	
	if(node == NULL){ return "";}
	if(node->isValue()){ return node->toString();}
	return node->toString() + " " + prefix(node->getLeftChild()) + " " +prefix(node->getRightChild());
}

/*
 * Helper function that takes given TreeNode node, returns a string that represents 
 * that same expression as the tree in infix notation.
 */
string infix(TreeNode * node){

	if(node == NULL){return "";}
	if(node->isValue()){ return node->toString();}
	return infix(node->getLeftChild()) + " " + node->toString() + " " + infix(node->getRightChild());
}

/*
 * Helper function that takes given TreeNode node, returns a string that represents 
 * that same expression as the tree in postfix notation.
 */
string postfix(TreeNode * node){

	if(node == NULL){return "";}
    if(node->isValue()){ return node->toString();}
	return postfix(node->getLeftChild()) + " " + postfix(node->getRightChild()) + " " + node->toString();
}

/*
 * Helper function that sets and returns weight of a operator.
 */
int getWeight(std::string op){
	
	int weight = -1;
	if(op == "+" || op == "-"){
		weight = 1;
	}else if(op == "*" || op == "/"){
		weight = 2;	
	}else if(op == "#"){
		weight = 3;
	}
	
	return weight;
}

/*
 * Helper function that tests whether a char is a operator or "(" ,")".
 */
bool isOperator(const char & c){

  switch (c) {
  case '+' :
  case '-' :
  case '*' :
  case '/' :
  case '(' :
  case ')' : return true;
  }

  return false;
   
}

/*
 * Helper function that tests whether a string is a opreator.
 */
bool isAOperator(std::string s){

	if(s=="+" || s=="-" || s=="*" || s=="/" || s=="#") {
		return true;
 }

 	return false;
}

/*
 * Helper function that check wether a operator is right associative or not. 
 */
bool isRightAssociative(std::string  op){
	 
	if(op == "#")
		return true;   // Since there is no # in our input string, this will always return false.
	return false;
}

/*
 * Helper function that check if the first operator has higher precedence than the second one.
 */
bool hasHigherPrecedence(std::string  op1, std::string op2){
	
	int op1Weight = getWeight(op1);
	int op2Weight = getWeight(op2);

	if(op1Weight == op2Weight){
	
		if(isRightAssociative(op1))
			return false;                // When op1 and op2 have the same weight, op1 has higer precedence.
		return true;
	}

	return op1Weight > op2Weight;

}

/*
 * Helper function that converts a Infix vector of string to to a Postfix vector of string.
 */
vector<string> infixToPostfix(vector<string> tokens){
  
  stack<string> temp;
  vector<string> postfix;

  for( int i = 0; i < tokens.size(); i++){

	  if(isAOperator(tokens[i])){

		  while(!temp.empty() && temp.top() != "(" && hasHigherPrecedence(temp.top(), tokens[i])){
			
			postfix.push_back(temp.top());   // If operator in satck has higher precedence, add string
			temp.pop();                      // in stack to result until reach a "(".
		  }
		  
		  temp.push(tokens[i]);

	  }else if(is_number(tokens[i])){
		  
		  postfix.push_back(tokens[i]);      

	  }else if(tokens[i] == "("){
		  
		  temp.push(tokens[i]);

	  }else if(tokens[i] == ")"){
		
		  while(!temp.empty() && temp.top() != "("){
			  
			  	postfix.push_back(temp.top());  // If it's a ), add strng in stack to result until reach a (.
				temp.pop();
			  }

			  temp.pop();
	  }
  }

  while(!temp.empty()){

	postfix.push_back(temp.top());  // Add anyting left in temp to result.
	temp.pop();
  }
  
  return postfix;
}

/*
 * Basic constructor that sets up an empty Expr Tree.
 */
ExprTree::ExprTree(){
	
	root = 0;
	_size = 0;
  
}

/*
 * Constructor that takes a TreeNode and sets up an ExprTree with that node at the root.
 */
ExprTree::ExprTree(TreeNode * r){

	root = r;
	_size = calTreeSize(r);

}

/*
 * Destructor to clean up the tree.
 */
ExprTree::~ExprTree(){
	
	deleteTree(root);
}

/*
 * This function takes a string representing an arithmetic expression and breaks
 * it up into components (number, operators, parentheses).
 * It returns the broken up expression as a vector of strings.
 */
vector<string> ExprTree::tokenise(string expression){

	vector<string> tokens;  // Stores the result.
	queue<char> buffer;    // Temporaryly stores the char read in.
	string element;       // Stores string going to add to the result.

	buffer.push(expression[0]); // Adds the first char into buffer

	for(int i = 1; i < expression.size(); i++){
		
		if(expression[i] == ' '){
		
			continue;          // If the char is a space, go to the next char.

		}else if(isdigit(buffer.back()) && isdigit(expression[i])){
		
			buffer.push(expression[i]);   // If the buffer stores a number and the char read in is a number, add the char to buffer.

		}else if((isdigit(buffer.back()) && isOperator(expression[i])) || (isOperator(buffer.back()) && isdigit(expression[i]))){
			
			while(!buffer.empty()){
				
				element += buffer.front();     // If the type in buffer is different from the char read in, take all the things from buffer,
				buffer.pop();                 //  and make a string with them, then add the string to the result. Add the current char to buffer.
			}

			tokens.push_back(element);
			element = "";
			buffer.push(expression[i]);

		}else if(isOperator(buffer.back()) && isOperator(expression[i])){
			
			tokens.push_back(string(1,buffer.front()));
			buffer.pop();                        // If the one in buffer and current char are both operators, take out and add the one in buffer to result.
			buffer.push(expression[i]);         //  And add the current one to the buffer.
		}
	}

	while(!buffer.empty()){
	
		element += buffer.front();        // Add anything left in buffer to a string then add it to result.
		buffer.pop();
	}

	tokens.push_back(element);
	return tokens;

}

/*
 * This function takes a vector of strings representing an expression (as produced
 * by tokenise(string), and builds an ExprTree representing the same expression.
 */
ExprTree ExprTree::buildTree(vector<string> tokens){   

	vector<string> postfix = infixToPostfix(tokens);   // Changes the vector of strings from infix to postfix.
	stack<TreeNode *> nodeStack;
	TreeNode * node;

	for(int i = 0; i < postfix.size(); i++){
		
		if(isAOperator(postfix[i])){
		
			node = createOperatorNode(postfix[i]);    // If it's a operator, create a TreeNode with it.
			
			node->setRightChild(nodeStack.top());
			nodeStack.pop();                         // Set top node of stack as its right child, pop the top node.
			node->setLeftChild(nodeStack.top());      //  Set top node of stack as its left child, pop the top node.
			nodeStack.pop();
			nodeStack.push(node);                    //  Push the newly create node to the stack.
		}else if(is_number(postfix[i])){
		
			int value = to_number(postfix[i]); 
			node = new TreeNode(value);             // If it's a number, create a treeNode with it then push it to stack.
			nodeStack.push(node);
		}
	}

	node = nodeStack.top();
	return ExprTree(node);       // Use top node on stack as a root node to create a new tree.
}

/*
 * This function takes a TreeNode and does the maths to calculate
 * the value of the expression it represents.
 */
int ExprTree::evaluate(TreeNode * n){
	
	// if it is a leaf node, return its value.
	if(n->isValue()){
	
	return n->getValue();
	
	}else if(n->isOperator()){
    
	// If it is a operator, recursively evaluate its children.
	if(n->toString() == "+"){
		
		return evaluate(n->getLeftChild()) + evaluate(n->getRightChild());
			
	}else if(n->toString() == "-"){
		
		return evaluate(n->getLeftChild()) - evaluate(n->getRightChild());
		
	}else if(n->toString() == "*"){
		
		return evaluate(n->getLeftChild()) * evaluate(n->getRightChild());
		 
	}else if(n->toString() == "/"){
	    
	    return evaluate(n->getLeftChild()) / evaluate(n->getRightChild());
	     
    }else{
	
	return -1;
	
	}
 }
}

/*
 * When called on an ExprTree, this function calculates the value of the
 * expression represented by the whole tree.
 */
int ExprTree::evaluateWholeTree(){
    
	if(!isEmpty()){
	
		return evaluate(root);
  }

	return 0;
}

/*
 * Given an ExprTree t, this function returns a string
 * that represents that same expression as the tree in
 * prefix notation.
 */
string ExprTree::prefixOrder(const ExprTree & t){

	return prefix(t.root); 
	
}

/*
 * Given an ExprTree t, this function returns a string
 * that represents that same expression as the tree in
 * infix notation.
 */
string ExprTree::infixOrder(const ExprTree & t){

	return infix(t.root);
}

/*
 * Given an ExprTree t, this function returns a string
 * that represents that same expression as the tree in
 * postfix notation.
 */
string ExprTree::postfixOrder(const ExprTree & t){

	return postfix(t.root);
}

/*
 * Returns the size of the tree. (i.e. the number of nodes in it)
 */
int ExprTree::size(){ return _size; }

/*
 * Returns true if the tree contains no nodes. False otherwise.
 */
bool ExprTree::isEmpty(){ return _size == 0; }

/*
 * Returns tree root node. 
 */
TreeNode * ExprTree::getRoot(){ return root; }
