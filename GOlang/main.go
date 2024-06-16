package main

import (
	"fmt"
)

type Message struct {
	DeviceAddress byte
	FunctionCode  byte
	MemoryCell    byte
	WriteValue    byte
	CRC           byte
}

func parseMessage(bytes []byte) (*Message, error) {
	if len(bytes) < 4 {
		return nil, fmt.Errorf("Message is too short")
	}

	msg := &Message{
		DeviceAddress: bytes[0],
		FunctionCode:  bytes[1],
		MemoryCell:    bytes[2],
	}

	if len(bytes) >= 5 {
		msg.WriteValue = bytes[3]
		msg.CRC = bytes[4]
	}

	return msg, nil
}

func crc8(data []byte) byte {
	const poly = 0x07
	var crc byte
	for _, v := range data {
		crc ^= v
		for i := 0; i < 8; i++ {
			if crc&0x80 != 0 {
				crc = (crc << 1) ^ poly
			} else {
				crc <<= 1
			}
		}
	}
	return crc
}

func main() {
	bytes := []byte{0x01, 0x03, 0x01, 0x53}
	msg, err := parseMessage(bytes)
	if err != nil {
		fmt.Println("Error:", err)
		return
	}

	fmt.Println("Адрес устройства: ", msg.DeviceAddress)
	fmt.Println("Код функции: ", msg.FunctionCode)
	fmt.Println("Адрес ячейки памяти, куда идёт запись: ", msg.MemoryCell)
	if msg.FunctionCode == 0x05 || msg.FunctionCode == 0x06 {
		fmt.Println("Записываемое значение: ", msg.WriteValue)
	}
	fmt.Println("CRC:", msg.CRC)

	// Check CRC
	if crc := crc8(bytes[:len(bytes)-1]); crc != msg.CRC {
		fmt.Println("CRC check failed:\n\tCalculated: ", crc, "\n\tExpected: ", msg.CRC)
	} else {
		fmt.Println("CRC check passed")
	}
}
