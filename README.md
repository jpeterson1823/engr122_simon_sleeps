# ENGR 122 - 002 Final Project : Simon Sleeps

## General Information
### What is this all about?
Simon Sleeps is an alarm clock system that has two modules: the alarm module, and the simon module.
Both of these modules work together to ensure that the user is fully awake before disabling the alarm.

## Module Information
### Alarm Module
The Alarm module is tasked with waking the user up. It does this by becoming one of the most annoying things known to humanity for the duration it takes you to complete the various rounds of Simon required to disable the alarm. It has a piezo speaker to bombard the user's ears and four 3W (9V @ 300mA) LEDs to light the room up. Strobing is an option, but is disabled by default. It has a 16x2 LCD screen to allow the user to see the current time and their alarm.

### Simon Module
The Simon module is in charge of disabling the alarm once it has sounded. It uses the roll and pitch of the module as the user's input for the Simon game. Once the various rounds of Simon have been successfully finished, it will send the disable command to the Alarm Module.

## Finances
![Image](readme_docs/e-and-l.pdf?raw=true "Energy and Labor Cost")
