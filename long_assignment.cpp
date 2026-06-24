#include <iostream>
#include <ctime>
#include <string>
#include <vector>
#include "classes.hpp"
using namespace std;

int main() {
    StrFileMap allfiles; // HashMap with Filename(string) as Key and File(object) as Value
	
    while (true) {
        // Refering Prompt User
        cout << ">>>";

        // command name and filename
        string cmdName, filename;
        cin >> cmdName;

        if (cmdName == "EXIT")
            break;

        if (cmdName == "CREATE") {
            cin >> filename;
            File newfile;
            if (allfiles.contains(filename)) {
                cout << "File with name " << filename << " already exist\n";
                continue;
            }
            allfiles.put(filename, newfile);
            allfiles.get(filename)->create();
            continue;
        }

        if (cmdName == "READ") {
            CHECK
            File* f = allfiles.get(filename);
            f->read();
        } else if (cmdName == "INSERT") {
            CHECK
            string content;
            if (allfiles.get(filename)->active_version->content == "")
                cin.ignore();
            getline(cin, content);
            allfiles.get(filename)->insert(content);
        } else if (cmdName == "UPDATE") {
            CHECK
            string content;
            cin.ignore();
            getline(cin, content);
            allfiles.get(filename)->update(content);
        } else if (cmdName == "SNAPSHOT") {
            CHECK
            string message;
            cin.ignore();
            getline(cin, message);
            allfiles.get(filename)->snapshot(message);
        } else if (cmdName == "ROLLBACK") {
            CHECK
            int versionID = -1;
            string vid;
            getline(cin, vid);
            if (!vid.empty())
                versionID = stoi(vid);
            allfiles.get(filename)->rollback(versionID);
        } else if (cmdName == "HISTORY") {
            CHECK
            allfiles.get(filename)->history();
        } else if (cmdName == "RECENT_FILES") {
            int num;
            cin >> num;

            MaxHeap heap;
            // traverse all files
            for (int i = 0; i < 103; i++) {
                auto cur = allfiles.buckets[i];
                while (cur != NULL) {
                    heap.push(cur->key, cur->value.last_modified);
                    cur = cur->next;
                }
            }
            cout << "Most recent " << num << " files:\n";
            for (int i = 0; i < num && !heap.empty(); i++) {
                auto node = heap.pop();
                time_t modifyTime = node.key;
                cout << node.filename << " last modified: " << ctime(&modifyTime);
            }
        } else if (cmdName == "BIGGEST_TREES") {
            int num;
            cin >> num;
            MaxHeap heap;
            for (int i = 0; i < 103; i++) {
                auto cur = allfiles.buckets[i];
                while (cur != NULL) {
                    heap.push(cur->key, cur->value.total_versions);
                    cur = cur->next;
                }
            }
            cout << "Top " << num << " files by version count:\n";
            for (int i = 0; i < num && !heap.empty(); i++) {
                auto node = heap.pop();
                cout << node.filename << " (versions: " << node.key << ")\n";
            }
        } else {
            BUFF;
            cout << "INVALID Command!\n";
        }
    }
    return 0;
}
