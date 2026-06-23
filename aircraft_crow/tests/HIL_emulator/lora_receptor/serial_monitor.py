import serial
import csv
import time

# Configurações da Porta Serial (Ajuste 'COM_PORT' para a sua porta, ex: 'COM3' no Windows ou '/dev/ttyUSB0' no Linux)
COM_PORT = '/dev/ttyACM0' 
BAUD_RATE = 115200
CSV_FILENAME = f"readings/telemetry_log_{time.strftime('%Y%m%d-%H%M%S')}.csv"

def main():
    try:
        # Inicializa conexão serial
        ser = serial.Serial(COM_PORT, BAUD_RATE, timeout=1)
        print(f"[*] Conectado à porta {COM_PORT} a {BAUD_RATE} bps.")
        print(f"[*] Gravando dados em: {CSV_FILENAME}")
        print("[*] Pressione Ctrl+C para encerrar.\n")

        with open(CSV_FILENAME, mode='a', newline='') as file:
            writer = csv.writer(file)
            
            while True:
                if ser.in_waiting > 0:
                    # Lê a linha, decodifica e remove espaços em branco/quebras de linha
                    line = ser.readline().decode('utf-8').strip()
                    
                    if line:
                        # Se for o cabeçalho impresso pelo ESP32, apenas salva
                        if line.startswith("timestamp"):
                            writer.writerow(line.split(","))
                            print(f"[CABEÇALHO] {line}")
                        else:
                            # Divide a string pela vírgula e salva como lista
                            data = line.split(",")
                            if len(data) == 6:  # Valida se a linha tem o número correto de colunas
                                writer.writerow(data)
                                print(f"[DADO RECEBIDO] {data}")

    except serial.SerialException as e:
        print(f"[ERRO] Não foi possível conectar à porta {COM_PORT}: {e}")
    except KeyboardInterrupt:
        print("\n[*] Captura de telemetria encerrada pelo usuário.")
    finally:
        if 'ser' in locals() and ser.is_open:
            ser.close()

if __name__ == '__main__':
    main()
