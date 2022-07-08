#include<iostream>
#include<fstream>
#include<stack>
#include <sstream> 
#include<stdio.h>
#include<string>
#include<stdlib.h>
#include<vector>

using namespace std;

void assignZero(int data[8]) {
    for (int i = 7; i >= 0; i--) {
        data[i] = 0;
    }
}

struct Node {
	float key;
	int data[8];
	Node* right;
	Node* left;

	Node(float key, int data[8], Node* right, Node* left) {
		this->key = key;
		for (int i = 7; i >= 0; i--) {
			this->data[i] = data[i];
		}
		this->right = right;
		this->left = left;
	}

};

template <typename T>
class bMinHeap {

    vector<Node*> h;

    inline int parent(int i) {
        return i / 2;
    }

    inline int lchild(int i) {
        return 2 * i;
    }

    inline int rchild(int i) {
        return 2 * i + 1;
    }


    inline bool isLeaf(int i) {
        return lchild(i) > size();

    }

    void percolateUp(int i) {
        //O(lgn)
        while (parent(i) > 0 && h[i]->key < h[parent(i)]->key) {
            swap(h[i], h[parent(i)]);
            i = parent(i);
        }
    }

    void heapify(int i) {//percolate down
        //O(lgn)
        while (!isLeaf(i)) {
            int mini = i;//node itself is min

            //update if lchild has smaller key
            if (h[lchild(i)]->key < h[mini]->key)
                mini = lchild(i);

            //update if rchild has even smaller key
            if (rchild(i) < size() && h[rchild(i)]->key < h[mini]->key)
                mini = rchild(i);

            if (i != mini) {
                swap(h[i], h[mini]);//swap ith node with the minimum of i, 2*i and 2*i+1
                i = mini;//move to one of the children
            }
            else
                break;//no violation
        }
    }



public:
    bMinHeap() {
        h.resize(1);
    }

    int size() {
        return h.size() - 1;
    }

    bool empty() {
        return size() == 0;
    }

    const T& getMin() {
        //O(1)
        return h[1];
    }


    void insert(Node* node) {

        h.push_back(node);//key becomes the next leaf the last level

        //now percolate up
        int i = size(); //index of the newly added node
        percolateUp(i);
    }

    Node* hybridNode() 
    {
        Node* node1 = getMin();
        deteleMin();

        Node* node2 = getMin();
        deteleMin();

        int arr[8];
        assignZero(arr);

        Node* node3= new Node((node1->key + node2->key), arr, node1, node2);
        return node3;
    }

    void makeBT() {

        while (size() > 1) //
        {
            insert(hybridNode());
        }
    }

    void deteleMin() {
        if (size() > 0) {
            h[1] = h[size()];//overwrite root with last leaf
            h.pop_back();//remove last leaf
            heapify(1);//percolateDown(1)
        }
    }


};

int binaryToDec(int a[]) {
	int dec_value = 0;
	int base = 1;

	for (int i = 7; i >= 0; i--) {
		dec_value = dec_value + a[i] * base;
		base = base * 2;
	}

	return dec_value;
}

void printArray(int arr[], int n) {
    int i;
    for (i = 0; i < n; ++i)
        cout << arr[i];

    cout << "\n";
}

int isLeaf( Node* root) {
    return !(root->left) && !(root->right);
}

void putinTable(int arr[], int top,int asci[], int hcode[257][9]) {
    int ascii= binaryToDec(asci);   //found the ascii fro the node
    int i;

    for (i = 0; i < top; i++) 
    {
        hcode[ascii][i]=arr[i];    //
    }
    hcode[ascii][8] = top;    //putting the size

}


//THIS FUNCTION PRODUCES TH HUFFMAN CODES FROM HEAP NODES DATA

void HCodes(Node* root, int arr[], int top,int hcode[257][9]) {

    if (root->left) {
        arr[top] = 0;
        HCodes(root->left, arr, top + 1, hcode);
    }

    if (root->right) {
        arr[top] = 1;
        HCodes(root->right, arr, top + 1, hcode);
    }

    if (isLeaf(root)) {
        
        putinTable(arr, top, root->data, hcode);
    }
}

