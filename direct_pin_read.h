//
// Created by Sergio Zambrano on 31/08/2019.
//

#ifndef direct_pin_read_h_
    #define direct_pin_read_h_

    #define IO_REG_TYPE                  uint32_t
    #define PIN_TO_BASEREG(pin)          (portInputRegister(digitalPinToPort(pin)))
    #define PIN_TO_BITMASK(pin)          (digitalPinToBitMask(pin))
    #define DIRECT_PIN_READ(base, mask)  (((*(base)) & (mask)) ? 1 : 0)

#endif //direct_pin_read_h_
