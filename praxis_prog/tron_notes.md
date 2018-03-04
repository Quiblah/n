# Flow
### Per-tick order

Draw

Get direction

Calculate move

Check collision

Move



### Draw
Colour the calculated coords for both p's.

If first tick, use some preset.

### Get direction
Check if any keys are being pressed are change direction variables accordingly.

Not sure how this is going to work with 2 keys being pressed at the same time.    Probably need something that works like this: https://msdn.microsoft.com/en-us/library/system.windows.input.keyboard.iskeydown(v=vs.110).aspx


### Calculate move
Just current location plus something to do with direction.

Results in new coords for each p.

### Check collision
If the new coords are the same then both p's lose.

If either p's next coords are for a location any other colour than the background then they lose. (have a border on the window)

### Move
If the game hasn't ended set the  coords to the next coords calculated.
