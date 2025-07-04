# tree

## intro

**tree properties**

- tree is a special graph with properties that
    - connected
    - acyclic
    - non-direction edges
    - one path between any two vertices/nodes
- important tree concepts
    - traversal of the tree
    - depth/height of tree
    - tree and its subtree
    - tree node’s degree
    - types of node
        - root
        - internal vs leaf
        - parent vs child vs sibling
        - root vs left boundary vs right boundary vs leaf
        - predecessor vs successor
        - lowest common ancestor (LCA) of two nodes
    - types of tree
        - binary tree
            - every node has at most 2 children (left and right)
        - binary search tree (BST)
            - for each node
                - nodes in left subtree have smaller keys
                - nodes in right subtree have larger keys
            - inorder traversal res is an ascending sorted list
        - height-balanced binary tree
            - depth of the subtrees of every node never differs by more than 1
        - perfect binary tree
            - every internal node has exactly 2 child nodes
            - every leaf nodes are at the same level
        - complete binary tree
            - every level is completely filled besides last level
            - nodes in last level align left
            - typically used in implementing heap
        - full/strictly binary tree
            - every node has exactly 0 or 2 children

**how to define tree node**

```cpp
// definition for a binary tree node
struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int v) : val(v), left(nullptr), right(nullptr) {}
};

// definition for a n-nary tree node
struct NaryTreeNode {
    int val;
    std::vector<NaryTreeNode*> children;
    NaryTreeNode(int v) : val(v) {}
};
```

---

### traversal of the tree

#### Preorder (Root-Left-Right)
```cpp
void preorder(TreeNode* root, std::vector<int>& res) {
    if (!root) return;
    res.push_back(root->val);
    preorder(root->left, res);
    preorder(root->right, res);
}
```

#### Inorder (Left-Root-Right)
```cpp
void inorder(TreeNode* root, std::vector<int>& res) {
    if (!root) return;
    inorder(root->left, res);
    res.push_back(root->val);
    inorder(root->right, res);
}
```

#### Postorder (Left-Right-Root)
```cpp
void postorder(TreeNode* root, std::vector<int>& res) {
    if (!root) return;
    postorder(root->left, res);
    postorder(root->right, res);
    res.push_back(root->val);
}
```

#### Level order (BFS)
```cpp
#include <queue>
std::vector<std::vector<int>> levelOrder(TreeNode* root) {
    std::vector<std::vector<int>> res;
    if (!root) return res;
    std::queue<TreeNode*> q;
    q.push(root);
    while (!q.empty()) {
        int sz = q.size();
        std::vector<int> level;
        for (int i = 0; i < sz; ++i) {
            TreeNode* node = q.front(); q.pop();
            level.push_back(node->val);
            if (node->left) q.push(node->left);
            if (node->right) q.push(node->right);
        }
        res.push_back(level);
    }
    return res;
}
```

---

### depth/height of tree

- **Definition:** Height of a tree is the number of edges in longest path from root to a leaf.

```cpp
int treeHeight(TreeNode* root) {
    if (!root) return -1; // height in edges, use 0 if you want height in nodes
    return 1 + std::max(treeHeight(root->left), treeHeight(root->right));
}
```

---

### tree and its subtree

- **Definition:** Any node and all its descendants form a subtree.

#### Example: Print all values in subtree rooted at a given node

```cpp
void printSubtree(TreeNode* node) {
    if (!node) return;
    std::cout << node->val << " ";
    printSubtree(node->left);
    printSubtree(node->right);
}
```

---

### tree node’s degree

- **Definition:** Degree of a node = the number of its children.

```cpp
int nodeDegree(TreeNode* node) {
    int deg = 0;
    if (node->left) ++deg;
    if (node->right) ++deg;
    return deg;
}

// For N-ary tree:
int nodeDegree(NaryTreeNode* node) {
    return node->children.size();
}
```

---

**if you need to do tree traversal, then try**

