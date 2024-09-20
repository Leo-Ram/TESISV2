import numpy as np
import matplotlib.pyplot as plt
from scipy.fft import fft, fftfreq

# Parámetros de las señales
duration = 1.0  # Duración en segundos
sample_rate = 1000  # Frecuencia de muestreo en Hz
t = np.linspace(0, duration, int(sample_rate * duration), endpoint=False)

# Generar dos señales sinusoidales
freq1 = 10  # Frecuencia de la primera señal en Hz
freq2 = 20  # Frecuencia de la segunda señal en Hz
signal1 = np.sin(2 * np.pi * freq1 * t)
signal2 = 0.5 * np.sin(2 * np.pi * freq2 * t)  # Amplitud reducida para distinguir mejor

# Sumar las señales
combined_signal = signal1 + signal2

# Calcular la Transformada de Fourier
fft_result = fft(combined_signal)
frequencies = fftfreq(len(t), 1 / sample_rate)

# Calcular la magnitud del espectro
magnitude = np.abs(fft_result)

# Crear la figura con dos subplots
fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(10, 10))

# Graficar la señal combinada
ax1.plot(t, combined_signal)
ax1.set_title('Señal Combinada')
ax1.set_xlabel('Tiempo (s)')
ax1.set_ylabel('Amplitud')

# Graficar la Transformada de Fourier
ax2.plot(frequencies[:len(frequencies)//2], magnitude[:len(frequencies)//2])
ax2.set_title('Transformada de Fourier')
ax2.set_xlabel('Frecuencia (Hz)')
ax2.set_ylabel('Magnitud')
ax2.set_xlim(0, 50)  # Limitamos el eje x para ver mejor los picos

plt.tight_layout()
plt.show()