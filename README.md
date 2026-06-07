## Tech Stack

- STM32F091RC
- ST X-NUCLEO-IKS4A1 MEMS sensor expansion board
- C / embedded C
- Python
- pandas / NumPy
- scikit-learn
- emlearn

Using an STM32 F091RC MCU and ST X-NUCLEO-IKS4A1 MEMS sensor expansion board, built a real-time embedded motion state classifier to infer driving motions with machine learning. 

Collected data for driving motions from the IKS4A1 over I2C:
    bumps,
    clockwise_circle,
    constant_speed,
    ctr_clockwise_circle,
    hard_accel,
    hard_decel,
    left_turn,
    left_u_turn,
    right_turn,
    right_u_turn,
    smooth_accel,
    smooth_decel,
    stationary,
    weave

Turn data was collected in a parking lot at low speeds, acceleration and constant speed data on a flat, medium-smooth road. 

Transmitted data to host machine using UART.

Cleaned data and extracted 86 features using pandas and NumPy. 
Offline classifier was trained with scikit-learn random forest, using 500 estimators and max tree depth of 10, achieving ~90% accuracy when real-time constraints were not a factor. In order to convert to real-time embedded inference, each class was segmented into discrete 30 sample windows (each sample ~60ms) to simulate sliding window data collection on the F091RC. This model was exported to embedded C using emlearn, and deployed to the board. Implemented a ring buffer to transition smoothly between states. 

The real-time classifier performed well on turns and u-turns, hard acceleration/deceleration, and smooth acceleration/deceleration. It performed moderately on stationary and bumps. It performed poorly on weave, and clockwise/ctr clockwise circle. 

For better performance, the greatest gains would likely come from increasing the sensor read rate, as well as collecting data over a variety of terrains (smooth & rough roads, banks, uphill/downhill). The classifier had the most trouble with classes where data is similar, e.g smooth_accel and stationary, where noisy data makes it difficult to distinguish between states. 
