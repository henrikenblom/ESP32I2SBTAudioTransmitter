#include <Arduino.h>
#include "AudioTools.h"
#include "AudioTools/AudioLibs/A2DPStream.h"

AudioInfo info48k(44100, 1, 16);
AudioInfo info44k1(44100, 2, 16);
BluetoothA2DPSource a2dp_source;
I2SStream i2s;
FormatConverterStream conv(i2s);
const int BYTES_PER_FRAME = 4;


int32_t get_sound_data(Frame *data, int32_t frameCount) {
    return conv.readBytes((uint8_t *) data, frameCount * BYTES_PER_FRAME) / BYTES_PER_FRAME;
}

// Arduino Setup
void setup() {
    setCpuFrequencyMhz(160);
    Serial.begin(115200);
    AudioToolsLogger.begin(Serial, AudioToolsLogLevel::Info);

    // setup conversion
    conv.setBuffered(false);
    conv.begin(info48k, info44k1);

    // start i2s input with default configuration
    Serial.println("starting I2S...");
    auto cfg = i2s.defaultConfig(RX_MODE);
    cfg.i2s_format = I2S_STD_FORMAT; // or try with I2S_LSB_FORMAT
    cfg.copyFrom(info48k);
    cfg.is_master = false;
    i2s.begin(cfg);

    // start the bluetooth
    Serial.println("starting A2DP...");
    // a2dp_source.set_auto_reconnect(false);
    a2dp_source.start("SRS-XB100", get_sound_data);

    Serial.println("A2DP started");
}

// Arduino loop - repeated processing
void loop() { delay(1000); }

// SRS-XB100
