import os
import pandas as pd
import matplotlib.pyplot as plt

from pandas import json_normalize
os.system('cls' if os.name == 'nt' else 'clear')  


data = pd.read_json("datos/2024-09-05-data1.json")

dataFlat = json_normalize(data['data']).reset_index()
dataFlat.rename(columns={'index': 'id'}, inplace=True)
dataFlat['timestamp'] = pd.to_datetime(dataFlat['timestamp'])

# print("hola")
# print(dataFlat['timestamp'].head(10))
# print("adios")


start_time = pd.to_datetime('2024-09-05 18:00:00').tz_localize('UTC')
end_time = pd.to_datetime('2024-09-05 19:00:00').tz_localize('UTC')

# Filtrar el DataFrame por el rango de tiempo
#filtered_da = dataFlat[(dataFlat['timestamp'] >= start_time) & (dataFlat['timestamp'] <= end_time)]
#filtered_dat = filtered_da[filtered_da['Bat1'] != 0]
#filtered_data = filtered_dat[filtered_dat['Cbat'] > -800]
filtered_data = dataFlat
print(filtered_data.head(10))



cleaned_data = filtered_data

cleaned_data.set_index('timestamp', inplace=True)

# Crear una figura y subgráficas en disposición 3x2
fig, axes = plt.subplots(nrows=3, ncols=2, figsize=(15, 15), sharex=True)

# Lista de nombres de las columnas de las baterías
bat_columns = ['Bat1', 'Bat2', 'Bat3', 'Bat4', 'Bat5', 'Bat6']

std_devs = filtered_data[bat_columns].std()
std_devs_max = std_devs.max()

# Aplanar el array de axes para facilitar la iteración
axes_flat = axes.flatten()

# Graficar cada columna de batería contra el tiempo
for i, column in enumerate(bat_columns):
    ax = axes_flat[i]
    ax.plot(cleaned_data.index, cleaned_data[column], label=column)
    ax.set_ylabel(column + " [V]")
    ax.set_ylim(0,4.4)
    ax.legend(loc='upper right')
    ax.grid(True)
    
    # Configurar etiqueta del eje x para las gráficas de la fila inferior
    if i >= 4:
        ax.set_xlabel('Time [DD HH:MM]')

# Configurar el título principal
fig.suptitle(f"Voltajes en Baterías, Desviación estándar {std_devs_max:.2f}V", y=1)

# Ajustar el diseño para evitar superposición
plt.tight_layout()

# Mostrar la gráfica
plt.show()