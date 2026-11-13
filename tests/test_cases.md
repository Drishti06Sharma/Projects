# EV Charging Station Placement and Trip Planning System
## Test Cases and Results

### 1. Scenario 1 – Normal Operation

| Test ID | Test Description | Expected Result | Actual Result | Status |
|---|---|---|---|---|
| TC01 | Load predefined Dehradun network | 10 locations should load | 10 locations loaded successfully | PASS |
| TC02 | Find shortest route using Dijkstra | Shortest route should be displayed | Clock Tower → Pacific Mall → DIT University, 18.50 km | PASS |
| TC03 | Find fastest route using Dijkstra | Minimum-time route should be displayed | Same route, 0.57 hours | PASS |
| TC04 | Find shortest route using Bellman-Ford | Shortest distance should be calculated | 18.50 km | PASS |

### 2. Scenario 2 – Capacity and Budget Constraints

| Test ID | Test Description | Expected Result | Actual Result | Status |
|---|---|---|---|---|
| TC05 | Run station placement with insufficient budget | No station should be installed | 0 stations installed | PASS |
| TC06 | Run station placement with sufficient budget | Affordable station should be selected | 1 station installed at Clock Tower | PASS |
| TC07 | Compare connectivity-based and demand-based strategies | Both strategies should respect budget and capacity | Both selected 2 stations, 20 chargers and cost 2000 | PASS |
| TC08 | Compare strategy priority scores | Scores should reflect the supplied demand and connectivity values | Connectivity: 185; Demand: 195 | PASS |

### 3. Scenario 3 – Large Data Stress Test

| Test ID | Test Description | Expected Result | Actual Result | Status |
|---|---|---|---|---|
| TC09 | Generate a large synthetic network | 50 locations should be created | 50 locations created | PASS |
| TC10 | Generate road connections | Network should contain multiple connections | 184 road connections | PASS |
| TC11 | Compare Dijkstra and Bellman-Ford | Both should calculate matching shortest distances | Matching distances verified | PASS |
| TC12 | Measure execution time | Average execution time should be displayed | Dijkstra: 0.0128 ms; Bellman-Ford: 0.0136 ms | PASS |
| TC13 | Display complexity information | Time complexity should be reported | Both algorithm complexities displayed | PASS |

### 4. Additional Input Validation

| Test ID | Test Description | Actual Result | Status |
|---|---|---|---|
| TC14 | Attempt to add a duplicate location | Duplicate location rejected | PASS |
| TC15 | Validate road input | Input validation was checked during testing | PASS |

### 5. Benchmark Summary

- Network size: 50 locations
- Road connections: 184
- Algorithm repetitions: 5000
- Dijkstra average execution time: 0.0128 ms
- Bellman-Ford average execution time: 0.0136 ms
- Correctness verification: Both algorithms produced matching distances.

### 6. Testing Notes

1. The Dehradun road network uses illustrative distance and travel-time estimates.
2. Demand scores used in strategy comparison are synthetic inputs.
3. The large-data benchmark uses a synthetic graph.
4. Execution times depend on the system, compiler and hardware.
5. Memory values are structural estimates, not measured peak memory usage.
6. The stress test does not modify the original Dehradun network.

## Conclusion

The implemented system successfully completed the tested normal-operation, capacity-constraint and large-data scenarios. The route-finding algorithms produced matching shortest-distance results, and the charging-station strategies were compared under the same budget and capacity constraints.
