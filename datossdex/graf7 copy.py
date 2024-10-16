import os
import pandas as pd
import matplotlib.pyplot as plt
import matplotlib.dates as mdates

from pandas import json_normalize
os.system('cls' if os.name == 'nt' else 'clear')  

direccion = "datossdex/2024-10-12-data1p"

df = pd.read_csv(direccion+".csv")

print(df.head(10))


df['TIME'] = pd.to_datetime(df['TIME'])

# Definir el intervalo de tiempo (ajusta estos valores según tus necesidades)
start_time = pd.to_datetime('2024-01-01 00:00:00')
end_time = pd.to_datetime('2024-01-01 07:33:34')

# Filtrar el DataFrame por el intervalo de tiempo especificado
mask = (df['TIME'] >= start_time) & (df['TIME'] <= end_time)
df = df.loc[mask]


# Crear una figura con un arreglo de 3 filas y 1 columna
fig, axs = plt.subplots(3, 1, figsize=(10, 8), sharex=True)
fig.suptitle('Voltaje, Corriente y Potencia vs Tiempo', fontsize=16)

# Primera gráfica: Voltajes VF, VB, VC contra tiempo
axs[0].plot(df['TIME'], df['VF'], label='VFuente')
axs[0].plot(df['TIME'], df['VB'], label='VBaterias')
axs[0].plot(df['TIME'], df['VC'], label='VCarga')
axs[0].set_ylabel('Voltaje [V]')
axs[0].legend(loc='upper right')
axs[0].grid(True)

# Segunda gráfica: Corrientes IF, IB, IC contra tiempo
axs[1].plot(df['TIME'], df['IF'], label='IFuente')
axs[1].plot(df['TIME'], df['IB'], label='IBaterias')
axs[1].plot(df['TIME'], df['IC'], label='ICarga')
axs[1].set_ylabel('Corriente [mA]')
axs[1].legend(loc='upper right')
axs[1].grid(True)

# Tercera gráfica: Potencias PVF, PVB, PVC contra tiempo
axs[2].plot(df['TIME'], df['PVF'], label='PVFuente')
axs[2].plot(df['TIME'], df['PVB'], label='PVBaterias')
axs[2].plot(df['TIME'], df['PVC'], label='PVCarga')
axs[2].set_ylabel('Potencia [W]')
axs[2].legend(loc='upper right')
axs[2].grid(True)

# Formatear el eje X (Tiempo)
axs[2].set_xlabel('Tiempo [DD HH:MM]')
plt.xticks(rotation=45)  # Rotar etiquetas de tiempo para mejor visibilidad

# Ajustar el espacio entre subplots
plt.tight_layout()

# Mostrar las gráficas
plt.show()

# Asumiendo que ya has cargado y procesado tu DataFrame 'df'

"""
def plot_variables_and_power(df, start_time, end_time):
    # Filtrar el DataFrame por el intervalo de tiempo especificado
    mask = (df['TIME'] >= start_time) & (df['TIME'] <= end_time)
    df_filtered = df.loc[mask]

    # Configurar el gráfico
    fig, axs = plt.subplots(7, 3, figsize=(20, 30), sharex='col')
    fig.suptitle('Voltaje, Corriente y Potencia vs Tiempo', fontsize=16)

    variables = ['B1', 'B2', 'B3', 'B4', 'B5', 'B6', 'VT']

    for idx, var in enumerate(variables):
        # Gráfico de la variable vs tiempo
        axs[idx, 0].plot(df_filtered['TIME'], df_filtered[var])
        axs[idx, 0].set_ylabel(var+"[V]")
        if idx == 0:
            axs[idx, 0].set_title("V vs Tiempo")

        # Gráfico de I vs tiempo
        axs[idx, 1].plot(df_filtered['TIME'], df_filtered['I'])
        axs[idx, 1].set_ylabel('I[mA]')
        if idx == 0:
            axs[idx, 1].set_title('I vs Tiempo')

        # Gráfico de la potencia vs tiempo
        power_col = f'P{var}'
        axs[idx, 2].plot(df_filtered['TIME'], df_filtered[power_col])
        axs[idx, 2].set_ylabel(power_col+"[W]")
        if idx == 0:
            axs[idx, 2].set_title("P vs Tiempo")

        # Ocultar etiquetas del eje x excepto en la última fila
        if idx != 6:
            for ax in axs[idx]:
                ax.xaxis.set_visible(False)

    # Configurar el formato de tiempo para la última fila
    for ax in axs[6]:
        ax.xaxis.set_major_formatter(mdates.DateFormatter('%H:%M'))
        ax.xaxis.set_major_locator(mdates.AutoDateLocator())
        plt.setp(ax.xaxis.get_majorticklabels(), rotation=45, ha='right')
        ax.set_xlabel('Tiempo (HH:MM)')

    plt.tight_layout()
    plt.show()


# Definir el intervalo de tiempo (ajusta estos valores según tus necesidades)
start_time = pd.to_datetime('2024-01-01 00:00:00')
end_time = pd.to_datetime('2024-01-01 17:17:44')

# Llamar a la función para crear los gráficos
plot_variables_and_power(df, start_time, end_time)

"""