1. BFS (levelorder)
    1. level 0 → level 1 → level2
    2. time `O(n)`, due to visiting all nodes
    3. space `O(n)`, due to perfect tree or complete tree, otherwise `O(diameter(count of nodes in same level))`
    4. use bfs when the operation related to level
    
    ```cpp
    #include <queue>
    bool bfs(TreeNode* root) {
        if (!root) return false;
        std::queue<TreeNode*> q;
        q.push(root);
        while (!q.empty()) {
            TreeNode* node = q.front(); q.pop();
            if (/* SOME_CONDITION */) {
                return /* FOUND */;
            }
            if (node->left) q.push(node->left);
            if (node->right) q.push(node->right);
        }
        return /* NOTFOUND */;
    }
    ```
    
2. DFS preorder
    1. node → left → right
    2. time `O(n)`, due to visiting all nodes
    3. space `O(n)`, due to skewed tree, otherwise `O(height)`
    4. use preorder when the operation has a top-down flow
    
    ```cpp
    // recursive
    void preorder_helper(TreeNode* root, std::vector<int>& preorder) {
        if (!root) return;
        preorder.push_back(root->val);
        preorder_helper(root->left, preorder);
        preorder_helper(root->right, preorder);
    }

    // iterative
    std::vector<int> preorder(TreeNode* root) {
        std::vector<int> res;
        if (!root) return res;
        std::stack<TreeNode*> st;
        st.push(root);
        while (!st.empty()) {
            TreeNode* node = st.top(); st.pop();
            res.push_back(node->val);
            if (node->right) st.push(node->right);
            if (node->left) st.push(node->left);
        }
        return res;
    }
    ```
    
3. DFS inorder
    1. left → node → right
    2. time `O(n)`, due to visiting all nodes
    3. space `O(n)`, due to skewed tree, otherwise `O(height)`
    4. use inorder when the operation consider the ascending val order (when using BST)
    
    ```cpp
    // recursive
    void inorder_helper(TreeNode* root, std::vector<int>& inorder) {
        if (!root) return;
        inorder_helper(root->left, inorder);
        inorder.push_back(root->val);
        inorder_helper(root->right, inorder);
    }

    // iterative
    std::vector<int> inorder(TreeNode* root) {
        std::vector<int> res;
        std::stack<TreeNode*> st;
        TreeNode* node = root;
        while (node || !st.empty()) {
            while (node) {
                st.push(node);
                node = node->left;
            }
            node = st.top(); st.pop();
            res.push_back(node->val);
            node = node->right;
        }
        return res;
    }
    ```
    
4. DFS postorder
    1. left → right → node
    2. time `O(n)`, due to visiting all nodes
    3. space `O(n)`, due to skewed tree, otherwise `O(height)`
    4. use postorder when the operation has a bottom-up flow
    
    ```cpp
    // recursive
    void postorder_helper(TreeNode* root, std::vector<int>& postorder) {
        if (!root) return;
        postorder_helper(root->left, postorder);
        postorder_helper(root->right, postorder);
        postorder.push_back(root->val);
    }

    // iterative
    std::vector<int> postorder(TreeNode* root) {
        std::vector<int> res;
        if (!root) return res;
        std::stack<TreeNode*> st;
        st.push(root);
        std::vector<int> rev;
        while (!st.empty()) {
            TreeNode* node = st.top(); st.pop();
            rev.push_back(node->val);
            if (node->left) st.push(node->left);
            if (node->right) st.push(node->right);
        }
        std::reverse(rev.begin(), rev.end());
        return rev;
    }
    ```
    
5. Morris inorder traversal
    1. in normal inorder traversal, we store cur node in stack then got to left, but morris traversal store cur node by link it from its predecessor
    2. link the predecessor and successor
        1. only the node with left subtree will need its predecessor to link to itself (as successor)
        2. the predecessor node is the right-most node in left subtree
        3. due to linking, will modify the tree data temporary
    3. time `O(n)`, due to visiting all nodes
        - each node is traversed 4 times at most
            - single upper node find its predecessor (and build its link) will pass through cur node
            - traverse to cur node
            - from cur node's predecessor, go back to cur node (this time will unlink the predecessor with cur node)
            - single upper node find its predecessor (and destroy its link) will pass through cur node
        - why finding predecessor would not increase time complexity
            - intuitive way to think about it is that 
              1. more right branch nodes will make link route longer, but more right branch nodes will not increase the need of more link route for themself
              2. only more left branch nodes will need more link route for themself, but more left branch nodes only need the short link route for themself
    4. space `O(1)`, due to no cost for stack, queue or recursion stack (not counting cost of output list)
    
    ```cpp
    std::vector<int> morris_inorder(TreeNode* root) {
        std::vector<int> res;
        TreeNode* cur = root;
        while (cur) {
            if (!cur->left) {
                res.push_back(cur->val);
                cur = cur->right;
            } else {
                TreeNode* pred = cur->left;
                while (pred->right && pred->right != cur)
                    pred = pred->right;
                if (!pred->right) {
                    pred->right = cur;
                    cur = cur->left;
                } else {
                    res.push_back(cur->val);
                    pred->right = nullptr;
                    cur = cur->right;
                }
            }
        }
        return res;
    }
    ```
    
