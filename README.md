wssd_cech
=========

Code to generate an (1+e)-approximation of the Cech complex using a Well-Separated Simplical Decomposition (WSSD). The code is based on the paper *"Approximate Cech Complexes in Low and High Dimensions"* by Michael Kerber and R. Sharathkumar (ArXiv copy: http://arxiv.org/abs/1307.3272). 

Use
---
Visual Studio 2012 project files are included. There are no external dependencies except for a C++11 compatible compiler. There are 2 compiler flags _WSSD_VALIDATION_ for enabling the code that can validate the WSSD, and _WSSD_THREADS_ to enable multithreading for finding tuples in the simplices.

Disclaimer
----------
The code is currently working, but not in it's most user-friendly form.

License
-------
The code is free to use for academic purposes, with the caveat that the authors should be cited in any paper, and contacted prior to submission. For any commercial use, the authors should be contacted.

Changes with regards to the paper
---------------------------------
There is a vast number of changes:

**QUADTREE**
- Quadtree is first built with normal construction algorithm, then compressed *O(n log S)* running time, where *S* is the spead of the point set.
- For each cell, we compute the smallest axis-aligned bounding box

**1-WSSD**
- We compute distance between 2 bounding boxes (finding the difference is done per dimension), compare this to the diameters of the AABBs of the quadtree cells.
- We discard tuples that where the diameter of the AABB of the tuple is larger than the largest alpha (since they will never be treated in the Cech complex approximation).

**K-WSSD**
- We separate the definition of WSSD from quadtrees. We use tight-fitting AABBs, but all properties should hold for bounding volume hierarchies (BVHs) where the bounding volume of descendants is completely contained in the bounding volume of their ancestors (so AABB trees are good, but sphere trees are not).
-- A corollary of this is that any mention of height should be replaced with diameter. All proofs should still go through with the exception of the size proof using the packing algorithm. For this we need that the BVH was creates from a quadtree (similar to the traditional WSPD proof).

- Lemma 10, we take *(1 + 1/D)/sqrt(1-1/(D*D))* instead of *2*
- Lemma 9, we take *diam(q') <= (eps*r)/(1+eps)* instead of *(eps*r)/2*
- We perform the range query by starting at the root and traversing top-down

**FILTRATION**
- We start by adding all the vertices
- For every WST we find the correct time to add, and store these in a list
- Once the cardinality of a tuple decreases, we discard is for a reason similar to Lemma 15.
- To update from one delta to the next we do:
-- See if two vertices should be collapsed. If so, do it. We have a scheme in place that always collapses all children of a node to the same vertex.
-- Go over the nodes in the WSSD that haven't been handled yet. We add a simplex to the filtration if it fulfils the conditions. We test for diameter of the smallest AABB.
- Use the collapse described in Dey Fen Wang
-- We don't create any simplices with dimension higher than *D+1*. This yields a subset of the persistence pairs.

Credit
------
We use Bernd Gaertner's Miniball code (http://www.inf.ethz.ch/personal/gaertner/miniball.html), which is under GPLv3, to determine the smallest enclosing ball of a set of quadtree nodes.
