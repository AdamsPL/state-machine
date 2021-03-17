#include "Door.h"

int main()
{
    Door door{ClosedState{true}, OpenState{}, LockedState{0}};

    door.handle(LockEvent{1234});
    door.handle(UnlockEvent{2});
    door.handle(UnlockEvent{1234});
    door.handle(OpenEvent{});
    door.handle(OpenEvent{});
    door.handle(LockEvent{123});
    door.handle(CloseEvent{});
    door.handle(CloseEvent{});
    door.handle(UnlockEvent{1234});
    door.handle(LockEvent{123});
    door.handle(LockEvent{123});
    door.handle(UnlockEvent{123});
    door.handle(123); // wont compile because of unknown type
    return 0;
}
