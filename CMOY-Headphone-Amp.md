
# CMoy Amplifier Build

## Schematic

![schematic](schematics/Cmoy%20Amp.svg)

![buffers](schematics/Cmoy%20Buffers.svg)

## Part selection

Part selection matters.  The BOM has pre-selected values for components, and should be all you need.

For the buffer op-amp, and also the first stage op-amp, I've selected the LM4562 - these are included in the BOM, and run at a few dollars each.  However, please feel free to order others - personally I've had great success in the past with some of the Analog Devices kit, the AD823, AD8066, AD8620 are some of my faves.

### LM4562

The LM4562 is a high-performance audio operational amplifier (op-amp) known for its outstanding audio performance and is often used in high-fidelity audio applications, including the first stage and buffer stage of headphone amplifiers. Here are the reasons why the LM4562 makes a good choice for these stages:

*   Ultra-Low Distortion and Noise: One of the hallmark features of the LM4562 is its ultra-low distortion and noise characteristics. This is critical for the first stage of a headphone amplifier, where the integrity of the audio signal must be preserved. The LM4562 offers a Total Harmonic Distortion + Noise (THD+N) specification that is among the lowest in audio op-amps, ensuring that the signal is amplified without introducing significant distortion or noise

*   High Slew Rate: The LM4562 boasts a high slew rate, which is the rate at which the output of the op-amp can change in response to a change in the input signal. A high slew rate is beneficial for audio applications because it enables the op-amp to handle rapid changes in the audio signal, preserving the nuances and transients in music. This makes it especially suitable for the first stage of a headphone amplifier, where the dynamic response is crucial.

*   Wide Bandwidth: This op-amp offers a wide bandwidth, which ensures that it can accurately reproduce a broad range of audio frequencies without attenuation or phase distortion. A wide bandwidth is essential for both the first stage and buffer stage to ensure that the audio signal is faithfully reproduced across the audible spectrum.

*   High Output Drive Capability: The LM4562 can drive a wide range of headphone impedances, from low to high, without significant loss of fidelity or power. This makes it an excellent choice for the buffer stage of a headphone amplifier, where it needs to provide sufficient power to drive the headphones directly.

*   Excellent Sound Quality: Beyond the technical specifications, the LM4562 is often praised for its subjective sound quality. It is known to provide a clear, detailed, and neutral sound signature, which is highly desirable in high-fidelity audio applications. The first stage and buffer stage of a headphone amplifier significantly influence the overall sound quality, and the LM4562 ensures that the signal is processed with high fidelity.

*   Flexibility and Compatibility: The LM4562 is designed to be compatible with a wide range of audio circuits and applications, offering designers flexibility in its implementation. Its dual-supply operation and the capability to operate at voltages as low as ±2.5V make it suitable for various audio applications, including portable headphone amplifiers.

In summary, the LM4562's low distortion and noise, high slew rate, wide bandwidth, high output drive capability, and excellent sound quality make it an outstanding choice for both the first stage and buffer stage of a headphone amplifier. Its specifications and performance align well with the requirements for high-fidelity audio amplification, ensuring that the audio signal is amplified with minimal alteration to its original quality.

[Datasheet for the LM4562](datasheet/lm4562.pdf)


### Capacitors

#### ESR

For now, you should know this - where the op-amp power supply is concerned, we should aim to use capacitors with a low ESR.  In a headphone amplifier power supply, low Equivalent Series Resistance (ESR) in capacitors is crucial for several reasons that directly impact performance and audio quality:

*   Improved Efficiency: Low ESR capacitors are more efficient at transferring energy. In the context of a power supply, this efficiency translates to better regulation and stability of the voltage supplied to the headphone amplifier. A stable power supply is vital for maintaining the fidelity of the audio signal, ensuring that the amplifier can produce a clear, distortion-free sound.
*   Enhanced Audio Quality: Power supply noise can significantly affect the quality of the audio output. Low ESR capacitors have better filtering capabilities, effectively smoothing out the ripple voltage in the power supply. This leads to a reduction in power supply noise and interference, which is crucial for achieving high-fidelity audio reproduction. By minimizing these unwanted noises, the amplifier can deliver a cleaner, more precise sound to the headphones.
*   Faster Response to Load Changes: Headphone amplifiers can experience rapid changes in load due to dynamic changes in the audio signal. Low ESR capacitors can respond more quickly to these changes, providing a stable power supply that can adapt without introducing distortion or lag in the audio output. This quick response is essential for preserving the dynamics and nuances of the music.

