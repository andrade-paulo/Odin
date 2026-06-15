import serial
import time
import math


PORTA_SERIAL = '/dev/ttyUSB0'  
BAUD_RATE = 115200


def iniciar_simulacao():
    try:
        print(f"Tentando conectar na porta {PORTA_SERIAL} a {BAUD_RATE} bps...")
        esp32 = serial.Serial(PORTA_SERIAL, BAUD_RATE, timeout=1)
        
        time.sleep(2)
        print("Conexão estabelecida com sucesso!")

        print("Enviando comando para mudar o estado para RECORDING...")
        esp32.write(b"CMD,START\n")
        time.sleep(1)

        print("Iniciando injeção de telemetria HIL. Pressione Ctrl+C para parar.")
        
        timestamp_ms = 0
        
        while True:
            # Simulando uma subida suave e oscilação de aceleração
            altitude_simulada = 100.0 + (timestamp_ms / 1000.0) * 5.0 # Sobe 5m/s
            pressao_simulada = 101325.0 - (altitude_simulada * 12.0)
            
            # Simulando vibração na IMU usando uma função seno
            accel_z = 9.81 + (math.sin(timestamp_ms / 100.0) * 0.5)

            # Formato IMU: IMU,timestamp,accel_x,accel_y,accel_z,gyro_x,gyro_y,gyro_z
            msg_imu = f"IMU,{timestamp_ms},0.0,0.0,{accel_z:.2f},0.0,0.0,0.0\n"
            
            # Formato BARO: BARO,timestamp,altitude,pressao
            msg_baro = f"BARO,{timestamp_ms},{altitude_simulada:.2f},{pressao_simulada:.2f}\n"

            esp32.write(msg_imu.encode('ascii'))
            esp32.write(msg_baro.encode('ascii'))

            print(f"Injetado [{timestamp_ms}ms]: ALT={altitude_simulada:.1f}m | ACC_Z={accel_z:.2f}m/s2")

            while esp32.in_waiting > 0:
                resposta = esp32.readline().decode('ascii', errors='ignore').strip()
                if resposta:
                    print(f"<- Retorno HIL: {resposta}")

            timestamp_ms += 100
            
            # Frequência de ~10Hz para o teste
            time.sleep(0.1)

    except serial.SerialException as e:
        print(f"\n[ERRO] Não foi possível abrir a porta {PORTA_SERIAL}.")
        print(f"Detalhe: {e}")
        print("Verifique se o adaptador FTDI está conectado e se a porta está correta.")
    
    except KeyboardInterrupt:
        print("\n\nSimulação interrompida pelo usuário.")
        if 'esp32' in locals() and esp32.is_open:
            print("Enviando comando para parar a gravação (Voltando para IDLE)...")
            esp32.write(b"CMD,STOP\n")
            time.sleep(0.5)
            esp32.close()
            print("Porta serial fechada com segurança.")

if __name__ == "__main__":
    iniciar_simulacao()
