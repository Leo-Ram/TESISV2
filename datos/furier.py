import os
import pandas as pd
import matplotlib.pyplot as plt

import numpy as np
from scipy.fft import fft

from pandas import json_normalize
os.system('cls' if os.name == 'nt' else 'clear')  


data = pd.read_json("datos/2024-09-10-data2.json")
#print(data.head(10))

dataFlat = json_normalize(data['data']).reset_index()
dataFlat.rename(columns={'index': 'id'}, inplace=True)
dataFlat['timestamp'] = pd.to_datetime(dataFlat['timestamp'])

print(dataFlat.head(10))

bate1 = dataFlat["Bat1"]



tiempos = (dataFlat["timestamp"] - dataFlat["timestamp"].min()).dt.total_seconds()

voltaje = bate1.values
tiempo = tiempos.values
#df['tiempo_segundos'] = (df['tiempo'] - df['tiempo'].min()).dt.total_seconds()

n = len(voltaje)
fft_result = fft(voltaje)
frecuencias = np.fft.fftfreq(n, (tiempo[1]-tiempo[0]))  # Esto asume un muestreo uniforme

# 6. Calcular la magnitud del espectro
magnitud = np.abs(fft_result)

fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(10, 10))

ax1.plot(dataFlat['timestamp'], bate1, marker='o', linestyle='-', color='b', label='Bat1')
ax1.set_xlabel('Tiempo [DD HH:MM]')
ax1.set_ylabel('Voltaje [V]')
ax1.set_title('Bateria')
ax1.set_ylim(0,5)

# 7. Graficar los resultados
ax2.plot(frecuencias[:n//2], magnitud[:n//2])  # Solo graficamos la mitad positiva
ax2.set_xlabel('Frecuencia (Hz)')
ax2.set_ylabel('Magnitud')
ax2.set_title('Espectro de Frecuencia del Voltaje de la Batería')
ax2.set_xlim(0, 20)  # Limitamos el eje x para ver mejor los picos

plt.tight_layout()
plt.show()
