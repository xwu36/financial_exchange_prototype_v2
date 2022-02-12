// https://stackoverflow.com/questions/71088486/how-to-mock-system-call-in-c-unit-testing-during-runtime-using-googlemock

#include <stdio.h>
#include <stdlib.h>

#include <exception>
#include <iostream>
#include <vector>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

using ::testing::_;
using ::testing::NiceMock;
using ::testing::Return;

// System wrapper interface class.
class MySystemWrapperInterface {
 public:
  virtual FILE* fopen(const char* filename, const char* mode) = 0;
  virtual int fseek(FILE* stream, long int offset, int whence) = 0;
  virtual size_t fread(void* ptr, size_t size, size_t nmemb, FILE* stream) = 0;
  virtual int ferror(FILE* stream) = 0;
  virtual int fclose(FILE* stream) = 0;
};

// System wrapper actual class used in production code.
class MySystemWrapperClass : public MySystemWrapperInterface {
 public:
  FILE* fopen(const char* filename, const char* mode) {
    return ::fopen(filename, mode);
  }
  int fseek(FILE* stream, long int offset, int whence) {
    return ::fseek(stream, offset, whence);
  }
  size_t fread(void* ptr, size_t size, size_t nmemb, FILE* stream) {
    return ::fread(ptr, size, nmemb, stream);
  }
  int ferror(FILE* stream) { return ::ferror(stream); }
  int fclose(FILE* stream) { return ::fclose(stream); }
};

// Mocked System wrapper used for testint.
class MySystemWrapperMockClass : public MySystemWrapperInterface {
 public:
  MOCK_METHOD(FILE*, fopen, (const char*, const char*), (override));
  MOCK_METHOD(int, fseek, (FILE*, long int, int), (override));
  MOCK_METHOD(size_t, fread, (void*, size_t, size_t, FILE*), (override));
  MOCK_METHOD(int, ferror, (FILE*), (override));
  MOCK_METHOD(int, fclose, (FILE*), (override));
};

// Wrapper class for your own readdata function.
class MyClass {
  // The system wrapper passed by dependency injection through constructor.
  MySystemWrapperInterface* system_wrapper_;

 public:
  // Initialize the system wrapper in constructor.
  MyClass(MySystemWrapperInterface* system_wrapper)
      : system_wrapper_(system_wrapper) {}

  int readdata(void* data, int size, int offset, char* filename) {
    if (data == NULL || size == 0 || filename == NULL) {
      return -1;
    }

    FILE* fp = system_wrapper_->fopen(filename, "rb");
    if (fp == NULL) {
      return -1;
    }

    if (system_wrapper_->fseek(fp, offset, SEEK_SET) != 0) {
      system_wrapper_->fclose(fp);
      return -1;
    }

    system_wrapper_->fread(data, size, 1, fp);

    if (system_wrapper_->ferror(fp)) {
      system_wrapper_->fclose(fp);
      return -1;
    }
    system_wrapper_->fclose(fp);
    return 1;
  }
};

TEST(test, Test1fseek) {
  // Create the mock object and inject it into your class.
  MySystemWrapperMockClass mock_system_wrapper;
  MyClass my_object(&mock_system_wrapper);

  // When fseek called in readdata API call mock fseek to hit fseek fail
  // fseek(){return -1;}

  // IMPORTANT: Don't forget to add EXPECT_CALL or ON_CALL for all functions
  // that are expected to be called.
  EXPECT_CALL(mock_system_wrapper, fopen)
      .Times(1)
      .WillOnce(Return(reinterpret_cast<FILE*>(0x1)));

  EXPECT_CALL(mock_system_wrapper, fseek).Times(1).WillOnce(Return(1));
  EXPECT_CALL(mock_system_wrapper, fclose).Times(1).WillOnce(Return(1));

  char data[10] = {0};
  int status = my_object.readdata(data, sizeof(data), 0, "test.txt");
  EXPECT_EQ(status, -1);
}

TEST(test, Test2fread) {
  // Create the mock object and inject it into your class.
  MySystemWrapperMockClass mock_system_wrapper;
  MyClass my_object(&mock_system_wrapper);

  // When fread called in readdata API call mock fread to hit read fail
  // fread(){return -1;}
  // IMPORTANT: Don't forget to add EXPECT_CALL or ON_CALL for all functions
  // that are expected to be called.
  EXPECT_CALL(mock_system_wrapper, fopen)
      .Times(1)
      .WillOnce(Return(reinterpret_cast<FILE*>(0x1)));
  EXPECT_CALL(mock_system_wrapper, fseek).Times(1).WillOnce(Return(0));
  EXPECT_CALL(mock_system_wrapper, fread).Times(1).WillOnce(Return(-1));
  EXPECT_CALL(mock_system_wrapper, ferror).Times(1).WillOnce(Return(-1));
  EXPECT_CALL(mock_system_wrapper, fclose).Times(1).WillOnce(Return(1));

  char data[10] = {0};
  int status = my_object.readdata(data, sizeof(data), 0, "test.txt");
  EXPECT_EQ(status, -1);
}