# Power Supply

# Schematic

![schematic](schematics/Power%20Supply.svg)

Few things of note here.

## Negative Voltage Generator (LM2663)

The LM2663 is a CMOS charge-pump voltage converter that inverts a positive voltage in the range of 1.5V to 5.5V to the corresponding negative voltage. It is designed to provide up to 200 mA of output current using two low-cost capacitors, eliminating the cost, size, and electromagnetic interference (EMI) associated with inductor-based converters. The device operates with a low current of only 300 uA, making it efficient for applications requiring negative voltage generation.

The LM2663 charge pump voltage converter requires two external capacitors for its operation: one flying capacitor (C1) and one reservoir capacitor (C2). The typical values for these capacitors depend on the specific application requirements, including the desired output current and voltage, but a general guideline is to use capacitors with values in the range of 10 µF to 100 µF.

The requirement for low Equivalent Series Resistance (ESR) in these capacitors is crucial for the proper functioning of the LM2663. Low ESR is important because:

    *   Efficiency: Low ESR capacitors reduce power loss within the capacitor, increasing the overall efficiency of the voltage conversion process. High ESR can lead to significant I²R losses, where I is the current through the capacitor and R is the ESR. 
    *   Voltage Ripple: Low ESR helps in minimizing the output voltage ripple. The ESR contributes to the ripple voltage seen at the output, in conjunction with the output current and the frequency of operation. A lower ESR will lead to a smaller ripple, which is desirable for most electronic applications that require a stable and clean power supply.
    *   Stability: The performance and stability of the charge pump circuit can be affected by the ESR of the capacitors. Low ESR capacitors help ensure that the circuit remains stable under various load conditions.
    *   Thermal Performance: High ESR values lead to higher power dissipation within the capacitor, which can raise its temperature and potentially affect its reliability and lifespan. Low ESR capacitors are better at managing thermal loads and can contribute to the overall reliability of the circuit.
    *   To meet these requirements, it's recommended to use capacitors that specifically mention low ESR characteristics for use with the LM2663.

The Panasonic EEE-FT series are a good fit here - part number EEEFT1C470AR is the 47uF version.  It is characterized by an ESR (Equivalent Series Resistance) of 850 mOhm (0.85 ohm) at 100kHz.  For info, the LM2663 switches at 150 kHz.

There is scope to use an alternative part here, as the LM2663 is now being deprecated.  Alternatives and further information is on the schematic.  But, if you can get them, use the lm2663.


## 3.3v Voltage Regulator

The LM1117 is a popular linear voltage regulator we use to power the DAC, which has both 5v and 3.3v requirements.  Its 3.3V requirement is primarily due to its internal architecture and the technology used in its design, which are optimized for low-power, high-performance audio applications.

    *   Low Power Consumption: Operating at lower voltages directly translates to lower power consumption, which is crucial for USB-powered devices. USB specifications limit the power available to devices, so maintaining high efficiency and low power usage is essential. A 3.3V operating voltage helps ensure the PCM2706 can deliver its functionality within the power constraints of a USB interface.
    *   Signal Integrity: Lower voltage levels can contribute to better signal integrity, especially in mixed-signal circuits like audio DACs, where both analog and digital circuits coexist. Operating at 3.3V helps reduce noise levels within the chip, which is beneficial for achieving high-quality audio reproduction.

The LM117  can deliver up to 800mA of output current, which makes it suitable for medium-power applications. You should have plenty left over for powering other 3.3v devices. 

## Power Supply Capacitors

To ensure we have enough power in the reservoir, there are quite a few caps in the circuit, mainly to support the Cmoy amplifier.  The schematic shows 220uF - I think these are reasonable values.

Going higher will increase the strain on the lm2663 at power on - the surge current while the caps charge up is not a small amount.  The USB specification will limit the device power to 500mA, so interesting things could happen here if you deviate too much.  If you think you're encountering issues with this, please raise an issue and we can try and work through it.