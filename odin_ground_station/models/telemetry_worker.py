import socket
from PyQt6.QtCore import QThread, pyqtSignal
import mavlink

class DummyFile:
    """Mock handler para o pymavlink suprimir outputs de I/O em disco"""
    def write(self, b):
        pass

class TelemetryWorker(QThread):
    # Sinais para comunicação thread-safe com a GUI
    imu_received = pyqtSignal(dict)
    baro_received = pyqtSignal(dict)
    gps_received = pyqtSignal(dict)
    cmd_received = pyqtSignal(dict)
    error_occurred = pyqtSignal(str)

    def __init__(self, udp_ip="0.0.0.0", udp_port=14550, parent=None):
        super().__init__(parent)
        self.udp_ip = udp_ip
        self.udp_port = udp_port
        self._is_running = False
        self.sock = None
        
        # System ID 255 (GCS), Component ID 190
        self.mav = mavlink.MAVLink(DummyFile(), srcSystem=255, srcComponent=190)

    def run(self):
        """Loop principal da thread."""
        self._is_running = True
        
        try:
            self.sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
            self.sock.bind((self.udp_ip, self.udp_port))
            # Timeout de 0.5s é mandatório para permitir verificação do flag _is_running
            self.sock.settimeout(0.5) 
        except Exception as e:
            self.error_occurred.emit(f"Falha ao vincular socket L4: {e}")
            self._is_running = False
            return

        while self._is_running:
            try:
                data, _ = self.sock.recvfrom(2048)
            except socket.timeout:
                continue
            except Exception as e:
                # Erros reais de hardware/SO derrubam a conexão
                self.error_occurred.emit(f"Falha crítica no socket L4: {e}")
                break

            try:
                messages = self.mav.parse_buffer(data)
                if not messages:
                    continue
                
                for msg in messages:
                    msg_type = msg.get_type()
                    msg_dict = msg.to_dict()
                    
                    if msg_type == 'CROW_IMU':
                        self.imu_received.emit(msg_dict)
                    elif msg_type == 'CROW_BARO':
                        self.baro_received.emit(msg_dict)
                    elif msg_type == 'CROW_GPS':
                        self.gps_received.emit(msg_dict)
                    elif msg_type == 'CROW_COMMAND':
                        self.cmd_received.emit(msg_dict)
                        
            except Exception as e:
                # Intercepta os erros de CRC ou frames fragmentados.
                # Imprime no terminal para debug, mas não aciona o alerta modal na GUI.
                print(f"[Drop] Frame MAVLink corrompido ignorado: {e}")
                continue
        
        # Teardown
        if self.sock:
            self.sock.close()

    def stop(self):
        """Sinaliza a interrupção do loop e aguarda o encerramento (Join)."""
        self._is_running = False
        self.wait()