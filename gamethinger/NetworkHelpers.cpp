#include "NetworkHelpers.h"

#pragma warning( disable : 4838 4293 4267)

void PackByte(uint8_t* buffer, size_t* offset, uint8_t number)
{
	buffer[*offset] = number;
	*offset += 1;
}

uint8_t ReadByte(uint8_t* packet, size_t* offset)
{
	// get the byte at the current offset
	uint8_t data = packet[(*offset)];

	// move the offset over 1 byte for the next read
	*offset += 1;

	return data;
}

void PackByteArray(uint8_t* buffer, size_t* offset, uint8_t data[], int dataLength)
{
	buffer[*offset];

	std::copy(&data[0], &data[dataLength], &buffer[*offset]);
	*offset += dataLength;
}

void PackShort(uint8_t* buffer, size_t* offset, short number)
{
	uint8_t watch[2] = {
		   number & 255,
		   (number >> 8) & 255, };

	buffer[*offset] = watch[0];
	buffer[*offset + 1] = watch[1];

	*offset += 2;
}

int16_t ReadShort(uint8_t* packet, size_t* offset)
{
	// cast the data to a byte at the offset
	int16_t data =
		packet[*offset + 0] +
		(packet[*offset + 1] << 8);

	// move the offset over 2 bytes for the next read
	*offset = (*offset) + 2;

	// cast the data pointer to a short and return a copy
	return *(int16_t*)data;
}

void PackInt(uint8_t* buffer, size_t* offset, int number)
{
	uint8_t watch[4] = {
		number & 255,
		(number >> 8) & 255,
		(number >> 16) & 255,
		(number >> 24) & 255 };

	buffer[*offset] = watch[0];
	buffer[*offset + 1] = watch[1];
	buffer[*offset + 2] = watch[2];
	buffer[*offset + 3] = watch[3];

	*offset += 4;
}

int32_t ReadInt(uint8_t* packet, size_t* offset)
{
	int32_t data =
		 packet[*offset + 0] +
		(packet[*offset + 1] << 8) +
		(packet[*offset + 2] << 16) +
		(packet[*offset + 3] << 24);

	// move the offset over 4 bytes for the next read
	*offset = (*offset) + 4;

	// cast the data pointer to a short and return a copy
	return data;
}

void PackLong(uint8_t* buffer, size_t* offset, long number)
{
	uint8_t watch[8] = {
		number & 255,
		(number >> 8) & 255,
		(number >> 16) & 255,
		(number >> 24) & 255,
		(number >> 32) & 255,
		(number >> 40) & 255,
		(number >> 48) & 255,
		(number >> 52) & 255, };

	buffer[*offset] = watch[0];
	buffer[*offset + 1] = watch[1];
	buffer[*offset + 2] = watch[2];
	buffer[*offset + 3] = watch[3];
	buffer[*offset + 4] = watch[3];
	buffer[*offset + 5] = watch[3];
	buffer[*offset + 6] = watch[3];
	buffer[*offset + 7] = watch[3];

	*offset += 8;
}

int64_t ReadLong(uint8_t* packet, size_t* offset)
{
	// cast the data to a byte at the offset
	int64_t data =
		 packet[*offset + 0] +
		(packet[*offset + 1] << 8) +
		(packet[*offset + 2] << 16) +
		(packet[*offset + 3] << 24) +
		(packet[*offset + 4] << 32) +
		(packet[*offset + 5] << 40) +
		(packet[*offset + 6] << 48) +
		(packet[*offset + 7] << 56);

	// move the offset over 8 bytes for the next read
	*offset = (*offset) + 8;

	// cast the data pointer to a short and return a copy
	return data;
}



void PackFloat(uint8_t* buffer, size_t* offset, float data)
{
	//unions might not be the best because different cpus represent data differently
	union {
		float f;
		uint8_t b[4];
	}floatUnion;

	floatUnion.f = data;

	buffer[*offset] = floatUnion.b[0];
	buffer[*offset + 1] = floatUnion.b[1];
	buffer[*offset + 2] = floatUnion.b[2];
	buffer[*offset + 3] = floatUnion.b[3];

	*offset += 4;
}

float_t ReadFloat(uint8_t* packet, size_t* offset)
{
	union {
		float f;
		uint8_t b[4];
	}floatUnion;

	floatUnion.b[0] = packet[*offset];
	floatUnion.b[1] = packet[*offset + 1];
	floatUnion.b[2] = packet[*offset + 2];
	floatUnion.b[3] = packet[*offset + 3];

	// move the offset over 4 bytes for the next read
	*offset = (*offset) + 4;

	return floatUnion.f;
}

void PackDouble(uint8_t* buffer, size_t* offset, double data)
{
	union {
		double d;
		uint8_t b[8];
	}d;

	d.d = data;

	buffer[*offset] = d.b[0];
	buffer[*offset + 1] = d.b[1];
	buffer[*offset + 2] = d.b[2];
	buffer[*offset + 3] = d.b[3];
	buffer[*offset + 4] = d.b[4];
	buffer[*offset + 5] = d.b[5];
	buffer[*offset + 6] = d.b[6];
	buffer[*offset + 7] = d.b[7];

	*offset += 8;
}

double_t ReadDouble(uint8_t* packet, size_t* offset)
{
	union {
		double d;
		uint8_t b[8];
	}d;

	d.b[*offset] = packet[*offset];
	d.b[*offset + 1] = packet[*offset + 1];
	d.b[*offset + 2] = packet[*offset + 2];
	d.b[*offset + 3] = packet[*offset + 3];
	d.b[*offset + 4] = packet[*offset + 4];
	d.b[*offset + 5] = packet[*offset + 5];
	d.b[*offset + 6] = packet[*offset + 6];
	d.b[*offset + 7] = packet[*offset + 7];

	*offset += 8;

	return d.d;
}

void PackBool(uint8_t* buffer, size_t* offset, bool data)
{
	PackByte(buffer, offset, (uint8_t)data);
}

bool ReadBool(uint8_t* packet, size_t* offset)
{
	// cast the data to a byte at the offset
	uint8_t data = ReadByte(packet, offset);

	// cast the data pointer to a short and return a copy
	return (bool)data;
}


void PackString(uint8_t* buffer, size_t* offset, std::string data)
{
	PackInt(buffer, offset, data.size());
	std::copy(data.begin(), data.end(), &buffer[*offset]);
	*offset += data.size();
}

std::string ReadString(uint8_t* packet, size_t* offset)
{
	int msgSize = ReadInt(packet, offset);

	std::string data;
	for (size_t i = 0; i < msgSize; i++)
		data += (char)ReadByte(packet, offset);
	return data;
}





