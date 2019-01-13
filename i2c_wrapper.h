#define WAIT_TIME 100
#define SIGNAL_TIME 1000

#define SIGNAL_WAIT usleep(1000)
#define OUTPUT_WAIT usleep(100)

int init_i2c();

int reset_i2c();

void set_scl_high();

void set_scl_low();

//int read_scl();

void set_sda_high ();

void set_sda_low ();

void set_input_sda();

void set_output_sda();

int read_sda();

void debug(char val);