In summary, low ESR is a key factor in the design of a headphone amplifier power supply due to its influence on heat generation, efficiency, audio quality, and the power supply's response to load changes. By minimizing energy loss and improving the stability and cleanliness of the power delivered to the amplifier, low ESR capacitors contribute significantly to the overall performance and reliability of the system, leading to a better listening experience.

#### C1V+, C1V-

These are the power caps for the +/- rails, that feed the demands of the Cmoy amplifier.  We've selected the APAQ Tech 160AVEA221M0608 (LCSC Part C494512) as these have a very low ESR (22m@100kHz~300kHz).  The value is 220uF, but higher wouldn't be problem (i.e. 330, 470), and you may even be able to get away with as low as 100uF.  If in doubt, just stick with the schematic value, but try and get some with a low ESR.  In general, for audio applications, reservoir capacitors with an ESR of less than 0.1 ohms are often considered to be in the low ESR category, and such values are typically sought after to ensure high-quality power delivery. The APAQ's are rated at 0.022 Ohms.

https://datasheet.lcsc.com/lcsc/2004140933_APAQ-Tech-160AVEA221M0608_C494512.pdf

#### C3V+, C3V-

Next up are the 10uF capacitors for the power rails - these provide power at a rate faster than the reservoir capacitors, and they fill the gap between them and the next ones, which are 0.1uF.

These are tantalum capacitors.  They primarily serve to improve the dynamic performance of the amp.  The size is CASE-A.

#### C2V+, C2V-

These 0603 sized, with 0.1uF ratings.  They're pretty generic, indeed when you order the BOM the minimum order will be for 100 pieces (total cost, 75 cents!), so they're sprinkled liberally over the board.  You'll need a couple to go here.


#### C2L, C2R

We have in the schematic for the Cmoy amplifier two capacitors, 0.1uF value, at C2L and C2R.  These need to be good quality and not affect the audio signal.

The tl;dr here is that you should not use MLCC capacitors anywhere in the signal path, as they will reduce audio quality very drastically.  The best type to use here for the components space (1206) are film capacitors.

Film capacitors are often considered superior for use in the signal path of audio circuits, including headphone amplifiers, over electrolytic or multilayer ceramic capacitors (MLCCs) for several key reasons that impact audio quality and performance:

*   Superior Frequency Characteristics: Film capacitors exhibit excellent frequency response characteristics. Unlike electrolytic capacitors, which can have significant inductance and resistance leading to poor performance at high frequencies, film capacitors maintain consistent capacitance over a wide frequency range. This characteristic ensures that the audio signal is preserved accurately across the audible spectrum, contributing to a more detailed and natural sound.
*   Lower Distortion: One of the most significant advantages of film capacitors is their ability to produce lower distortion than electrolytics or MLCCs. Film capacitors have very low dielectric absorption, meaning they can release stored energy more efficiently and with less distortion. This quality is essential in the signal path of an amplifier, where preserving the integrity of the audio signal is paramount to achieving high-fidelity reproduction.
*   Non-Polarized Nature: Film capacitors are non-polarized, making them more versatile in AC signal applications than polarized electrolytic capacitors. This feature allows for more straightforward implementation in the signal path, as there is no need to consider the orientation of the capacitor, reducing the risk of installation errors that could affect the signal integrity.
*   Low ESR and ESL: Film capacitors generally have low equivalent series resistance (ESR) and equivalent series inductance (ESL), which translates to better high-frequency performance and lower self-resonance issues compared to electrolytic and some MLCCs. This quality is particularly important in the signal path, where preserving the integrity of high-frequency signals is crucial for detailed and transparent audio reproduction.

The BOM identifies these as being Panasonic ECPU1C104MA5, details are here:  https://industrial.panasonic.com/ww/products/pt/film-cap-electroequip/models/ECPU1C104MA5 

