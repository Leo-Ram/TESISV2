import os
import pandas as pd
import numpy as np

# Limpiar la consola
os.system('cls' if os.name == 'nt' else 'clear')

# Definir la dirección del archivo
direccion = "datossdex/2024-10-12-data1"

# Definir los nombres de las columnas
nameColumn = ["VF", "IF", "VB", "IB", "VC", "IC", "TIME"]

# Leer el archivo CSV
df = pd.read_csv(direccion + ".txt", delimiter=',', names=nameColumn)

# Invertir la corriente IB
df["IB"] = -df["IB"]

# Convertir la columna TIME a datetime
df['TIME'] = pd.to_datetime(df['TIME'])

# Calcular las potencias
df["PVF"] = df["VF"] * df['IF'] / 1000
df["PVB"] = df["VB"] * df['IB'] / 1000
df["PVC"] = df["VC"] * df["IC"] / 1000

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

# Guardar la data procesada
output_filename = direccion + "p" + ".csv"
df.to_csv(output_filename, index=False)

print("Procesamiento completado. Archivo guardado como:", output_filename)

