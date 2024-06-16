#include <iostream>
#include <vector>

struct Message {
    unsigned char DeviceAddress;
    unsigned char FunctionCode;
    unsigned char MemoryCell;
    unsigned char WriteValue;
    unsigned char CRC;
};

Message parseMessage(const std::vector<unsigned char>& bytes) {
    if (bytes.size() < 4) {
        throw std::runtime_error("Message is too short.");
    }

    Message msg;
    msg.DeviceAddress = bytes[0];
    msg.FunctionCode = bytes[1];
    msg.MemoryCell = bytes[2];

    if (bytes.size() >= 5) {
        msg.WriteValue = bytes[3];
        msg.CRC = bytes[4];
    }

    return msg;
}

unsigned char crc8(const std::vector<unsigned char>& data) {
    const unsigned char poly = 0x07;
    unsigned char crc = 0;
    for (const auto& v : data) {
        crc ^= v;
        for (int i = 0; i < 8; i++) {
            if (crc & 0x80) {
                crc = (crc << 1) ^ poly;
            }
            else {
                crc <<= 1;
            }
        }
    }
    return crc;
}

int main() {
    std::vector<unsigned char> bytes = { 0x01, 0x03, 0x01, 0x53 };
    Message msg = parseMessage(bytes);

    std::cout << "Device Address: " << (int)msg.DeviceAddress << std::endl;
    std::cout << "Function Code: " << (int)msg.FunctionCode << std::endl;
    std::cout << "Memory Cell Address: " << (int)msg.MemoryCell << std::endl;
    if (msg.FunctionCode == 0x05 || msg.FunctionCode == 0x06) {
        std::cout << "Write Value: " << (int)msg.WriteValue << std::endl;
    }
    std::cout << "CRC: " << (int)msg.CRC << std::endl;

    // Check CRC
    unsigned char crc = crc8(bytes);
    if (crc != msg.CRC) {
        std::cout << "CRC check failed: calculated " << (int)crc << " but expected " << (int)msg.CRC << std::endl;
    }
    else {
        std::cout << "CRC check passed" << std::endl;
    }

    return 0;
}