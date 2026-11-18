# EV Charging Station Placement and Trip Planning System
## Test Cases and Experimental Verification Results

### 1. Scenario 1 – Normal Operation (Dehradun Network)

| Test ID | Test Description | Expected Result | Actual Result | Status |
|---|---|---|---|---|
| TC01 | Load predefined Dehradun network | 10 locations and 16 roads should load | 10 locations, 16 roads loaded successfully | PASS |
| TC02 | Find shortest route using Dijkstra (Clock Tower -> DIT University) | Shortest route should be displayed with distance | Clock Tower → Pacific Mall → DIT University, 18.50 km | PASS |
| TC03 | Find fastest route using Dijkstra (Clock Tower -> DIT University) | Minimum-time route should be displayed | Clock Tower → Pacific Mall → DIT University, 0.57 hours | PASS |
| TC04 | Find shortest route using Bellman-Ford | Shortest distance should match Dijkstra | Clock Tower → Pacific Mall → DIT University, 18.50 km | PASS |

### 2. Scenario 2 – Capacity and Budget Constraints

| Test ID | Test Description | Expected Result | Actual Result | Status |
|---|---|---|---|---|
| TC05 | Run station placement with insufficient budget | No station should be installed | 0 stations installed, capital preserved | PASS |
| TC06 | Run station placement with budget = ₹2000, cost = ₹1000 | 2 stations selected | 2 stations installed (Clock Tower, etc.) | PASS |
| TC07 | Compare connectivity vs demand strategies | Both respect budget and max charger caps | Both selected 2 stations, 20 chargers, ₹2000 cost | PASS |
| TC08 | Compare strategy priority scores | Scores reflect connectivity vs demand priorities | Connectivity: 183.0; Demand: 185.0 | PASS |

### 3. Scenario 3 – Large Data Stress Test

| Test ID | Test Description | Expected Result | Actual Result | Status |
|---|---|---|---|---|
| TC09 | Generate synthetic network | 50 locations created | 50 locations created | PASS |
| TC10 | Generate road connections | 184 road connections created | 184 road connections verified | PASS |
| TC11 | Compare Dijkstra and Bellman-Ford | Matching shortest distances | PASS: Both algorithms produced matching distances | PASS |
| TC12 | Measure execution time (5,000 reps) | Real CPU time measured | Dijkstra: ~0.0120 ms; Bellman-Ford: ~0.0132 ms | PASS |
| TC13 | Structural memory estimation | Memory consumption reported | Graph: 63,608 bytes; Arrays: 600 bytes | PASS |

### 4. Advanced DAA Algorithms (Sorting & Searching)

| Test ID | Test Description | Expected Result | Actual Result | Status |
|---|---|---|---|---|
| TC14 | Quick Sort on unsorted array | Ascending sorted array in $O(N \log N)$ | Correctly sorted array | PASS |
| TC15 | Merge Sort on unsorted array | Ascending sorted array in $O(N \log N)$ | Correctly sorted array | PASS |
| TC16 | Iterative Binary Search | Return index of target key or -1 | Found at index 3 ($O(\log N)$) | PASS |
| TC17 | Recursive Binary Search | Return matching index | Found at index 3 ($O(\log N)$) | PASS |

### 5. GUI & REST Integration

| Test ID | Test Description | Expected Result | Actual Result | Status |
|---|---|---|---|---|
| TC18 | HTTP GET `/api/network` | JSON containing 10 nodes and 16 edges | HTTP 200 OK, valid JSON payload | PASS |
| TC19 | HTTP POST `/api/route` | JSON with path, distance, time, and telemetry | HTTP 200 OK, path [0, 4, 6], 18.5 km | PASS |
| TC20 | Static Web Asset Serving | Serve index.html, style.css, app.js | HTTP 200 OK with correct MIME types | PASS |
