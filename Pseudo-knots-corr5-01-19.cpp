//Petingi 01/19  
//to detect pseudo-graph in dual graphs \copyright 
//collaboraton project with Tamar Schlick and Namhee Kim (NYU)
//we are reading AdJ Matrix thus we convert to Linked List DS
// Correction SWati found a mistake in the code (the last parallel edges were counted twice). 11/11/17
// Modified on 07/18 to delete procedures not related to the partition algorithm.
// Revised changes completed on 12/18 to detect recursive Pseudoknots. 

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <ctime>
#include <stack>

using namespace std;


//node
struct edge
{ int v1;
  int v2;
};
struct node
{
    int vertex;
	int binary;
	float rel;
    node * next=NULL;


};
int nods =0;
int motif_counter= 0;
int numreg_blocks = 0;
int numpseudo_blocks = 0;
int numrpseudo_blocks = 0;
int edgs =0;
int dim = 0;
int iterat = 0;
int term = 0;
stack<edge> mystack;
stack<edge> mystack1;
stack<edge> mystack2;
bool * Visited; //used in Bi_Connect
bool * Visited1; //used in isRecursive
int *d_cmpnt;
node * glovalnode;
int  weight[100][100];
ifstream infile;
ofstream outfile;

bool cycles = false;// determine if a graph has cycles.



class Graph {
private:
	int n; //number of nodes
    int e; // number of edges
	float A; //??
	int B; //??
	int b_num1; //??
	int b_low1; //??

	int * b_num; //??
	int * b_low; //??
	int D; //??
	edge x;
	int terminal;
	long counter;
	int * temp;

	bool * T; //??
	int * L; //??
	int * father;
	node * w;
    edge* edge_irrel;
    int * Ls; //??
    int * Lt; //??
public:
	 node * headnodes;

	 Graph() //default constructor
	 {

	 }

	 Graph (int nods, int edgs, int dim, int term)
	// constructor
	{
		//infile.open("C:/users/petingi/desktop/RnaDv/V7adj.txt");
	    terminal=term;
		n=nods;
		b_num1 = 0;
		b_low1 = 0;
		b_low = new int [n];
		b_num = new int [n];

		e = edgs;
		D= dim;
		counter=0;
        headnodes= new node [n];
		//int temp[200];			//temporary data structure which is needed to
		T = new bool[n];
		L= new int[n];
		father = new int[n];
		d_cmpnt = new int [n];
        temp=new int[n];
        edge_irrel = new edge [e];
        Ls = new int [n];
        Lt = new int [n];
        //headnodes_A= new node [n];
		// cout << "ok until here 2" << endl;

		// headnodes is an array of nodes.
        for (int i=0; i < n; i++)
         {
			 headnodes[i].vertex=i;
             headnodes[i].next=0;

			 b_num1 = 1;
		     b_low1 = 0;
		     b_low [i] = 0;
		     b_num [i] = 0;
			 d_cmpnt [i] = 0;
         }
		// cout << "ok until here 3" << endl;

	}
	  void init ()
	// construtor
	{
		//infile.open("C:/users/petingi/desktop/RnaDv/V7adj.txt");
	    //terminal=term;
        counter=0;
		n=nods;
		b_num1 = 0;
		numreg_blocks = 0;
        numpseudo_blocks = 0;

		//b_low = new int [n];
		//b_num = new int [n];

		//e = edgs;
		//D= dim;
		//counter=0;
        //headnodes= new node [n];
		//int temp[200];			//temporary data structure which is needed to
		//T = new bool[n];
		//L= new int[n];
		//father = new int[n];
		//d_cmpnt = new int [n];
        //temp=new int[n];
        //edge_irrel = new edge [e];
        //Ls = new int [n];
        //Lt = new int [n];
        //headnodes_A= new node [n];
		//cout << "ok until here 2: " << n << endl;

		// headnodes is an array of nodes.
        for (int i=0; i < n; i++)
		{
			 headnodes[i].vertex=i;
             headnodes[i].next=0;
		     b_num1 = 1;
		     b_low1 = 0;
		     b_low [i] = 0;
		     b_num [i] = 0;
			 d_cmpnt [i] = 0;
		}
		while (!mystack.empty())
				mystack.pop();
		while (!mystack1.empty())
				mystack1.pop();


	  }
	 ~Graph ()
	 {  delete [ ] headnodes;
		//int temp[200];			//temporary data structure which is needed to
		delete [] T;
		delete [] L;
		delete [] father;
        delete [] edge_irrel;
        delete [] Ls;
        delete [] Lt;
	 }
	 void Del ( )
	 {  delete [] headnodes;
	   //delete [] headnodes_A;
		delete [] T;
		delete [] L;
		delete [] father;
        delete [] edge_irrel;
        delete [] Ls;
        delete [] Lt;
		//delete [][] weight;
	 }

