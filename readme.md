# Steam Deck chorded keyboard

Tiny program to convert Steam Decks gamepad into a chorded virtual keyboard. Made for desktop mode, untested anywhere else. Take it more as a proof of concept, rather than a tool, as I have no intention of further support. But it works and I'm using it. Pdf file contains chords, it can be printed and taped to the console above the screen(mind the vents though) while you training in [Klavaro](https://klavaro.sourceforge.io/en/index.html)(search for it in Discover app on the Deck) or alike. Tested on 512GB OLED and 64GB LCD from first batches.

## Using

Buttons L1,L4,L5,R1,R4,R5 are used as chords for total of 63 buttons. X,Y and three dotted one - LShift, LCtrl and LAlt respectively. All three modifiers have sticky mode. Double press, confirmed by vibro, - "oneshot"(released after first chorded input). To get permanent stickieness, you should hold second press until second vibro. Sticky button can be released at any time by short pressing it again.

## Installing

No special treatment. Just run from where you like it to be. No elevation of privileges or udev magic requred for Steam Deck. Just remember to unbind buttons L1,L4,L5,R1,R4,R5,X,Y in Steam(Settings->Gamepad->Desktop mode layout). A lifehack would be to add executable into plasma autostart(this way it will be launched in background on desktop session start and killed on exit to game mode). But you totaly free to run it as needed in Konsole, or invent your own way:)

## Compiling

On the Deck I recommend using [distrobox](https://distrobox.it/) (should be preinstalled since SteamOS 3.5 [If not.](https://github.com/89luca89/distrobox/blob/main/docs/posts/steamdeck_guide.md)). Inside selected distro you'll need build tools and hidapi. Then, just
```text
make
```
is enough.

## Used sources

[hidapitester](https://github.com/todbot/hidapitester)
[SDL](https://github.com/libsdl-org/SDL)
