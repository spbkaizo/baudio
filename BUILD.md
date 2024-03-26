# Building

The general rule remains - start from smallest to tallest, and inside to out to save you ending up with components in the way.

You can use this guide as help for how to build each section in turn, testing along the way.  It's absolutely a great idea to not simply start and carry on till exhausted - have breaks.  There's no rush, and you can research things which you are unsure of or want to know more about.  Slow down.

If using solder paste - less is more.  You can always add more later, but if you use too much it has the opposite to the desired effect, and globules of solder have a habit of sticking to chip pins causing shorts.  

## Stage 1 - Power

Let's start with power, as without that we're going nowhere.  

### USB Connector

Solder this in place.  

Using a multimeter check for shorts between 5v and GND in the bottom left corner of the board.  

Once happy, connect power and measure the voltage.  Disconnect from power.

### 3.3v LDO

Next let's get 3.3v connected - solder the LM1117 into place, check for shorts again between 3.3v and 5v, and 3.3v and GND.  

Once happy, connect power and measure the 3.3v voltage.  Disconnect from power.

### LM2663 / Voltage Invertor

Pay attention to the chip pins, 1 is the one with the dot.  Match it up on the pcb.  Solder the chip in place, and then solder PSC2 (47uF 4.0mm cap) into place, as this is essential.

At this point, you should also solder into place UC1, UC2, UC5.  These are the reservoir caps for the USB 5v supply.

After those, place PSC1, PSC3, PSC4, PSC5 and PSC6.

There are two sets of solder bridge pads above and below the LM2663 IC.  

The bridge pad above the LM2663 connects PIN6 (LV) to ground, if required.  The LM2663 datasheet specifies this can be tied to GND, or left floating.  It specifies that the device should operate in low voltage mode, and we don't need that here so leave it alone - it's there in case you are using an alternative device and it's required.

The bridge pad (with 3 connections) below the LM2663 does require configuration.  This is connected to pin 1 (SD LM2663, FC on the LM2662).  On the LM2663 we are using, this pin needs to be tied to ground.  Grab the multimeter and figure out which pad you need to bridge (the other goes to +v!).  If you have an LM2262 or other chip, go read the datasheet to figure out what you need to do.

Now, check for shorts.  Once happy, connect to power and check 3.3, 5v voltages again.  

Then check at the top left of the board the voltage between V+ and V-.  It should be in the region of 10v.  Check the volatages between 5v and GND here, and then -5v and GND.  The should be almost equal.  If not, somethings gone wrong and needs investigating.  See if the inverter feels hot, disconnect from power if so.

### DAC

Assuming all has gone well, it's now time to solder the DAC IC.  You should have a reasonable amount of space for this.  You will want to warm the board, if using a hot air rework, to about 80-100c before applying the paste and soldering it down.  Once the DAC is in place, work on soldering the crystal resonator (12mHz) onto the board.  Two of the connections go to ground, two to the DAC itself.  The two components to the side of the oscillator are 20pF capactitors, do these next.

Next, do the two unmarked 0603 capacitors nearby, these are 0.1uF and used in filtering the power supply to the DAC.  Then, solder the two R22 resistors marked on the PCB - these go onto the USB D+ and D- signal lines.

At this point, you should have soldered 6 components near the DAC.  Next up are the two to the right of it, again these are 0603 sized 0.1uF filtering caps.  Immediately above this goes yet another 0603 0.1uF cap, and above this one another 0.1uF connecting to the LM1117 copper plane.

This should leave you with a space for one of the tantalum capacitors to the right of UC2.  These are polarized, so check the orientation.  To the right of that, yet another 0.1uF 0603 capacitor.  Then, in between the LM1117 and PSC2 marking goes another 10uF tantalum.

Erroneously marked (10u) on the board is an 0805 space for a 1u capacitor (C9 on the PCM2706 Schematic).  Fit this part next. 

Next, solder into place R2, just above the i2s/spdif connector, which needs soldering in next.  This is a 3P male header, which you will jumper to toggle between the two modes.  It doesn't make which mode you select, just make sure that one is selected as I'm unsure what happens if it's left floating.

At this point, the DAC should be operational - try plugging it into a USB power monitor if you have one, then into the PC and see if the device is detected.

Assuming the device is detected, you have a choice - you can carry on building out the board following the volume control instructions below, or if you actually want to test the sound then you can flip the board over and bridge the two DAC connections here.  Then skip to the 'Testing the DAC' section.  *Don't forget to remove the solder bridge before building the volume control*!

### Volume Control

