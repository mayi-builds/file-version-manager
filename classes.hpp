#include <iostream>
#include <ctime>
#include <string>
#include <vector>
using namespace std;

class TreeNode {
public:
    int version_id;
    string content;
    string message;
    time_t created_timestamp;
    time_t snapshot_timestamp;
    TreeNode* parent;
    vector<TreeNode*> children;

    TreeNode()
        : version_id(0), content(""),
          message("Starting Version of this file."),
          created_timestamp(time(0)), snapshot_timestamp(time(0)),
          parent(NULL) {}
    TreeNode(int vid, TreeNode* par)
        : version_id(vid), content(par->content), message(""),
          created_timestamp(time(0)), snapshot_timestamp(0), parent(par) {}
};

/* IntNodeMap : HashMap with each Entry (int -> TreeNode*) */

class IntNodeMap {
    struct Entry {
        int key;
        TreeNode* value;
        Entry* next;
        Entry(int k, TreeNode* v) : key(k), value(v), next(NULL) {}
    };

    vector<Entry*> buckets; // array of linked lists
    int capacity;

    int hash(int key) { return key % capacity; }

public:
    IntNodeMap(int cap = 103) { // Let default 103 buckets
        capacity = cap;
        buckets.assign(capacity, NULL);
    }

    /* Insert a Key Value pair into this HashMap */
    void put(int key, TreeNode* value) {
        int idx = hash(key);
        Entry* cur = buckets[idx];
        while (cur != NULL) {
            if (cur->key == key) { // update if exists
                cur->value = value;
                return;
            }
            cur = cur->next;
        }
        // insert at head if doesn't exists
        Entry* newEntry = new Entry(key, value);
        newEntry->next = buckets[idx];
        buckets[idx] = newEntry;
    }

    /* Accessing a value refering to a key in this HashMap */
    TreeNode* get(int key) {
        int idx = hash(key);
        Entry* cur = buckets[idx];
        while (cur != NULL) {
            if (cur->key == key)
                return cur->value;
            cur = cur->next;
        }
        return NULL;
    }

    /* Checking the existence of key in this hashMap */
    bool contains(int key) { return get(key) != NULL; }
};

class File {
public:
    TreeNode* root;
    TreeNode* active_version;
    IntNodeMap version_map;
    int total_versions;
    time_t last_modified;

    /* Create the root version of File */
    void create() {
        root = new TreeNode();
        active_version = root;
        version_map.put(0, root);
        total_versions = 1;
        last_modified = time(0);
    }

    /* Read content from active version of this file */
    void read() {
        cout << "Current Active Version:" << active_version->version_id << '\n';
        cout << active_version->content << '\n';
    }

    /* Insert content to active version of this file */
    void insert(string text) {
        if (active_version->snapshot_timestamp != 0) {
            /* If active version of this file is already a snapshot we create a new version such that active version is parent of new version */
            TreeNode* newver = new TreeNode(total_versions, active_version);

            newver->content += text;
            active_version->children.push_back(newver);

            version_map.put(total_versions, newver);
            total_versions++;
            active_version = newver;
        } else
            active_version->content += text;

        last_modified = time(0);
    }

    /* Overwrite(Update) content of active version of this file */
    void update(string text) {
        if (active_version->snapshot_timestamp != 0) {
            /* If active version of this file is already a snapshot we create a new version such that active version is parent of new version */

            TreeNode* newver = new TreeNode(total_versions, active_version);

            newver->content = text;
            active_version->children.push_back(newver);

            version_map.put(total_versions, newver);
            total_versions++;

            active_version = newver;
        } else
            active_version->content = text;
        last_modified = time(0);
    }

    /* Marking active version of file as Snapshot (Immutable)*/
    void snapshot(string text) {
        active_version->snapshot_timestamp = time(0);
        active_version->message = text;
        last_modified = time(0);
    }

    /* changing my current active version to specified version */
    void rollback(int vid = -1) {
        if (vid >= 0) {
            TreeNode* node = version_map.get(vid);
            if (node)
                active_version = node;
            else
                cout << "Version not found!\n";
        } else if (active_version != root) {
            // Rollback to its parent by default
            active_version = active_version->parent;
        }
    }

