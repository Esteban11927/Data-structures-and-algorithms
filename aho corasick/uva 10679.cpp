#include<iostream>
#include<vector>
#include<queue>
using namespace std;
const int alp = 58, mod = 10000;
bool indexes[mod];

int hashit(string s){
    long long int p = 31, ret = 0, mul = 1;
    for(auto c : s){
        ret = (ret + c*mul)%mod;
        mul *= p;
        mul %= mod;
    }
    return ret;
}

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
        int c = s[i]-'A';
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
                if(child->ancestor->s.size() && child->s.size() == 0)
                    child->s.emplace_back(child->ancestor->s[0]);
            }
        }
    }
    return;
}

void mark_ancestors(node* nod, node* root){
    node* p = nod;
    while(p != root){
        if(p->s.size()) indexes[hashit(p->s[0])] = true;
        p = p->ancestor;
    }
    return;
}

void search_indexes(node* root, string s){
    node* p = root;
    int n = s.size();
    for(int i = 0; i < n; i++){
        int ind = s[i]-'A';
        while(p != root && !p->child[ind]) p = p->ancestor;
        if(p->child[ind]) p = p->child[ind];
        if(p->s.size()){
            mark_ancestors(p, root);
        }
    }
    return;
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    int n, k, q;
    string text;
    cin >> k;
    while(k--){
        for(auto &i : indexes) i = false;
        node *root = getnode("");
        cin >> text >> q;
        string t[q];
        for(int i= 0; i < q; i++){
            cin >> t[i];
            insert(root, t[i]);
        }
        build_fails(root);
        search_indexes(root, text);
        for(int i = 0; i < q; i++){
            if(indexes[hashit(t[i])]) cout << "y\n";
            else cout << "n\n";
        }
    }
    return 0;
}