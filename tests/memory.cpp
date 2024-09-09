#include <core.h>
#include <gtest/gtest.h>

class MemoryAPIFixture : public testing::Test {
 protected:
  cpu6502 cpu;
};

TEST_F(MemoryAPIFixture, ReadByte) {
  cpu.write(0x1000, 0x64);
  EXPECT_EQ(cpu.read(0x1000), 0x64);
}

TEST_F(MemoryAPIFixture, ReadWord) {
  cpu.write16(0x1000, 0x1234);

  EXPECT_EQ(cpu.read(0x1000), 0x34);
  EXPECT_EQ(cpu.read(0x1001), 0x12);
  EXPECT_EQ(cpu.read16(0x1000), 0x1234);
}
