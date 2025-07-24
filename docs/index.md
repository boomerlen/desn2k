# DESN2k Project

## Signal Table 

This table contains details of every "signal".

Signals either trigger an event (call a function), or signify system status, therefore changing how some function behaves.

Signals are generally considered global.

## High Level Diagrams

High level system diagram:
![](assets/system/HighLevelDiagram.drawio)

"There" flow:
![](assets/system/ThereFlow.drawio)

"Away" flow:
![](assets/system/AwayFlow.drawio)

Full system diagram:
![](assets/system/FullSystemDiagram.drawio)

## Event Diagrams 

Clock alarm event:
![](assets/events/ClockAlarm.drawio)

Coffee event:
![](assets/events/CoffeeEvent.drawio)

Doorbell button event:
![](assets/events/DoorbellButtonEvent.drawio)

Idle:
![](assets/events/Idle.drawio)

Sensor timer event:
![](assets/events/SensorTimerEvent.drawio)

Touch event:
![](assets/events/TouchEvent.drawio)

## Notes

#### 24.7.25

Note that touch event will need to refactor a bit since we have to poll the sensor. 

Polling, however, is a fast operation which can be interleaved with other functionality. 
Some thinking will be required, however, about when to disable touch sensor polling, when to disable other signals, etc.
