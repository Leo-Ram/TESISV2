import os
import pandas as pd
import matplotlib.pyplot as plt
import matplotlib.dates as mdates

from pandas import json_normalize
os.system('cls' if os.name == 'nt' else 'clear')  

nameColumn = ["B1", "B2", "B3", "B4", "B5", "B6", "I","T","TIME","nones"]
df=pd.read_csv("datossd/2024-09-25-sddata4.txt",delimiter=',',names=nameColumn)

#print(df.head(10))




df['TIME'] = pd.to_datetime(df['TIME'])

# Calcular las diferencias de tiempo
#df['TIME_DIFF'] = df['TIME'].diff()
#print(df.head(10))
# Detectar reinicios (diferencias negativas)
#reinicios = df['TIME_DIFF'] < pd.Timedelta(0)

#for i in range(1, len(reinicios)):
#    if reinicios[i] == True:
#        a = i
#print(a)
#print(df.loc[a,"TIME"])
# Corregir los tiempos reiniciados
#for i in range(a, len(df)):
#    df.loc[i:, 'TIME'] = (df.loc[i-1, 'TIME']) + df.loc[i,'TIME_DIFF']

# Eliminar la columna de diferencias de tiempo
#df.drop(columns=['TIME_DIFF'], inplace=True)
df.set_index('TIME', inplace=True)

#df.set_index('TIME', inplace=True)
print(df.head(10))


fig, axes = plt.subplots(nrows=3, ncols=2, figsize=(15, 15), sharex=True)

# Lista de nombres de las columnas de las baterías
bat_columns = ["B1", "B2", "B3", "B4", "B5", "B6"]




std_devs = df[bat_columns].std()
std_devs_max = std_devs.max()

# Aplanar el array de axes para facilitar la iteración
axes_flat = axes.flatten()

# Graficar cada columna de batería contra el tiempo
for i, column in enumerate(bat_columns):
    ax = axes_flat[i]
    ax.plot(df.index, df[column], label=column)
    ax.set_ylabel(column + " [V]")
    ax.set_ylim(2.5,4.4)
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