    void history() {
        // Storing Snapshot versions from current active version to root (All ancestors)
        vector<TreeNode*> snaps;
        TreeNode* presentVer = active_version;
        while (presentVer != NULL) {
            // check if present Verion is snapshot or not
            if (presentVer->snapshot_timestamp)
                snaps.push_back(presentVer);
            presentVer = presentVer->parent;
        }
        // printing all
        for (int i = snaps.size() - 1; i >= 0; i--) {
            cout << "Version ID: " << snaps[i]->version_id << '\n';
            cout << "Message: " << snaps[i]->message << '\n';
            cout << "Snapshot Time: " << ctime(&snaps[i]->snapshot_timestamp)
                 << '\n';
        }
    }
};

/* some kinda buffer to handle(ignore) unneccesary argument(data) in commands by user */
#define BUFF                                                                   \
    string temp;                                                               \
    getline(cin, temp)

/* checking existence of a file with given file name by user */
#define CHECK                                                                  \
    cin >> filename;                                                           \
    if (!allfiles.contains(filename)) {                                        \
        cout << "No file with name " << filename << " exist!\n";               \
        BUFF;                                                                  \
        continue;                                                              \
    }

/* StrFileMap : HashMap with each entry storing string -> File */
class StrFileMap {
public:
    struct Entry {
        string key;
        File value;
        Entry* next;
        Entry(string k, File v) : key(k), value(v), next(NULL) {}
    };

    vector<Entry*> buckets;
    int capacity;

    // Simple string hash function (djb2-like)
    unsigned long hash(string s) {
        unsigned long h = 5381;
        for (char c : s) {
            h = ((h << 5) + h) + c; // h * 33 + c
        }
        return h % capacity;
    }

public:
    StrFileMap(int cap = 103) {
        capacity = cap;
        buckets.assign(capacity, NULL);
    }

    /*Inserting or Updating */
    void put(string key, File value) {
        int idx = hash(key);
        Entry* cur = buckets[idx];
        while (cur != NULL) {
            if (cur->key == key) { // update existing
                cur->value = value;
                return;
            }
            cur = cur->next;
        }
        // insert new entry if doesn't exist
        Entry* newEntry = new Entry(key, value);
        newEntry->next = buckets[idx];
        buckets[idx] = newEntry;
    }

    /* Accessing Value using Key */
    File* get(string key) {
        int idx = hash(key);
        Entry* cur = buckets[idx];
        while (cur != NULL) {
            if (cur->key == key)
                return &cur->value;
            cur = cur->next;
        }
        return NULL;
    }

    /* Checking existence of File with filename as key */
    bool contains(string key) { return get(key) != NULL; }
};

/* Custom MaxHeap */
class MaxHeap {
    struct Node {
        string filename;
        long long key; // can be last_modified or version_count
        Node(string f, long long k) : filename(f), key(k) {}
    };

    vector<Node> heap;

    bool compare(Node a, Node b) { return a.key > b.key; }

    void heapifyUp(int idx) {
        while (idx > 0) {
            int parent = (idx - 1) / 2;
            if (compare(heap[idx], heap[parent])) {
                swap(heap[idx], heap[parent]);
                idx = parent;
            } else
                break;
        }
    }

    void heapifyDown(int idx) {
        int n = heap.size();
        while (true) {
            int left = 2 * idx + 1;
            int right = 2 * idx + 2;
            int largest = idx;

            if (left < n && compare(heap[left], heap[largest]))
                largest = left;
            if (right < n && compare(heap[right], heap[largest]))
                largest = right;

            if (largest != idx) {
                swap(heap[idx], heap[largest]);
                idx = largest;
            } else
                break;
        }
    }

public:
    void push(string filename, long long key) {
        heap.push_back(Node(filename, key));
        heapifyUp(heap.size() - 1);
    }

    Node pop() {
        Node top = heap[0];
        heap[0] = heap.back();
        heap.pop_back();
        heapifyDown(0);
        return top;
    }

    bool empty() { return heap.empty(); }
};
