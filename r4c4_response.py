import numpy as np
import matplotlib.pyplot as plt

# Constants
R = 10000  # Resistor value in ohms
capacitance_values = [22e-12, 47e-12, 100e-12, 220e-12, 470e-12]  # Capacitor values in farads

# Frequency range for simulation, from 1 Hz to 20 kHz
f = np.logspace(0, np.log10(20000), 400)
w = 2 * np.pi * f

# Plotting
plt.figure(figsize=(10, 6))

# Loop through each capacitor value and plot its frequency response
for C in capacitance_values:
    gain = 1 / np.sqrt(1 + (w * R * C)**2)
    label = f'With {C*1e12}pF Capacitor'
    plt.semilogx(f, 20 * np.log10(gain), label=label)

plt.xlabel('Frequency (Hz)')
plt.ylabel('Gain (dB)')
plt.title('Frequency Response with Different Capacitor Values (pF)')
plt.legend()

# Set x-axis ticks
plt.xticks([1, 10, 100, 1000, 10000, 20000], ['1', '10', '100', '1k', '10k', '20k'])

plt.grid(which='both', linestyle='--', linewidth=0.5)
plt.show()

