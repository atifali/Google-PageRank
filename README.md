Google Page Rank Algorithm
==
This project is a simple implementation of the Google's iconic Page Rank Algorithm.
--
It features the following things:

* Dynamic Memory Allocation
* MATLAB Engine Integration for the matrix computations
* Modular I/O structure to facilitate a bottom-up design approach on top of this existing project

Some Details Regarding the "simplistic" PageRank Algorithm employed in the program: 

1. Let W be a set of webpages (a web) and let the set have size n.

2. Let G be the square n by n connectivity matrix for the web where:
g_ij = 1 if there is a hyperlink from page j to page i, where i is the row and j is the column, and g_ij = 0 otherwise. For example, suppose we have a tiny web of D pages: A, B, C, and D, and apart from page D which has no links to it or from it, each page contains one link to each of the other pages, but not to itself. That is, page A contains a link to page B, and to page C, but not to page A or to page D. Page B contains a link to page A, and to page C, but not to page B or page D. And so on. We say that the jth column of the connectivity matrix contains the outbound links from page j. Our connectivity matrix will look like this :
```Matlab
G=
[[0,1,1,0]
[1,0,1,0]
[1,1,0,0]
[0,0,0,0]]
```
3. Let's define the importance of each page by the importance of the pages that link to it. (Hey wait, that sounds a little like recursion!). If we
do this, we can use the connectivity matrix as a launching pad for determining the relative importance of each page in our web. Note that a
typical connectivity matrix G is probably very big (how many billions of web pages are there these days?), but very sparse (lots of zeros). Its jth
column shows the links on the jth page, and the total number of nonzero entries in the entire matrix is the total number of links in our web W. In
fact, we can define ri and cj to be the row and column sums of G respectively. These quantities are the indegree and outdegree.The indegree is the number of pages that have links to our ith page, and the outdegree is the number of links on our jth page to other pages.

4. Mathematically speaking, the "importance" of page i (xi) equals the sum over all pages j that link to i of the importance of each page j divided
by the number of links in page j.

5. We can modify our connectivity matrix to show us this "importance" if we divide each value in each column by the sum of each column. Since it is a connectivity matrix, the values the cells are either 0 or 1, so each cell containing a 1 is divided by the number of 1s in the column.
For example, in our connectivity matrix G, the sum in the first column (column A) is 2, so we divide each element by 2 to get 1/2. We can
observe that every nonzero column now adds up to 1. Let S be the matrix constructed according to this rule:
```Matlab
S=
[[0,0.5,0.5,0];
 [0.5,0,0.5,0];
 [0.5,0.5,0,0];
 [0, 0, 0, 0]];
```
6. In matrix S, the value in S[i][j] is the "probability" of going from page j to page i. Note that every nonzero column adds up to 1, but we have that pesky final column of zeros to worry about. There are no links to page D, and there are no links away from page D. You may remember from your linear algebra that we have to do something about this, otherwise page D is going to end up at the bottom of the PageRank no matter how interesting its contents (and if you don't remember this, trust us!). We want to replace every column of zeros with a column whose elements equal 1/n (recall n is the dimension of our square connectivity matrix):
```Matlab
S=
[[0,0.5,0.5,0.25];
[0.5,0,0.5,0.25];
[0.5,0.5,0,0.25];
[0, 0, 0, 0.25]];
```
7. The matrix S is now a stochastic matrix, or to be more specific, a left stochastic matrix because the columns all sum to 1. In a left stochastic matrix, the elements are all strictly between 0 and 1 and its columns all sum to 1. We're almost finished. We can also call S a probability matrix, and we will use our probability matrix to construct a transition matrix for a Markov process. This sounds much more complicated than it really is. There are two steps:
		..1. We need to introduce the notion of a random walk. We need to 	multiply our probability matrix by a random walk probability factor.For our lab, we will designate this variable p, and set p = 0.85.
		..2. If we multiply our probability matrix by p, then you might think we need to add something to compensate for the overall reduction in value. You're right. We need to add (1 p) multipled by a rank 1 matrix Q, whose every element = 1 / n. (recall that a rank 1 matrix has a single linearly independent column). Let's call the transition matrix that results M.
		
8. The rest is easy. Let's create a column vector (or we can call it an array) of length n called rank. We'll initialize each element to 1:
```Matlab
rank=
[[1];
 [1]; 
 [1];
 [1]]; 	
```
9. The final 2 steps are the Markov process (aka the dynamical system aka the power method):
		..1. Multiply the transition matrix M by our column vector (array) rank, and then multiply M by the result and then keep doing this until the and rank stops changing (result converges), e.g., M * rank = rank. In this case, we get:
		```Matlab
		rank=
			[[1.2698];
			 [1.2698]; 
			 [1.2698];
			 [0.1905]]; 	
		```
		..2. Divide each element in rank by the sum of the values in rank (scale rank so its elements sum to 1):
		```Matlab
		rank= 	
		[[1.2698 / 3.999 = 0.3175];
		 [1.2698 / 3.999 = 0.3175];
		 [1.2698 / 3.999 = 0.3175];
		 [0.1905 / 3.999 = 0.0476]];
		```
And thats all there is to it! 
(Courtesy of Prof. Awad at UBC).