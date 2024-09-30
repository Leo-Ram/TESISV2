import os
import pandas as pd
import matplotlib.pyplot as plt
import matplotlib.dates as mdates

from pandas import json_normalize
os.system('cls' if os.name == 'nt' else 'clear')  

direccion = "datossd/2024-09-28-sddata7p"

df = pd.read_csv(direccion+".csv")

print(df.head(10))


df['TIME'] = pd.to_datetime(df['TIME'])


def plot_single_variable(df, variable, start_time, end_time):
    # Verificar si la variable es válida
    if variable not in ['B1', 'B2', 'B3', 'B4', 'B5', 'B6', 'VT']:
        raise ValueError("Variable debe ser una de: B1, B2, B3, B4, B5, B6, VT")

    # Filtrar el DataFrame por el intervalo de tiempo especificado
    mask = (df['TIME'] >= start_time) & (df['TIME'] <= end_time)
    df_filtered = df.loc[mask]

    # Configurar el gráfico
    fig, (ax1, ax2, ax3) = plt.subplots(3, 1, figsize=(12, 15), sharex=True)
    fig.suptitle(f'{variable}, Corriente y Potencia vs Tiempo', fontsize=16)

    # Gráfico de la variable vs tiempo
    ax1.plot(df_filtered['TIME'], df_filtered[variable], color='blue')
    ax1.grid(True)
    ax1.set_ylabel(variable+"[V]")
    ax1.set_title(f'{variable} vs Tiempo')

    # Gráfico de I vs tiempo
    ax2.plot(df_filtered['TIME'], df_filtered['I'], color='red')
    ax2.grid(True)
    ax2.set_ylabel('I[mA]')
    ax2.set_title('I vs Tiempo')

    # Gráfico de la potencia vs tiempo
    power_col = f'P{variable}'
    ax3.plot(df_filtered['TIME'], df_filtered[power_col], color='green')
    ax3.grid(True)
    ax3.set_ylabel(power_col+"[W]")
    ax3.set_title(f'{power_col} vs Tiempo')

    # Configurar el formato de tiempo para el eje x
    ax3.xaxis.set_major_formatter(mdates.DateFormatter('%H:%M'))
    ax3.xaxis.set_major_locator(mdates.AutoDateLocator())
    plt.setp(ax3.xaxis.get_majorticklabels(), rotation=45, ha='right')
    ax3.set_xlabel('Tiempo')

    plt.tight_layout()
    plt.show()

# Ejemplo de uso:
# Asegúrate de que 'df' esté definido y cargado correctamente antes de llamar a esta función
# limite   2024-01-01 00:06:54      2024-01-01 08:12:35    
start_time = pd.to_datetime('2024-01-01 00:10:00')
end_time = pd.to_datetime('2024-01-01 03:50:04')
variable_to_plot = 'VT'  # Puedes cambiar esto a cualquier variable que desees graficar

plot_single_variable(df, variable_to_plot, start_time, end_time)