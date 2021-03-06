// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "mock_io_data.h"

#include <stdint.h>
#include <stdio.h>

#include "gtest/gtest.h"

enum ExpectedKindOfFunction { kIoDataRead, kIoDataWrite, NoExpectedValue = -1 };

typedef struct Expectation {
  int kind;
  ioAddress offset;
  ioData data;
} Expectation;

static const char* kReportExpectWriteWasRead =
    "Expected IoData_Write(0x%x, 0x%x)\n"
    "\t        But was IoData_Read(0x%x)";
static const char* kReportReadWrongAddress =
    "Expected IoData_Read(0x%x) returns 0x%x;\n"
    "\t        But was IoData_Read(0x%x)";
static const char* kReportExpectReadWasWrite =
    "Expected IoData_Read(0x%x) would return 0x%x)\n"
    "\t        But was IoData_Write(0x%x, 0x%x)";
static const char* kReportWriteDoesNotMatch =
    "Expected IoData_Write(0x%x, 0x%x)\n"
    "\t        But was IoData_Write(0x%x, 0x%x)";

static Expectation* its_expectations = nullptr;
static int set_expectation_count;
static int get_expectation_count;
static int max_expectation_count;
static bool failure_already_reported = false;
static Expectation its_expected;
static Expectation its_actual;

void MockIoData_Create(int expectation_count) {
  if (expectation_count <= 0) return;

  its_expectations =
      (Expectation*)calloc(expectation_count, sizeof(Expectation));
  set_expectation_count = 0;
  get_expectation_count = 0;
  max_expectation_count = expectation_count;
  failure_already_reported = false;
}

void MockIoData_Destroy(void) {
  if (!its_expectations) return;

  free(its_expectations);
  its_expectations = nullptr;
}

static void ReportFailIfNeeded(const char* message) {
  if (failure_already_reported) return;

  failure_already_reported = true;
  FAIL() << message;
}

static int FailWhenNotCreated(void) {
  if (its_expectations) return 0;

  ReportFailIfNeeded("MockIoData is not created, call MockIoData_Create()");
  return -1;
}

static int FailWhenNoRoomForExpectations(const char* function_name) {
  if (set_expectation_count < max_expectation_count) return 0;

  char message[100];
  int size = sizeof message;
  snprintf(message, size, "%s%s", function_name, ": Too many expectations");
  ReportFailIfNeeded(message);
  return -1;
}

static void RecordExpectation(int kind, ioAddress offset, ioData data) {
  its_expectations[set_expectation_count].kind = kind;
  its_expectations[set_expectation_count].offset = offset;
  its_expectations[set_expectation_count].data = data;
  set_expectation_count++;
}

void MockIoData_ExpectWrite(ioAddress offset, ioData data) {
  if (FailWhenNotCreated()) return;
  if (FailWhenNoRoomForExpectations("MockIoData_ExpectWrite")) return;
  RecordExpectation(kIoDataWrite, offset, data);
}

void MockIoData_ExpectReadThenReturn(ioAddress offset, ioData to_return) {
  if (FailWhenNotCreated()) return;
  if (FailWhenNoRoomForExpectations("MockIoData_ExpectReadThenReturn")) return;
  RecordExpectation(kIoDataRead, offset, to_return);
}

static void FailWhenNotAllExpectationsUsed(void) {
  if (get_expectation_count == set_expectation_count) return;

  char format[] = "Expected %d function(s) used but got %d";
  char message[sizeof(format) + 5 + 5];
  snprintf(message, sizeof(message), format, set_expectation_count,
           get_expectation_count);
  ReportFailIfNeeded(message);
}

void MockIoData_VerifyCompletion(void) {
  if (!failure_already_reported) FailWhenNotAllExpectationsUsed();
}

static void SetExpectedAndActual(ioAddress offset, ioData data) {
  its_expected.offset = its_expectations[get_expectation_count].offset;
  its_expected.data = its_expectations[get_expectation_count].data;
  its_actual.offset = offset;
  its_actual.data = data;
}

static void FailWhenNoUnusedExpectations(const char* format) {
  if (get_expectation_count < set_expectation_count) return;

  char message[100];
  int size = sizeof message;
  int offset = snprintf(message, size, "(%d): No more expectations but was ",
                        get_expectation_count + 1);
  snprintf(message + offset, size - offset, format, its_actual.offset,
           its_actual.data);
  ReportFailIfNeeded(message);
}

static void FailExpectation(const char* expectationFailMessage) {
  char message[100];
  int size = sizeof message - 1;
  int offset = snprintf(message, size, "R/W %d: ", get_expectation_count + 1);
  snprintf(message + offset, size - offset, expectationFailMessage,
           its_expected.offset, its_expected.data, its_actual.offset,
           its_actual.data);
  ReportFailIfNeeded(message);
}

static void FailWhen(int condition, const char* expectationFailMessage) {
  if (condition) FailExpectation(expectationFailMessage);
}

static int ExpectationIsNot(int kind) {
  return kind != its_expectations[get_expectation_count].kind;
}

static int ExpectedAddressIsNot(ioAddress offset) {
  return offset != its_expected.offset;
}

static int ExpectedDataIsNot(ioData data) { return its_expected.data != data; }

static void IoData_Write(ioAddress offset, ioData data) {
  if (FailWhenNotCreated()) return;

  SetExpectedAndActual(offset, data);
  FailWhenNoUnusedExpectations("IoData_Write(0x%x, 0x%x)");
  FailWhen(ExpectationIsNot(kIoDataWrite), kReportExpectReadWasWrite);
  FailWhen(ExpectedAddressIsNot(offset), kReportWriteDoesNotMatch);
  FailWhen(ExpectedDataIsNot(data), kReportWriteDoesNotMatch);
  get_expectation_count++;
}

void IoData_Write8bit(ioAddress offset, uint8_t data) {
  IoData_Write(offset, (uint8_t)data);
}

void IoData_Write16bit(ioAddress offset, uint16_t data) {
  IoData_Write(offset, (uint16_t)data);
}

void IoData_Write32bit(ioAddress offset, uint32_t data) {
  IoData_Write(offset, (uint32_t)data);
}

static ioData IoData_Read(ioAddress offset) {
  if (FailWhenNotCreated()) return 0;

  SetExpectedAndActual(offset, NoExpectedValue);
  FailWhenNoUnusedExpectations("IoData_Read(0x%x)");
  FailWhen(ExpectationIsNot(kIoDataRead), kReportExpectWriteWasRead);
  FailWhen(ExpectedAddressIsNot(offset), kReportReadWrongAddress);
  return its_expectations[get_expectation_count++].data;
}

uint8_t IoData_Read8bit(ioAddress offset) {
  return (uint8_t)IoData_Read(offset);
}

uint16_t IoData_Read16bit(ioAddress offset) {
  return (uint16_t)IoData_Read(offset);
}

uint32_t IoData_Read32bit(ioAddress offset) {
  return (uint32_t)IoData_Read(offset);
}
