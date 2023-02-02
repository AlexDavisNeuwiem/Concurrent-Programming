from time import sleep
from random import randint
from threading import Thread, Semaphore

def produtor():
  global buffer
  for i in range(10):
    semVazio.acquire()
    sleep(randint(0,2))
    item = 'item ' + str(i)
    buffer.append(item)
    print('Produzido %s (ha %i itens no buffer)' % (item,len(buffer)))
    semCheio.release()

def consumidor():
  global buffer
  for i in range(10):
    semCheio.acquire()
    item = buffer.pop(0)
    print('Consumido %s (ha %i itens no buffer)' % (item,len(buffer)))
    sleep(randint(0,2))
    semVazio.release()

# Inicializando o buffer
buffer = []
tam_buffer = 3

# Inicializando os semáforos
semVazio = Semaphore(tam_buffer)
semCheio = Semaphore(0)

# Inicializandos as threads
produtor = Thread(target=produtor) 
consumidor = Thread(target=consumidor)

# Encaminhando as threads para suas funções
produtor.start()
consumidor.start()

# Esperando a finalização das threads
produtor.join()
consumidor.join()
