#include <map>
#include <vector>

#include "gmock/gmock.h"
#include "gtest/gtest.h"
using ::testing::Lt;
using ::testing::PrintToString;

struct Person {
  Person(std::string_view name, int age) : name(name), age(age) {}

  // Define this for EXPECT_EQ and ASSERT_EQ.
  bool operator==(const Person& rhs) const {
    return (name == rhs.name && age == rhs.age);
  }

  // Google test will call either operator<< or PrintTo functions to print your
  // object. If both are defined, it will use PrintTo.
  friend std::ostream& operator<<(std::ostream& os, const Person& person) {
    return os << person.DebugString();
  }

  friend void PrintTo(const Person& person, std::ostream* os) {
    *os << person.DebugString();
  }

  // This function will be called by testing::PrintToString.
  std::string DebugString() const {
    std::string result;
    return result + "name: " + name + ", age: " + std::to_string(age) + "\n";
  }

  std::string name;
  int age;
};

TEST(PersonTest, CanAssertEqualitySimple) {
  Person p_ari("Ari", 25);
  Person p_ted("Ted", 21);

  EXPECT_EQ(p_ari.name, p_ted.name);
  EXPECT_EQ(p_ari.age, p_ted.age);
}

TEST(PersonTest, CanDirectlyAssertEquality) {
  Person p_ari("Ari", 25);
  Person p_ted("Ted", 21);

  EXPECT_EQ(p_ari, p_ari);
  EXPECT_EQ(p_ari, p_ted);
}

TEST(PersonTest, CanDirectlyAssertEqualityAndPrint) {
  Person p_ari("Ari", 25);
  Person p_ted("Ted", 21);

  EXPECT_EQ(p_ari, p_ted) 
    << PrintToString(p_ari) 
    << p_ted.DebugString();
}
