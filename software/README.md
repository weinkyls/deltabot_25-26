# Installing core tools
```bash
sudo apt install libopencv-dev opencv-data python3-opencv
sudo apt install opencl-headers ocl-icd-opencl-dev
sudo apt install libgstreamer1.0-dev gstreamer1.0-plugins-base gstreamer1.0-plugins-good gstreamer1.0-tools
```

# Build the project
```bash
cmake .
make
```

# Run the tests
If you want to test the wheels
```bash
./TestServoMotor <left_speed> <right_speed>
```
- `<left_speed>`: Speed value for the left servo motor(-10~10)
- `<right_speed>`: Speed value for the right servo motor(-10~10)
- `speed range`: -10 is full speed counterclockwise, 10 is full speed clockwise, 0 is no rotation
or
```bash
./TestSpeedChange
```
This test will demonstrate the motors accelerating, decelerating, and stopping automatically.

If you want to test the robot's movement
```bash
./TestBotMotion
```
This test allows you to control a robot using your keyboard in a terminal environment. You can drive the robot forward, backward, turn, adjust speed, stop, and exit the program using designated keys.
## Controls
| Key | Action         |
|-----|---------------|
| W   | Forward       |
| S   | Backward      |
| A   | Turn Left     |
| D   | Turn Right    |
| R   | Speed Up      |
| F   | Decelerate    |
| X   | Stop          |
| Z   | Quit          |

## Example

```text
Use the keyboard to control the bot: W(forward) S(backward) A(turn left) D(turn right) R(speed up) F(decelerate) X(stop) Z(quit)
Forward.....The speed is: 3
Turn left.....The speed is: 3
Speed up.....The speed is: 4
Stop.....
Quit.....
```
# Credits
- Saleh AlMulla - 2721704A@student.gla.ac.uk
- Bernd Porr -  bernd.porr@glasgow.ac.uk
- Yixuan Zha - 2974642Z@student.gla.ac.uk
