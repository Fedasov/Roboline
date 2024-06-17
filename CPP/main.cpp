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
    if (bytes.size() < 4 || bytes.size() > 5) {
        throw std::runtime_error("Invalid message length. The message should be 4 or 5 bytes long.");
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
    //std::vector<unsigned char> bytes = { 0x01, 0x03, 0x01, 0x53 };
    //std::vector<unsigned char> bytes = { 0x3A, 0x01, 0x10, 0x03 };
    //std::vector<unsigned char> bytes = { 0xAB, 0x06, 0x4C, 0x13, 0xA8 };
    std::vector<unsigned char> bytes = { 0x0C, 0x05, 0x04, 0x00, 0x7C };
    Message msg = parseMessage(bytes);

    std::cout << "Device Address: " << (int)msg.DeviceAddress << std::endl;
    std::cout << "Function Code: " << (int)msg.FunctionCode << std::endl;
    std::cout << "Memory Cell Address: " << (int)msg.MemoryCell << std::endl;
    if (msg.FunctionCode == 0x05 || msg.FunctionCode == 0x06) {
        std::cout << "Write Value: " << (int)msg.WriteValue << std::endl;
    }
    std::cout << "CRC: " << (int)msg.CRC << std::endl;

    // Check CRC
    unsigned char crc = crc8(std::vector<unsigned char>(bytes.begin(), bytes.end() - 1));
    if (crc != msg.CRC) {
        std::cout << "CRC check failed: calculated " << (int)crc << " but expected " << (int)msg.CRC << std::endl;
    }
    else {
        std::cout << "CRC check passed" << std::endl;
    }

    return 0;
}