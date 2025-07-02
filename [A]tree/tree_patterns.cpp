# Tree Patterns (with C++ code)

This section explains common tree problem-solving patterns, with code samples for each.

---

## 1. **Divide and Conquer**

Divide the problem into subproblems, solve them recursively, and combine results.

### (a) Two-branch top-down

- **Example:** Check if two trees are identical

```cpp
bool isSameTree(TreeNode* p, TreeNode* q) {
    if (!p && !q) return true;
    if (!p || !q || p->val != q->val) return false;
    return isSameTree(p->left, q->left) && isSameTree(p->right, q->right);
}
```

---

### (b) Tree Reconstruction: Top-Down

- **Example:** Build binary tree from preorder and inorder

```cpp
TreeNode* buildTreeHelper(
    std::vector<int>& preorder, int preStart, int preEnd,
    std::vector<int>& inorder, int inStart, int inEnd,
    std::unordered_map<int, int>& inMap
) {
    if (preStart > preEnd || inStart > inEnd) return nullptr;
    int rootVal = preorder[preStart];
    TreeNode* root = new TreeNode(rootVal);
    int inRoot = inMap[rootVal];
    int numsLeft = inRoot - inStart;
    root->left = buildTreeHelper(preorder, preStart+1, preStart+numsLeft, inorder, inStart, inRoot-1, inMap);
    root->right = buildTreeHelper(preorder, preStart+numsLeft+1, preEnd, inorder, inRoot+1, inEnd, inMap);
    return root;
}

TreeNode* buildTree(std::vector<int>& preorder, std::vector<int>& inorder) {
    std::unordered_map<int, int> inMap;
    for (int i = 0; i < inorder.size(); ++i)
        inMap[inorder[i]] = i;
    return buildTreeHelper(preorder, 0, preorder.size()-1, inorder, 0, inorder.size()-1, inMap);
}
```

---

### (c) Rebuild BST from preorder (top-down, value bounds)

```cpp
TreeNode* buildBST(std::vector<int>& preorder, int& idx, int bound) {
    if (idx == preorder.size() || preorder[idx] > bound) return nullptr;
    TreeNode* root = new TreeNode(preorder[idx++]);
    root->left = buildBST(preorder, idx, root->val);
    root->right = buildBST(preorder, idx, bound);
    return root;
}
```

---

### (d) Use Divide & Conquer to find LCA (Lowest Common Ancestor)

```cpp
TreeNode* lowestCommonAncestor(TreeNode* root, TreeNode* p, TreeNode* q) {
    if (!root || root == p || root == q) return root;
    TreeNode* left = lowestCommonAncestor(root->left, p, q);
    TreeNode* right = lowestCommonAncestor(root->right, p, q);
    if (left && right) return root;
    return left ? left : right;
}
```

---

## 2. **Preorder Pattern**

- Use when top-down calculations or actions are needed (e.g., path sums, mirror/invert, serialization).

### (a) Mirror/Invert Binary Tree

```cpp
TreeNode* invertTree(TreeNode* root) {
    if (!root) return nullptr;
    std::swap(root->left, root->right);
    invertTree(root->left);
    invertTree(root->right);
    return root;
}
```

### (b) Serialize Tree to String (DFS Preorder)

```cpp
std::string serialize(TreeNode* node) {
    if (!node) return "#null";
    return "#" + std::to_string(node->val) + serialize(node->left) + serialize(node->right);
}
```

---

## 3. **Inorder Pattern**

- Use when working with BSTs or when need ascending order.

### (a) BST to Sorted Array (Inorder Traversal)

```cpp
void inorder(TreeNode* root, std::vector<int>& res) {
    if (!root) return;
    inorder(root->left, res);
    res.push_back(root->val);
    inorder(root->right, res);
}
```

---

## 4. **Postorder Pattern**

- Use when bottom-up computations are required (e.g., compute height, sizes, or delete tree).

### (a) Compute Tree Height

```cpp
int treeHeight(TreeNode* root) {
    if (!root) return -1;
    return 1 + std::max(treeHeight(root->left), treeHeight(root->right));
}
```

### (b) Delete Tree (free memory)

```cpp
void deleteTree(TreeNode* root) {
    if (!root) return;
    deleteTree(root->left);
    deleteTree(root->right);
    delete root;
}
```

---

## 5. **BFS Pattern**

