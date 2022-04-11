#include <bits/stdc++.h>
using namespace std;

class node
{
public:
  node(int);
  virtual ~node();

  void setChild(vector<node *>);

  bool searchVal(int);
  bool insertVal(int);

  int cap;
  vector<int> vec;
  vector<node *> child;
  node *parent;
};
//
node::node(int incap)
{
  parent = nullptr;
  cap = incap;
  child.push_back(nullptr);
}
//
node::~node()
{
}
//
bool node::searchVal(int val)
{
  for (auto i : vec)
    if (i == val)
      return true;
  return false;
}
//
bool node::insertVal(int val)
{
  vec.push_back(val);
  sort(vec.begin(), vec.end());
  child.push_back(nullptr);
  return true;
}
//

//////////////////////////////////////////////////////////

class bptree
{
public:
  bptree(int, int);
  virtual ~bptree();

  bool searchVal(int);
  bool addData(int);
  node *addIndex(int, node *, node *);
  void status();

  node *getNode(int, node *);

private:
  bool searchInTree(int, node *);
  node *root;
  int t, d, nDataNodes, nIndexNodes;
};
//
bptree::bptree(int iSizeIn, int dSizeIn)
{
  root = nullptr;
  t = iSizeIn;
  d = dSizeIn;
  nDataNodes = 0;
  nIndexNodes = 0;
}
//
bptree::~bptree()
{
}
//
bool bptree::searchVal(int val)
{
  return searchInTree(val, root);
}
//
bool bptree::searchInTree(int val, node *nodePtr)
{
  vector<int> values(nodePtr->vec);
  vector<node *> child(nodePtr->child);
  // node is a data node
  if (nodePtr->cap == d)
    return nodePtr->searchVal(val);
  // get correct pointer to child and search in that node
  int i;
  for (i = 0; i < values.size(); i++)
    if (values[i] > val)
      return searchInTree(val, child[i]);
  // when given value is bigger than values in index
  return searchInTree(val, child[i]);
}
//
node *bptree::getNode(int val, node *nodePtr)
{
  vector<int> values(nodePtr->vec);
  vector<node *> child(nodePtr->child);
  // node is a data node
  if (nodePtr->cap == d)
    return nodePtr;
  // get correct pointer to child and search in that node
  int i;
  for (i = 0; i < values.size(); i++)
    if (values[i] > val)
      return getNode(val, child[i]);
  // when given value is bigger than values in index
  return getNode(val, child[i]);
}

node *bptree::addIndex(int val, node *nodePtr, node *c)
{
  cout << "t";
  // parent dont exist
  if (nodePtr == nullptr)
  {
    nodePtr = new node(t);
    nIndexNodes++;
    root = nodePtr;
    cout << "y";
    return addIndex(val, nodePtr, c);
  }
  for (int i = 0; i < (nodePtr->child).size(); i++)
  {
    if (nodePtr->child[i] == c)
    {
      if (i == nodePtr->child.size() - 1)
      {
        nodePtr->vec.push_back(val);
        nodePtr->child.push_back(nullptr);
      }
      else
      {
        nodePtr->vec.insert(nodePtr->vec.begin() + i, val);
        nodePtr->child.insert(nodePtr->child.begin() + i + 1, nullptr);
      }
      // general case
      if (nodePtr->vec.size() <= 2 * t + 1)
      {
        return nodePtr->child[i + 1];
      }
      break;
    }
  }

  // parent is full
  node *newnode = new node(t);
  nIndexNodes++;
  for (int i = t + 1; i < 2 * t + 2; i++)
  {
    newnode->vec.push_back((nodePtr->vec)[i]);
    newnode->child.push_back(nodePtr->child[i + 1]);
  }
  newnode->child.push_back(nodePtr->child[2 * t + 2]);
  nodePtr->child.resize(t + 1);
  // pushing split upwards
  newnode->parent = addIndex(nodePtr->vec[t], nodePtr->parent, nodePtr);
  nodePtr->vec.resize(t);
  for (int i = 0; i < nodePtr->child.size(); i++)
  {
    if (nodePtr->child[i] == c)
    {
      if (i == nodePtr->child.size() - 1)
      {
        return newnode->child[0];
      }
      return nodePtr->child[i + 1];
    }
  }
  for (int i = 0; i < newnode->child.size(); i++)
  {
    if (newnode->child[i] == c)
    {
      return newnode->child[i + 1];
    }
  }
  return nullptr;
}

bool bptree::addData(int val)
{
  node *nodePtr = root;
  // when no data is added in tree
  if (nodePtr == nullptr)
  {
    nodePtr = new node(d);
    nDataNodes++;
    root = nodePtr;
    nodePtr->insertVal(val);
    return true;
  }

  nodePtr = getNode(val, root);
  nodePtr->insertVal(val);
  // when there is space in data node
  if ((nodePtr->vec).size() <= d * 2)
  {
    return true;
  }

  // no space then,split
  node *newnode = new node(d);
  nDataNodes++;
  cout << "n";
  for (int i = d; i < nodePtr->vec.size(); i++)
  {
    cout << 'f';
    newnode->insertVal((nodePtr->vec)[i]);
  }
  nodePtr->vec.resize(d);
  nodePtr->child.resize(d + 1);

  // pushing split upwards
  newnode->parent = addIndex(newnode->vec[0], nodePtr->parent, nodePtr);
  return true;
}
//
void bptree::status()
{
  cout << nIndexNodes << " " << nDataNodes;
  for (auto i : root->vec)
    cout << " " << i;
  cout << endl;
}

//////////////////////////////////////////////////////////

int main()
{
  int d, t, opcode, val;
  cin >> d >> t >> opcode;
  bptree tree(t, d);
  while (opcode != 3)
  {
    if (opcode == 1)
    {
      cin >> val;
      tree.addData(val);
    }
    else
    {
      tree.status();
    }
    cin >> opcode;
  }
  return 0;
}