If you are going to use the digital volume control, then now is a good time to solder the components into place, before it gets busier.  You have to solder into place ATTINY1614VOL, LPOT and RPOT, again paying attention to their rotation and PIN1 positions.  

Then, solder in C8 (another 0604 0.1uF capacitor).  Next, add RLED which is a 1K resistor to limit the current going to the LED, which is an 0604 led.  We will use this as 'proof of life' of the attiny, you can leave the LED and RLED out if you don't want them, with no ill effect.  Make sure you orient the led the correct way round, with the 'arrow' on the back showing the direction the current will take.

Leave the switches for now - we can add them once we've built out the CMoy Amplifier.

*The volume control will be entirely useless until you have uploaded the firmware.  The digipots will initialise themselves to be at 50% at startup, i.e. the equivalent of a knob neing turned half way.

### Testing the DAC

Solder in the male 3 pin headers for DAC, AMP, LINE-IN connections.  

Connect jumper wires from the DAC outputs into the LINE-IN inputs.  

Find a cheap pair of earbuds or headphones, and plug them into the line-in connector which you will need to solder into place.

Once done, plug the board back in and reduce the hosts volume mixer to the minimum.  Hopefully you can't hear anything through the headphones, otherwise something is wrong somewhere.

Play something on the host, and slowly raise the volume.  Hopefully you will start to hear music...

### Bulding the CMoy

This is where you'll need to build out the CMoy part of the board.  

By now, hopefully you've got a good idea of how best to approach soldering the components - add everything in that is unpopulated (and small sized) until you reach the 8 LEDs at the top of the board, which we will complete last.

Start with C2L and C2R - these are the film capacitors, and are 0805 sized.  These are in the signal path, hence why we need to use the Panasonic film caps here.  The schematic specifies 0.1uF here, though you could fit two piggbacked if you're careful, taking you 0.2uF....  Note this will affect (improve) the frequency response of the amplifier.  *The next version of the board (1.2) will have space for a 1210 sized component, giving more choices to you.

The combination of C2 (0.1uF) and R2 (100k) forms a high-pass filter. This type of filter allows frequencies higher than its cutoff frequency to pass through with little to no attenuation, while it attenuates frequencies below the cutoff frequency.  For the values above, this cutoff frequency is around 16Hz.  With C2 being a piggbacked 2x0.1uF, the cut off frequency is 7.96hz.  

The python3 script in `calc_fc.py` can work this all out for you, for example:

```
python3 calc_fc.py 100K 0.47u
The cutoff frequency is: 3.386275384933944 Hz
```

Before you deviate from the recommended settings, you should read the excellent article at https://tangentsoft.com/audio/input-cap.html and then decide what to do.  It also explains in more detail why we are using film capacitors.

There's a lot more details in (CMOY-Headphone-Amp.md)[CMOY-Headphone-Amp.md] about the rest of the components.  

Finally, add in the power caps, the 220uF capacitors near the power supply. These are marked C1V+ and C1V-.

#### Adding the buffers

You don't need to add the buffer stage if you don't want to.  

If you are deciding against it, then you will need to solder bridge 'BL' and 'BR'.  If you are unsure if you need to add something to the circuit to populate it, then you can add it, as long as you don't add the BUF IC.  If you do add the BUF IC, then you've added the buffers so need to remove entirely the solder bridge!

If you added the buffers, then populate BR and BL, with either 100R resistor or a 0R resistor.  If unsure, go with the 100.  You could also try a 22 which you should have in the parts.

### CMoy Final Checks

Before plugging it in, check meticulously the connections according to the schemative with a multimeter, paying particular attention to the two Op-Amps and making sure there are no shorts.  Once complete, power up the bard and feel if the chips are getting hot - unplug it if so.  Also now is a good time to test the quiescent current of the device to see how much current is being drawn.  It shouldn't be much, <100mA.

At this point, we are entirely bypassing the potentiometers/volume controllers and simply using the mixer on the machine.  Now is the moment of truth, we will direct the audio via the CMoy.

Unplug it.

Using Jumpers, connect DAC to AMP on the connectors, and plug your headphones into the OUT 3.5mm connector which you'll need to solder.  Remember - this volume will be amplified now, so reduce it really low.  Would be a rather good idea to not have these headphones in your ears at this point!

Try playing something, changing the volume, etc to ensure all sounds ok.

### Building the VU Meter

At this point, you should build out the VU meter.  By now, it should be obvious how you will do this, so just crack on!  Make sure the led's are orientated the correct way.  

### Anything Missing?

Have a look on the board, everything should be mostly populated, apart from the analogue pot unless you intend to use it.  Add in the switches that are missing, and anything else that hasn't been populated.
