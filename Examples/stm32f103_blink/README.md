#STM32F103c8t6 Led Blink Example without HAL
1. Download and run STM32CubeIDE.
2. Press File -> New -> STM32 Project.
3. Select your device. Here it is STM32F103c8t6:

![Create project](https://raw.githubusercontent.com/teterevlev/STM32_No_HAL/main/Examples/stm32f103_blink/Images/1.create.png)

4. Enter **name** and **location** of your project. Set Targeted language to **C++** and **Empty** project type:

![Create project](https://raw.githubusercontent.com/teterevlev/STM32_No_HAL/main/Examples/stm32f103_blink/Images/2.configure.png)

5. Right click on your project and select **Properties**

![Create project](https://raw.githubusercontent.com/teterevlev/STM32_No_HAL/main/Examples/stm32f103_blink/Images/3.properties.png)

6. Go to C/C++ Build -> Settings -> MCU G++ Compiler -> Include paths and press ADD:

![Create project](https://raw.githubusercontent.com/teterevlev/STM32_No_HAL/main/Examples/stm32f103_blink/Images/4.settings.png)

7. Add these two paths (they should exist):

> C:\Users\%USERNAME%\STM32Cube\Repository\STM32Cube_FW_F1_V1.8.0\Drivers\CMSIS\Include
> C:\Users\%USERNAME%\STM32Cube\Repository\STM32Cube_FW_F1_V1.8.0\Drivers\CMSIS\Device\ST\STM32F1xx\Include

%USERNAME% - your username on your PC
STM32Cube_FW_F1_V1.8.0 can have another version

8. Rename **main.c** to **main.cpp**

9. Paste the example code