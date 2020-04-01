#define BUT 6
#define RLED 9
#define GLED 10
#define BLED 11

#define minDelays 2             //Minimum ammount of delays user has to repeat
#define maxDelays 3             //Maximum ammount of delays user has to repeat
#define minDelay 250            //Minimum delay between blinks
#define maxDelay 750           //Maximum delay between blinks
#define tolerance 250           //Tolerance for how off player can be

void setup() {
        pinMode(BUT, INPUT);
        pinMode(RLED, OUTPUT);
        pinMode(GLED, OUTPUT);
        pinMode(BLED, OUTPUT);
        digitalWrite(RLED, 1);          //Turn off the LEDs
        digitalWrite(GLED, 1);
        digitalWrite(BLED, 1);
}

void Blink(int pin) {           //Short blink with given LED
        for (int i = 100; i != 0; i--) {
                analogWrite(pin, i);
                delay(1);
        }
        for (int i = 0; i != 100; i++) {
                analogWrite(pin, i);
                delay(1);
        }
        digitalWrite(pin, 1);
}

void Blink(int r, int g, int b) {           //Short blink with given LEDs
        for (int i = 100; i != 0; i--) {
                analogWrite(RLED, 255 - i  * r);
                analogWrite(BLED, 255 - i  * g);
                analogWrite(GLED, 255 - i  * b);
                delay(1);
        }
        for (int i = 0; i != 100; i++) {
                analogWrite(RLED, 255 - i  * r);
                analogWrite(BLED, 255 - i  * g);
                analogWrite(GLED, 255 - i  * b);
                delay(1);
        }
        digitalWrite(RLED, 1);
        digitalWrite(BLED, 1);
        digitalWrite(GLED, 1);
}

void Wait() {           //Waving animation with chosen LED when not playing
        bool flag = 0;
        int brightness = 200;
        while (digitalRead(BUT) == 0) {
                if (brightness != 0 && flag == 0) brightness--;
                if (brightness != 200 && flag == 1) brightness++;
                analogWrite(BLED, brightness);
                if (brightness == 0  && flag == 0) flag = 1;
                if (brightness == 200  && flag == 1) flag = 0;
                delay(10);
        }
        digitalWrite(BLED, 1);
}

bool userWon(int *delays, int amount) {
        int mil = 0;
        for (int i = 0; i < amount; i++) {
                while (digitalRead(BUT) == 1);
                while (digitalRead(BUT) == 0) {
                        delay(1);
                        mil++;
                        if (mil > maxDelay + tolerance) return false;
                }
                if (mil < delays[i] - tolerance || mil > delays[i] + tolerance) return false;
                else if (i != amount - 1) Blink(BLED);          //Don't blink for the last tap if it is right
                mil = 0;
        }
        return true;
}

void Play() {
        int amount = random(minDelays, maxDelays + 1);
        int delays[amount];
        for (int i = 0; i < amount; i++) {
                delays[i] = random(minDelay, maxDelay + 1);
        }
        for (int i = 0; i < amount; i++)  {
                Blink(255, 255, 0);
                delay(delays[i]);
        }
        Blink(255, 255, 0);             //Last blink to show last delay
        int mil = 0;
        while (digitalRead(BUT) == 0 && mil < 3000) {           //count ms till click
                delay(1);
                mil++;
        }
        if (mil != 3000) {              //discard game if user does not responf within 3s
                Blink(BLED);
                if (userWon(delays, amount)) {
                        Blink(GLED);
                        delay(250);
                        Blink(GLED);
                }
                else {
                        Blink(RLED);
                        delay(250);
                        Blink(RLED);
                }
        }
}

void loop() {
        Wait();
        Play();
}
