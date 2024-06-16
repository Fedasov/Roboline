def parse_message(bytes):
    if len(bytes) < 4:
        raise Exception("Message is too short.")

    msg = {
        'DeviceAddress': bytes[0],
        'FunctionCode': bytes[1],
        'MemoryCell': bytes[2],
    }

    if len(bytes) >= 5:
        msg['WriteValue'] = bytes[3]
        msg['CRC'] = bytes[4]

    return msg

def crc8(data):
    poly = 0x07
    crc = 0
    for v in data:
        crc ^= v
        for _ in range(8):
            if crc & 0x80:
                crc = (crc << 1) ^ poly
            else:
                crc <<= 1
    return crc & 0xFF

def main():
    bytes = [0x01, 0x03, 0x01, 0x53]
    msg = parse_message(bytes)

    print("Адрес устройства: ", msg['DeviceAddress'])
    print("Код функции: ", msg['FunctionCode'])
    print("Адрес ячейки памяти, куда идёт запись: ", msg['MemoryCell'])
    if msg['FunctionCode'] == 0x05 or msg['FunctionCode'] == 0x06:
        print("Записываемое значение: ", msg['WriteValue'])
    if 'CRC' in msg:
        print("CRC:", msg['CRC'])

    # Check CRC
    crc = crc8(bytes[:-1])
    if 'CRC' in msg:
        if crc != msg['CRC']:
            print("CRC check failed: calculated", crc, "but expected", msg['CRC'])
        else:
            print("CRC check passed")

if __name__ == "__main__":
    main()
