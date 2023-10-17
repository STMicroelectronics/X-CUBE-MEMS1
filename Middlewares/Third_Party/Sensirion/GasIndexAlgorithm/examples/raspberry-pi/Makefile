driver_common_sources = sensirion_common.c sensirion_i2c.c sensirion_i2c_hal.c
driver_common_headers = $(driver_common_sources:%.c=%.h)
sgp41_sources = sgp41_i2c.c
sgp40_sources = sgp40_i2c.c
sht4x_sources = sht4x_i2c.c
sht4x_version = 0.1.0
sht4x_release_name = raspberry-pi-i2c-sht4x-${sht4x_version}
sht4x_tarball = ${sht4x_release_name}.tar.gz
algorithm_dir = ../../sensirion_gas_index_algorithm
algorithm_sources = sensirion_gas_index_algorithm.c

LDFLAGS = -lm
CFLAGS = -Os -Wall -fstrict-aliasing -Wstrict-aliasing=1 -Wsign-conversion -fPIC -I.

ifdef CI
    CFLAGS += -Werror
endif

.PHONY: all clean


all: clean algorithm_example_usage

.ONESHELL:
download:
	wget -q -O ${sht4x_tarball} https://github.com/Sensirion/raspberry-pi-i2c-sht4x/archive/refs/tags/${sht4x_version}.tar.gz
	tar --strip-components=1 -xzf ${sht4x_tarball} $(driver_common_sources:%=${sht4x_release_name}/%)
	tar --strip-components=1 -xzf ${sht4x_tarball} $(driver_common_headers:%=${sht4x_release_name}/%) ${sht4x_release_name}/sensirion_config.h
	tar --strip-components=1 -xzf ${sht4x_tarball} $(sht4x_sources:%=${sht4x_release_name}/%)
	tar --strip-components=1 -xzf ${sht4x_tarball} $(sht4x_sources:%.c=${sht4x_release_name}/%.h)
	wget -q -O sgp41_i2c.c https://raw.githubusercontent.com/Sensirion/raspberry-pi-i2c-sgp41/master/sgp41_i2c.c
	wget -q -O sgp41_i2c.h https://raw.githubusercontent.com/Sensirion/raspberry-pi-i2c-sgp41/master/sgp41_i2c.h
	wget -q -O sgp40_i2c.c https://raw.githubusercontent.com/Sensirion/raspberry-pi-i2c-sgp40/master/sgp40_i2c.c
	wget -q -O sgp40_i2c.h https://raw.githubusercontent.com/Sensirion/raspberry-pi-i2c-sgp40/master/sgp40_i2c.h
	rm -f ${sht4x_tarball}
	cp ${algorithm_dir}/sensirion_gas_index_algorithm.c .
	cp ${algorithm_dir}/sensirion_gas_index_algorithm.h .

algorithm_example_usage:
	$(CC) $(CFLAGS) -o $@ ${driver_common_sources} ${sgp41_sources} ${sht4x_sources} ${algorithm_sources} algorithm_example_usage.c $(LDFLAGS)

low_power_example:
	$(CC) $(CFLAGS) -o $@ ${driver_common_sources} ${sgp40_sources} ${sht4x_sources} ${algorithm_sources} low_power_example.c $(LDFLAGS)

clean:
	$(RM) algorithm_example_usage
	$(RM) low_power_example