Unless you have trouble sourcing these, stick with them for the best results.  

Version 1.1 of the board has space for an 0805, which means we're pretty limited to what we can place here.  V1.2 will have a 1210 sized component placement, giving you much more choice.

### Resistors

Mostly you only need to be overly concerned with resistors in the signal path, as thesse could result in drastically different volumes in the L/R output channels.  Here you want to be using 1% tolerances, though in the BOM I managed to find some with 0.1% tolerances, which frankly is a modern marvel.

For the Cmoy, we have these which we need to be concerned with.

#### R2L, R2R

Here the bom specifies Panasonic ERA6AEB104V, these again have a 0.1% tolerance.  The value according to the schematic is 100k, but you can change this (more on that later...).  They are 0805 sized.

#### R3L, R3R

Here the bom specifies Bourns CR0603-FX-1001ELF, which have a 1% tolerance.  They are 0603 sized, with a value of 1k.

R3 sets the gain of the device, in tandem with R4.  We'll come onto that later...

#### R4L, R4R

Here the bom specifies Bourns CMP0603AFX-1002ELF, again with a 1% tolerance.  They are 0603 sized, with a value of 10k.

R4 sets the gain of the device, in tandem with R3.

You can see this by running `calc_gain.py` in the root directory of the project, for example:

```
python3 calc_gain.py 1K 2K  
The gain of the CMoy amplifier is: 3.00
```

If you want to tinker with the gain, you should read this: https://tangentsoft.com/audio/cmoy//tweaks.html

You may wish to lower the gain, if you are building the amp with the following conditions in mind:

*   Primarily to be used as a USB dac
*   You will be using the PCM5102A daugherboard.

This is because the PCM5102 will output true high voltages for line level at 2.1v.

If you are just going to be using the PCM2706, then the output line voltages will be 0.55v.

Further, you should be aware of anything you plug into the line in port, and what voltage that can put out.  To determine it, play a 1Khz sine wave from the device at full volume, and measure the AC voltage with a multimeter.  You should have a good ball park figure to play with.  

The default gain in the schematic is 11, because 11 is louder than 10.  It also means it can work with a source that is low.  This is particuarly important if you're subject to EU volume regulations.  For example, the apple dongles will only put out 0.5 Vrms to comply with these standards.

If unsure, just go with the defaults, and if you find the gain is too high, you can always change it later.

*Optional Changes*

*   For a gain of 8, use 7K for R4
*   For a gain of 6, use 5K for R4
*   For a gain of 3, use 2K for R4
*   For a gain of 4, use 3K for R4

Obviously you can see the relationship, so just pick a value close to what you are after (e.g. 4.7K)


#### R5L, R5R (Optional)

Here's where it gets interesting.  You can jumper this part via the exposed pads near it, or use a 0 ohm resister here.  Alternatively you can leave it unpopulated, in which case you will want to build the amplifier buffer circuit.

If you want the best possible sound out of this thing, you should populate the buffer circuit. 

There another reason it exists - when shorted this gives the amp the best control over the headphone drivers, and also results in the best sound in most circumstances.  However you might feel there is excessive noise in the output - for that I will defer to Tangent, who recommends 10-100 ohms here if required as a lasst resort to help address any excessive noise issues you get in the headphones.  See https://tangentsoft.com/audio/cmoy//tweaks.html for details on this.  

#### BUFL, BUFR (Optional)

These are the output 100 ohm resistors in the signal path.  I've identified the TR0603B100RP0525Z part, made by a company called 'Ever Ohms' (LCSC Part number C881023), as they are capable of 100mW and have a tolerance of 0.1%, which is absoluttely perfect to get balanced output.

#### Other Resistors

Some other resistors, that are in fact nothing to do with the build - but I have a handful of 1W or 2W resistors, in 16, 32, 100, 250, 600 ohms resistances.  When I'm doing testing or checking responses via a signal generator->AMP->Scope I use these as a dummy load, to see how the amp performs.

Have a read below for how to perform one of the tests:

https://tangentsoft.com/audio/pimeta2/bench/square.html

FFT's are another area to dig into:  https://www.youtube.com/watch?v=u--OsHMGMYc


