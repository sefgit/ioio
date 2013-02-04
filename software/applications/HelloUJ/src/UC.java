public class UC{

//clocking
	public static int clkGet(){return 0;}		//get num clocks expired so far
	public static int clkSpeed(){return 0;}		//get num clock ticks per second
	public static int instrsGet(){return 0;}	//number of java instructions executed so far
	public static void msDelay(int ms) {};
	public static void HardReset() {};
	public static void SoftReset() {};
    // digital
	public static void toggleLED(int state) {}; // 0: off, 1: on; -1: toggle
	public static void SetDigitalOutLevel(int pin, int value) {};
/*	
	public static int input(int maxsize, byte[] data) {return 0;};
	public static void output(int maxsize, byte[] data) {};
	public static void error(int maxsize, byte[] data) {};
    // ADC
	public static void ADCSetScan(int pin, int enable) {};
	public static void ADCClrScan(int pin) {};
    // I2C
    // rate is 0:off 1:100KHz, 2:400KHz, 3:1MHz
	public static void I2CConfigMaster(int i2c_num, int rate, int smbus_levels) {};
	public static void I2CWriteRead(int i2c_num, int addr, byte[] data, int write_bytes, int read_bytes) {};
    // InCap
            // mode:
            //   0: off
            //   1: high-pulse
            //   2: low-pulse
            //   3: rise-to-rise
            //   4: rise-to-rise x 4
            //   5: rise-to-rise x 16
            //
            // clock:
            //   0: 16MHz
            //   1: 2MHz
            //   2: 250KHz
            //   3: 62.5KHz
	public static void InCapConfig(int incap_num, int double_prec, int mode, int clock) {};
    // PWM
	public static void SetPwmDutyCycle(int pwm_num, int dc, int fraction) {};
	public static void SetPwmPeriod(int pwm_num, int period, int scale) {};
    // SPI
	public static void SPIConfigMaster(int spi_num, int scale, int div, int smp_end, int clk_edge, int clk_pol) {};
	public static void SPITransmit(int spi_num, int dest, byte[] data, int data_size, int total_size, int trim_rx) {};
    // UART
	public static void UARTConfig(int uart_num, int rate, int speed4x, int two_stop_bits, int parity) {};
	public static void UARTTransmit(int uart_num, byte[] data, int size) {};
*/	
}