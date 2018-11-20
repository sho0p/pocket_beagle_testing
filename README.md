# pocket_beagle_testing

A whole bunch of testing with the PocketBeagle in a bare metal environment.

Built off of David Welch's (dwelch67 on GitHub) pocket_beagle_samples repositorty.

GPIO_Blink_Poll is a test with the user GPIO lights using a polling method, also known as a terrible method to do so. It counts up in binary from 0x0 to 0xF in an infinite loop, just as a test with it.

The idea of GPIO_Blink_Int is to do the exact same thing as GPIO_Blink_Poll but with interrupts in order to both familiarize myself with the interrupt interfacing and to save on power consumption (not constantly checking, just noping to save on power).
