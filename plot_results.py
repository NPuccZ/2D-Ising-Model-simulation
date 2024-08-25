import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import os

#create a folder named 'results' in the same directory as this script to put the plots in
if not os.path.exists('results'):
    os.makedirs('results')

# Leggi il file results.txt
filename = 'results.txt'

# Utilizza pandas per leggere il file, ignorando la riga di intestazione che inizia con '#'
data = pd.read_csv(filename, sep=r'\s+', comment='#', header=None)

# Assegna nomi significativi alle colonne
data.columns = ['Temperature', 'Energy', 'Magnetization', 'HeatCapacity']

# Imposta una risoluzione alta per i grafici
dpi = 300

# Plot di Energy in funzione della Temperature e salva il grafico
plt.figure(figsize=(10, 6))
plt.plot(data['Temperature'], data['Energy'], marker='o', linestyle='-', color='b', label='Energy')
plt.xlabel('Temperature')
plt.ylabel('Energy')
plt.title('Energy vs Temperature')
plt.grid(True)
plt.legend()
plt.savefig('results/Energy_vs_Temperature.png', dpi=dpi)
plt.close()

# Plot di Magnetization in funzione della Temperature e salva il grafico
plt.figure(figsize=(10, 6))
plt.plot(data['Temperature'], data['Magnetization'], marker='o', linestyle='-', color='r', label='Magnetization')
plt.xlabel('Temperature')
plt.ylabel('Magnetization')
plt.title('Magnetization vs Temperature')
plt.grid(True)
plt.legend()
#superimpose the function y = (1 - np.sinh(2 / data['Temperature']) ** (-4)) ** (1/8) on the plot
plt.plot(data['Temperature'], (1 - np.sinh(2 / data['Temperature']) ** (-4)) ** (1/8), linestyle='--', color='k', label='Analytical Solution')
plt.legend()
plt.savefig('results/Magnetization_vs_Temperature.png', dpi=dpi)
plt.close()

# Plot di HeatCapacity in funzione della Temperature e salva il grafico
plt.figure(figsize=(10, 6))
plt.plot(data['Temperature'], data['HeatCapacity'], marker='o', linestyle='-', color='g', label='HeatCapacity')
plt.xlabel('Temperature')
plt.ylabel('HeatCapacity')
plt.title('HeatCapacity vs Temperature')
plt.grid(True)
plt.legend()
plt.savefig('results/HeatCapacity_vs_Temperature.png', dpi=dpi)
plt.close()

