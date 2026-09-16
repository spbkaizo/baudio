# PCB rev 1.5 change specification

One change, on one sheet, to make the VU meter's right channel readable. See
[HARDWARE-ISSUES.md](HARDWARE-ISSUES.md) HW-1 for why the current board cannot
work.

Base revision: v1.4 (`Gerber_BAUDIO_v1.4_2024-05-25.zip`), VU meter sheet rev
1.3.

---

## The change

Swap two nets on the ATtiny1614 (`AT1614VU`) on the **Tiny VU Meter** sheet.

| Pin | Symbol label | Net now | Net in rev 1.5 |
| --- | --- | --- | --- |
| 7 | `TOSC/PB2` | `adcro` | **`rled1`** |
| 8 | `PB1` | `rled1` | **`adcro`** |

Nothing else changes. No component values, no additions, no removals, and no
other sheet is touched.

### Why this way round

`adcro` has to reach a pin with an ADC channel. On this part those are PA0-PA7
(AIN0-AIN7), PB0 (AIN11) and PB1 (AIN10). Every one is already used, so the
right channel can only move by displacing something.

`rled1` is the cheapest thing to displace. An LED needs a pin that can be
driven high and low, which PB2 does perfectly well — PB2 simply cannot
*measure* a voltage. Swapping the two therefore costs nothing: the LED does not
need the ADC it currently occupies, and the audio input gets one.

Pins 7 and 8 are adjacent on the package, which should keep the re-route
short.

## Doing it in EasyEDA

1. Open the **Tiny VU Meter** schematic sheet.
2. On the `AT1614VU` symbol, change the net label on pin 7 from `adcro` to
   `rled1`, and the label on pin 8 from `rled1` to `adcro`.
3. Check the two nets now read as in the table above, and that no net name got
   duplicated or orphaned in the edit.
4. Update the PCB from the schematic. EasyEDA will flag the two ratlines that
   no longer match the routed copper.
5. Re-route those two traces.
6. Run DRC. Expect zero new errors: the swap does not change trace count,
   component count, or pin count.
7. Bump the sheet revision to 1.5 and the date.
8. Re-export gerbers, BOM and the PCB SVG into this repository, replacing the
   v1.4 files.

### Routing notes

`adcro` is an analogue input carrying an audio-derived signal at roughly
0-4.3 V. `rled1` is a digital output switching a LED. Now that they trade
places, keep the usual care:

- Route `adcro` away from switching traces where it is convenient to do so. It
  comes off a 100k series resistor (R8), so it is a relatively high-impedance
  node and will pick up crosstalk more readily than the LED line it replaces.
- The LED line can be routed with no particular care, as before.
- Keep `adcro` clear of the `pb3` button line and the `updi` line if the layout
  allows.

This is not a critical analogue path — the firmware only takes a rectified
envelope from it, not a waveform — so do not distort the layout to achieve it.

## After the respin

### Firmware

One line, in `vu-meter/led_control.c`. LED 0 moves from PB1 to PB2:

```c
case 0: PORTB.OUT = (PORTB.OUT & ~(1<<PIN2_bp)) | (state << PIN2_bp); break;
```

And in `vu-meter/main.c`, `init_ports()` must set PB2 as an output instead of
PB1:

```c
PORTB.DIR |= (1 << PIN0_bp) | (1 << PIN2_bp);
```

`ADC_CHANNEL_RIGHT` in `vu-meter/adc.h` already names `AIN10`, which is PB1, so
the define itself needs no change — it becomes correct the moment the board is
respun.

Comments in four further places describe the current, broken arrangement and
will be wrong after the change:

| File | What it says |
| --- | --- |
| `vu-meter/adc.h` | The whole HW-1 explanatory note above the define |
| `vu-meter/main.c` | `// Ensure PB0 and PB1 are set as output` on the DIR line |
| `vu-meter/main.h` | `USE_LCD` notes describing LEDs on PB0 and PB1 |
| `vu-meter/cylon.c` | `LED 0 is PB1` in the comment explaining the LED mapping |

The pin budget table in HARDWARE-ISSUES.md needs updating too, and HW-1 and
HW-2 can both be marked resolved against rev 1.5.

Verified: the two code changes above compile clean for the ATtiny1614, at
4498 bytes of flash against 4494 for the current build.

These changes are deliberately **not** made yet: applied now they would break
the working v1.4 LED output in exchange for fixing a channel that cannot work
on that board either way. They belong in the same commit as the rev 1.5
gerbers.

### Verification on the assembled board

1. Confirm continuity from the right channel audio input to **pin 8** of the
   VU meter ATtiny, and from the left to **pin 5**.
2. Flash the rev 1.5 firmware.
3. Play audio panned hard left: LEDs 0-3 should move, LEDs 4-7 stay dark.
4. Pan hard right: the opposite.
5. Centre: both halves should move together.

Step 3 and 4 are the real test. Before this fix the right half either sat dead
or followed the left, because both halves were being driven from readings that
had nothing to do with the right channel.

### Also resolved by this change

HW-2 goes away. `adc-debug` transmits on PB2, which currently collides with
`adcro`. Once `adcro` moves to PB1, PB2 carries only an LED, and the serial
debug firmware can use it without contention — the LED will flicker with the
serial traffic, which is harmless and arguably useful.

Confirm that in rev 1.5 PB2 connects **only** to the LED and its resistor, with
no remaining stub to the audio divider. A leftover connection would load the
TX line.
