#include "mgos.h"

#include <stdint.h>

#include <stddef.h>

#include <mgos_i2c.h>

#include <mgos_system.h>

int32_t bh1750_init() {
  // Get the i2c bus instance
  struct mgos_i2c * i2c = mgos_i2c_get_global();
  if (NULL == i2c) {
    LOG(LL_INFO, ("Could not get i2c global instance"));
    return -1;
  }

  // power ups
  int8_t cmd = 0x01;
  bool ok = mgos_i2c_write(i2c, 0x23, & cmd, 1, true); // dh1750 default address is 0x23 (no register)
  if (!ok) {
    return -2;
  }

  // Set continuous H resolution mode
  cmd = 0x10;
  ok = mgos_i2c_write(i2c, 0x23, & cmd, 1, true); // dh1750 default address is 0x23 (no register)
  return ok ? 0 : -3;
}

int32_t bh1750_read() {
  // Get the i2c bus instance
  struct mgos_i2c * i2c = mgos_i2c_get_global();
  if (NULL == i2c) {
    LOG(LL_INFO, ("Could not get i2c global instance"));
    return -1;
  }

  uint16_t data = 0;
  uint16_t lux = 0;

  // Continuous H resolution data reading
  // bh1750 default address is 0x23, no register, read directly from the bus
  bool ok = mgos_i2c_read(i2c, 0x23, & data, 2, true);
  if (ok) {
    // The data is 2 bytes, the first byte received is high, and the second byte is low.
    // After the transformation according to the datesheet calculation requirements divided by 1.2
    ((uint8_t * )( & lux))[0] = ((uint8_t * )( & data))[1];
    ((uint8_t * )( & lux))[1] = ((uint8_t * )( & data))[0];
    lux /= 1.2;
  }
  return ok ? lux : -2;
}

// app default initialization, if you do not want bh1750 does not start can be detected here
enum mgos_app_init_result mgos_app_init(void) {
  LOG(LL_INFO, ("Hi there"));
  return MGOS_APP_INIT_SUCCESS;
}