### Tinkering

#### High Pass Filter

The first place you can start to tinker is with the R2 and C2 combinations.  

An RC high-pass filter in the context of a CMoy headphone amplifier serves to block low-frequency signals and only allow higher frequency audio signals to pass through to the output. The "RC" stands for "Resistor-Capacitor," which are the two components used to create this filter.

In a CMoy amplifier, the audio signal passes through this filter before being amplified. The purpose of the high-pass filter is to prevent any DC (Direct Current) component from the audio input or bias from the amplifying circuit from reaching the headphones. DC can potentially harm the headphones and produce an audible pop when the amplifier is switched on or off.

The filter works on the principle that capacitors resist changes in voltage. At low frequencies, the capacitor in the RC high-pass filter has enough time to charge and discharge, which creates a high impedance path, effectively blocking those frequencies. At high frequencies, the capacitor does not have sufficient time to charge fully, which means it presents less impedance to the signal, allowing these frequencies to pass through more easily.

The cutoff frequency of the filter is determined by the values of the resistor (R2) and the capacitor (C2), which can be calculated using the formula 

$f_c = \frac{1}{2\pi R2 C2}$

This frequency is the point at which the signal starts to be attenuated. Frequencies above this point pass through with less attenuation, while those below are increasingly attenuated.

By carefully choosing R2 and C2, you can set the desired cutoff frequency to ensure that the full range of audio frequencies important for music reproduction are passed through to the amplifier, while blocking unwanted low-frequency noise and DC offset.

```python
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

```

With R2 at 100k and C2 at 0.1uF, the cutoff frequency is approximately 15.92 Hz. This is below the typical human hearing range (20 Hz - 20 kHz), so it would allow almost all audible frequencies to pass through while filtering out very low-frequency noise and DC offset.

If C2 were 1uF (with R2 still at 100k), the cutoff frequency would be lower, around 1.59 Hz. This would allow even lower frequencies to pass through, which might not be necessary for audio applications, as most audio content doesn't contain significant information in that range. Additionally, a lower cutoff frequency could allow more unwanted low-frequency noise or rumble to pass through, which could potentially affect the sound quality or cause more power to be consumed in reproducing inaudible frequencies.