	void createFromFile()
	//create function
	{   //cout << "create: " << endl;
		node *pre;
		node * nextn;
	    node *newnode;
		for (int i=0; i < n; i++)
		 for (int j=0; j < n; j ++)
		 {
			infile >> A;
			//cout << "entry: " << A;

			if (i==j) A=A/2;
			weight [i][j] = A;
			if (A > 0 ) // exclude self-loops
			{ for (int k = 1; k <= 1;  k++)
			   { newnode= new node;
                 newnode->vertex = j;
				 //cout << endl << "here I am";
                 if( headnodes[i].next == NULL )
			    {
			  	    newnode->next= NULL;

				    headnodes[i].next=newnode;
			   }
			else
			   {
				 pre= &headnodes[i];
				while( pre->next != NULL )
				{
					 pre = pre->next;
				}
				newnode->next = NULL;
				pre->next = newnode;
				 }


		//ADJACENT NODES
		/*
			newnode= new node;
			newnode->vertex = i;

			if( headnodes[j].next == NULL )
			{
				newnode->next= NULL;
				headnodes[j].next=newnode;
			}
			else
			{
				pre= &headnodes[j];
				while( pre->next != NULL )
				{
					pre = pre->next;
				}
				newnode->next = NULL;
				pre->next = newnode;
			}
			*/
			} // for internal
			} // if
			} // for external



	}
	void create(int v1, int v2)
	{
	    node *newNode= new node;
	    node *previousNode = new node;
	    newNode->vertex=v2;
	    if(headnodes[v1].next==NULL)
	    {
	        newNode->next= NULL;
	        headnodes[v1].next=newNode;
	    }
	    else
	    {
	        previousNode=&headnodes[v1];
	        while(previousNode->next != NULL)
	        {
	            previousNode= previousNode->next;
	        }
	        newNode->next = NULL;
	        previousNode ->next = newNode;
	    }

	    //Connecting the adjacent nodes
	    newNode= new node;
	    newNode->vertex= v1;

	    if (headnodes[v2].next== NULL)
	    {
	        newNode->next= NULL;
	        headnodes[v2].next=newNode;
	    }
	    else
	    {
	        previousNode= &headnodes[v2];
	        while(previousNode->next != NULL)
	        {
	            previousNode=previousNode->next;
	        }
	        newNode->next= NULL;
	        previousNode->next= newNode;
	    }

	}
	void DFS(int father, int v)
		// DFS function
	{
		Visited1[v] = true;
		bool adjtoa = true;

		node* adjnode = headnodes[v].next;
		while (adjnode) // visit all vertices adjacent to v
		{
			if (!Visited1[adjnode->vertex])
			{//if adjacent vertex to v was not visited previously
				DFS(v, adjnode->vertex);
			}
			else if (father != adjnode->vertex) // if the vertex adjacent to v is not the father, we have a 
			{
				cycles = true;
			}

			adjnode = adjnode->next;

		}
	}