void HCodes(Node* root, int ftable[257][9]) {
    int arrr[50], top = 0;
    HCodes(root, arrr, top, ftable);
}

void buildHeap(bMinHeap<Node*>& heap, int ftable[257][9], int bits) 
{
    int symbols = bits / 8;

    for (int i = 0; i < 256; i++) {
        if (ftable[i][8] != 0) {

            int arr[8];
            for (int j = 7; j >= 0; j--) {
                arr[j] = ftable[i][j];
            }

            float repete = ftable[i][8];
            float freq = (repete / symbols);

            Node* s = new Node(freq, arr, NULL, NULL);

            heap.insert(s);

        }
    }
}

bool Comp(char compress[],int s, string comp) {
    for (int i = 0; i < s; i++) {
        if (compress[i] != comp[i]) {
            return false;
        }
    }
    return true;
}


void gatherNodes(ofstream& outdata, Node* root) {
    if (root->left) {
        gatherNodes(outdata, root->left);
    }

    if (root->right) {
        gatherNodes(outdata, root->right);
    }

    if (isLeaf(root)) {
        outdata << root->key;
        outdata << ",";
        for (int i = 0; i < 8; i++) {
            outdata << root->data[i];
        } 
        outdata << ",";
    }

}


void NodeCount(Node* root, int& c)
{
    if (root->left) {
        NodeCount(root->left, c);
    }
    if (root->right) {      
        NodeCount(root->right, c);
    }
    if (isLeaf(root)) {
        c++;
    }
}
void putDataInFile(ofstream& outdata, Node* root) {
    
}
void Uncompress(ofstream& outdata, vector<char>& f, Node* root, Node* copyroot, char c, int count ) {
    if(count<f.size()){
        if (root == NULL) 
            return;
        
        if (f[count] == '0' && isLeaf(root) != true) {  //root->left        
            //f.get(c);
            count++;          
            Uncompress(outdata, f, root->left,copyroot, c, count);                    
        }
       
        if (f[count]=='1' && isLeaf(root)!=true) {   //root->right          
            //f.get(c);
            count++;
            Uncompress(outdata, f, root->right,copyroot, c, count );            
        }

        if (isLeaf(root)) {
            //f.get(c);
            char o;
            for (int i = 0; i < 8; i++) {
                //cout << root->data[i];
                o = root->data[i] + '0';
                outdata << o;
            }
            Uncompress(outdata, f, copyroot, copyroot, c, count + 1);
        }
    }
}

void decode(ofstream& outdata, Node* root, vector<char>& s)
{
    Node* curr = root;
    for (int i = 0; i < s.size(); i++)
    {
        if (s[i] == '0')
            curr = curr->left;
        else
            curr = curr->right;

        if (curr->left == NULL and curr->right == NULL)
        {
            char o;
            for (int i = 0; i < 8; i++) {
                o = curr->data[i] + '0';
                outdata << o;
            }
            curr = root;
        }
    }
}