If you wanted for whatever reason, to use a 1u cap (you'll struggle to find one in that size for the footprint on the board!) then R2 should be set to 10K.  For a 0.47uF, this would be a 22k.

Why might you want to be changing these values?  I'm glad you asked....

#### Resistor Noise

tl;dr - stick with the defaults on the schematic, don't worry about it, and the answer is approximately 5.74mV.

Johnson-Nyquist noise, often just called thermal noise, is a type of background noise that's present in all electronic devices, including the CMoy headphone amplifier you're building. It comes from the random motion of electrons in the conductive materials (like the resistors) due to their temperature. This noise is always there, as long as the temperature is above absolute zero (-273.15°C), so even in a perfectly designed circuit, you can't get rid of it entirely.

Think of it like this: in a quiet room, you might hear a consistent, low-level hum or hiss. That's similar to what thermal noise is in an electronic circuit. For your CMoy amplifier, this noise sets a baseline for the quietest sound level it can produce. Even if no music is playing, tiny electrical currents are still moving through the components, generating this unavoidable hum.

The amount of noise depends on a few things: the resistance of the components (higher resistance means more noise), the temperature (hotter means more noise), and the bandwidth (the range of frequencies the amplifier works over; a wider range means more noise). For a headphone amplifier, we're especially interested in the noise because we want the music to sound as clear as possible, without any extra hiss in the background.

In practice, for building a CMoy or any audio equipment, you choose high-quality components and design the circuit to minimize this noise, keeping your music as crisp and clear as the artists intended. While you can't eliminate Johnson-Nyquist noise, understanding it helps you make better design choices, like selecting the right resistors and planning your circuit layout to keep that "room" as quiet as possible, so you can enjoy the music without unwanted noise.

In the context of adjusting the values of C2 and R2 in your CMoy amplifier, calculating Johnson-Nyquist noise becomes particularly relevant for a few reasons:

*   Optimizing Sound Quality: The primary goal in any audio equipment, including a CMoy amplifier, is to maximize sound quality. Johnson-Nyquist noise represents an unavoidable background hiss that can affect the clarity and purity of the audio signal. By understanding how different values of C2 and R2 influence the level of thermal noise, you can make informed decisions to minimize its impact, ensuring a clearer sound output.
*   Component Selection: Different values of R2 (the resistor in the RC high-pass filter or elsewhere in the amplifier) directly affect the level of thermal noise, with higher resistance values generally leading to higher noise levels. Knowing the noise implications helps in selecting resistor values that strike a balance between the desired filtering characteristics (or other circuit requirements) and acceptable noise levels.
*   Frequency Response and Noise: The choice of C2, especially in the context of an RC filter, impacts the amplifier's frequency response, altering how different frequencies are attenuated or amplified. While the primary focus might be on shaping the frequency response, these choices also indirectly affect noise characteristics. For instance, a wider bandwidth (which can be influenced by C2 and R2 values) may allow more noise to pass through the amplifier, as thermal noise is present across all frequencies but becomes more significant over a wider bandwidth.
*   Impedance Matching and Noise Consideration: In designing or tweaking a CMoy amplifier, the impedance of components like headphones and the input source must be considered for optimal performance. The values of C2 and R2 can affect the amplifier's input and output impedance, which in turn influences noise levels. Understanding the noise contributions helps in ensuring that the amplifier's design is compatible with the intended headphones, minimizing noise while maintaining the desired audio fidelity.
*   Practical Expectations and Design Trade-offs: Calculating Johnson-Nyquist noise can set realistic expectations for the amplifier's performance. It emphasizes that while striving for perfection in audio clarity, some level of background noise is inherent to electronic devices. This understanding guides the selection of R2 and C2 values within practical bounds, acknowledging the trade-offs between noise, frequency response, and other design criteria.

In summary, calculating Johnson-Nyquist noise when changing C2 and R2 in a CMoy amplifier isn't just about quantifying the background hiss; it's a crucial step in the holistic design process. It influences component selection, shapes the amplifier's sound quality, and helps in making informed design choices that balance ideal audio reproduction with the physical realities of electronic noise.



```python
import sys
import math

def parse_bandwidth(bandwidth_str):
    ""Parses the bandwidth string and returns the delta frequency.""
    start_freq, end_freq = map(float, bandwidth_str.split('-'))
    return end_freq - start_freq

def calculate_johnson_noise(R, Δf):
    ""Calculates Johnson-Nyquist noise.""
    k_B = 1.380649e-23  # Boltzmann constant in J/K
    T = 298.15  # Room temperature in Kelvin (25°C)
    return math.sqrt(4 * k_B * T * R * Δf)

if len(sys.argv) == 3:
    try:
        R = float(sys.argv[1])  # Resistor value in ohms
        Δf = parse_bandwidth(sys.argv[2])  # Bandwidth in Hz
        noise_voltage = calculate_johnson_noise(R, Δf)
        print(f"Johnson-Nyquist noise: {noise_voltage} Vrms")
    except ValueError as e:
        print(f"Error processing input: {e}")
        print("Usage: python script.py <resistance in ohms> <bandwidth in Hz (start-end)>")
else:
    print("Usage: python script.py <resistance in ohms> <bandwidth in Hz (start-end)>")
")"
```

Let's work through an example, as it will lead us onto the next design consideration.  With C2 as 0.1uf and R2 as 100k (as in the schematic) the value is $5.74 \mu V_{\text{rms}}$.  This is before we head into the 'tweaking the gain' section, so if you use the schematic values as is, we would have a gain of 11.  As a result, we will expect to have $63.09 \mu V_{\text{rms}}$.

If you were to swap these out for C2 1uF and R2 10k, then the value pre amp drops to $1.81 \mu V_{\text{rms}}$, and post amp to $19.95 \mu V_{\text{rms}}$... This however will introduce some new issues and challenges, with the pot.  You generally need to have a pot 10x the value of R2, so you're now likely to need to a 100K pot.  My advice would be to stick with the default values, and only change them after doing a lot more research on how all these components work together in tandom.  A brief overview of this is given below. 

#### Pot Selection

Adding a potentiometer in front of C2, which is part of the RC high-pass filter in a CMoy headphone amplifier, introduces an additional variable resistance into the signal path. The choice of potentiometer value (e.g., 10kΩ, 50kΩ, or 100kΩ) can affect the overall impedance seen by the input signal as well as the filter's behavior and, consequently, the choices for R2. Here’s how different potentiometer values could influence the design:

In the design we have used a 10K, but you could change this to a 50k or a 100k.  If you decide you want to deviate, you need to be aware of the impacts.

*   Input Impedance and Signal Attenuation: The potentiometer, when used as a volume control, directly impacts the amplifier's input impedance. A higher-value potentiometer (e.g., 50kΩ or 100kΩ) increases the input impedance, which can be beneficial for compatibility with a wider range of source devices. However, it also means that the signal may undergo more attenuation before reaching the amplifier, potentially requiring adjustments to R2 to maintain the desired frequency response.
*   Interplay with R2: The effective resistance seen by C2 (and therefore the behavior of the high-pass filter) is influenced by the setting of the potentiometer and the value of R2. If R2 is chosen without considering the potentiometer, the filter's cutoff frequency may vary significantly with the potentiometer's position. For a stable and predictable cutoff frequency, the values of the potentiometer and R2 should be considered together. For example, a larger potentiometer value might necessitate adjusting R2 downward to maintain the desired cutoff frequency.
*   Noise Considerations: A potentiometer adds its own thermal noise to the circuit, although this contribution is generally small compared to other noise sources. However, the resistance value of the potentiometer can influence the overall noise level of the pre-amplifier stage. Higher resistance values (e.g., 100kΩ) could potentially increase the thermal noise floor. When designing the amplifier, this factor might lead to a preference for lower potentiometer values, provided they meet other design criteria.
*   Design Flexibility vs. Complexity: Using a higher-value potentiometer might offer more flexibility in adjusting the input impedance and the filter characteristics but at the cost of increased complexity in choosing R2.

With our amp design, we can have multiple sources:

    *   The PCM2706 output as an input to the amp stage
    *   Another DAC daisy chained to the PCM2706 output, then used as an input to the amp stage
    *   Anything else hooked up to the line in

There's an interesting design choice here - do you even need a volume control?  If you're intending to use this board as a DAC with the PCM2706, this DAC has a built in hardware mixer, which means you can simply adjust the volume on the output from the host OS.  

If you want to do this, there are 2 sets of bridging solder pads at the DAC output pins which will entirely bypass the potentiometer.  

However, if you are intending to use the daughterboard PCM5102A DAC, then you need to be aware that this only produces line-level output - and in addition this is a true high level output at 2.1v.  

Given we are building a Cmoy amplifier with a +5v/-5v arrangement, not only will it not cope with the gain of 11  * 2.1v, equally the output will be massive and almost certain to destroy something.  

Of note is that the PCM2706's line level output is far lower, at around 0.55v.  If you do decide to bridge these pads, you will need to unbridge them later if you want to change the arrangement.

##### Analog Pot

I've deliberately left out this part from the BOM, as you may not need or want it.  If you do want it, then you can skip the digital volume control on the attiny, either by not bothering to populate it, or configuring it do something else.

Another reason I've left it out, as these are easily found on AliExpress, if you search for Alps RK097 you will find a million of them.  Note if you go this route, these are likely to be counterfeit (but will probably still work fine!).  You will want the 10K version with either 'audio' or 'log/logarthmic' in the title, with 6 pins on the bottom.  If you're after a legitimate original part, the mfr number is RK09712200HA - I think.  Caveat Emptor.

##### Digital Pot

Honestly, try and go with this.  It's way cooler, and gives you expansion options and an ability to change it's behaviour via code.  For example, you can change the code so it increses in steps 0-255 in 1, 2, 3, 5, 10 increments.  You can also potentially do some other stuff, as it's entirely code configurable.  Firmware for the attiny1614 is available in the directory for the volume control.

The parts list BOM has the 10K pot version in it - I'd recommend just going with that, and you shouldn't go far wrong...

