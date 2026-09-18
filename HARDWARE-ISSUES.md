# Hardware issues

Faults in the board itself, as opposed to the firmware. Each entry records what
is wrong, how it was established, and what a fix would involve. Firmware
workarounds are noted where one exists.

Board revision under discussion: **v1.4** (`gerber/Gerber_BAUDIO_v1.4_2024-05-25.zip`,
VU meter sheet rev 1.3, 2024-02-28).

---

## HW-1: VU meter right channel is wired to a pin with no ADC

**Severity:** the right channel cannot work as designed. Blocks v2.0 sign-off.

**Status:** open, needs a board change.

### What is wrong

The Tiny VU Meter sheet routes the right channel audio net, `adcro`, to pin
**PB2** of the ATtiny1614. PB2 has no ADC channel on this part, so the
microcontroller cannot measure that signal at all.

The ATtiny1614 analogue inputs are:

| Channel | Pin |
| --- | --- |
| AIN0 - AIN7 | PA0 - PA7 |
| AIN10 | PB1 |
| AIN11 | PB0 |

PB2 does not appear. It carries TOSC and the default USART0 TXD, but no AIN
function.

The left channel is fine: `adclo` goes to **PA7**, which is AIN7.

### How this was established

Text elements in `schematics/Tiny VU Meter.svg` were matched to pin labels by
position. Both net labels sit the same distance from their pin, 35.9 units:

| Net | Nearest pin | Distance |
| --- | --- | --- |
| `adclo` | PA7 | 35.9 |
| `adcro` | TOSC/PB2 | 35.9 |

The same pattern holds for the LED nets down the right side of the symbol
(`rled6` to PA6, `rled7` to PA5, `rled8` to PA4), which confirms the offset is
how this schematic places net labels rather than a coincidence.

The channel list above comes from the ATtiny1614 device definition in
Microchip's ATtiny DFP (`atdf/ATtiny1614.atdf`), filtered to `group="AIN"`.

### Second, separate fault in the firmware

The firmware does not select PB2 either. `vu-meter/adc.h` sets
`ADC_CHANNEL_RIGHT` to `AIN10`, which is **PB1** — and `vu-meter/main.c`
configures PB1 as an **output** driving LED 0. The right channel reading is
therefore taken from a pin the firmware is driving itself.

Both the board and the firmware are wrong about this channel, independently,
which is likely why the right channel has never behaved. The comment in
`adc.h` claiming AIN10 is PB2 probably hid the mismatch: it made the firmware
look like it agreed with the schematic.

Corrected in v1.2.15 as a comment only. The channel selection was deliberately
left alone, because changing it without the board in hand would only move the
reading to a different wrong pin.

### Pin budget

Every ADC-capable pin on the part is already committed:

| Pin | ADC | Current use |
| --- | --- | --- |
| PA0 | AIN0 | UPDI / reset, via R17 470R |
| PA1 | AIN1 | LED (`rled3`) |
| PA2 | AIN2 | LED (`rled4`) |
| PA3 | AIN3 | LED (`rled5`) |
| PA4 | AIN4 | LED (`rled8`) |
| PA5 | AIN5 | LED (`rled7`) |
| PA6 | AIN6 | LED (`rled6`) |
| PA7 | AIN7 | **left channel** |
| PB0 | AIN11 | LED (`rled2`) |
| PB1 | AIN10 | LED (`rled1`) |
| PB2 | none | **right channel (unusable)** |
| PB3 | none | mode button |

So the right channel cannot simply be moved to a free analogue pin: there is
not one. Any fix has to give something up.

### Options for v2.0

1. **Move the right channel to PB1 (AIN10) and drop that LED to PB2.**
   The smallest change. PB2 can drive an LED perfectly well — it just cannot
   read one. Eight LEDs are retained and both channels become readable. The
   firmware change is limited to swapping LED 0's pin in `led_control.c`, as
   `ADC_CHANNEL_RIGHT` already names AIN10.

2. **Multiplex both channels onto one ADC pin** with an analogue switch, and
   alternate. Saves a pin, costs a part and adds switching artefacts to a
   signal path that is already only a rectified envelope.

3. **Drop to seven LEDs** and free a PA pin for the right channel. Wasteful
   given option 1 exists, and it breaks the symmetric four-per-channel display.

4. **Move to a part with more analogue inputs.** Only worth considering if
   v2.0 is re-spinning the microcontroller for other reasons.

Option 1 is the recommendation: one net swap on the board, one line in the
firmware, no new components, no loss of function. It is specified for the next
board revision in [PCB-REV-1.5.md](PCB-REV-1.5.md).

### Workaround until then

None that restores the right channel. The meter currently drives all eight
LEDs from whatever AIN10 returns, which is LED 0's own output pin rather than
audio.

---

## HW-2: PB2 is shared between the VU meter input and USART TX

**Severity:** low. Affects debugging only.

**Status:** open, resolved by whatever fixes HW-1.

`adc-debug` transmits over USART0, whose default TXD is **PB2** — the same pin
the VU meter sheet uses for `adcro`. The two firmwares are separate builds and
never run at once, so this is not an active conflict, but it does mean the
serial debug tool cannot be used while that input is connected.

If HW-1 is fixed by option 1 above, `adcro` moves to PB1 and PB2 is free for
serial. Worth confirming that PB2 is not left connected to the audio divider
in v2.0, since driving TXD into a divider network would load the line.

Noted in `adc-debug/usart.c` alongside the pin assignment.

---

## Revision history of fixes

Recorded so the reason for a change is not lost.

### v1.3 to v1.4: a resistor to ground on each audio input

Recorded from the author's recollection: v1.4 fixed a bug that needed a
resistor to ground on each of the two audio channels.

On the v1.4 schematic the resistors that fit that description are **R11 and
R12**, 10k each, tying the VU meter's two audio inputs to DGND. With R9 and
R10 feeding the same nodes from +5V they form 10k/10k dividers biasing each
input to roughly mid-rail, which is what the ADC needs to see a signal that
swings both ways.

Without a leg to ground the input floats toward +5V, the ADC sits near full
scale, and the firmware's baseline calibration has nothing usable to centre
on.

Not verified against a v1.3 schematic, which is not in this repository, so
treat the specific designators as likely rather than confirmed. What matters
in practice: **do not use a v1.3 board to test HW-1 below.** It carries this
fault as well, so the right channel behaviour there says nothing about the
pin allocation. Use a v1.4 board.

---

## Confirmed working

Recorded so they are not re-investigated:

- **Left channel:** `adclo` to PA7, AIN7. Schematic and firmware agree.
- **AD5242 volume control:** the BOM and Cmoy sheet both fit `AD5242BRZ10-REEL7`,
  a dual RDAC, which matches the two-channel firmware. Address 0x2C is correct
  for AD0 and AD1 tied low, per the AD5241/AD5242 datasheet: the five high bits
  of the slave address are fixed at `01011`.
- **Mode button:** PB3, with an internal pull-up. No external components needed.
