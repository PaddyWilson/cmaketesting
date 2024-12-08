#pragma once

#include <stdint.h>
#include <math.h>
#include <string>

void PackByte(uint8_t* buffer, size_t* offset, uint8_t data);
uint8_t ReadByte(uint8_t* packet, size_t* offset);

void PackByteArray(uint8_t* buffer, size_t* offset, uint8_t data[], int dataLength);

void PackShort(uint8_t* buffer, size_t* offset, short data);
int16_t ReadShort(uint8_t* packet, size_t* offset);

void PackInt(uint8_t* buffer, size_t* offset, int data);
int32_t ReadInt(uint8_t* packet, size_t* offset);

void PackLong(uint8_t* buffer, size_t* offset, long data);
int64_t ReadLong(uint8_t* packet, size_t* offset);

void PackFloat(uint8_t* buffer, size_t* offset, float data);
float_t ReadFloat(uint8_t* packet, size_t* offset);

void PackDouble(uint8_t* buffer, size_t* offset, double data);
double_t ReadDouble(uint8_t* packet, size_t* offset);

void PackBool(uint8_t* buffer, size_t* offset, bool data);
bool ReadBool(uint8_t* packet, size_t* offset);

void PackString(uint8_t* buffer, size_t* offset, std::string data);
std::string ReadString(uint8_t* packet, size_t* offset);