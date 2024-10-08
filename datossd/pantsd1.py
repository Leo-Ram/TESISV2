import os
import pandas as pd
import matplotlib.pyplot as plt
import matplotlib.dates as mdates

from pandas import json_normalize
os.system('cls' if os.name == 'nt' else 'clear')  

nameColumn = ["B1", "B2", "B3", "B4", "B5", "B6", "I","T","VT","TIME"]
df = pd.read_csv("datossd/2024-09-26-sddata5.txt",delimiter=',',names=nameColumn)

df['TIME'] = pd.to_datetime(df['TIME'])

print(df.head(10))


# Encontrar el índice donde ocurre el reinicio
reset_index = df['TIME'].dt.time.argmin()

if reset_index > 0:
    # Calcular la diferencia de tiempo en el punto de reinicio
    time_difference = df['TIME'].iloc[reset_index - 1] - df['TIME'].iloc[reset_index]
    
    # Ajustar los tiempos después del reinicio
    df.loc[reset_index:, 'TIME'] += time_difference

    # Asegurar que los intervalos se mantengan
    time_intervals = df['TIME'].diff()
    df.loc[reset_index:, 'TIME'] = df.loc[reset_index-1, 'TIME'] + time_intervals[reset_index:].cumsum()

# para guardar la data procesada
#output_filename = "datos_corregidos.csv"
#df.to_csv(output_filename, index=False)

nombre = "B5"

fig, ax1 = plt.subplots(figsize=(10,6))
ax1.plot(df['TIME'], df[nombre], marker='o', linestyle='-', color='b', label='B1')
ax1.set_xlabel('Tiempo[DD HH:MM]')
ax1.set_ylabel(nombre+'[V]', color='b')
ax1.tick_params('y', colors='b')
ax1.set_ylim(0,4.4)

ax2 = ax1.twinx()
ax2.plot(df['TIME'],df['I'], marker='o', linestyle='-', color='r', label='I')
ax2.set_ylabel('I[mA]', color='r')
ax2.tick_params('y', colors='r')
ax2.set_ylim(-1300,1000)

lines1, labels1 = ax1.get_legend_handles_labels()
lines2, labels2 = ax2.get_legend_handles_labels()
ax1.legend(lines1 + lines2, labels1 + labels2, loc='upper left')

plt.title(f'Voltaje total y corriente en el tiempo')
plt.grid(True)
plt.show()

