# MegaTapePunch
Get your paper tape machine running with an Arduino.

This Project was made for the Enter Museum in Solothurn.

You have 3 diffrent modes:

1. Original Terminal Mode
2. Graphical Terminal Mode
3. Graphical Print Mode


The Original Terminal Mode is used to Print the input with ascii code. It translates the input to binary and prints them. This is the function, which was used when these machines were first used. 

The Graphical Terminal Mode is a modern approach to make the text visible with punchholes. It takes the text input, and converts each character to a graphical charachter on the tape instead of simple binary code. 

The Graphical Print Mode is used to Print out a predetermined text with the push of a button.  This mode doesnt need the input form an external computer. You can just push a button and it will print. 

## Connect Arduino to punch tape Machine.

I used the Ghielmetti FACIT 4070 to test this project. The FER 201 connector is used on this machine. I think you can use pretty much any punch tape machine with parallel input. 

This is the way i connected the FACIT 4070:

| FACIT 4070 |        | Arduino Mega |
|------------|--------|--------------|
| Pin        | Signal | Pin          |
| 1          | Ch1    | 22           |
| 2          | Ch2    | 23           |
| 3          | Ch3    | 24           |
| 4          | Ch4    | 25           |
| 5          | Ch5    | 26           |
| 6          | Ch6    | 27           |
| 7          | Ch7    | 28           |
| 8          | Ch8    | 29           |
| 10         | SD     | GND          |
| 11         | PI     | 36           |
| 12         | PR     | 35           |


Ch 1-8 are the data lines. SD is stepper direction and defines direction of the tape. If its set to high, the machine will pull the tape back in. For our purpose we can pull SD to GND. PI is Print instruction and is used to print. After the Ch lines are set, the PI will be high for 10ms and go back low. After the prit instruction is sent, the FACIT 4070 will print the holes and send the PR signal. PR is Print ready and is sent by the tape punch machine after it has punched its holes and is ready to get new instructions. 






## Set the jumpers on the Arduino. 

To Select which mode you want to use, you have to set the jumpers. 

| Pin | Mode                    |
|-----|-------------------------|
| 50  | Original Terminal Mode  |
| 51  | Graphical Terminal Mode |
| 53  | Graphical Print Mode    |


You can select the mode you want to use, by pulling the pin low (to GND). This can be done with dip switches or hardwired with jumper cables. The pins are internally connected to a pullup (20k).
Please make shure only one pin is activated at a time.





## Connect Arduino to PC (Input for mode 1/2)

For both of the terminal you can use the Serial monitor built in the Arduino IDE to send input data. It is also possible to use the Terminal on linux/mac with the command "screen <port_name> <baud_rate>". Baud_rate is set to 9600 in this case. On windows Putty is the easiest software to connect via Serial Terminal.


## Connect Button (mode 3)
To print text in mode 3, you need to connect a pushbutton to pin 52. The Button should be connectet to ground to pull the pin 52 low once pressed. 

On line 90 (or somewhere in the ButtonMode() function) in the code you can set your prefered string which will be printed.