int main() {

    char compress[40];
    string comp= "> compress ";
    cout << "\nEnter the command : " << endl;
    cin.get(compress, 40);


    if (Comp(compress, 11, comp)) {
        int i, j;
        for (i = 11, j=0 ; compress[i]!='\0' ; i++, j++) {
            comp[j] = compress[i];
        }
        comp[j] = '\0';


        const char* inFile = "sample1.8b";

        int  ftable[257][9];
        float bits = 0, cbits = 0;
        char c;     //char for reading file 

        for (int i = 0; i < 257; i++) {	//intializing all counts with 0
            ftable[i][8] = 0;
        }

        //READING DATA FROM THE FILE

        //ifstream object
        ifstream file1(comp, ios::binary | ios::in);  //the reading file 
        ifstream f(comp, ios::binary | ios::in);

        //Builds the Frequency Table
        f.get(c);
        while (!f.eof())
        {
            int arr[8];
            for (int i = 0; i <= 7; i++)
            {
                bits++;         //increase total nmber of bits
                //cout<<c;
                arr[i] = c - '0';
                f.get(c);	//reading from file   
            }

            //cout << ", "<<endl;

            int ascii = binaryToDec(arr);

            for (int i = 7; i >= 0; i--) {	//putting the binary at the ascii of the character
                ftable[ascii][i] = arr[i];
            }
            ftable[ascii][8]++;	//putting frequency itself at 8th index
        }
        f.close();


        //BUILD HEAP WITH FREQUENCY TABLE
        bMinHeap<Node*> heap;
        buildHeap(heap, ftable, bits);


        //BUILDS BINARY TREE IN HEAP AT ROOT NODE
        heap.makeBT();


        //EXTRACT THE HUFFMAN CODE'S OF LEAF NODES, IN FTABLE AT ASCII
        Node* root = heap.getMin();
        HCodes(root, ftable);


        //WRITTING THE HUFF CODES IN .CPM FILE    
        ofstream outdata;

        outdata.open("compressedFile.cmp"); // opens the file
        if (!outdata) { // file couldn't be opened
            cerr << "Error: file could not be opened" << endl;
            exit(1);
        }
        else {
            int N = 0;
            NodeCount(root, N);
            outdata << N ; ///puts the total number of nodes in the file
            outdata << ",";
            cout << "Number Of Leaf Nodes= " << N << endl;
            gatherNodes(outdata, root);
            file1.get(c);	//reading from file
            if (file1.is_open())
            {
                while (!file1.eof()) {
                    int arr[8];
                    for (int i = 0; i <= 7; i++)
                    {
                        arr[i] = c - '0';
                        file1.get(c);	//reading from file

                    }

                    int ascii = binaryToDec(arr);

                    for (int i = 0; i < ftable[ascii][8]; i++)
                    {
                        //EXTRACT THE HUFFCODE FROM FTABLE, USING ASCII
                        if (outdata.is_open()) {
                            outdata << ftable[ascii][i]; //putiing the huffman code in the file
                        }
                        cbits++;    //INCREASES THE NUMBER OF COMPRESSED BITS
                    }
                }
            }

            // Close the file
            outdata.close();
            file1.close();
            //2. COMPRESSION PERCENTAGE

            float p = (1 - (cbits / bits)) * 100;
            cout << " COMPRESSION PERCENTAGE = " << p << "%\n\n";
        }
    }
    

    
    comp = "> decompress ";
    string comp2 = "                          ";
    if (Comp(compress, 13, comp)) {
        int i, j;
        for (i = 13, j = 0; compress[i] != '\0'; i++, j++) {
            comp2[j] = compress[i];
        }
        comp2[j] = '\0';

        //OPEN THE SPECIFIED FILE
        ifstream f(comp2, ios::binary | ios::in);
        char c; float freq = 0; char arr[9];
        
        //GETS THE NUMBER COMMAS USED TO SEPERATE LEAF NODES
        f.get(c);
        for (i = 0; c != ','; i++)
         arr[i] = c,  f.get(c);	//reading next char from file   
        
        arr[i] = '\0';
        int noOfnodes = atoi(arr);

        //MAKING THE HEAP TREE AGAIN
        bMinHeap<Node*> heap2;

        //LOOP TO PICK LEAF NODES
        for (int l=0; l < noOfnodes; l++)
        {
            //gets the frequency
            f.get(c);
            char fre[12];
            for (i = 0; c != ','; i++)
                fre[i] = c, f.get(c);	//reading next char from file   

            fre[i] = '\0';
            freq = atof(fre);

            //gets the data for node
            int data[8];
            f.get(c);
            for (i = 0; c != ','; i++)
                data[i] = c-'0', f.get(c);

            //make the node
            Node* s = new Node(freq, data, NULL, NULL);
            heap2.insert(s);
        }

        //BUILDS BINARY TREE IN HEAP AT ROOT NODE
        heap2.makeBT();

        //GET THE ROOT NODE OF THE BINARY TREE
        Node* root = heap2.getMin();

        //MAKE DECOMPRESS FILE
        

        ofstream outdata; 

        outdata.open("decompressFile.8b"); // opens the file
      
        //UNCOMPRESS FILE
      
        f.get(c);
       
        vector<char> read;
        while (!f.eof())
        {
            read.push_back(c);
            f.get(c);
        }
       
        decode(outdata, root, read);
        //Uncompress(outdata, read, root, root, c,0);
        

        //CLOSE FILES
        f.close();
        outdata.close();
    }

    system("pause");
	return 0;

}