	void Bi_Connect(int father, int v)
	// DFS function
	{
		Visited [v]=true;
		bool adjtoa=true;
		edge b_e;
		int b_min;
		b_num [v] = b_num1; //tree edge
		b_low [v] = b_num [v]; b_num1 ++;


		node* adjnode=headnodes[v].next;
		while (adjnode) // visit all vertices adjacent to v
		{
			if ((adjnode -> vertex !=father) && (b_num [adjnode -> vertex] < b_num [v])) // push edge to the stack
			{
				b_e.v1 = v;
				b_e.v2 = adjnode->vertex;
				//cout << endl << "edge: (" << b_e.v1 << "," << b_e.v2 << ")";
		        mystack.push (b_e);
		        mystack1.push (b_e); // auxiliary stack
		   //cout << endl << "stack size: " << mystack.size(); //pushing it but not popping it.
			}
			if (!b_num[adjnode->vertex])
			{
				Bi_Connect(v,adjnode->vertex);
				if (b_low [v] > b_low [adjnode->vertex])
					b_low [v] = b_low [adjnode->vertex];

				//cout << endl << "b-low of w= " << adjnode->vertex << "-" << b_low [adjnode->vertex] << "b_num v=" << v << "-" << b_num [v];
				if (b_low [adjnode->vertex] >= b_num [v])
				{
					outfile << endl << "===================== New Block ================== \n" << endl;



					do {
					    // delete an edge from the stack of the stack
						b_e = mystack.top();
						mystack.pop();
						//cout << endl << b_e.v1 << "," << b_e.v2 << "  - weight: " << weight[b_e.v1][b_e.v2];
						for (int l = 1; l <= weight[b_e.v1][b_e.v2]; l++)
							outfile << "(" << b_e.v1 << "," << b_e.v2 << ") - ";
					} while (!((b_e.v1 == adjnode->vertex && b_e.v2 == v) || (b_e.v2 == adjnode->vertex && b_e.v1 == v)));
					outfile << endl;
					if (isPseudoknot(v, adjnode->vertex)) {

						if (isRecursive()) {
							numrpseudo_blocks++;
							outfile << " ---- this pseudoknot is recursive ---- " << endl;
						}
						else {
							numpseudo_blocks++; {
								outfile << endl << endl << " ---- this block represents a pseudoknot ---- " << endl;
							}
						}
					}
					else {
						numreg_blocks++;
						outfile << endl << " ---- this block represents a regular-region ---- "<< endl;
					}

				}

			}

		else if ( adjnode->vertex != father)

		          {  if (b_num [adjnode->vertex] < b_low [v])
					 b_low [v]=b_num [adjnode->vertex];
		          }
		adjnode = adjnode->next;
	}

	}


	bool isPseudoknot (int v, int w)
    {    bool pseudoknot = false;
	     edge b_e;


	     for (int i = 0 ; i < n ; i++) {
	    	 //T[i] = false;
	    	 d_cmpnt [i] = 0;
	     }

		 if (!mystack2.empty())
	    	 do {
	    		 b_e = mystack2.top();
	    		 mystack2.pop();

	    	 } while (!mystack2.empty());
	     do {
	    	 // delete an edge from the stack
	    	 b_e=mystack1.top();
			 mystack1.pop();
			 mystack2.push(b_e);
			 for (int l=1; l <= weight [b_e.v1] [b_e.v2]; l++){ // parallel edges
				 //outfile << "(" << b_e.v1 << "," << b_e.v2 <<") - ";
				 d_cmpnt [b_e.v1]++;
				 d_cmpnt [b_e.v2]++;

			 }
			 if (d_cmpnt [b_e.v1] > 2 || d_cmpnt [b_e.v2] > 2){
				pseudoknot = true;//biconnected component has a vertex of degree 3ff
				if (d_cmpnt [b_e.v1] > 2)
					outfile << endl << "degree of " << b_e.v1 << " is " << d_cmpnt [b_e.v1] ;
				if (d_cmpnt [b_e.v2] > 2)
					outfile << endl << "degree of " << b_e.v2<< " is " << d_cmpnt [b_e.v2] ;
			}
	     } while ( !((b_e.v1==w && b_e.v2==v) || (b_e.v2==w && b_e.v1==v)));

	     return pseudoknot;
	}


