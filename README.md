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
ANT25_Habit/
├── MakeChart/
│   ├── MakeChart.ipynb
│   ├── ComparisonChart.ipynb
├── Simulation/
│   ├── main.cpp
│   ├── input.txt
├── Example/
│   ├── NoneSample/
│       ├── no-habit/
│           ├── agent_utility.zip/
│       ├── habit(1)/
│           ├── agent_utility.zip/
│   ├── SOCSample/
│       ├── (Soc1)habit/
│           ├── agent_utility.zip/
│       ├── (Soc1)no-habit/
│           ├── agent_utility.zip/
├── Release/
│   ├── Simulation/
│       ├── Simulation.zip/
│   ├── MakeChart/
│       ├── MakeChart.zip/
├── LICENSE
├── README.md
```
- **MakeChart**: Python scripts for visualizing traffic patterns and analyzing agent behaviors.
- **Simulation**: C++ program for simulating agent-based travel behavior on road and rail networks.
- **Release**: Packaged distributions of the simulation and charting tools for easy deployment.
- **Example**: Contains example outputs and datasets for testing.

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
Simulation Output Data
- Format: CSV

Generated Visualizations
- Example Chart
   - Average Utility Score Over Iterations
     ![image](https://github.com/user-attachments/assets/ba28eab6-9bf5-4f39-8bbc-c95b354c384b)
   - Agent Count For Zone Over Iteration
     ![image](https://github.com/user-attachments/assets/75b86c61-2a7b-482a-b169-b8b44ce2de42)
   - Average Travel Time For Each Path Over Iterations
     ![image](https://github.com/user-attachments/assets/10f82d71-af34-4c03-b8f7-200f65c002bf)
   - Average Travel Time For Zone Over Iterations
     ![image](https://github.com/user-attachments/assets/6c8a487a-ee39-4af1-b43c-d4c118d6523c)
   - Average Selected Location Frequency Over Iterations
     ![image](https://github.com/user-attachments/assets/0c338f19-3d15-4e37-9710-601e66a8050b)
   - Average Mode Share Over Iterations
     ![image](https://github.com/user-attachments/assets/a9bd2d1f-9177-4e26-a685-e008a5381e22)
   - Average Habit Utility Over Iterations
     ![image](https://github.com/user-attachments/assets/abc6bd35-b53b-457b-b5b4-87bc4e2cb988)
   - Route Distribution Over Iterations
     ![image](https://github.com/user-attachments/assets/d823ed7d-1ddf-4f61-8ffa-eb8a3096dba5)
   - Number_of_Agents_with_Habit_Utility
     ![image](https://github.com/user-attachments/assets/3972e987-e9eb-4d97-9f06-77bc33ea9adb)

## License
This project is licensed under the MIT License. You are free to use, modify, and distribute this project under the terms of the license.
