import csv
import os
import shutil
import tempfile
from datetime import datetime

class TelemetryLogger:
    FIELDNAMES = [
        'local_timestamp', # Timestamp do SO (Ground Station)
        'msg_type',        # Identificador da mensagem (ex: CROW_IMU)
        'time_boot_ms',    # Timestamp do ESP32
        'accel_x', 'accel_y', 'accel_z',
        'gyro_x', 'gyro_y', 'gyro_z',
        'mag_x', 'mag_y', 'mag_z',
        'pressure_delta', 'temperature',
        'latitude', 'longitude', 'altitude_msl', 'ground_speed', 'satellites',
        'command'
    ]

    def __init__(self):
        self.is_logging = False
        self.temp_file = None
        self.csv_writer = None
        self.temp_filepath = ""

    def start_new_log(self):
        """Inicializa um novo arquivo temporário e escreve o cabeçalho."""
        if self.is_logging:
            self.stop_log()

        # Cria um arquivo temporário no diretório nativo do SO (/tmp ou AppData/Local/Temp)
        fd, self.temp_filepath = tempfile.mkstemp(prefix="crow_flight_", suffix=".csv")
        self.temp_file = os.fdopen(fd, 'w', newline='')
        
        self.csv_writer = csv.DictWriter(self.temp_file, fieldnames=self.FIELDNAMES)
        self.csv_writer.writeheader()
        
        self.is_logging = True

    def log_message(self, msg_type: str, msg_dict: dict):
        """
        Recebe o dicionário bruto da telemetria e grava no CSV temporário.
        Deve ser alimentado com os dados BRUTOS (antes do DataStore) para preservar a integridade.
        """
        if not self.is_logging:
            return

        # Prepara a linha baseando-se nas chaves da mensagem atual
        row = {key: msg_dict.get(key, '') for key in self.FIELDNAMES}
        
        # Adiciona metadados da Ground Station
        row['local_timestamp'] = datetime.now().isoformat()
        row['msg_type'] = msg_type

        self.csv_writer.writerow(row)
        
        # Força a escrita no disco para evitar perda de dados em caso de crash
        self.temp_file.flush()
        os.fsync(self.temp_file.fileno())

    def stop_log(self):
        """Encerra o descritor de arquivo temporário."""
        if self.temp_file and not self.temp_file.closed:
            self.temp_file.close()
        self.is_logging = False

    def save_log_to_destination(self, destination_path: str) -> bool:
        """
        Copia o arquivo temporário para o destino escolhido pelo usuário e limpa o temp.
        Retorna True se sucesso, False caso contrário.
        """
        self.stop_log() # Garante que o arquivo foi fechado para leitura

        if not os.path.exists(self.temp_filepath):
            return False

        try:
            shutil.copy2(self.temp_filepath, destination_path)
            # Limpa o arquivo temporário após cópia bem-sucedida
            os.remove(self.temp_filepath)
            self.temp_filepath = ""
            return True
        except Exception as e:
            print(f"Erro ao salvar arquivo final: {e}")
            return False