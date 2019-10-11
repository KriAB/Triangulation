#ifndef QUADTREE_H
#define QUADTREE_H
#include "vector3d.h"
struct Node
{
   gsl::Vector3D pos;
   int data; //Entity ID
   Node(gsl::Vector3D mPos, int mData)
   {
       pos = mPos;
       data = mData;
   }
   Node()
   {
       data = 0;
   }
};

class QuadTree
{
    //Details of the boundry of this node
    gsl::Vector3D topLeft;
    gsl::Vector3D botRight;
    //Details of node
    Node *mNode;
    //Children of this tree
    QuadTree *topLeftTree;
    QuadTree *topRightTree;
    QuadTree *botLeftTree;
    QuadTree *botRightTree;
public:
    QuadTree()
    {
        topLeft = gsl::Vector3D(0,0,0);
        botRight = gsl::Vector3D(0,0,0);
        mNode = nullptr;
        topLeftTree = nullptr;
        topRightTree = nullptr;
        botLeftTree = nullptr;
        botRightTree = nullptr;
    }
    QuadTree (gsl::Vector3D topL, gsl::Vector3D botR)
    {
        mNode = nullptr;
        topLeftTree = nullptr;
        topRightTree = nullptr;
        botLeftTree = nullptr;
        botRightTree = nullptr;
        topLeft = topL;
        botRight = botR;
    }
    void insert(Node*);
    Node* search(gsl::Vector3D);
    bool inBoundary(gsl::Vector3D);


};

#endif // QUADTREE_H
