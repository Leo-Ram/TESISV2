import os
import pandas as pd
import matplotlib.pyplot as plt
import matplotlib.dates as mdates

from pandas import json_normalize
os.system('cls' if os.name == 'nt' else 'clear')  

direccion = "datossd/2024-10-12-data1"

nameColumn = ["B1", "B2", "B3", "B4", "B5", "B6", "I","T","VT","TIME"]
df = pd.read_csv(direccion+".txt",delimiter=',',names=nameColumn)

df['TIME'] = pd.to_datetime(df['TIME'])

for i in range(1, 7):
    df[f'PB{i}'] = (df[f'B{i}'] * df['I'])/1000

df['PVT'] = (df['VT'] * df['I'])/1000

#print(df.head(10))


# Encontrar el índice donde ocurre el reinicio
reset_index = df['TIME'].dt.time.argmin()


# Función para detectar y corregir múltiples reseteos
def corregir_reseteos(df):
    # Calcular las diferencias de tiempo
    time_diff = df['TIME'].diff()
    
    # Detectar los reseteos (diferencias negativas en el tiempo)
    reseteos = time_diff < pd.Timedelta(0)
    
    # Inicializar el tiempo acumulado
    tiempo_acumulado = pd.Timedelta(0)
    
    # Iterar sobre los reseteos
    for i in range(1, len(df)):
        if reseteos.iloc[i]:
            # Calcular la diferencia de tiempo en el punto de reseteo
            diferencia = df['TIME'].iloc[i-1] - df['TIME'].iloc[i] + time_diff.iloc[i-1]
            tiempo_acumulado += diferencia
        
        # Ajustar el tiempo
        df.loc[i, 'TIME'] += tiempo_acumulado
    
    return df

# Aplicar la corrección de reseteos
df = corregir_reseteos(df)

#print(df.head(10))

output_filename = direccion+"p"+".csv"
df.to_csv(output_filename, index=False)

