#!/bin/bash
rm -r T*
g++ -std=c++20 -o 2D 2D.cpp
seq 1.6 0.01 3.3 | xargs -I{} -P 8 bash -c './2D {} $RANDOM 0.0'
chmod +x combine_results.sh
./combine_results.sh
rm thermodynamic_results_T*_B*.txt
python3 plot_results.py
python3 anim.py "T1.60" 1.6
python3 anim.py "T2.30" 2.3
python3 anim.py "T3.30" 3.3
echo "SIMULATION FINISHED"