# ESP32 Flow Meter
This project is a simple flow meter driver using an ESP32 and comparator. The ESP32 will read the pulses from the sensor and calculate the flow rate and total volume of water that has passed through the sensor.

## Hardware
This project was made using a bare flow meter that behaves similarly to a generator, producing a sinusoidal waveform with a frequency proportional to the flow rate. An LM393 comparator was used to convert the sinusoidal waveform to a square wave that can be read by the ESP32. The ESP32 was used to read the pulses from the comparator and calculate the flow rate and total volume of water that has passed through the sensor based on the specified conversion factor. The configuration of the pulse generation hardware is shown in the schematic below:

<div style="position: relative; width: 100%; padding-top: calc(max(56.25%, 400px));">
  
  <a href="https://app.cirkitdesigner.com/project/9b82d255-7e2a-4ff6-a056-30086dd47079?view=interactive_preview" style="position: absolute; top: 0; left: 0; width: 100%; height: 100%; border: none;">
    <img src="Schematic.png" alt="Schematic">
  </a>

</div>
