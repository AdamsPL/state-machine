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
    uint32_t newKey;    
};

struct UnlockEvent
{
    uint32_t key;    
};

struct ClosedState;
struct OpenState;
class LockedState;

struct ClosedState : public Will<ByDefault<Nothing>,
                                 On<LockEvent, TransitionTo<LockedState>>,
                                 On<OpenEvent, TransitionTo<OpenState>>>
{
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

    void onEnter(const LockEvent& e)
    {
        key = e.newKey;
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

using Door = StateMachine<ClosedState, OpenState, LockedState>;
