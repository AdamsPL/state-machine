#include "Door.h"

int main()
{
    Door door{ClosedState{}, OpenState{}, LockedState{0}};

    door.handle(LockEvent{1234});
    door.handle(UnlockEvent{2});
    door.handle(UnlockEvent{1234});
    return 0;
}
