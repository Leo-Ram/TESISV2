import os
import pandas as pd
import matplotlib.pyplot as plt
import matplotlib.dates as mdates

from pandas import json_normalize
os.system('cls' if os.name == 'nt' else 'clear')  

direccion = "datossd/2024-09-28-sddata7"

nameColumn = ["B1", "B2", "B3", "B4", "B5", "B6", "I","T","VT","TIME"]
df = pd.read_csv(direccion+".txt",delimiter=',',names=nameColumn)

df['TIME'] = pd.to_datetime(df['TIME'])

for i in range(1, 7):
    df[f'PB{i}'] = (df[f'B{i}'] * df['I'])/1000

df['PVT'] = (df['VT'] * df['I'])/1000

#print(df.head(10))


# Encontrar el índice donde ocurre el reinicio
reset_index = df['TIME'].dt.time.argmin()


print(reset_index)

if reset_index > 0:
    # Calcular la diferencia de tiempo en el punto de reinicio
    time_difference = df['TIME'].iloc[reset_index - 1] - df['TIME'].iloc[reset_index]
    
    # Ajustar los tiempos después del reinicio
    df.loc[reset_index:, 'TIME'] += time_difference

    # Asegurar que los intervalos se mantengan
    time_intervals = df['TIME'].diff()
    df.loc[reset_index:, 'TIME'] = df.loc[reset_index-1, 'TIME'] + time_intervals[reset_index:].cumsum()

# para guardar la data procesada


#print(df.head(10))

output_filename = direccion+"p"+".csv"
df.to_csv(output_filename, index=False)

