#include "../lib/encoder.h"


        Encoder::Encoder(int a, int b, float pulses, float diameter):
         pinA(a), pinB(b), ppr(pulses), D(diameter){}; 

        void Encoder::begin(){
            pinMode(pinA, INPUT_PULLUP);
            pinMode(pinB, INPUT_PULLUP);
         }

         /* IRAM_ATTR adalah instruksi khusus untuk ESP32 agar fungsi 
         diletakkan di RAM internal (bukan Flash memory). Biar lebih cepat*/ 

         void IRAM_ATTR Encoder::handleInterrupt() {
            //menentukan apakah roda bergerak maju atau mundur
            if (digitalRead(pinB) == HIGH) count++;
            else count--;
        }

        void Encoder::update(){
            unsigned long Tsekarang = millis();
            unsigned deltaTime = Tsekarang - Tlama;
            if(deltaTime > 50){ //setiap 50 ms akan dijalankan
                long deltaCount = count - lastCount;
                RPMsekarang = ((float)deltaCount/ppr)*(60000/ (float) deltaTime);
                lastCount = count;
                Tlama = Tsekarang;
            }
        }

        float Encoder::getRPM() const { return RPMsekarang; }