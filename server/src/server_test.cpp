#include "server.hpp"
#include <userver/utest/utest.hpp>

UTEST(GetCheckStatus, Basic) {
    EXPECT_EQ(
        apotheosis::CheckStatus("123"),
        "Check status for task 123: In progress\n"
    );
    EXPECT_EQ(apotheosis::CheckStatus(""), "Error: Task ID is required!\n");
}

UTEST(UploadCode, Basic) {
    EXPECT_EQ(
        apotheosis::LoadCodes("print('Hello, World!')"),
        "Code received: print('Hello, World!')\n"
    );
    EXPECT_EQ(apotheosis::LoadCodes(""), "Error: No code provided!\n");
}