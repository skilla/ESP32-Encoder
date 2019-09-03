//
// Created by Sergio Zambrano on 31/08/2019.
//

#ifndef econder_h_
    #define encoder_h_

    #ifndef ENCODER_DIVISOR
        #define ENCODER_DIVISOR 1;
    #endif

    #include "direct_pin_read.h"
    #include "interrupt_pins.h"
    #define ENCODER_ARGLIST_SIZE CORE_NUM_INTERRUPT

    typedef struct {
        volatile IO_REG_TYPE * pin1_register;
        volatile IO_REG_TYPE * pin2_register;
        volatile IO_REG_TYPE * pin3_register;
        IO_REG_TYPE            pin1_bitmask;
        IO_REG_TYPE            pin2_bitmask;
        IO_REG_TYPE            pin3_bitmask;
        uint8_t                state;
        int32_t                position;
        bool                   button;
    } Encoder_internal_state_t;

    class ESP32Encoder {
    public:
        ESP32Encoder(uint8_t pin1, uint8_t pin2, uint8_t button) {
            pinMode(pin1, INPUT_PULLUP);
            pinMode(pin2, INPUT_PULLUP);
            pinMode(button, INPUT_PULLUP);
            encoder.pin1_register = PIN_TO_BASEREG(pin1);
            encoder.pin1_bitmask = PIN_TO_BITMASK(pin1);
            encoder.pin2_register = PIN_TO_BASEREG(pin2);
            encoder.pin2_bitmask = PIN_TO_BITMASK(pin2);
            encoder.pin3_register = PIN_TO_BASEREG(button);
            encoder.pin3_bitmask = PIN_TO_BITMASK(button);
            encoder.position = 0;
            // allow time for a passive R-C filter to charge
            // through the pullup resistors, before reading
            // the initial state
            delayMicroseconds(2000);
            uint8_t s = 0;
            if (DIRECT_PIN_READ(encoder.pin1_register, encoder.pin1_bitmask)) s |= 1;
            if (DIRECT_PIN_READ(encoder.pin2_register, encoder.pin2_bitmask)) s |= 2;
            encoder.button = !(DIRECT_PIN_READ(encoder.pin3_register, encoder.pin3_bitmask));
            encoder.state = s;
            interrupts_in_use = attach_interrupt(pin1, &encoder);
            interrupts_in_use += attach_interrupt(pin2, &encoder);
            interrupts_in_use += attach_interrupt(button, &encoder);
        }

        inline int32_t read() {
            if (interrupts_in_use < 2) {
                noInterrupts();
                update(&encoder);
            } else {
                noInterrupts();
            }
            int32_t ret = encoder.position;
            interrupts();
            return ret / ENCODER_DIVISOR;
        }

        inline int32_t readAndReset() {
            if (interrupts_in_use < 2) {
                noInterrupts();
                update(&encoder);
            } else {
                noInterrupts();
            }
            int32_t ret = encoder.position;
            encoder.position = 0;
            interrupts();
            return ret;
        }

        inline void write(int32_t p) {
            noInterrupts();
            encoder.position = p;
            interrupts();
        }

        inline bool button() {
            return encoder.button;
        }

    public:
        static Encoder_internal_state_t * interruptArgs[ENCODER_ARGLIST_SIZE];

        // update() is not meant to be called from outside ESP32Encoder,
        // but it is public to allow static interrupt routines.
        // DO NOT call update() directly from sketches.
        static void update(Encoder_internal_state_t *arg) {
            uint8_t p1val = DIRECT_PIN_READ(arg->pin1_register, arg->pin1_bitmask);
            uint8_t p2val = DIRECT_PIN_READ(arg->pin2_register, arg->pin2_bitmask);
            uint8_t button = DIRECT_PIN_READ(arg->pin3_register, arg->pin3_bitmask);
            uint8_t state = arg->state & 3;
            if (p1val) state |= 4;
            if (p2val) state |= 8;
            arg->state = (state >> 2);
            arg->button = !button;
            switch (state) {
                case 1: case 7: case 8: case 14:
                    arg->position++;
                    return;
                case 2: case 4: case 11: case 13:
                    arg->position--;
                    return;
                case 3: case 12:
                    arg->position += 2;
                    return;
                case 6: case 9:
                    arg->position -= 2;
                    return;
            }
        }

    private:
        Encoder_internal_state_t encoder;
        uint8_t interrupts_in_use;

        static uint8_t attach_interrupt(uint8_t pin, Encoder_internal_state_t *state) {
            switch (pin) {
                case CORE_INT0_PIN:
                    interruptArgs[0] = state;
                    attachInterrupt(0, isr0, CHANGE);
                    break;
                case CORE_INT1_PIN:
                    interruptArgs[1] = state;
                    attachInterrupt(1, isr1, CHANGE);
                    break;
                case CORE_INT2_PIN:
                    interruptArgs[2] = state;
                    attachInterrupt(2, isr2, CHANGE);
                    break;
                case CORE_INT3_PIN:
                    interruptArgs[3] = state;
                    attachInterrupt(3, isr3, CHANGE);
                    break;
                case CORE_INT4_PIN:
                    interruptArgs[4] = state;
                    attachInterrupt(4, isr4, CHANGE);
                    break;
                case CORE_INT5_PIN:
                    interruptArgs[5] = state;
                    attachInterrupt(5, isr5, CHANGE);
                    break;
                case CORE_INT12_PIN:
                    interruptArgs[12] = state;
                    attachInterrupt(12, isr12, CHANGE);
                    break;
                case CORE_INT13_PIN:
                    interruptArgs[13] = state;
                    attachInterrupt(13, isr13, CHANGE);
                    break;
                case CORE_INT14_PIN:
                    interruptArgs[14] = state;
                    attachInterrupt(14, isr14, CHANGE);
                    break;
                case CORE_INT15_PIN:
                    interruptArgs[15] = state;
                    attachInterrupt(15, isr15, CHANGE);
                    break;
                case CORE_INT16_PIN:
                    interruptArgs[16] = state;
                    attachInterrupt(16, isr16, CHANGE);
                    break;
                case CORE_INT17_PIN:
                    interruptArgs[17] = state;
                    attachInterrupt(17, isr17, CHANGE);
                    break;
                case CORE_INT18_PIN:
                    interruptArgs[18] = state;
                    attachInterrupt(18, isr18, CHANGE);
                    break;
                case CORE_INT19_PIN:
                    interruptArgs[19] = state;
                    attachInterrupt(19, isr19, CHANGE);
                    break;
                case CORE_INT21_PIN:
                    interruptArgs[21] = state;
                    attachInterrupt(21, isr21, CHANGE);
                    break;
                case CORE_INT22_PIN:
                    interruptArgs[22] = state;
                    attachInterrupt(22, isr22, CHANGE);
                    break;
                case CORE_INT23_PIN:
                    interruptArgs[23] = state;
                    attachInterrupt(23, isr23, CHANGE);
                    break;
                case CORE_INT25_PIN:
                    interruptArgs[25] = state;
                    attachInterrupt(25, isr25, CHANGE);
                    break;
                case CORE_INT26_PIN:
                    interruptArgs[26] = state;
                    attachInterrupt(26, isr26, CHANGE);
                    break;
                case CORE_INT27_PIN:
                    interruptArgs[27] = state;
                    attachInterrupt(27, isr27, CHANGE);
                    break;
                case CORE_INT32_PIN:
                    interruptArgs[32] = state;
                    attachInterrupt(32, isr32, CHANGE);
                    break;
                case CORE_INT33_PIN:
                    interruptArgs[33] = state;
                    attachInterrupt(33, isr33, CHANGE);
                    break;
                case CORE_INT34_PIN:
                    interruptArgs[34] = state;
                    attachInterrupt(34, isr34, CHANGE);
                    break;
                case CORE_INT35_PIN:
                    interruptArgs[35] = state;
                    attachInterrupt(35, isr35, CHANGE);
                    break;
                case CORE_INT36_PIN:
                    interruptArgs[36] = state;
                    attachInterrupt(36, isr36, CHANGE);
                    break;
                case CORE_INT39_PIN:
                    interruptArgs[39] = state;
                    attachInterrupt(39, isr39, CHANGE);
                    break;
                default:
                    return 0;
            }
            return 1;
        }

        static void isr0(void) { update(interruptArgs[0]); }
        static void isr1(void) { update(interruptArgs[1]); }
        static void isr2(void) { update(interruptArgs[2]); }
        static void isr3(void) { update(interruptArgs[3]); }
        static void isr4(void) { update(interruptArgs[4]); }
        static void isr5(void) { update(interruptArgs[5]); }
        static void isr12(void) { update(interruptArgs[12]); }
        static void isr13(void) { update(interruptArgs[13]); }
        static void isr14(void) { update(interruptArgs[14]); }
        static void isr15(void) { update(interruptArgs[15]); }
        static void isr16(void) { update(interruptArgs[16]); }
        static void isr17(void) { update(interruptArgs[17]); }
        static void isr18(void) { update(interruptArgs[18]); }
        static void isr19(void) { update(interruptArgs[19]); }
        static void isr21(void) { update(interruptArgs[21]); }
        static void isr22(void) { update(interruptArgs[22]); }
        static void isr23(void) { update(interruptArgs[23]); }
        static void isr25(void) { update(interruptArgs[25]); }
        static void isr26(void) { update(interruptArgs[26]); }
        static void isr27(void) { update(interruptArgs[27]); }
        static void isr32(void) { update(interruptArgs[32]); }
        static void isr33(void) { update(interruptArgs[33]); }
        static void isr34(void) { update(interruptArgs[34]); }
        static void isr35(void) { update(interruptArgs[35]); }
        static void isr36(void) { update(interruptArgs[36]); }
        static void isr39(void) { update(interruptArgs[39]); }
};

#undef attachInterrupt

#endif //encoder_h_
