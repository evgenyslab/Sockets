#include "common_header.h"
#include "server.h"
#include "client.h"
#include <gtest/gtest.h>


TEST(BasicGTEST, assetion){
    ASSERT_EQ(0,0);
}

TEST(ServerClientSuite, oneByteArray){

    //create server:
    Server s(7272);
    s.open();
    s.start();


    // create client:
    Client c(7272);
    c.start();

    //TODO create random char array message


    s.write("randomeMessage");
    // blocking read from client:
    // c.read();

    // TODO get message from client

    // TODO: assert equality of send and receive
    EXPECT_EQ(0,0);

    s.stop();
    c.stop();

}


int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}