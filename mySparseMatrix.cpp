#include<iostream>
#include<string>
#include<fstream>
using namespace std;

class Node {
public:
    Node(int x, int y, int d = 0) :
    x(x), y(y), data(d) {
        right = this;
        down = this;
    }
    int x, y;
    int data;
    Node* right;
    Node* down;
};

class SparseMatrix {
public:
    SparseMatrix(int n, int m) : n(n), m(m) {
        head = new Node(0, 0);
    }

    SparseMatrix(string path){
        head = new Node(0, 0);
        load(path);
    }

    ~SparseMatrix() {
        delete head;
    }

    bool add(int x, int y, int d) {
        if (x > m || y > n) {
            cout << "index out of range" << endl;
            return 0;
        }
        if (x <= 0 || y <= 0) {
            cout << "indexs must be positive" << endl;
            return 0;
        }

        Node* node = new Node(x, y, d);
        Node* p = findHorizontal(x, head);
        if (p->right->x != x) {
            Node *vhead = new Node(x, 0);
            insertHorizontal(vhead, p);
        }
        addVertical(node, p->right);
        p = findVertical(y, head);
        if (p->down->y != y) {
            Node *hhead = new Node(0, y);
            insertVertical(hhead, p);
        }
        addHorizontal(node, p->down);
        //cout << "inserted " << d << " at row:" << y << " column:" << x <<".\n";
        //cout<<endl;
        return 1;
    }

    void delete(int x, int y) {
        if (x > m || y > n) {
            cout << "index out of range" << endl;
            return;
        }
        if (x < 0 || y < 0) {
            cout << "indexs must be positive" << endl;
            return;
        }
        Node* p = findHorizontal(x, head);
        if (p->right->x != x) {
            cout << "error:No data in column " << x << " \n";
            return;
        }
        Node *q = findVertical(y, p->right);
        if (q->down->y != y) {
            cout << "error:No data in row:" << y << " column:" << x << " \n";
            return;
        }
        Node *r = q->down;
        while (r->right->x != x)
            r = r->right;

        Node* t;
        q->down = q->down->down;
        if (q->down == q) {
            t = p->right;
            p->right = p->right->right;
            delete t;
        }
        t = r->right;
        r->right = r->right->right;
        cout << "Data:" << t->data << " at column:" << t->x << " row:" << t->y << " has be erased\n";
        delete t;
        if (r->right == r) {
            while (r->down->y != y)
                r = r->down;
            t = r->down;
            r->down = r->down->down;
            delete t;
        }
    }

    void output() {
      //cout << "=========================" << endl;
      //cout << "Output the matrix:" << endl;
        Node *p = head->right;
        while (p != head) {
            int column = p->x;
            Node* q = p->down;
            //cout << "-------------------------" << endl;
	    //  cout << "output column" << column << ":\n";
            while (q != p) {
                cout << "(" << q->y<< "," << q->x<< ")\t" << q->data << ".\n";
                q = q->down;
            }
            p = p->right;
        }
        //cout << "=========================" << endl;
    }

    void save(string path) {
        ofstream out(path.c_str());
        if(!out){
            cout<<"Cannot open\n";
            return;
        }

	out<<m<<' '<<n<<endl;
        Node *p = head->right;
        while (p != head) {
            int column = p->x;
            out<<column;
            Node* q = p->down;
            while (q != p) {
                out<<' '<<q->y<<' '<<q->data;
                q = q->down;
            }
            out<<endl;
            p = p->right;
        }
        out.close();
        cout << "Saved to " << path << endl;
    }

    void load(string path) {
        cout << "load from " << path << endl;
        ifstream in(path.c_str());
        if(!in){
            cout<<"Cannot open file\n";
            return;
        }
        in>>m;in>>n;
        int column,row,data;
        Node* currentColumn=head;
        while(!in.eof()){
            in>>column;
            currentColumn->right=new Node(column,0);
            currentColumn=currentColumn->right;
            //add column
            char temp=in.get();
            cout<<(int)temp;
            while(temp!='\n'){
                //add data column row
                in>>row>>data;
                Node* node = new Node(column, row, data);
                addVertical(node, currentColumn);
                Node* p = findVertical(row, head);
                if (p->down->y != row) {
                    Node *hhead = new Node(0, row);
                    insertVertical(hhead, p);
                }
                addHorizontal(node, p->down);
                temp=in.get();
                cout<<(int)temp;
            }
        }
        currentColumn->right=head;
        in.close();
    }
private:
    int n, m;//n:columns,m:rows
    Node* head;

    void addVertical(Node* node, Node *head) {
        if (node->x != head->x) {
            cout << "fuck" << endl;
            return;
        }
        Node* p = head;
        while (p->down->y < node->y && p->down != head)
            p = p->down;
        if (p->down->y == node->y) {
            node->down = p->down->down;
            delete p->down;
            p->down = node;
        }
        node->down = p->down;
        p->down = node;
    }

    Node* findVertical(int y, Node *head) {
        Node* p = head;
        while (p->down->y < y && p->down != head)
            p = p->down;
        return p;
    }

    void addHorizontal(Node* node, Node *head) {
        if (node->y != head->y) {
            cout << "fuck" << endl;
            return;
        }
        Node* p = head;
        while (p->right->x < node->x && p->right != head)
            p = p->right;
        if (p->right->x == node->x) {
            node->right = p->right->right;
            delete p->right;
            p->right = node;
        }
        node->right = p->right;
        p->right = node;
    }

    Node* findHorizontal(int x, Node *head) {
        Node* p = head;
        while (p->right->x < x && p->right != head)
            p = p->right;
        return p;
    }

    void insertHorizontal(Node *q, Node *p) {
        q->right = p->right;
        p->right = q;
    }

    void insertVertical(Node *q, Node *p) {
        q->down = p->down;
        p->down = q;
    }
};

