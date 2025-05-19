# Better Button Test
**[DOWNLOAD LATEST ROM HERE](https://github.com/orangeglo/better-button-test/releases/latest)**

<img width="250" alt="Better Button Test v5" src="https://github.com/orangeglo/better-button-test/assets/2780987/86d14f5f-8d38-422f-b695-2be29e841950">
<img width="250" alt="Better Button Test v5" src="https://github.com/orangeglo/better-button-test/assets/2780987/fcc13bf2-f2a1-45b6-9aec-6d0282be1e51">
<img width="250" alt="Better Button Test v5" src="https://github.com/orangeglo/better-button-test/assets/2780987/da4b6023-8ec4-40d6-a649-e414d8228d18">

## 

- Counts number of button inputs per button
- Warns if your d-pad produces invalid input (more than two directions pressed at once)
  - If your d-pad presses more than two direction at once, glitches can occur in some games - notably Link's Awakening. 
- Produces pleasant tones
- 16 different color themes
- Tastes and smells better than the competition

## Color Themes

To customize your color theme...

1. Enter the "Secret Menu" by inputing the Konami Code - `Up Up Down Down Left Right Left Right B A`.
1. Scroll through the different color themes with the left and right directional buttons.
1. Press a non directional button to exit the menu and save your selection.

> [!NOTE]
> Note that the menu only opens on GBA, CGB, and SGB systems (color systems)

### Saving Your Theme

Better Button Test saves your theme selection using your cartridge's save memory. If your cartridge does not have save memory, your theme selection will not persist. However, Better Button Test _does_ support battery-less saving on cartridges using SST39SF010 flash memory, a relatively popular chip for simple homemade cartridges. This is accomplished by using flash programming commands to alter the rom stored in the flash memory.

If your cartridge does not have save memory, and your flash chip is not a SST39SF010 series chip, you have the option to customize the theme by altering the rom file prior to flashing. To select your theme, change the byte at location `0x7FFF` from `0xFF` to a number between `0x00` and `0x0F` that correlates to the index of the theme you'd like to use.
