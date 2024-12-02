# Paper Title: **Adding habit effects on scoring function in agent based transport simulation**
**Authors:** Chansung Kima, Kyoungju Kimb, Jiyoung Park  
**Publication:** <!--[Conference/Journal Name], [Year]-->  
**DOI:** <!--[DOI Link or "TBA"]-->  
**GitHub Repository:** [ANT25_Habit](https://github.com/kj1241/ANT25_Habit)  

## Overview
This repository contains the implementation of a C++ agent-based simulation and a Python-based analysis tool used in the study "Adding habit effects on scoring function in agent based transport simulation". The study evaluates the impact of SOC (Social Overhead Capital) investments, such as road and rail network expansions, on individual travel behaviors and overall traffic system efficiency.

Key Contributions:
1. C++ Agent-Based Simulation: 
   - Models individual travel behaviors, including departure time, route selection, and habitual preferences.
   - Simulates road and rail network expansions to analyze their effects on congestion and travel patterns.
   - Provides outputs for agent-level travel data, including utility scores and route choices.
2. Python Analysis Tool:
   - Processes simulation data to generate insights and visualizations.
   - Evaluates the effectiveness of infrastructure investments through charts and graphs.

## Repository Structure
```

```
- Python_Graph/: Python scripts for visualizing traffic patterns and analyzing agent behaviors.
- Cpp_Simulation/: C++ program for simulating agent-based travel behavior on road and rail networks.
- data/: Contains input datasets and sample output results for testing.

## Installation


## Usage
1. Setup Simulation:
   - Define network nodes (e.g., stations, intersections) and links (e.g., roads, railways) in ''input.txt''
   - Initialize agent behaviors with parameters like departure times, habitual preferences, and route options.
2. Run C++ Simulation:
   - Simulate agent travel over defined iterations.
   - Outputs include travel times, congestion patterns, and utility scores for each agent.
3. Analyze with Python:
   - Use the Python analysis tool to generate insights and visualizations from simulation outputs.

## Features
### C++ Agent-Based Simulation
- Models dynamic agent behavior with:
  - Departure points, routes, timing, and habitual preferences.
- Supports scenario analysis:
  - Road network expansions: Evaluates the impact of new or widened roads.
  - Rail network expansions: Measures the effectiveness of rail infrastructure improvements.
- Reassigns the bottom 10% of agents to achieve **Stochastic User Equilibrium (SUE)** by adjusting their departure locations, routes, and departure times.
  
### Python Graph Analysis
- Visualizes trends in:
- Evaluates the overall impact of infrastructure investments through intuitive charts and graphs.




## Example Results


## License
This project is licensed under the MIT License. You are free to use, modify, and distribute this project under the terms of the license.