- Use when a problem is level-based, or when you want shortest path in an unweighted tree.

### (a) Level Order Traversal

```cpp
std::vector<std::vector<int>> levelOrder(TreeNode* root) {
    std::vector<std::vector<int>> res;
    if (!root) return res;
    std::queue<TreeNode*> q;
    q.push(root);
    while (!q.empty()) {
        int sz = q.size();
        std::vector<int> lvl;
        for (int i = 0; i < sz; ++i) {
            TreeNode* node = q.front(); q.pop();
            lvl.push_back(node->val);
            if (node->left) q.push(node->left);
            if (node->right) q.push(node->right);
        }
        res.push_back(lvl);
    }
    return res;
}
```

### (b) Build Child-Parent Map (for upward traversal)

```cpp
#include <unordered_map>
std::unordered_map<TreeNode*, TreeNode*> buildParentMap(TreeNode* root) {
    std::unordered_map<TreeNode*, TreeNode*> parent;
    std::queue<TreeNode*> q;
    if (!root) return parent;
    parent[root] = nullptr;
    q.push(root);
    while (!q.empty()) {
        TreeNode* node = q.front(); q.pop();
        for (TreeNode* child : {node->left, node->right}) {
            if (child) {
                parent[child] = node;
                q.push(child);
            }
        }
    }
    return parent;
}
```

---

## 6. **Assign Indices/Coordinates (Heap-style/BFS)**

- Useful for problems where node position matters (e.g., width of tree, vertical order).

### Assign Heap Index

```cpp
#include <unordered_map>
std::unordered_map<TreeNode*, int> assignHeapIndex(TreeNode* root) {
    std::unordered_map<TreeNode*, int> idx;
    if (!root) return idx;
    std::queue<std::pair<TreeNode*, int>> q;
    q.push({root, 1});
    while (!q.empty()) {
        auto [node, i] = q.front(); q.pop();
        idx[node] = i;
        if (node->left) q.push({node->left, i*2});
        if (node->right) q.push({node->right, i*2+1});
    }
    return idx;
}
```

---

## 7. **Use BST Attributes**

- For BST-specific logic (search, insert, predecessor/successor, delete).

### (a) BST Search

```cpp
TreeNode* searchBST(TreeNode* root, int val) {
    if (!root || root->val == val) return root;
    if (val < root->val) return searchBST(root->left, val);
    return searchBST(root->right, val);
}
```

### (b) BST Delete

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

---

## 8. **Unique BST / Catalan Number**

- Used to count or generate all unique BSTs for `n` nodes.

```cpp
int numTrees(int n) {
    std::vector<int> dp(n+1, 0); dp[0] = 1;
    for (int nodes = 1; nodes <= n; ++nodes)
        for (int root = 1; root <= nodes; ++root)
            dp[nodes] += dp[root-1] * dp[nodes-root];
    return dp[n];
}
```

---

## 9. **Populate Next Pointers (Perfect Binary Tree BFS)**

```cpp
struct Node {
    int val;
    Node* left;
    Node* right;
    Node* next;
    Node(int val):val(val),left(nullptr),right(nullptr),next(nullptr){}
};

Node* connect(Node* root) {
    if (!root) return nullptr;
    std::queue<Node*> q;
    q.push(root);
    while (!q.empty()) {
        int sz = q.size();
        Node* prev = nullptr;
        for (int i = 0; i < sz; ++i) {
            Node* node = q.front(); q.pop();
            if (prev) prev->next = node;
            prev = node;
            if (node->left) q.push(node->left);
            if (node->right) q.push(node->right);
        }
        prev->next = nullptr;
    }
    return root;
}
```

---

**Summary Table**

| Pattern                     | Use cases                                    |
|-----------------------------|----------------------------------------------|
| Divide and Conquer          | Build/compare trees, LCA, recursion          |
| Preorder                    | Top-down ops, serialization, inversion       |
| Inorder                     | BST, ascending order                         |
| Postorder                   | Bottom-up, delete, compute size/height       |
| BFS                         | Level-based logic, shortest path, parent map |
| Assign indices/coordinates  | Position-based problems (width, vertical)    |
| BST attributes              | Search, insert, delete, predecessor/successor|
| Unique BST/Catalan          | Count/generate unique BSTs                   |
| Populate next pointers      | Link siblings in tree                        |
