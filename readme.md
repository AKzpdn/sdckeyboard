# Steam Deck chorded keyboard

Tiny program to convert Steam Decks gamepad into a chorded virtual keyboard. Made for desktop mode, untested anywhere else. Take it more as a proof of concept, rather than a tool, as I have no intention of further support. But it works and I'm using it. Pdf file contains chords, that can be printed and taped to the console above the screen(mind the vents though) while you learning in [Klavaro](https://klavaro.sourceforge.io/en/index.html)(search for it in Discover app on the Deck) or alike. Tested on 512GB OLED and 64GB LCD from first batches.

## Installing

No special treatment. Just run from where you like it to be. No elevation of privileges ur udev magic requred for Steam Deck. Just remember to unbind buttons L1,L4,L5,R1,R4,R5,X,Y from Steam(Settings->Gamepad->Desktop mode layout). A lifehack would be to add executable into plasma autostart(this way it will be launched in background on desktop session start and killed on exit to game mode). But you totaly free to run it "as-needed", or invent your own way:)

## Compiling

On the Deck I recommend using [distrobox](https://distrobox.it/) (should be preinstalled since SteamOS 3.5). [If not.](https://github.com/89luca89/distrobox/blob/main/docs/posts/steamdeck_guide.md) Inside selected distro you'll need build tools and hidapi. Then, just
```text
make
```
is enough.

## Thanks

[hidapitester](https://github.com/todbot/hidapitester) for extremely clear code I could base on.
