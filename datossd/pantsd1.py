import os
import pandas as pd
import matplotlib.pyplot as plt
import matplotlib.dates as mdates

from pandas import json_normalize
os.system('cls' if os.name == 'nt' else 'clear')  

nameColumn = ["B1", "B2", "B3", "B4", "B5", "B6", "I","T","TIME","nones"]
df=pd.read_csv("datossd/2024-09-24-sddata3.txt",delimiter=',',names=nameColumn)

#print(df.head(10))




df['TIME'] = pd.to_datetime(df['TIME'])

# Calcular las diferencias de tiempo
df['TIME_DIFF'] = df['TIME'].diff()
#print(df.head(10))
# Detectar reinicios (diferencias negativas)
reinicios = df['TIME_DIFF'] < pd.Timedelta(0)

#for i in range(1, len(reinicios)):
#    if reinicios[i] == True:
#        a = i
#print(a)
#print(df.loc[a,"TIME"])
# Corregir los tiempos reiniciados
#for i in range(a, len(df)):
#    df.loc[i:, 'TIME'] = (df.loc[i-1, 'TIME']) + df.loc[i,'TIME_DIFF']

# Eliminar la columna de diferencias de tiempo
df.drop(columns=['TIME_DIFF'], inplace=True)

#df.set_index('TIME', inplace=True)
print(df.head(10))


nombre = "B1"

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
ax2.set_ylim(-100,100)

lines1, labels1 = ax1.get_legend_handles_labels()
lines2, labels2 = ax2.get_legend_handles_labels()
ax1.legend(lines1 + lines2, labels1 + labels2, loc='upper left')

plt.title(f'Voltaje total y corriente en el tiempo')
plt.grid(True)
plt.show()

