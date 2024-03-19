import sys
import math

def parse_value(input_value):
    unit_multipliers = {'k': 1e3, 'M': 1e6, 'u': 1e-6, 'm': 1e-3}
    # Check for the presence of unit letters and remove them, apply the multiplier
    for unit, multiplier in unit_multipliers.items():
        if input_value.lower().endswith(unit):
            return float(input_value[:-1]) * multiplier
    return float(input_value)

if len(sys.argv) == 3:
    try:
        R2 = parse_value(sys.argv[1])  # Convert first argument to float for R2 with unit handling
        C2 = parse_value(sys.argv[2])  # Convert second argument to float for C2 with unit handling
        f_c = 1 / (2 * math.pi * R2 * C2)  # Calculate the cutoff frequency
        print(f"The cutoff frequency is: {f_c} Hz")
    except ValueError:
        print("Please provide numeric values with or without units for R2 and C2.")
else:
    print("Usage: python script.py <R2 with or without unit> <C2 with or without unit>")
    print("e.g 100k 1u")

