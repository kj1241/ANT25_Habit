# Paper Title: **Incorporating the habit effect on scoring function in agent based transport simulation**
**Authors:** Chansung Kim, Kyoungju Kim, Jiyoung Park  
**Publication:** <!--[Conference/Journal Name], [Year]-->  
**DOI:** <!--[DOI Link or "TBA"]-->  
**GitHub Repository:** [ANT25_Habit](https://github.com/kj1241/ANT25_Habit)  

## Overview
This repository contains the implementation of a C++ agent-based simulation and a Python-based analysis tool used in the study "Adding habit effects on scoring function in agent-based transport simulation". The study evaluates the impact of infrastructure investments, such as road and rail network expansions, on individual travel behaviors and overall traffic system efficiency.

Key Contributions:
1. C++ Agent-Based Simulation: 
   - Models individual travel behaviors, including location, mode, departure time, route, and habitual preferences.
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
│   ├── PipInstall.ipynb
├── Simulation/
│   ├── main.cpp
│   ├── input.txt
├── Example/
│   ├── None_Sample/
│       ├── no-habit/
│           ├── agent_utility.zip/
│       ├── habit(1)/
│           ├── agent_utility.zip/
│       ├── Comparison/
│   ├── Infrastructure_Sample/
│       ├── (Infrastructure1)no-habit/
│           ├── agent_utility.zip/
│       ├── (Infrastructure1)habit/
│           ├── agent_utility.zip/
│       ├── (Infrastructure1)Comparison/
│   ├── Habit_Mode_Comparison/
│       ├── mode1/
│           ├── agent_utility.zip/
│       ├── mode2/
│           ├── agent_utility.zip/
│       ├── mode3/
│           ├── agent_utility.zip/
│       ├── Comparison/
├── Release/
│   ├── Simulation/
│       ├── Simulation.zip/
├── LICENSE
├── README.md
```
- **MakeChart**: Python scripts for visualizing traffic patterns and analyzing agent behaviors.
- **Simulation**: C++ program for simulating agent-based travel behavior on road and rail networks.
- **Release**: Packaged distributions of the simulation and charting tools for easy deployment.
- **Example**: Contains example outputs and datasets for testing.
   - **None_Sample**: Compares the general state with the habit-affected state in a basic scenario.
   - **Infrastructure_Sample**: Compares the general state with the habit-affected state in an infrastructure investment scenario (e.g., road expansion, rail network improvements, population growth).
   - **Habit_Mode_Comparison**: Compares different methods of applying habits across states.
      - **mode1**: Positive feedback (habit effect increases travel utility when the mode of transport remains unchanged, and decreases when changed).
      - **mode2**: Positive feedback (habit effect increases travel utility when the mode of transport remains unchanged, and remains neutral when changed).
      - **mode3**: Negative feedback (habit effect decreases travel utility when the mode of transport changes, and remains neutral when unchanged).

## Installation
The system was tested on Windows 10 for compatibility.

1. Python Setup:
   - To set up the Python environment, simply run the PipInstall.ipynb notebook.
   - This will automatically install all required Python dependencies for analysis and chart generation.

2. Simulation Setup:
   - To run the C++ agent-based simulation, ensure that you have Visual Studio Redistributable Packages installed on your system.
   - Once the redistributable packages are installed, you can compile and run the simulation directly from the source code using Visual Studio.


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
  - o	Location, departure points, routes, travel time, and habitual preferences.
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
     ![image](https://github.com/kj1241/ANT25_Habit/blob/main/Example/None_Sample/habit(1)/Average%20Utility%20Score%20Over%20Iterations.png?raw=true)
   - Agent Count For Zone Over Iteration
     ![image](https://github.com/kj1241/ANT25_Habit/blob/main/Example/None_Sample/habit(1)/Agent%20Count%20For%20Zone%20Over%20Iterations.png?raw=true)
   - Average Travel Time For Each Path Over Iterations
     ![image](https://github.com/kj1241/ANT25_Habit/blob/main/Example/None_Sample/habit(1)/Average%20Travel%20Time%20For%20Each%20Path%20Over%20Iterations.png?raw=true)
   - Average Travel Time For Zone Over Iterations
     ![image](https://github.com/kj1241/ANT25_Habit/blob/main/Example/None_Sample/habit(1)/Average%20Travel%20Time%20For%20Zone%20Over%20Iterations.png?raw=true)
   - Average Selected Location Frequency Over Iterations
     ![image](https://github.com/kj1241/ANT25_Habit/blob/main/Example/None_Sample/habit(1)/Average%20Selected%20Location%20Frequency%20Over%20Iterations.png?raw=true)
   - Average Mode Share Over Iterations
     ![image](https://github.com/kj1241/ANT25_Habit/blob/main/Example/None_Sample/habit(1)/Average%20Mode%20Share%20Over%20Iterations.png?raw=true)
   - Average Habit Utility Over Iterations
     ![image](https://github.com/kj1241/ANT25_Habit/blob/main/Example/None_Sample/habit(1)/Average_Habit_Utility_per_Iteration.png?raw=true)
   - Route Distribution Over Iterations
     ![image](https://github.com/kj1241/ANT25_Habit/blob/main/Example/None_Sample/habit(1)/Route%20Distribution%20Over%20Iterations.png?raw=true)
   - Number of Agents with Habit Utility
     ![image](https://github.com/kj1241/ANT25_Habit/blob/main/Example/None_Sample/habit(1)/Number_of_Agents_with_Habit_Utility.png?raw=true)
   - Average Mode Share Over Iterations by File and Transport
     ![image](https://github.com/kj1241/ANT25_Habit/blob/main/Example/None_Sample/Comparison/Average%20Mode%20Share%20Over%20Iterations%20by%20File%20and%20Transport.png?raw=true)

## License
This project is licensed under the MIT License. You are free to use, modify, and distribute this project under the terms of the license.
