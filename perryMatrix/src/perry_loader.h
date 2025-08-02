// © 2025 SC5K Systems

#pragma once

#include <Arduino.h>
#include "matrix_config.h"
#include "globals.h"

/*/
initPerryLoader
initialize perry loader animation: measure line lengths, reset write/decrypt indices, set timing, center vertically, clear display
/*/
void initPerryLoader();

/*/
getRandomCharacterP
return a random printable ASCII character (33–126) for obfuscation
/*/
char getRandomCharacterP();

/*/
writeEncryptedText4
write perry lines one char at a time as random glyphs; show and advance write index until complete
/*/
void writeEncryptedText4();

/*/
displayRandomText4
render all perry lines with decrypted chars in blue and random glyphs in gold for those still locked, then show
/*/
void displayRandomText4();

/*/
updateDecryption4
reveal one more random char per call across lines; when line fully revealed, increment p_linesDone, and refresh display
/*/
void updateDecryption4();
