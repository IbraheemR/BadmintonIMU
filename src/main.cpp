#include <Arduino.h>
#include <SPI.h>
#include <SD.h>

#include <MPU6050_tockn.h>
#include <Wire.h>

MPU6050 mpu6050(Wire);

long timer = 0;

File datafile;

#define ENABLE_PIN 4

void setup()
{
    Serial.begin(115200);

    pinMode(ENABLE_PIN, INPUT_PULLUP);
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);

    randomSeed(analogRead(0));

    while (!Serial)
        ;
    Serial.print("Initializing SD card...");
    if (!SD.begin(4))
    {
        Serial.println("initialization failed!");
        while (1)
            ;
    }
    Serial.println("initialization done.");

    Wire.begin();
    mpu6050.begin();
    mpu6050.calcGyroOffsets(true);

    Serial.println("STARTING LOGGING");
}

void loop()
{
    mpu6050.update();

    bool shouldLog = digitalRead(ENABLE_PIN);

    if (shouldLog)
    {
        datafile = SD.open("data.csv", FILE_WRITE);

        datafile.print(millis());
        datafile.print(",");
        datafile.print(mpu6050.getAccX());
        datafile.print(",");
        datafile.print(mpu6050.getAccY());
        datafile.print(",");
        datafile.print(mpu6050.getAccZ());
        datafile.println();

        datafile.close();
    } else {
        Serial.println("DONE LOGGING");
        while (1) {};
    }

    digitalWrite(LED_BUILTIN, shouldLog);
}