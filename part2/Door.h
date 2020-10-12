#include <cstdint>

#include "fsm/StateMachine.h"
#include "fsm/actions/Nothing.h"
#include "fsm/actions/TransitionTo.h"
#include "fsm/actions/Maybe.h"
#include "fsm/actions/ByDefault.h"
#include "fsm/actions/On.h"
#include "fsm/actions/Will.h"

struct OpenEvent
{
};

struct CloseEvent
{
};

struct LockEvent
{
    explicit LockEvent(uint32_t newKey) : newKey(newKey) {}
    uint32_t newKey;    
};

struct UnlockEvent
{
    explicit UnlockEvent(uint32_t key) : key(key) {}
    uint32_t key;    
};

struct ClosedState;
struct OpenState;
class LockedState;

struct ClosedState : public Will<ByDefault<Nothing>,
                                 On<LockEvent, TransitionTo<LockedState>>,
                                 On<OpenEvent, TransitionTo<OpenState>>>
{
    ClosedState(bool autoLock = false) : autoLock(autoLock) {}

    auto onEnter(const CloseEvent&) -> Maybe<TransitionTo<LockedState>>
    {
        if (autoLock)
            return TransitionTo<LockedState>{};
        else
            return Nothing{};
    }

    bool autoLock;
};

struct OpenState : public Will<ByDefault<Nothing>,
                               On<CloseEvent, TransitionTo<ClosedState>>>
{
};

class LockedState : public ByDefault<Nothing>
{
public:
    using ByDefault::handle;

    LockedState(uint32_t key)
        : key(key)
    {
    }

    auto onEnter(const LockEvent& e)
    {
        key = e.newKey;
        return Nothing{};
    }

    Maybe<TransitionTo<ClosedState>> handle(const UnlockEvent& e)
    {
        if (e.key == key) {
            return TransitionTo<ClosedState>{};
        }
        return Nothing{};
    }

private:
    uint32_t key;
};

using Door = StateMachine<States<ClosedState, OpenState, LockedState>,
                          Events<CloseEvent, OpenEvent, LockEvent, UnlockEvent>>;
