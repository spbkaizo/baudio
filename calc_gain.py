import sys

def parse_resistor_value(resistor_value):
    """
    Parses a resistor value string and returns its value in ohms.
    Supports 'k' as a suffix for kilohms.
    """
    if resistor_value.lower().endswith('k'):
        return float(resistor_value[:-1]) * 1000
    else:
        return float(resistor_value)

def calculate_gain(r1, r2):
    """
    Calculates the gain of a CMoy amplifier.
    Gain formula: Gain = 1 + (R2 / R1)
    """
    return 1 + (r2 / r1)

if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Usage: python cmoy_gain.py <R1> <R2>")
        print("Example: python cmoy_gain.py 10k 47k")
        sys.exit(1)

    r1_value = parse_resistor_value(sys.argv[1])
    r2_value = parse_resistor_value(sys.argv[2])

    gain = calculate_gain(r1_value, r2_value)
    print(f"The gain of the CMoy amplifier is: {gain:.2f}")

