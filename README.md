# RGB Reader

**Abstract**

I will be attempting to create a small colour reader device. The RGB Reader will include a colour sensor to read the rgb values and oled sceen to display. For the demo, I intend to to get paint sample strips of red, green, and, blue when I demo my reader.

**Hardware**

I have chosen the [STM32G031](https://www.digikey.ca/en/products/detail/stmicroelectronics/STM32G031K6T6/10300266?gclsrc=aw.ds&gad_source=1&gad_campaignid=17336435733&gbraid=0AAAAADrbLlg0N1Kd5EXrgzAC7LUfLDUnC&gclid=CjwKCAjwiY_GBhBEEiwAFaghvhG4pRAglKmtf1LMJkXjHH9BUgq6LzggdycpZ285lm5ClKIdZo-x5xoCx5gQAvD_BwE) as the microcontroller as there are resources given from the instructor to give me a soft start. I have not this microcontroller before, so I will have to do some research.
Supply Voltage: 1.7 V to 3.6 V

The RGB Reader will be based on a photoelectric/colour sensor. From a wide selection, I have chosen the [TCS34725 COLOR LIGHT-TO-DIGITAL CONVERTER](https://cdn-shop.adafruit.com/datasheets/TCS34725.pdf). I intend to use it because it contains an I<sup>2</sup>C interface, but seeing it is a popular choice with the STM32 gives me more confidence in choosing it. I do not have a library at the moment, but adafruit has library support for the arduino so I can start from there.
Supply Voltage: 2.7 V to 3.6 V

The display to the user will be a standard [SSD1306 128x64 I2C OLED display](https://www.amazon.ca/IZOKEE-Display-SSD1306-Raspberry-Yellow-Blue-IIC/dp/B076PNP2VD/ref=sr_1_4?crid=YXRJPVLMW9EL&dib=eyJ2IjoiMSJ9.OiybhU4XaNG6ShwJSbKTDJvkYVE5uWbfoGIpsV5YLpBxJZDEaTAqdDtPy0tWRoJ52zOXuq_cxh_j7TJz6rQsvuurKs-8koC1GKyBl8pSYDMVw_H9vG4T5x3UwTL4c-0G3metweLWxYmtzTT2dfp_ICcF3H6XFZ6SrumeL0oEe4TA_5mWreyuYLR2lQpLqmIKqqdcu85n4UoGaor2EPOOpFk7buL7VHgKdXirYBa8lvgnERpXD73-6vSLab_szAls09LM63wOs_da7XiV6JCm6I8Yke89vqq1vI80AT_NoQA.8lL2_YlI-GB7R8w4wl2GavAd1IDedNfI48C0q6DILyc&dib_tag=se&keywords=ssd1306&qid=1757697723&s=electronics&sprefix=ssd1306%2Celectronics%2C93&sr=1-4). I will be leveraging the libraries resourced from the class BrightSpace.

Supply Voltage: The logic and I2C interface for the SSD1306 typically operate at 3.3V or 5V. 
Panel Voltage: The OLED panel itself requires a higher voltage, usually between 7V and 15V, to light  the pixels. (STILL IT ONLY NEEDS 5V)
Onboard Circuitry: Many SSD1306 boards include an integrated charge pump or boost regulator, which takes the input voltage and generates the higher voltage needed by the panel. 

This device doesn't really need to be portable, so I can power it using a micro usb.

**Research**

Things I don't know, that I will need to do research on:
- Using [SEGGER](https://www.segger.com/downloads/embedded-studio/), I haven't done it before. I will need to refer to some tutorials to get started.
- I have briefly looked at the TCS34725, all I saw was that it was poplular, uses I<sup>2</sup>C too. There are many resources on the internet for this specific sensor which gives me more confidence
- I will need to research how to implement a micro usb plug in for my design. I will need it to program and power my microcontroller
  
**Proof of Concept**

For my proof-of-life build, I will be using a development board that fits into a breadboard. I have chosen the presxisting [NUCLEO-G031K8](https://www.digikey.ca/en/products/detail/stmicroelectronics/NUCLEO-G031K8/10321671?s=N4IgTCBcDaIHIFcDGAbApgewAQHEAMAzAIwgC6AvkA) for my build. This will be powered with a USB connector from a host pc. I probably have a cable for this at home

**Stretch Goals**

If I enough time is permitted, I would like to try adding a RBG light to replicate the colour values from the sensor. The only issues that I have is relying on true colour detection. Amibient light could affect the true colour. I can decide this later.

**Final Build**

For my Final build, I would like to design and 3D Print a case for my RGB Reader. My main concern is that I do not have too much experience 3D printing, so I will have to do some research. For designing, I will the software [TinkerCAD](https://www.tinkercad.com/). I have used TinkerCAD before, but never got the chance or oppurtunity to physically print out anything. I need to design my case to enclose the sensor in a way where amibient light does not affect the values read. For example, the cool lighting could affect the output by increasing the B value.

The PCB will include the main components listed earlier. The only difference between the PCB build and the proof of life build the use of the Nucleo development board in the breadboard version. I will have to design my PCB and case to allocate space for the programmer, and other parts needed for it to work. More research needs to be done before I get to the case desgining.

**Unpopulated PCB Photos**

<img width="500" alt="image" src="https://github.com/user-attachments/assets/54e98bd3-ac96-42f9-8c12-bd65687e4d98" />
<img width="500" alt="image" src="https://github.com/user-attachments/assets/47cee29b-9ade-4c23-b42e-2633fe9e5aba" />



**Schedule**

Week 1-2:
- [x] Finalize Design (Project Proposal)
- [x] Procure proof-of-life components (MCU carrier, battery options)

Week 3-4:
- [x] Assemble proof-of-life components
- [x] Master toolchain, begin software development

Week 5-6:
- [x] Finalize design, complete initial software design

Week 7-9:
- [x] Complete PCB design, software essentially complete
- [x] Complete dimensions for case know, begin 3D case design around PCB dimensions

Week 10:
- [x] Send out PCB design for manufacture

When PCBs arrive:
- [x] assemble, program, and test with current software.
- [x] complete added features, assemble into case

Finished Dec 12 2025!
