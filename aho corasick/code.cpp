#include<iostream>
#include<map>
#include<vector>
#include<queue>
using namespace std;
const int alp = 26;
struct node{
    node* ancestor = NULL;
    node* child[alp];
    vector<string> s;
    void init(string st){
        for(auto &i : child) i = NULL;
        if(st.size()) s.emplace_back(st);
        return;
    }
};

node* getnode(string s){
    node* ret = new node;
    ret->init(s);
    ret->ancestor = ret;
    return ret;
}

void insert(node* root, string s){
    node* p = root;
    int n = s.size();
    for(int i = 0; i < n; i++){
        int c = s[i]-'a';
        string st = (i == n-1 ? s : "");
        if(!p->child[c]) p->child[c] = getnode(st);
        else if(st.size()) p->child[c]->s.emplace_back(st);
        p = p->child[c];
    }
    return;
}

node* search_ancestor(node* nod, int val){
    node* p = nod->ancestor;
    while(p->ancestor != p){
        if(p->child[val]) return p->child[val];
        p = p->ancestor;
    }
    if(p->child[val]) return p->child[val];
    return p;
}

void build_fails(node* root){
    queue<node*> q;
    for(int i = 0; i < alp; i++){
        if(root->child[i]){
            q.push(root->child[i]);
            root->child[i]->ancestor = root;
        }
    }
    while(!q.empty()){
        node *actual = q.front();
        q.pop();
        for(int i = 0; i < alp; i++){
            node *child = actual->child[i];
            if(child){
                q.push(child);
                child->ancestor = search_ancestor(actual, i);
                for(int i = 0; i < child->ancestor->s.size(); i++){
                    child->s.emplace_back(child->ancestor->s[i]);
                }
            }
        }
    }
    return;
}

map<string, vector<int>> search_indexes(node* root, string s){
    map<string, vector<int>> ret;
    node* p = root;
    int n = s.size();
    for(int i = 0; i < n; i++){
        int ind = s[i]-'a';
        while(p != root && !p->child[ind]) p = p->ancestor;
        if(p->child[ind]) p = p->child[ind];
        if(p->s.size()){
            for(int j = 0; j < p->s.size(); j++){
                ret[p->s[j]].emplace_back(i - p->s[j].size() + 1);
            }
        }
    }
    return ret;
}

int main(){
    int n;
    node *root = getnode("");
    string s;
    cout << "How many words has your dictionary?: ";
    cin >> n;
    cout << "Insert those words: \n";
    for(int i= 0; i < n; i++){
        cin >> s;
        insert(root, s);
    }
    build_fails(root);
    cout << "Insert your text: ";
    cin >> s;
    map<string, vector<int>> indexes = search_indexes(root, s);
    for(auto i : indexes){
        cout << i.first << ":\n";
        for(auto j : i.second) cout << "    from " << j << " to " << j+i.first.size()-1 << '\n';
    }
    return 0;
}