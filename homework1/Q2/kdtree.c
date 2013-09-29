/* $Revision: 2.1 $ */
/* $Author: christos $ */
/* $Id: kdtree.c,v 2.1 2001/02/11 16:22:16 christos Exp Locker: christos $ */

#include "kdtree.h"
#include "vector.h"

/* inserts a vector into the tree, and returns the new root */

TREENODE *insert(TREENODE *subroot, VECTOR *vp){
    #ifdef DEBUG
    printf("tree insert was called with \n");
    vecprint(vp);
    #endif

    return( rinsert(subroot, vp, 0) );
}

TREENODE *rinsert(TREENODE *subroot, VECTOR *vp, int level){

    if(subroot == NULL){ /* we hit the bottom of the tree */
	subroot = talloc();
	subroot->pvec  = veccopy(vp);
	subroot->fetched = 0;
	return(subroot);
    }

    if( (vp->vec)[level] <= ((subroot->pvec)->vec)[level] ){
	subroot->left = rinsert(subroot->left, vp, (level+1) % (vp->len) );
    }else{
	subroot->right = rinsert(subroot->right, vp, (level+1) % (vp->len) );
    }
	
    return(subroot); /* although it didn't change */
}

void tprint(TREENODE *subroot){
    rtprint(subroot, 0);
}
void rtprint(TREENODE *subroot, int level){
    int i;

    if( subroot != NULL ){
	rtprint(subroot->left, level+1);
	for(i=0; i<level;i++){
	    printf("\t");
	}
	vecprint(subroot->pvec);
	rtprint(subroot->right, level+1);
    }
}

TREENODE *talloc(){
    TREENODE *tp;

    tp = (TREENODE *) malloc( sizeof(TREENODE) );
    assert( tp != NULL);
    tp->left = NULL; /* added 2/7/97, to avoid runtime errors */
    tp->right = NULL; /* added 2/7/97, to avoid runtime errors */
    return(tp);
}

void tfree(TREENODE *tp){
    if(tp != NULL ){
	tfree(tp->left);
	tfree(tp->right);
	vecfree(tp->pvec);
	free( (char *) tp);
    }
}

void merge(VECTOR **bestn, NUMBER *bestn_d, VECTOR **leftn, NUMBER *leftn_d, 
		VECTOR **rightn, NUMBER *rightn_d, int middle, int end) {
	int i = 0; int j = 0; int k = 0;
	while(i < middle && j < end){
		if(leftn_d[i] < rightn_d[j]) {
			bestn[k] = leftn[i];
			bestn_d[k] = leftn_d[i];
			i++;
		}
		else {
			bestn[k] = rightn[j];
			bestn_d[k] = rightn_d[j];
			j++;
		}
		k++;
	}	
	while(i < middle) {
		bestn[k] = leftn[i];
		bestn_d[k] = leftn_d[i];
		i++;
		k++;
	}
	while(j < end) {
		bestn[k] = rightn[j];
		bestn_d[k] = rightn_d[j];
		j++;
		k++;
	}
}

void mergeSort(VECTOR **bestn, NUMBER *bestn_d, int end) {
	if (end > 1) {
		int middle = end/2;
		int i = 0,j = middle;
		VECTOR **leftn = (VECTOR**)malloc(sizeof(VECTOR**)*middle);
		NUMBER *leftn_d = (NUMBER*)malloc(sizeof(NUMBER) * middle);
		VECTOR **rightn = (VECTOR**)malloc(sizeof(VECTOR**)*(end-middle));
		NUMBER *rightn_d = (NUMBER*)malloc(sizeof(NUMBER) * (end-middle));
		while(i < middle) {
			leftn[i] = bestn[i];
			leftn_d[i] = bestn_d[i];
			i++;
		}
		while(j < end) {
			rightn[j - middle] = bestn[j];
			rightn_d[j - middle] = bestn_d[j];
			j++;
		}
		
		mergeSort(leftn, leftn_d, middle); 
		mergeSort(rightn, rightn_d, end - middle); 
		merge(bestn, bestn_d, leftn, leftn_d, rightn, rightn_d, middle, end - middle);
		free(leftn);
		free(leftn_d);
		free(rightn);
		free(rightn_d);
	} 	
}

void getNNode(TREENODE *subroot, VECTOR *vp, int count,
		VECTOR **bestn, NUMBER *min_nd, int *max_id, int *cnt){
	if ((*cnt) < count && subroot) {
		(*cnt)++;
		if ((*cnt) <= count ) {
			bestn[*cnt-1] = subroot->pvec;	
			subroot->fetched = 1;
			if (*cnt == 1) {
				*max_id = 0;
				*min_nd = myvecdist2(vp, bestn[*cnt-1]);
			}
			else {
				if ((*min_nd) < myvecdist2(vp, bestn[*cnt-1])) {
					*max_id = *cnt-1;
					*min_nd = myvecdist2(vp, bestn[*cnt-1]);
				}
			}
		}
		getNNode(subroot->left, vp, count, bestn, min_nd, max_id, cnt);
		getNNode(subroot->right, vp, count, bestn, min_nd, max_id, cnt);
	}
}

