import serial
import threading
import time
import matplotlib.pyplot as plt
import matplotlib.animation as animation
from collections import deque

# ================= CONFIGURAÇÕES =================
PORTA_SERIAL = '/dev/ttyUSB0'  # Altere para /dev/ttyUSB0 se estiver no Linux
BAUD_RATE = 115200
MAX_PONTOS = 200       # Quantidade de pontos visíveis no gráfico (eixo X)

# ================= BUFFERS THREAD-SAFE =================
# IMU
tempo_imu = deque(maxlen=MAX_PONTOS)
accel_x = deque(maxlen=MAX_PONTOS)
accel_y = deque(maxlen=MAX_PONTOS)
accel_z = deque(maxlen=MAX_PONTOS)

# Barômetro
tempo_baro = deque(maxlen=MAX_PONTOS)
pressao = deque(maxlen=MAX_PONTOS)

# Controle
rodando = True

def ler_serial():
    """Thread dedicada apenas à extração bruta e parse do CSV."""
    global rodando
    try:
        ser = serial.Serial(PORTA_SERIAL, BAUD_RATE, timeout=1)
        print(f"[*] Conectado na porta {PORTA_SERIAL} a {BAUD_RATE} bps.")
    except serial.SerialException as e:
        print(f"[!] Erro fatal: Não foi possível abrir {PORTA_SERIAL}. Fechou o PuTTY/Monitor?")
        rodando = False
        return

    while rodando:
        try:
            if ser.in_waiting > 0:
                linha = ser.readline().decode('ascii', errors='ignore').strip()
                partes = linha.split(',')

                if not partes:
                    continue

                if partes[0] == 'IMU' and len(partes) == 8:
                    try:
                        # IMU, timestamp, Ax, Ay, Az, Gx, Gy, Gz
                        t = int(partes[1])
                        ax, ay, az = float(partes[2]), float(partes[3]), float(partes[4])
                        
                        tempo_imu.append(t)
                        accel_x.append(ax)
                        accel_y.append(ay)
                        accel_z.append(az)
                    except ValueError:
                        pass # Ignora linhas corrompidas

                elif partes[0] == 'BARO' and len(partes) == 4:
                    try:
                        # BARO, timestamp, Pressao, Temp
                        t = int(partes[1])
                        p = float(partes[2])
                        
                        tempo_baro.append(t)
                        pressao.append(p)
                    except ValueError:
                        pass

                elif partes[0] == 'GPS' and len(partes) == 7:
                    # Imprime o GPS direto no console, pois coordenadas não cabem no gráfico de linha temporal
                    sats, lat, lon = partes[2], partes[3], partes[4]
                    alt, vel = partes[5], partes[6]
                    print(f"[GPS] Sats: {sats} | Lat/Lon: {lat},{lon} | Alt: {alt}m | Vel: {vel}m/s")
                    
        except Exception as e:
            print(f"[!] Erro na leitura: {e}")
            break

    ser.close()
    print("[*] Conexão serial encerrada.")

# ================= CONFIGURAÇÃO DO PLOT =================
fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(10, 6))
fig.canvas.manager.set_window_title('Odin Telemetry Dashboard')

# Eixos da Aceleração
linha_ax, = ax1.plot([], [], 'r-', label='Ax')
linha_ay, = ax1.plot([], [], 'g-', label='Ay')
linha_az, = ax1.plot([], [], 'b-', label='Az')
ax1.set_title('Aceleração Linear (m/s²)')
ax1.set_ylabel('m/s²')
ax1.legend(loc='upper right')
ax1.grid(True)

# Eixos da Pressão
linha_p, = ax2.plot([], [], 'purple', label='Pressão (Pa)')
ax2.set_title('Pressão Atmosférica - MS5611')
ax2.set_ylabel('Pascal')
ax2.set_xlabel('Tempo (ms)')
ax2.legend(loc='upper right')
ax2.grid(True)

def atualizar_grafico(frame):
    """Função chamada pelo Matplotlib para redesenhar a tela."""
    # Atualiza IMU
    if len(tempo_imu) > 0:
        ax1.set_xlim(tempo_imu[0], tempo_imu[-1])
        # Ajuste dinâmico do eixo Y para a aceleração
        min_accel = min(min(accel_x), min(accel_y), min(accel_z))
        max_accel = max(max(accel_x), max(accel_y), max(accel_z))
        ax1.set_ylim(min_accel - 2, max_accel + 2)

        linha_ax.set_data(tempo_imu, accel_x)
        linha_ay.set_data(tempo_imu, accel_y)
        linha_az.set_data(tempo_imu, accel_z)

    # Atualiza Barômetro
    if len(tempo_baro) > 0:
        ax2.set_xlim(tempo_baro[0], tempo_baro[-1])
        min_p, max_p = min(pressao), max(pressao)
        ax2.set_ylim(min_p - 10, max_p + 10)

        linha_p.set_data(tempo_baro, pressao)

    return linha_ax, linha_ay, linha_az, linha_p

# ================= EXECUÇÃO =================
if __name__ == '__main__':
    # Inicia a thread de leitura serial
    thread_serial = threading.Thread(target=ler_serial, daemon=True)
    thread_serial.start()

    # Inicia a animação (50ms = 20 FPS para a interface gráfica)
    ani = animation.FuncAnimation(fig, atualizar_grafico, interval=50, blit=False, cache_frame_data=False)

    try:
        plt.tight_layout()
        plt.show() # A thread principal fica bloqueada aqui renderizando a tela
    except KeyboardInterrupt:
        pass
    finally:
        print("[*] Encerrando o sistema...")
        rodando = False
        thread_serial.join(timeout=1.0)