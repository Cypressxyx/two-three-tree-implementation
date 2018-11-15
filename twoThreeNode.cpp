
/** A Index Generator implemented using a two-three b-tree **/

#include <iostream>
#include <string>
#include <fstream>
#include <ctime>
#include <iomanip>

struct word {
  std::string value, lineOccurences;
};

class twoThreeNode { 
  public:
    twoThreeNode(word *key): leftNode(nullptr), rightNode(nullptr), leftVal(key), centerNode(nullptr), rightVal(nullptr), height(0) {}

    /** Accessors **/
    twoThreeNode *insertVal(twoThreeNode *, word *);
    twoThreeNode *add(twoThreeNode *);
    word *find(twoThreeNode *, std::string );
    int getNumNodes(twoThreeNode *);
    int getHeight(twoThreeNode *);

    /** Adders **/
    void setLeftNode(twoThreeNode *node)    { leftNode = node;    }
    void setRightNode(twoThreeNode *node)   { rightNode = node;   }
    void setCenterNode(twoThreeNode *node)  { centerNode = node;  }

    /** Getters **/
    bool isLeaf()                  { return leftNode == nullptr; }
    twoThreeNode *leftChild()      { return leftNode;     }
    twoThreeNode *rightChild()     { return rightNode;    }
    twoThreeNode *centerChild()    { return centerNode;   }
    word  *getLeftVal()            { return leftVal;      }
    word  *getRightVal()           { return rightVal;     }

    /** Pre Order Traversal Printer **/
    void printVals(twoThreeNode *,std::ostream &);

  private:
    twoThreeNode *leftNode, *rightNode, *centerNode;
    word *leftVal, *rightVal;
    int  height;
};

int twoThreeNode::getNumNodes(twoThreeNode *node) {
   if(node == nullptr) 
     return 0;
   return 1 + getNumNodes(node->leftChild()) + getNumNodes(node->centerChild()) + getNumNodes(node->rightChild());
}

int twoThreeNode::getHeight(twoThreeNode *node) {
   if(node == nullptr)
	return 0;
   return 1 + getHeight(node->leftChild());
}

void twoThreeNode::printVals(twoThreeNode *node, std::ostream &ostr) {
  if(node == nullptr )
    return ;
  printVals(node->leftChild(), ostr);
 
  if(node->getLeftVal() != nullptr) {
    word *leftWord = node->getLeftVal();
    ostr << std::setw(12) << leftWord->value << leftWord->lineOccurences << std::endl;
   }
 
  printVals(node->centerChild(),ostr);

  if(node->getRightVal() != nullptr) {
    word *rightWord = node->getRightVal();
    ostr << std::setw(12) << rightWord->value  << rightWord->lineOccurences << std::setw(12) << std::endl;
  }

  printVals(node->rightChild(),ostr);
}

twoThreeNode *twoThreeNode::insertVal(twoThreeNode *node, word *val) {

  if(node == nullptr)
    return new twoThreeNode(val);

  if(node->isLeaf()) { //no children
    twoThreeNode *newNode = new twoThreeNode(val);
    return node->add(newNode);
  }

   /** Value must be added to one of the child Nodes  **/
   word *leftWord = node->getLeftVal();
   word *rightWord = node->getRightVal();  

   if(val->value < leftWord->value) { 
     twoThreeNode *newNode = insertVal(node->leftChild(), val);
      if ( newNode->getLeftVal() == (node->leftChild())->getLeftVal() && newNode->getRightVal() == (node->leftChild())->getRightVal()) {
        return node; 
       }
     return node->add(newNode);
   } 

  else if( node->getRightVal() == nullptr || val->value < rightWord->value) { //traverse through the center
    twoThreeNode *newNode = insertVal(node->centerChild(),val);
    if( newNode == node->centerChild()) 
      return node;
    return node->add(newNode);
  }
   
  else { //insert on a right node
    twoThreeNode *newNode = insertVal(node->rightChild(),val);
    
      if ( newNode->getLeftVal() == (node->rightChild())->getLeftVal() && newNode->getRightVal() == (node->rightChild())->getRightVal()) 
        return node; 
    return node->add(newNode);
   }
  

  return nullptr; 
}

twoThreeNode *twoThreeNode::add(twoThreeNode *node) {

  if(getLeftVal() == nullptr ) //empty tree
    return node;

  else if(rightVal == nullptr ) {  //only left is present, add right value
    if((node->getLeftVal())->value < (getLeftVal())->value) {
      rightVal = getLeftVal();
      leftVal  = node->getLeftVal();

      this->rightNode = this->centerNode;
      this->leftNode = node->leftChild();
      this->centerNode = node->centerChild();
    }

    else { 
      rightVal = node->getLeftVal();
      centerNode = node->leftChild();
      rightNode = node->centerChild();
    }  
    return this;
  }

  else if((node->getLeftVal())->value < (getLeftVal())->value) { //add as a left child

    twoThreeNode *newNode = new twoThreeNode(getLeftVal());
    newNode->setLeftNode(node);
    newNode->setCenterNode(this);

    leftNode = centerNode;
    centerNode = rightNode;
    rightNode = nullptr;

    leftVal = rightVal;
    rightVal = nullptr;
	
    return newNode;
  }

  else if((node->getLeftVal())->value < (getRightVal())->value) { //add as a center child
    twoThreeNode *newNode = new twoThreeNode(getRightVal());
    newNode->setLeftNode(node->centerChild());
    newNode->setCenterNode(rightNode);
    node->setCenterNode(newNode);
    node->setLeftNode(this);
    rightVal = nullptr;
    rightNode = nullptr;
    return node;
  
  }
    
  else { //add as a right child

    twoThreeNode *newNode = new twoThreeNode(getRightVal());
    newNode->setLeftNode(this);
    newNode->setCenterNode(node);

    rightVal = nullptr;
    rightNode = nullptr;


    return newNode;
  }
  return nullptr; 
}

word *twoThreeNode::find(twoThreeNode *node, std::string val) {
  if(node == nullptr) 
    return nullptr;

  if( node->getLeftVal() != nullptr && (node->getLeftVal())->value == val )
    return node->getLeftVal();

  if( node->getRightVal() != nullptr && (node->getRightVal())->value == val )
    return node->getRightVal();

  if( node->getLeftVal() != nullptr && val < (node->getLeftVal())->value ) //search left
    return find(node->leftChild(), val);

  else if ( node->getRightVal() == nullptr || val < (node->getRightVal())->value) //search center
    return find(node->centerChild(), val);

  else  //search right
    return find(node->rightChild(), val);
}

int main()
{
   twoThreeNode *node = new twoThreeNode();
   word *w = new word();
   word->value = "hi";
   node = node->insertVal(word,node);
   printVals(word);

}


