import numpy as np

# Constants
R0 = 100  # ohms
B = 3470  # K
T0 = 298.15  # K, reference temperature (25°C)

# Temperatures to check
temperatures_K = np.array([273.15, 298.15, 323.15])

# Calculate resistance at each temperature
R = R0 * np.exp(B * (1/temperatures_K - 1/T0))

for T, resistance in zip(temperatures_K, R):
    print(f"At {T-273.15}°C ({T}K), Resistance = {resistance:.2f}Ω")

