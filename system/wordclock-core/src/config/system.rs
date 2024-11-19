pub struct SystemConfig;

impl SystemConfig {
    pub const LED_PIN: u8 = 18;  // GPIO pin connected to the pixels (18 uses PWM!).
    // pub const LED_PIN: u8 = 10;  // GPIO pin connected to the pixels (10 uses SPI /dev/spidev0.0).

    // Number of LED pixels.
    pub const LED_COUNT: u8 = 124;

    pub const LED_FREQ_HZ: u8 = 800000; // LED signal frequency in hertz (usually 800khz)
    pub const LED_DMA: u8 = 10;  // DMA channel to use for generating signal (try 10)
    pub const LED_BRIGHTNESS: u8 = 100;  // Set to 0 for darkest and 255 for brightest

    // True to invert the signal (when using NPN transistor level shift)
    pub const LED_INVERT: bool = false;
    pub const LED_CHANNEL: u8 = 0;  // set to '1' for GPIOs 13, 19, 41, 45 or 53
}
    