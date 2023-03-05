# Better Button Test
**[DOWNLOAD LATEST ROM HERE](https://github.com/orangeglo/better-button-test/releases/latest)**

<img width="270" alt="Screen Shot 2023-02-09 at 9 48 06 PM" src="https://user-images.githubusercontent.com/2780987/217987860-c118f25b-4669-44fd-9e62-ad5741315710.png">

- Counts number of button inputs per button
- Warns if your d-pad produces invalid input (more than two directions pressed at once)
- Produces pleasant tones
- 16 different color themes
- Tastes and smells better than the competition

## Color Themes

To customize your color theme...

1. Enter the "Secret Menu" by inputing the Konami Code - `Up Up Down Down Left Right Left Right B A`.
1. Scroll through the different color themes with the left and right directional buttons.
1. Press a non directional button to exit the menu and save your selection.

### Notes On Saving

Better Button Test saves your theme selection using your cartridge's save memory. If your cartridge does not have save memory, your theme selection will not persist. However, Better Button Test _does_ support battery-less saving on cartridges using SST39SF010 flash memory, a relatively popular chip for simple homemade cartridges. This is accomplished by using flash programming commands to alter the rom stored in the flash memory.

If your cartridge does not have save memory, and your flash chip is not a SST39SF010 series chip, you have the option to customize the theme by altering the rom file prior to flashing. To select your theme, change the byte at location `0x7FFF` from `0xFF` to a number between `0x00` and `0x0F` that correlates to the index of the theme you'd like to use.