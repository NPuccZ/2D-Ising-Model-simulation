# 2D Ising Model Monte Carlo Simulation

## Overview

This repository contains a Monte Carlo simulation of the two-dimensional Ising model using the Metropolis algorithm, as well as several supporting scripts for running simulations, processing results, and generating visualizations. Below, we detail the physical and mathematical foundations of the main simulation, as well as explanations for the accompanying scripts used to manage the simulation workflow.

## Physical and Mathematical Details of the Simulation

The simulation implemented in `2D.cpp` is a classic example of how the two-dimensional Ising model can be studied using Monte Carlo methods, specifically the Metropolis algorithm. This section explores the key formulas and the physics behind the main functions of the code.

### 1. The Ising Model

The Ising model considers a two-dimensional lattice of size \( L \times L \), where each site on the lattice is associated with a spin \( s_{ij} \) that can take values of +1 or -1. The spins interact with their nearest neighbors and can also be influenced by an external magnetic field \( B \).

### 2. Energy of the System

The total energy of the system, \( E \), is given by the sum of the interaction energy between adjacent spins and the contribution from the external magnetic field. Mathematically, the energy per site is calculated as:

\[
E = -J \sum_{\langle i,j \rangle} s_{ij} s_{i'j'} - \mu B \sum_{i,j} s_{ij}
\]

where:

- \( J \) is the coupling constant between spins (normalized to 1 in the code).
- \( \langle i,j \rangle \) denotes the sum over nearest neighbors.
- \( \mu \) is the magnetic moment associated with each spin.
- \( B \) is the strength of the external magnetic field.

In the code, this formula is implemented in the function `calculate_energy`. For each site on the lattice, the interaction energy is calculated by considering the interactions with the neighboring spins above, below, left, and right. The magnetic field term is added by summing the contribution from each spin in the presence of the external field.

### 3. Magnetization

The total magnetization, \( M \), is defined as the sum of all the spins in the lattice, normalized by the total number of sites:

\[
M = \frac{1}{L^2} \left| \sum_{i,j} s_{ij} \right|
\]

In the code, the magnetization is calculated in the function `calculate_magnetization` by simply summing all the spins and normalizing by the number of sites. The absolute value is taken to obtain the average magnetization.

### 4. Metropolis Algorithm

The Metropolis algorithm is used to simulate the evolution of the lattice at a given temperature \( T \). The algorithm involves the following steps:

1. Randomly select a site on the lattice.
2. Calculate the energy difference \( \Delta E \) that would result from flipping the spin at that site.
   
   \[
   \Delta E = 2 s_{ij} \left( J \sum_{(i'j')} s_{i'j'} + \mu B \right)
   \]

   where the sum is taken over the nearest neighbors of the site \( (i,j) \).

3. If \( \Delta E \leq 0 \), accept the spin flip.
4. If \( \Delta E > 0 \), accept the spin flip with probability \( \exp(-\Delta E / k_B T) \).

This process is repeated \( L \times L \) times for each Monte Carlo "sweep", ensuring that each spin on the lattice has the opportunity to be updated.

### 5. Heat Capacity

The heat capacity \( C_V \) is a fundamental quantity that measures the system's response to temperature changes. It is calculated as the variance of the energy:

\[
C_V = \frac{1}{k_B T^2} \left( \langle E^2 \rangle - \langle E \rangle^2 \right)
\]

In the code, the heat capacity is calculated as part of the thermodynamic analysis after the simulation, using the average energy and energy squared values accumulated during the simulation.

### 6. Simulation and Output

At the end of each simulation, the results are saved to a text file. This includes the temperature, average energy per site, average magnetization per site, and heat capacity. These values can be used later to study the phase transition of the Ising model, particularly to identify the critical point where the system transitions from a disordered (high-temperature) state to an ordered (low-temperature) state.

## Running the Simulation

### Prerequisites

Ensure you have a C++ compiler that supports C++20, Python 3, and the necessary Python libraries such as `matplotlib` for plotting. Additionally, Bash and `xargs` are required for running the script.

### Run the Simulation

The entire simulation workflow is managed by a Bash script that compiles the code, runs the simulation across a range of temperatures, combines the results, and generates plots and animations. The key steps are as follows:

1. **Clone the Repository**:
   ```bash
   git clone <repository-url>
   cd <repository-directory>
2. **Enable the running of run.sh**:
   ```bash
   chmod +x run.sh
3. **Run the simulation**:
   ```bash
   ./run.sh
The script does the following:
* Cleans up the previous results.
* Compiles the 2D.cpp code.
* Parallel execution: Runs the simulation for temperatures ranging from 1.6 to 3.3 in increments of 0.01, using different random seeds for each run.
* Combines the results of the different temperatures in a single file to perform the plots, then it eliminates the single files for each temperature.
* Plots the results for energy, magnetization and specific heat as a function of temperature.
* Performs the animation of the evolution of the spin states.

### Conclusions 
This repository provides a comprehensive toolset for simulating and studying the two-dimensional Ising model using Monte Carlo methods. The combination of C++ for high-performance simulation and Python for data visualization offers a powerful framework for exploring statistical mechanics and phase transitions. Whether you are conducting research or learning about computational physics, these scripts and programs provide a solid foundation for experimentation and discovery.
