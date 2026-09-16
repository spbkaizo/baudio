# BAUDI/O

![Photo](images/IMG_3298.jpeg "Photo")
![PCB](images/PCB_v1.4_2024-05-25.svg "PCB")

Known faults in the current board revision are recorded in
[HARDWARE-ISSUES.md](HARDWARE-ISSUES.md), and the fix for the next revision in
[PCB-REV-1.5.md](PCB-REV-1.5.md).

## What is it?

An experiment in exactly how much can you cram into a business card PCB, for audio electronics.

It has a plethora of features, and opportunities for uses that I've not considered.

## DAC

At the heart of this is a PCM2706 DAC, which when connected via USB to a host will appear in the USB devices as a sound card, supporting up to 48kHz/16bit audio.  This is beyond CD quality, and before you start on the band waggon of 'yeah but muh 192kHz 24 bit...' pop over and read Monty's amazing article [here](https://people.xiph.org/~xiphmont/demo/neil-young.html) and then watch his equally epic videos on the subject.  

For more details on the DAC, have a look at [DAC.md](DAC.md)

## CMoy

The CMoy headphone amplifier, named after its inventor, Chu Moy, is a renowned DIY project within the audiophile community, celebrated for its simplicity and exceptional performance. Chu Moy introduced this design on HeadWize, a pioneering online forum dedicated to headphone enthusiasts, in the late 1990s. The amplifier is notable for its use of commonly available components, including operational amplifiers (op-amps) housed within an Altoids tin or similar small cases, making it both accessible and portable. The CMoy has become a rite of passage for DIY audio hobbyists, offering an affordable entry point into high-quality audio amplification. Its design principles encourage customization and experimentation, leading to numerous variations and enhancements by the DIY community. 

Yes, we've got one crammed in here.

For more details on this, have a look at [CMOY-HeadphoneAmp.md](CMOY-Headphone-Amp.md)

## Power Supply

Another use for this PCB is to provide accessible power, not for just the usual 3.3v/5v combination, but also a dual power supply of circa $\pm$ 5v.

For more details on the power supply, have a look at [Power-Supply.md](Power-Supply.md)

## MicroControllers

The ATtiny1614, part of Microchip's tinyAVR® 1-series, is a marvel of microcontroller efficiency and versatility, encapsulating the essence of what makes modern embedded systems so powerful. Despite its diminutive size, the ATtiny1614 boasts an impressive array of features that make it a go-to choice for hobbyists and professionals alike, working on compact and energy-efficient projects.

Firstly, its high processing power, courtesy of the AVR core running at up to 20 MHz, is remarkable. This speed, combined with a rich set of peripherals, including multiple analog-to-digital converters (ADCs), a digital-to-analog converter (DAC), and a versatile set of communication interfaces (USART, SPI, and I2C), provides a solid foundation for a wide range of applications, from simple LED controllers to more complex sensor networks.

We've got two of them on the board for you to play with.

We chose these, as they support the new programming interface known as UPDI.  UPDI operates over a single wire for programming and debugging, plus ground and optional power connections.  No more fighting with ISP cables or soic-8 clips!  With UPDI, you can program the flash, EEPROM, and even adjust fuse settings through the same interface used for debugging.

### Blinkenlighten

Now we start to delve into the fun side of things, we've got an array of 8 LEDs on the board, driven by an ATTiny1614 connected to both the left and right pre-amp audio sources.  And, in case you don't want this distraction, then the pins are broken out and accessible for your own code and use.

Source code in the `vu-meter` directory; ready-to-flash firmware is on the [releases page](https://github.com/spbkaizo/baudio/releases).

### Digital Volume Control

The other attiny is used as a digital volume controller - but if you want to use an old school analogue potentiometer, you can.  Just don't populate this part of the board.  But, read on for why you should...

*   Digital pots can be adjusted with precise increments, allowing for exact resistance settings - no more channel imbalances!
*   You can program the microcontroller to switch between different resistance profiles or settings, enabling one device to perform multiple functions or adapt to different operating conditions.  Instead of simply doing n+1 you could cycle through pre-defined settings!
* You can save the previous settings to the eeprom, or have a default 'low' setting so you don't blow your ears out
* If, like me, you are a little more deaf in one ear than the other, you can implement balancing
* If you don't like the response curve, change it!
* Unlike mechanical analog pots, digital pots and microcontrollers are not subject to physical wear from adjustments, leading to a longer lifespan and higher reliability

## Firmware

Three separate firmwares live in this repository, all for the ATtiny1614:

| Directory | What it does |
| --- | --- |
| `vu-meter` | Drives the eight LEDs: VU meter, spectrum, knight rider, binary count and middle-out modes |
| `volume-control` | Drives the AD5242 digital potentiometer from the two volume buttons |
| `adc-debug` | Prints ADC readings over serial, for bringing a board up |

### Flashing a released build

You do not need a toolchain to flash the board. Every tagged version has its
`.hex` files attached to a [GitHub release](https://github.com/spbkaizo/baudio/releases),
built by CI from that tag:

```sh
curl -LO https://github.com/spbkaizo/baudio/releases/latest/download/vu-meter.hex
avrdude -c serialupdi -P /dev/ttyUSB0 -p attiny1614 -U flash:w:vu-meter.hex:i
```

Adjust the port to suit your UPDI programmer. Each release also carries a
`SHA256SUMS` file if you want to check a download. It lists all three
firmwares, so check just the one you fetched rather than the whole file:

```sh
sha256sum --ignore-missing -c SHA256SUMS
```

### Building it yourself

The builds need `avr-gcc` and Microchip's ATtiny device family pack. avr-gcc
has no built-in support for the tinyAVR 0/1-series, so without the pack it
fails with `device type not defined` and every register name comes back
undeclared.

1. Install a toolchain, for example `brew tap osx-cross/avr && brew install avr-gcc`
   on macOS, or `apt install gcc-avr binutils-avr avr-libc` on Debian and Ubuntu.
2. Download `Microchip.ATtiny_DFP` from
   [packs.download.microchip.com](https://packs.download.microchip.com) and
   unzip it. Only the `gcc` and `include` directories are needed.
3. Build, pointing at wherever you put it:

```sh
make -C vu-meter DFP=/path/to/dfp
```

The Makefiles default to `~/local/avr-dfp`, so putting it there lets you run
`make` on its own. `make flash` then programs the board over UPDI; check the
`PORT` line in the Makefile matches your programmer.

### Build outputs are not committed

`.hex`, `.elf`, `.o` and `.lst` files are generated by `make` and are
gitignored. They were tracked until v1.2.18, which meant every local build
showed up as a modified file and the committed copies drifted from the source
beside them. Releases are the distribution point now: those are built by CI
from a clean checkout of the tag, so a released `.hex` always matches the
source it claims to come from.

They are still present in the history before that commit, so an old checkout
will have them. `make clean` removes the local copies.

Note that firmware built locally will not byte-match a release unless your
compiler version matches the one CI uses, which is whatever `gcc-avr` ships on
the current Ubuntu runner. Different avr-gcc versions produce different, and
differently sized, output from the same source.

## Getting started

At this point if you're still reading, then perhaps you're interested in building one.  Head over to the [FAQ.md](FAQ.md) to see how to get started!