# FAQ

I'll try and be as terse as possible, please open an issue on github if you have other questions and I'll get them answered and added to this!

# How do I get a board?

Easiest way is to download the gerber file from the gerber directory.  You can then upload this to a PCB manufacturing service, I recommend https://jlcpcb.com

The minimum order will be 5 boards, but the cost should be aroung $10 including shipping for this.  Please, give away the others and pay it forward!

# How much will this cost?

It depends - you can spend a lot, or a little.  The recommended BOM is available in the BOM directory, and uses LCSC for parts - just upload it using their BOM tool.  If you go with the defaults, it's around $32 in parts.

You can cheap out on the amplifier parts, but I've tried to find the cheapest yet still decent op-amp, and settled on the LM4562.  Other op-amps should work, such as the AD823, the AD8066, the AD8620 are all potential candidates for either of the stages.  The goal here is experimentation, so give them a whirl, write a review, and create a PR to get it added to the as-yet-nonexistant reviews directory.

# I've never soldered SMT before

This is going to be challenging for you.  I'm not going to lie, it's a bit of a learning curve.

There are hundreds of practice kits all over AliExpress and eBay - get a few of them, they're usually like $1 each.

https://www.instructables.com/SMD-Soldering-Practice-Kit-or-How-I-Learned-to-Sto/

If you only have a soldering iron, ensure you have a very fine tip, and even then it will be challenging.

If you can afford it, then look into buying a hot air rework station.  https://learn.sparkfun.com/tutorials/how-to-use-a-hot-air-rework-station

They're not terribly expensive, and using one of these along with soldering paste you'll soo get the hang of it, after doing a few of the practice boards.

# Will you build one of these for me?

Nope.  Sorry, the fun is in the building challenges.