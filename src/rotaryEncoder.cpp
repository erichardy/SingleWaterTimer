// #define ENCODER_OPTIMIZE_INTERRUPTS
#include <Encoder.h>

#include "rotaryEncoderEH.h"

bool lastClic = HIGH;
unsigned long tAntiRebond = 0;
const unsigned long DELAI_REBOND = 30;

int value = 50;
int validValue = 50;

long lastPos = 0;

int rotaryEncoderEH(Encoder encoder) {
    long pos = encoder.read();
    while (1) {
        if (pos != lastPos) {
            lastPos = pos;
            value = constrain(pos, MIN_VAL, MAX_VAL);
            // Empêche de dépasser les bornes
            encoder.write(value);
            lastPos = value;
            // Serial.print("Valeur = ");
            // Serial.println(valeur);
            // disp(value / 4);
            // return(valeur);
        // delay(100);
        }

            // ---------- Gestion du bouton ----------
        bool clic = digitalRead(PIN_SW);

            /* */
        if (clic != lastClic) {
            tAntiRebond = millis();
            // Serial.println(clic);
        }

            if ((millis() - tAntiRebond) > DELAI_REBOND) {
                static bool etatStable = HIGH;
                if (clic != etatStable) {
                    etatStable = clic;

                    if (etatStable == LOW) {
                        validValue = value;
                        // Serial.print("Valeur validée : ");
                        // Serial.println(valeurValidee);
                        return(validValue / 4);
                    }
                }
            }

        lastClic = clic;
        pos = encoder.read();
        // return(valeur / 4);
        /* */
    }
}