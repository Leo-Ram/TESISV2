import os
import pandas as pd
import matplotlib.pyplot as plt

from pandas import json_normalize
os.system('cls' if os.name == 'nt' else 'clear')  


data = pd.read_json("datos/2024-09-25-data4.json")

dataFlat = json_normalize(data['data']).reset_index()
dataFlat.rename(columns={'index': 'id'}, inplace=True)
dataFlat['timestamp'] = pd.to_datetime(dataFlat['timestamp'])

print("hola")
print(dataFlat.head(10))
nombre = "B2(V)"

fig, ax1 = plt.subplots(figsize=(10,6))
ax1.plot(dataFlat['timestamp'], dataFlat[nombre], marker='o', linestyle='-', color='b', label='Bat1')
ax1.set_xlabel('Tiempo[DD HH:MM]')
ax1.set_ylabel(nombre+'[V]', color='b')
ax1.tick_params('y', colors='b')

ax2 = ax1.twinx()
ax2.plot(dataFlat['timestamp'],dataFlat['Cbat'], marker='o', linestyle='-', color='r', label='I')
ax2.set_ylabel('I[mA]', color='r')
ax2.tick_params('y', colors='r')

lines1, labels1 = ax1.get_legend_handles_labels()
lines2, labels2 = ax2.get_legend_handles_labels()
ax1.legend(lines1 + lines2, labels1 + labels2, loc='upper left')

plt.title(f'Voltaje total y corriente en el tiempo')
plt.grid(True)
plt.show()