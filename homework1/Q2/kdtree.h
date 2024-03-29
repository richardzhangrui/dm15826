/* $Revision: 1.1 $ */
/* $Author: christos $ */
/* $Id: kdtree.h,v 1.1 2001/02/11 16:16:03 christos Exp $ */

#ifndef __kdtree_h
  #define __kdtree_h
  #include <stdio.h>
  #include <assert.h>
  #include "vector.h"
  #include "dfn.h"
  
  
  /* tree node */
  typedef struct TreeNode { 
     struct TreeNode *left;
     struct TreeNode *right;
     VECTOR *pvec;
     int fetched;
  } TREENODE;
  
  /* insert a vector into the tree, and return the new root */
  TREENODE *insert(TREENODE *subroot, VECTOR *vp);

  /* recursive insert, using the level info */
  TREENODE *rinsert(TREENODE *subroot, VECTOR *vp, int level);

  TREENODE *talloc();
  void tfree();

  /* prints the whole tree - for debugging purposes */
  void tprint(TREENODE *subroot);
  void rtprint(TREENODE *subroot, int level);

  /* returns the 'count' nearest neighbors in the subtree */
  void nnsearch(TREENODE *subroot, VECTOR *vp, int count);
  void rnnsearch(TREENODE *subroot, VECTOR *vp, int level, int n,
                 VECTOR **bestn, NUMBER *min_nd, int *max_id, int *cnt);

  void rangesearch(TREENODE *subroot, VECTOR *vpLow, VECTOR *vpHigh);
  void rrangesearch(TREENODE *subroot, VECTOR *vpLow, VECTOR *vpHigh, int level);

   BOOLEAN contains( VECTOR *vpLow, VECTOR *vpHigh, VECTOR *vp);

   NUMBER myvecdist2( VECTOR *vp1, VECTOR *vp2);

#endif
