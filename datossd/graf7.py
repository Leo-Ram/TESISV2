import os
import pandas as pd
import matplotlib.pyplot as plt
import matplotlib.dates as mdates

from pandas import json_normalize
os.system('cls' if os.name == 'nt' else 'clear')  

direccion = "datossd/2024-09-26-sddata5p"

df = pd.read_csv(direccion+".csv")

print(df.head(10))


df['TIME'] = pd.to_datetime(df['TIME'])
import pandas as pd
import matplotlib.pyplot as plt
import matplotlib.dates as mdates

# Asumiendo que ya has cargado y procesado tu DataFrame 'df'

def plot_variables_and_power(df, start_time, end_time):
    # Filtrar el DataFrame por el intervalo de tiempo especificado
    mask = (df['TIME'] >= start_time) & (df['TIME'] <= end_time)
    df_filtered = df.loc[mask]

    # Configurar el gráfico
    fig, axs = plt.subplots(7, 3, figsize=(20, 30))
    fig.suptitle('Variables, Corriente y Potencia vs Tiempo', fontsize=16)

    variables = ['B1', 'B2', 'B3', 'B4', 'B5', 'B6', 'VT']

    for idx, var in enumerate(variables):
        # Gráfico de la variable vs tiempo
        axs[idx, 0].plot(df_filtered['TIME'], df_filtered[var])
        axs[idx, 0].set_title(f'{var} vs Tiempo')
        axs[idx, 0].set_xlabel('Tiempo')
        axs[idx, 0].set_ylabel(var)

        # Gráfico de I vs tiempo
        axs[idx, 1].plot(df_filtered['TIME'], df_filtered['I'])
        axs[idx, 1].set_title('I vs Tiempo')
        axs[idx, 1].set_xlabel('Tiempo')
        axs[idx, 1].set_ylabel('I')

        # Gráfico de la potencia vs tiempo
        power_col = f'P{var}'
        axs[idx, 2].plot(df_filtered['TIME'], df_filtered[power_col])
        axs[idx, 2].set_title(f'{power_col} vs Tiempo')
        axs[idx, 2].set_xlabel('Tiempo')
        axs[idx, 2].set_ylabel(power_col)

        # Formatear el eje x para mostrar las fechas correctamente
        for ax in axs[idx]:
            ax.xaxis.set_major_formatter(mdates.DateFormatter('%Y-%m-%d %H:%M:%S'))
            plt.setp(ax.xaxis.get_majorticklabels(), rotation=45, ha='right')

    plt.tight_layout()
    plt.show()

# Ejemplo de uso:
# Definir el intervalo de tiempo (ajusta estos valores según tus necesidades)
start_time = pd.to_datetime('2024-01-01 00:02:50')
end_time = pd.to_datetime('2024-01-01 08:11:26')

# Llamar a la función para crear los gráficos
plot_variables_and_power(df, start_time, end_time)