# Better Button Test
**[DOWNLOAD LATEST ROM HERE](https://github.com/orangeglo/better-button-test/releases/latest)**

----

<img width="250" alt="Better Button Test v5" src="https://github.com/user-attachments/assets/b575f158-c5df-430e-abb1-f4534b8a18f4">
<img width="250" alt="Better Button Test v5" src="https://github.com/user-attachments/assets/1ea87cbe-704e-488b-9d0e-7f895ba0ab78">
<img width="250" alt="Better Button Test v5" src="https://github.com/user-attachments/assets/8fe58052-b523-41d6-b63d-f7de04ccc03a">

-----

- Counts number of button inputs per button
- Warns if your d-pad produces invalid input (more than two directions pressed at once)
- Produces pleasant tones (in stereo with headphones)
- Color gradient to help detect CPU transplant skill issues
- Border to check screens alignment
- Link port test
- Detects CPU type (**D**MG, **M**GB, **S**GB, **C**GB, **A**GB)
- Speedrun Mode: Press 1000 buttons as fast as you can to get a time displayed
- 16 different color themes
- Tastes and smells better than the competition

## D-Pad Test

Along with testing the response of your buttons, the rom will warn you if your d-pad is able to press more than 2 directions at once. Pressing more than 2 directions together can cause glitches in some games, notably Link's Awakening.

## Link Port Test

The rom will send an incremental byte of data over the link cable port to do a basic test. You can test the link port on two systems by linking them together, and then pressing a button on one system to set it as the "master" system. Both programs will increment a number in the bottom right, indicating they are sending a byte back and forth.

You can also test the link port of a single system by connecting the rx and tx connections on a link cable together, to make a "loopback" cable.

## Color Themes

To customize your color theme...

1. Enter the "Secret Menu" by inputing the Konami Code - `Up Up Down Down Left Right Left Right B A`.
1. Scroll through the different color themes with the left and right directional buttons.
1. Press a non directional button to exit the menu and save your selection.

> [!NOTE]
> Note that the menu only opens on CGB & AGB (color systems)

### Saving Your Theme

Better Button Test saves your theme selection using your cartridge's save memory. If your cartridge does not have save memory, your theme selection will not persist. However, Better Button Test _does_ support battery-less saving on cartridges using SST39SF010 flash memory, a relatively popular chip for simple homemade cartridges. This is accomplished by using flash programming commands to alter the rom stored in the flash memory.

If your cartridge does not have save memory, and your flash chip is not a SST39SF010 series chip, you have the option to customize the theme by altering the rom file prior to flashing. To select your theme, change the byte at location `0x7FFF` from `0xFF` to a number between `0x00` and `0x0F` that correlates to the index of the theme you'd like to use.
