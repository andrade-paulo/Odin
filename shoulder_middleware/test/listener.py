import socket
import time

# Importa o dialeto gerado pelo mavgen
import crow_telemetry

UDP_IP = "0.0.0.0"  # Escuta em todas as interfaces de rede locais
UDP_PORT = 14550    # A porta configurada no udp_streamer.c do ESP32

def main():
    print(f"Iniciando Cliente de Teste MAVLink UDP na porta {UDP_PORT}...")
    
    # 1. Configuração do Socket L4
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    sock.bind((UDP_IP, UDP_PORT))
    
    # 2. Inicialização do Parser MAVLink
    # O pymavlink espera um objeto "file-like" para output de logs internos.
    # Criamos um dummy handler para evitar que ele exija um arquivo físico.
    class DummyFile:
        def write(self, b):
            pass
            
    dummy = DummyFile()
    
    # Instancia a classe principal do dialeto (System ID 255 = GCS)
    mav = crow_telemetry.MAVLink(dummy, srcSystem=255, srcComponent=190)

    print("Aguardando telemetria do middleware...")

    # Variáveis para métricas simples
    pacotes_recebidos = 0
    start_time = time.time()

    while True:
        try:
            # Bloqueia até receber um datagrama (buffer de 1024 bytes é suficiente para MAVLink)
            data, addr = sock.recvfrom(1024)
            
            # O parse_buffer lida com pacotes fragmentados ou múltiplos no mesmo datagrama
            messages = mav.parse_buffer(data)
            
            if messages:
                for msg in messages:
                    pacotes_recebidos += 1
                    
                    # O método to_dict() formata o output lindamente com as chaves do XML
                    msg_dict = msg.to_dict()
                    msg_type = msg.get_type()
                    
                    print(f"[{msg_type}] {msg_dict}")
                    
        except KeyboardInterrupt:
            print("\nEncerrando cliente...")
            break
        except Exception as e:
            print(f"Erro no decodificador: {e}")

if __name__ == "__main__":
    main()