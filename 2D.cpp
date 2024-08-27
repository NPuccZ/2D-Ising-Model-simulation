#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <random>
#include <numeric>
#include <iomanip>  // For std::setw and std::setprecision
#include <filesystem>  // For filesystem operations
#include <sstream>  // For std::ostringstream
#include <chrono>  // For timing the simulation
#include <algorithm>  // For std::clamp

namespace fs = std::filesystem;
using namespace std;

// Model constants
const int L = 40;  // Lattice size (width)
const int n = 300000;  // Number of Monte Carlo sweeps
const double mu = 1.0;  // Magnetic moment of each spin

// Function prototypes
vector<vector<int>> initialize_lattice(int L, unsigned int seed);
double calculate_energy(const vector<vector<int>>& s, int L, double B, double mu);
double calculate_magnetization(const vector<vector<int>>& s, int L);
double calculate_energy_difference(const vector<vector<int>>& s, int i, int j, int L, double B, double mu);
void metropolis_update(vector<vector<int>>& s, double Temp, int L, double B, double mu);
void save_lattice(const vector<vector<int>>& lattice, const string& directory, int step);
void run_simulation(double T, int n, int L, double B, double mu, unsigned int seed, ofstream& results_file, int save_freq = 100);

int main(int argc, char* argv[]) {
    // Validate command-line arguments
    if (argc != 4) {
        cerr << "Usage: " << argv[0] << " <temperature> <seed> <magnetic_field>" << endl;
        return 1;
    }

    double T = stod(argv[1]);
    if (T <= 0) {
        cerr << "Error: Temperature must be positive." << endl;
        return 1;
    }

    unsigned int seed = stoul(argv[2]);
    double B = stod(argv[3]);

    if (B < 0 || B > 10) {
        cerr << "Error: Magnetic field should be between 0 and 10." << endl;
        return 1;
    }

    // Timing the simulation
    auto start_time = chrono::high_resolution_clock::now();

    // Format temperature directory name
    ostringstream temp_stream;
    temp_stream << "T" << fixed << setprecision(2) << T;
    string temp_dir = temp_stream.str();

    // Create directory for this temperature
    if (T == 1.6 || T == 2.3 || T == 3.3)
    {
        if (!fs::exists(temp_dir)) 
        {
            if (!fs::create_directory(temp_dir)) 
            {
                cerr << "Error creating directory: " << temp_dir << endl;
                return 1;
            }
        }
    }
    // Open a file to save the thermodynamic results
    ofstream results_file("thermodynamic_results_T" + to_string(T) + "_B" + to_string(B) + ".txt");
    if (!results_file.is_open()) {
        cerr << "Error opening results file!" << endl;
        return 1;
    }

    // Write the header of the results file
    results_file << "# Temperature Energy Magnetization HeatCapacity \n";

    // Run the simulation for the specified temperature and seed
    cout << "Simulating for T = " << T << " with seed = " << seed << " and B = " << B << "\n";
    run_simulation(T, n, L, B, mu, seed, results_file);

    // End timing the simulation
    auto end_time = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = end_time - start_time;
    cout << "Simulation completed in " << elapsed.count() << " seconds." << endl;

    // Close the results file
    results_file.close();

    return 0;
}

// Initialize the lattice with random spins (+1 or -1)
vector<vector<int>> initialize_lattice(int L, unsigned int seed) {
    vector<vector<int>> lattice(L, vector<int>(L));
    mt19937 gen(seed);  // Initialize random number generator with seed
    uniform_int_distribution<> dis(0, 1);

    // Assign random spins to each site in the lattice
    for (int i = 0; i < L; ++i) {
        for (int j = 0; j < L; ++j) {
            lattice[i][j] = dis(gen) ? 1 : -1;
        }
    }
    return lattice;
}

// Calculate the total energy of the lattice
double calculate_energy(const vector<vector<int>>& s, int L, double B, double mu) {
    double E = 0;
    for (int i = 0; i < L; ++i) {
        for (int j = 0; j < L; ++j) {
            // Interaction energy between adjacent spins
            E += -2 * s[i][j] * (s[(i - 1 + L) % L][j] + s[(i + 1) % L][j] +
                                 s[i][(j - 1 + L) % L] + s[i][(j + 1) % L]);
        }
    }

    // Magnetic field contribution to the energy
    E -= mu * B * accumulate(s.begin(), s.end(), 0.0,
        [](double sum, const vector<int>& row) {
            // Accumulate the sum of all spins in the lattice
            return sum + accumulate(row.begin(), row.end(), 0.0);
        });

    return E / (L * L);  // Return energy per site
}