	bool isRecursive(){
		Graph *block=new Graph(n,0,0,0);
		vector<edge> edges;
		int numConnectedComponents;
		int initialNumConnectedComponents;
		while(!mystack2.empty()){ //move edges from mystack2 to vector edges
			block->create(mystack2.top().v1, mystack2.top().v2);
			edges.push_back(mystack2.top());
			mystack2.pop();
		}

		Visited1 = new bool[n]; //initialize visited array to false
		for (int a = 0; a < n; a++)
		{
			Visited1[a] = false;
		}
		block->DFS(-1, 0);
		initialNumConnectedComponents = 1;
		for (int l = 0; l < n; l++)
		{
			if (!Visited1[l]) {
				initialNumConnectedComponents++;
				block->DFS(l - 1, l);
			}
		}

		//cout << "\n----------------------block-----------------------\n";
		//block->display();
		delete block;
		delete Visited1;
		//cout << "--------------------------------------------------\n";

		for (int i = 0; i < edges.size(); i++) //print edges weights for block
		{
			cout<<"("<<  edges[i].v1 << ", " << edges[i].v2 << ") weight is " << weight[edges[i].v1][edges[i].v2]<<endl;
		}

		for (int i = 0; i < edges.size(); i++) {
			for (int j = i+1; j < edges.size(); j++) {
				block = new Graph(n, 0, 0, 0);
				for (int k = 0; k < edges.size(); k++) {
					if (k != i && k != j)
						block->create(edges[k].v1, edges[k].v2);
					else if(weight[edges[k].v1][edges[k].v2]>1)
						block->create(edges[k].v1, edges[k].v2);
				}
				//block->display();
				cout << "removed edges (" << edges[i].v1 << "," << edges[i].v2 << ") and (" << edges[j].v1 << "," << edges[j].v2 << ")\n";
				Visited1 = new bool[n];
				for (int a = 0; a < n; a++)
				{
					Visited1[a] = false;
				}
				block->DFS(-1, 0);
				numConnectedComponents = 1;
				for (int l = 0; l < n; l++)
				{
					if (!Visited1[l]) {
						numConnectedComponents++;
						block->DFS(l - 1, l);
					}
				}
				//cout << "this subgraph has " << numConnectedComponents << " connected components\n";
				delete block;
				delete Visited1;
				if (numConnectedComponents != initialNumConnectedComponents) {
					outfile << endl << "removed edges (" << edges[i].v1 << "," << edges[i].v2 << ") and (" << edges[j].v1 << "," << edges[j].v2 << ")\n";
					outfile << "these two edges are a disconnecting set:" << endl;
					outfile << "The block is a recursive PK." << endl;
					return true;
				}
				else cout << "these two edges are not a disconnecting set:" << endl;
			}
		}
		cout << endl;
		return false;
	}

	void showstack(stack <edge> gq)
	{
	    stack <edge> g = gq;
	    cout<<"stack state\n";
	    while (!g.empty())
	    {
	        cout << g.top().v1<<','<<g.top().v2<<endl;
	        g.pop();
	    }
	    cout << '\n';
	}

	void display()
	{
		node *temp1;
		cout << endl << "==============================================";
		cout << endl;

		for(int i = 0;i<n;i++)
		{
			temp1 = &headnodes[i];
			while( temp1!=NULL )
			{
				cout << temp1->vertex<<" -> ";// show the data in the linked list
				temp1 = temp1->next;   // tranfer the address of 'temp->next' to 'temp'
			}
			cout <<endl;
		}

		//cout<<"Number of vertices: "<<n<<endl;
        //cout<<"Number of edges: "<<e<<endl;

	}



};// end class


int main(int argc, char* argv[])

{
	if (argc != 3) {
        cerr << "Usage: " << argv[0] << " <input_file> <output_file>" << endl;
        return 1;
    }

	string input_file = argv[1];
    string output_file = argv[2];

    infile.open(input_file);
    if (!infile) {
        cerr << "Error: Could not open input file " << input_file << endl;
        return 1;
    }

    outfile.open(output_file);
    if (!outfile) {
        cerr << "Error: Could not open output file " << output_file << endl;
        infile.close();
        return 1;
    }

	clock_t start;
	clock_t end;
	float facto_time;
	float facto_irr_time;
	float facto_irr_e_time;
	cout<< endl << "Please enter the number of vertices: ";
	cin >>nods;

	edgs=0;
	dim=0;
	term = 0;
    Visited=new bool [nods];
	int b_counter = 0;

	outfile << endl << "--------------------- Motif :" << "PartAlg3" << " -----------------------------" << endl;


    //outfile << " +++++++++++++++ number of vertices: " << nods << "    ====================" << endl;
	Graph G(nods,edgs,dim,term);
	while ( !infile.eof () && b_counter < 1)
	{G.init();
	    b_counter ++;

	    G.createFromFile();
		G.display();


        G.Bi_Connect(-1, 0);

		outfile << endl << "----------- Summary information for Motif :" << "PartAlg3" << " --------------------------------" << endl;
		outfile << "----------- Total number of blocks: " << numreg_blocks + numpseudo_blocks + numrpseudo_blocks << endl;
		outfile << "----------- number of non-recursive PK blocks: " << numpseudo_blocks << endl;
		outfile << "----------- number of recursive PK blocks: " << numrpseudo_blocks << endl;
		outfile << "----------- number of regular blocks : " << numreg_blocks << endl;
		outfile << "-------------------------------------------------------------------------------------" << endl;

	}

    infile.close();
	outfile.close();
	
	return 0;

}




