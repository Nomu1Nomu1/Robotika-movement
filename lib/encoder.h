#ifndef ENCODER_H
#define ENCODER_H
#include <Arduino.h>

class encoder{
    private:
        int pinA, pinB;
        float ppr;
        float D; // diameter roda omni

        volatile long count = 0;
        long lastCount = 0;
        unsigned long Tlama = 0;
        float RPMsekarang = 0;

    public:
        encoder(int a, int b, float pulses, float diameter);
        void begin();

         /* IRAM_ATTR adalah instruksi khusus untuk ESP32 agar fungsi 
         diletakkan di RAM internal (bukan Flash memory). Biar lebih cepat*/ 

        void IRAM_ATTR handleInterrupt();
        void update(); // menghitung rpm dari ticks setiap 5ms
        float getRPM() const ; // mengoutputkan rpm

};

#endif