// Calculate the total magnetization of the lattice
double calculate_magnetization(const vector<vector<int>>& s, int L) {
    double M = 0;
    // Sum all spins and calculate the average magnetization
    for (const auto& row : s) {
        M += accumulate(row.begin(), row.end(), 0);
    }
    return abs(M) / (L * L);  // Magnetization per site
}

// Calculate the energy difference when flipping a spin at position (i, j)
double calculate_energy_difference(const vector<vector<int>>& s, int i, int j, int L, double B, double mu) {
    int t = s[(i - 1 + L) % L][j];  // Spin above
    int b = s[(i + 1) % L][j];      // Spin below
    int l = s[i][(j - 1 + L) % L];  // Spin to the left
    int r = s[i][(j + 1) % L];      // Spin to the right

    double neighbor_interaction = 2 * s[i][j] * (t + b + l + r);  // Interaction with neighboring spins
    double magnetic_contribution = 2 * mu * B * s[i][j];  // Contribution from the external magnetic field

    return neighbor_interaction + magnetic_contribution;
}

// Perform a single Metropolis update of the lattice
void metropolis_update(vector<vector<int>>& s, double Temp, int L, double B, double mu) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, L-1);
    uniform_real_distribution<> disReal(0.0, 1.0);

    // Perform L*L spin flip attempts
    for (int k = 0; k < L * L; ++k) {
        int i = dis(gen);  // x-coordinate
        int j = dis(gen);  // y-coordinate
        double ediff = calculate_energy_difference(s, i, j, L, B, mu);

        // Accept or reject the spin flip based on the energy difference
        if (ediff <= 0 || disReal(gen) < exp(-ediff / Temp)) {
            s[i][j] = -s[i][j];
        }
    }
}

// Save the current lattice state to a file
void save_lattice(const vector<vector<int>>& lattice, const string& directory, int step) {
    string filename = directory + "/lattice_step" + to_string(step) + ".txt";
    ofstream outFile(filename);
    if (!outFile.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        return;
    }
    // Write the coordinates and spin states of the lattice to the file
    for (int y = 0; y < lattice.size(); ++y) {
        for (int x = 0; x < lattice[y].size(); ++x) {
            outFile << x << " " << y << " " << (lattice[y][x] == 1 ? 1 : 0) << "\n";  // Save spin as 1 or 0
        }
    }
}

// Function to run the entire Monte Carlo simulation
void run_simulation(double T, int n, int L, double B, double mu, unsigned int seed, ofstream& results_file, int save_freq) {
    // Initialize the lattice with random spins
    vector<vector<int>> s = initialize_lattice(L, seed);

    // Variables for accumulating results over all sweeps
    double En_avg, mag, CV, chi;  // Averages and thermodynamic quantities
    double En = 0;
    double En_sq = 0;
    double Mg = 0;
    double Mg_sq = 0;

    // Format the temperature for directory naming (not used in this function)
    ostringstream temp_stream;
    temp_stream << "T" << fixed << setprecision(2) << T;
    string temp_dir = temp_stream.str();

    // Perform the Monte Carlo sweeps
    for (int i = 0; i < n; ++i) {
        metropolis_update(s, T, L, B, mu);
        double E = calculate_energy(s, L, B, mu);  // Calculate energy per site
        double M = calculate_magnetization(s, L);  // Calculate magnetization per site

        En += E;
        Mg += M;
        En_sq += E * E;

        // Save the lattice every 'save_freq' steps
        if (T == 1.6 || T == 2.3 || T == 3.3) 
        {
            if (i % save_freq == 0) 
            {
                save_lattice(s, temp_dir, i);
            }   
        }
        

        // Progress indicator
        
        if (i % (n / 10) == 0) {
            cout << "T: " << T << "  Progress: " << (100 * i) / n << "%\n";
        }
    }

    // Calculate average values and thermodynamic properties
    En_avg = En / n;
    mag = Mg / n;
    CV = (En_sq / n - (En / n) * (En / n)) / (T * T);

    // Write the results to the output file
    results_file << fixed << setprecision(6);
    results_file << T << " " << En_avg << " " << mag << " " << CV << "\n";
}