void nnsearch(TREENODE *subroot, VECTOR *vp, int count){

    VECTOR **bestn = (VECTOR**)malloc(sizeof(VECTOR*) * count);
    NUMBER *bestn_d = (NUMBER*)malloc(sizeof(NUMBER) * count);
    NUMBER min_nd = HUGE;
    int max_id = 0;
    int cnt = 0;
    int i = 0;

    #ifdef DEBUG
    printf("nn tree search was called with \n");
    vecprint(vp);
    #endif


    /* fetch count tree nodes first in case that there're less than count nodes */    
    getNNode(subroot, vp, count, bestn, &min_nd, &max_id, &cnt);

    rnnsearch(subroot, vp, 0, count, 
		bestn, &min_nd, &max_id, &cnt);

    while(i < cnt) {
	bestn_d[i] = myvecdist2(vp, bestn[i]);
	i++;
    }

    if (cnt == 0) {
	printf("empty tree\n");
    }else if(cnt < count) {
	printf("Only %d nodes found\n",cnt);
    }
    
    mergeSort(bestn, bestn_d, cnt);
    
    printf("nearest %d neighbors:\n",cnt);
    for(i = 0; i < cnt; i++){
	vecprint(bestn[i]);
	printf("distance: %f\n", bestn_d[i]);
    }


}
void rnnsearch(TREENODE *subroot, VECTOR *vp, int level, int n, 
		VECTOR **bestn, NUMBER *min_nd, int *max_id, int *cnt){

    NUMBER rootdist;  /* the distance from the root */
    int numdims;

    numdims = vp->len;

    if( subroot == NULL){ return; } /* empty tree */

    rootdist = myvecdist2( vp, subroot->pvec);
    #ifdef DEBUG
    printf("rootdist %g nth mindist %g\n", rootdist, *min_nd);
    #endif

	if(rootdist < *min_nd && !subroot->fetched) {
		int i;
		bestn[*max_id] = subroot->pvec;
		*min_nd = myvecdist2(vp, bestn[0]);
		*max_id = 0;
		for(i = 1; i < n; i++) {
			if (myvecdist2(vp, bestn[i]) > *min_nd) {
				*min_nd = myvecdist2(vp, bestn[i]);
				*max_id = i;
			}
		}
	}

    /* check the subtrees - start from the most promising first */
    if( (vp->vec)[level] > ((subroot->pvec)->vec)[level]){
	/* the right subtree is more promising */
	rnnsearch( subroot->right, vp,  (level+1)% numdims, n, 
		bestn, min_nd, max_id, cnt);

	/* now check the left subtree */
	if( (*cnt) < n || (vp->vec)[level] <= ( ((subroot->pvec)->vec)[level] + (*min_nd) ) ){
	    /* only then is the left subtree promising */
	    /* notice that we use the UPDATED mindist */
	    rnnsearch( subroot->left, vp, (level+1)% numdims, n,
		bestn, min_nd, max_id, cnt);
        }
    }else{
	/* the left subtree is more promising */
	rnnsearch( subroot->left, vp, (level+1)% numdims, n,
		bestn, min_nd, max_id, cnt);

	/* now check the right subtree */
	if( (*cnt) < n || ( (vp->vec)[level] + (*min_nd)) > ((subroot->pvec)->vec)[level] ){
	    rnnsearch( subroot->right, vp, (level+1)%numdims, n,
		bestn, min_nd, max_id, cnt);
	}
    }

    return;
}

/* like vecdist2, but returns HUGE if one of the vectors is NULL */
NUMBER myvecdist2( VECTOR *vp1, VECTOR *vp2){
    if( (vp1 == NULL) || (vp2 == NULL) ) { return (HUGE) ;}
    else{ return( sqrt(vecdist2(vp1, vp2)) ) ; }
}


void rangesearch(TREENODE *subroot, VECTOR *vpLow, VECTOR *vpHigh){
    #ifdef DEBUG
    printf("tree range search was called with \n");
    vecprint(vpLow);
    vecprint(vpHigh);
    #endif
    rrangesearch(subroot, vpLow, vpHigh, 0);
}

void rrangesearch(TREENODE *subroot, VECTOR *vpLow, VECTOR *vpHigh, int level){
    #ifdef DEBUG
    printf("recursive tree range search was called with \n");
    vecprint(vpLow);
    vecprint(vpHigh);
    printf("level=%d\n", level);
    #endif

    /*** this part was disabled, for homework1 **/

    int numdims;

    if( subroot != NULL ){
        numdims = (subroot->pvec)->len;
	if( contains( vpLow, vpHigh, subroot->pvec ) ){
	    vecprint(subroot->pvec);
	}
	if( (vpLow->vec)[level] <= ((subroot->pvec)->vec)[level] ){
	    /* left branch can not be excluded */
	    rrangesearch( subroot->left, vpLow, vpHigh, (level+1)% numdims);
	}
	if( (vpHigh->vec)[level] > ((subroot->pvec)->vec)[level] ){
	    /* right branch can not be excluded */
	    /* notice the '>' as opposed to '>=' */
	    rrangesearch( subroot->right, vpLow, vpHigh, (level+1)% numdims);
	}
    }
    /*****/
    return;
}

/* returns TRUE if the interval contains the vp point */
/* The interval is CLOSED, ie it contains its endpoints */

BOOLEAN contains( VECTOR *vpLow, VECTOR *vpHigh, VECTOR *vp){
    BOOLEAN res;
    int i;
    int numdims;

    numdims = vp->len;

    for(i=0, res=TRUE; res && (i< numdims); i++){
       if( ( (vp->vec)[i] < (vpLow->vec)[i] ) ||
	   ((vp->vec)[i] > (vpHigh->vec)[i]) ){
	   res = FALSE;
       }
    }
    return(res);

}
