#pragma once
typedef unsigned int uint;

class StepperController {
private:
    uint stepsPerRevolution, stepDelay, currentPosition, currentStep, pin1, pin2, pin3, pin4;
    void motorStep(bool isClockwise);
public:
    void init(uint stepsPerRevolution, uint speed, uint pin1, uint pin2, uint pin3, uint pin4);
    void step(int steps);
    uint getCurrentStep();
};