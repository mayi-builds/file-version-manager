# Time-Travelling File System 🗂️

A Git-inspired, in-memory version control system built from scratch in C++ — no STL containers used. Manages versioned files with full branching, snapshot, and rollback support.

---

## Overview

This project implements a simplified version control system modelled after Git's core internals. Every file maintains a **tree of versions**, enabling historical inspection, branching, and rollback — all powered by custom-built data structures.

Built as part of COL106 (Data Structures & Algorithms) at IIT Delhi.

---

## Key Features

- **Versioned file management** — every write creates a traceable version node
- **Snapshot & Rollback** — mark immutable checkpoints and revert to any past state
- **Full version history** — traverse the version tree from active node to root
- **System-wide analytics** — query most recently modified files or files with the most versions
- **All data structures implemented from scratch** — no STL (Trees, HashMaps, Heaps)

---

## Data Structures Used

| Structure | Purpose |
|-----------|---------|
| Custom Tree | Models version history per file |
| Custom HashMap | O(1) version lookup by ID |
| Custom Heap | Tracks file metrics (recency, version count) |

---

## Supported Commands

### Core File Operations
| Command | Description |
|---------|-------------|
| `CREATE <filename>` | Creates a new file with an empty root version |
| `READ <filename>` | Displays content of the current active version |
| `INSERT <filename> <content>` | Appends content; creates new version if current is a snapshot |
| `UPDATE <filename> <content>` | Replaces content; follows same versioning logic as INSERT |
| `SNAPSHOT <filename> <message>` | Marks current version as immutable with a message and timestamp |
| `ROLLBACK <filename> [versionID]` | Reverts active pointer to specified version or parent |
| `HISTORY <filename>` | Lists all snapshots from active node to root with timestamps |

### System Analytics
| Command | Description |
|---------|-------------|
| `RECENT_FILES [num]` | Lists files sorted by last modification time |
| `BIGGEST_TREES [num]` | Lists files sorted by total version count |

### Terminate
| Command | Description |
|---------|-------------|
| `EXIT` | Shuts down the system |

---

## Sample Usage

```bash
CREATE F1
INSERT F1 Hello
SNAPSHOT F1 "Making it immutable"
UPDATE F1 "How are you"
CREATE F2
HISTORY F1
ROLLBACK F1 1
READ F1
RECENT_FILES 2
BIGGEST_TREES 1
EXIT
```

---

## How to Run

**Prerequisites:** `g++` installed

```bash
# Clone the repo
git clone https://github.com/YOUR_USERNAME/file-version-manager.git
cd file-version-manager

# Compile
bash compileCode.sh

# Run
./runcode
```

---

## Project Structure

```
.
├── long_assignment.cpp   # Main driver — command parsing and system logic
├── classes.hpp           # All custom data structures (Tree, HashMap, Heap)
├── compileCode.sh        # Compilation script
└── README.md
```

---

## Implementation Highlights

- Version immutability is enforced at the data structure level — snapshotted nodes cannot be modified
- Rollback preserves the full version tree; no history is ever deleted
- HashMap ensures O(1) access to any version by ID
- Heap maintains sorted file metrics without repeated sorting

---

## Author

**Jagarapu Shreemayi**  
B.Tech, Mathematics & Computing  
Indian Institute of Technology Delhi
