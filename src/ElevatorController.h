class ElevatorController {
private:
    enum State {
        IDLE,
        GOING_UP,
        GOING_DOWN,
        DOOR_OPENING,
        DOOR_CLOSING
    };

    State state;
public:
    void init();
    void run();
};