/*
 * subscription_ut.cc
 */

#include "utils/ref_frames/include/subscription.hh"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

using namespace jeod;

class SubscriptionTest : public Subscription
{
public:
    bool get_active()
    {
        return active;
    }

    void set_active_status(bool status)
    {
        Subscription::set_active_status(status);
    }
};

TEST(Subscription, create)
{
    Subscription staticInst;
    Subscription * dynInst = new Subscription;
    delete dynInst;
}

TEST(Subscription, activate) {}

TEST(Subscription, deactivate) {}

TEST(Subscription, subscribe) {}

TEST(Subscription, unsubscribe) {}

TEST(Subscription, set_active_status)
{
    SubscriptionTest staticInst;

    EXPECT_FALSE(staticInst.get_active());

    staticInst.set_active_status(true);

    EXPECT_TRUE(staticInst.get_active());
}