6. Divide and Conquer
    - use tree’s properties
        1. idx range (l and r)
            - the idx from preorder array, inorder array, postorder array, or linked list
                - BST inorder’s val has ascending order
        2. val range (lower and upper)
            - validate the range of value (BST only)
        3. relation between parent node and child node

## pattern

- divide and conquer
    - two branch top-down
        - operation can
            - start from two diff trees
            - start from both side’s children (left subtree and right subtree)
    - re-build tree (top-down)
        - use divide and conquer
            - parameters we need to pass down: 2 left_idx, 2 right_idx, node_count
            - when using preorder/postorder and inorder to re-build
                - notice: val in tree should be unique
                - preorder’s first is root
                - postorder’s last is root
                - inorder’s middle is root (idx not sure yet)
                    - inorder: left subtree + root + right subtree
                    - we already have the root’s val (from preorder or postorder)
                    - now use hashmap to quickly find the val’s idx in inorder array
            - when using preorder and postorder to re-build
                - notice: val in tree should be unique
                - preorder’s first is root
                    - Root + [Left Subtree] + Right Subtree
                    - Root + [Left Subtree Root + Rest of Left Subtree] + Right Subtree
                - postorder’s last is root
                    - [Left Subtree] + Right Subtree + Root
                    - [Rest of Left Subtree + Left Subtree Root] + Right Subtree + Root 
                - can not confirm the re-built tree is the only possible tree
                - only if we know every internal/non-leaf node in tree has 2 children (full binary tree), then we can try to re-build the full binary tree version as only possible tree
                - notice: when node_count is 1, just return that node. no need for building left and right subtrees
    - re-build BST
        - top-down approach
            - with preorder
                - in preorder: first node is root of tree/subtree
                - BST properties: use the lower and upper bound of val to check node is valid or not
            - with inorder
                - use the left and right ptr to locate mid/root node
                    - array is already ascending order (BST's inorder res)
                - build mid/root first, then left subtree, right subtree last
                - notice: res tree will be height-balanced
        - inorder approach
            - use the left and right ptr to locate mid/root node
                - array is already ascending order (BST's inorder res)
            - build left subtree first, then root, right subtree last
                - can use an idx or ptr to retrieve val for building node
            - notice: res tree will be height-balanced
    - use BST attributes
        - left subtree nodes’ val is less than the node’s val
        - right subtree nodes’ val is larger than the node’s val
        - left or right subtree is also BST
        - BST inorder traversal is ascending order
        - predecessor and successor
        - delete node in BST
            - use the delete node's left child as substitute
            - we do not modify node's left child's left child and right child directly
            - we link the delete node's right child to delete node's predecessor’s right side
                - notice: predecessor in the right-most node in left subtree
            - how to understand this method
              - treat delete node as old tree
              - we replace this old tree with its left subtree
              - we need to find the largest node in its left subtree (which is delete node's pred)
              - link delete node's right subtree to this pred node's right side
            ```cpp
            TreeNode* deleteNode(TreeNode* root, int key) {
                if (!root) return nullptr;
                if (root->val == key) {
                    if (!root->left) return root->right;
                    TreeNode* pred = root->left;
                    while (pred->right) pred = pred->right;
                    pred->right = root->right;
                    return root->left;
                } else if (root->val < key) {
                    root->right = deleteNode(root->right, key);
                } else {
                    root->left = deleteNode(root->left, key);
                }
                return root;
            }
            ```
    - find LCA
        - LCA in BST
            1. p and q node are inside LCA’s left and right subtree
            2. p is the root/LCA, and q is inside p's left or right subtree
        - find LCA without root and each node has stored parent info
            - use two pointers
            - [ p->LCA + LCA->root + q->LCA ] is same as [ q->LCA + LCA->root + p->LCA ]
    - populate next ptr
        - perfect binary tree
            - two nodes between the link (next ptr)
                - belong to same parent
                - belong to parents next to each other
            - when at level 0, need to build next ptrs at level 1 and so on
            - maintain
              - upper level's start ptr
              - upper level's cur ptr
        - regular binary tree
            - maintain
                - upper level’s start ptr
                - upper level's cur ptr
                - lower level’s start ptr
                - lower level’s end ptr
    - unique BST
        - concept of Catalan Number
        - determine the root first
        - then we can know the choices of left subtree and right subtree
- preorder
    - use preorder when the operation has a top-down flow
      - eg. invert
    - turn tree to string
        ```cpp
        // dfs preorder
        std::string build(TreeNode* node) {
            if (!node) return "#null";
            std::string cur = "#" + std::to_string(node->val);
            std::string left = build(node->left);
            std::string right = build(node->right);
            return cur + left + right;
        }
        ```
    - record leaves
        ```cpp
        // dfs preorder
        std::vector<int> leaves;
        std::stack<TreeNode*> st;
        st.push(root);
        while (!st.empty()) {
            TreeNode* node = st.top(); st.pop();
            if (node != root && !node->left && !node->right)
                leaves.push_back(node->val);
            if (node->right) st.push(node->right);
            if (node->left) st.push(node->left);
        }
        ```
- inorder
    - use inorder when the operation consider the ascending val order (when using BST)
- postorder
    - use postorder when the operation has a bottom-up flow
    - we will utilize the return value
- bfs
    - level related
    - notice the length of the queue means the count of cur level's nodes (we can process all nodes in same level in a for loop)
    - build a child_parent hashmap
        ```cpp
        // bfs
        #include <unordered_map>
        std::unordered_map<TreeNode*, TreeNode*> node_parent(TreeNode* root) {
            std::unordered_map<TreeNode*, TreeNode*> parent;
            if (!root) return parent;
            parent[root] = nullptr;
            std::queue<TreeNode*> q;
            q.push(root);
            while (!q.empty()) {
                TreeNode* node = q.front(); q.pop();
                for (TreeNode* child : {node->left, node->right}) {
                    if (child) {
                        q.push(child);
                        parent[child] = node;
                    }
                }
            }
            return parent;
        }
        ```
    - assign idx
        - using bfs
        - next level’s left is cur * 2
        - next level’s right is cur * 2 + 1
        - eg. root is 1, left child is 2, right child is 3
        - notice: we can turn this idx to binary to see we choose to go left or go right (eg. idx 11 is 1011 means we go left then go right then go right again (check the route from 2nd large digit to last digit))
        ```cpp
        #include <unordered_map>
        std::unordered_map<TreeNode*, int> assign_idx(TreeNode* root) {
            std::unordered_map<TreeNode*, int> idx;
            if (!root) return idx;
            std::queue<std::pair<TreeNode*, int>> q;
            q.push({root, 1});
            while (!q.empty()) {
                auto [node, i] = q.front(); q.pop();
                idx[node] = i;
                if (node->left) q.push({node->left, i * 2});
                if (node->right) q.push({node->right, i * 2 + 1});
            }
            return idx;
        }
        ```
    - assign coordinates
        - using bfs
        - treat root node as (0, 0), then let row and col to define (x, y)
        - often use hashmap to record as assistance
            - key can be row or col, value is list
        ```cpp
        #include <unordered_map>
        std::unordered_map<int, std::vector<int>> assign_coords(TreeNode* root) {
            std::unordered_map<int, std::vector<int>> row_vals;
            if (!root) return row_vals;
            std::queue<std::tuple<TreeNode*, int, int>> q;
            q.push({root, 0, 0});
            while (!q.empty()) {
                auto [node, row, col] = q.front(); q.pop();
                row_vals[row].push_back(node->val);
                if (node->left) q.push({node->left, row + 1, col - 1});
                if (node->right) q.push({node->right, row + 1, col + 1});
            }
            return row_vals;
        }
        ```
