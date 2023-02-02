from time import sleep
from random import randint
from threading import Thread, Lock, Condition

def produtor():
  global buffer
  for i in range(10):
    sleep(randint(0,2))
    item = 'item ' + str(i)
    with lock:
      while len(buffer) == tam_buffer:
        print('>>> Buffer cheio. Produtor ira aguardar.')
        lugar_no_buffer.wait()
      buffer.append(item)
      print('Produzido %s (ha %i itens no buffer)' % (item,len(buffer)))
      item_no_buffer.notify()

def consumidor():
  global buffer
  for i in range(10):
    with lock:
      while len(buffer) == 0:
        print('>>> Buffer vazio. Consumidor ira aguardar.')
        item_no_buffer.wait()
      item = buffer.pop(0)
      print('Consumido %s (ha %i itens no buffer)' % (item,len(buffer)))
      lugar_no_buffer.notify()
    sleep(randint(0,2))

# Inicializando o buffer
buffer = []
tam_buffer = 5

# Inicializando o mutex
lock = Lock()
lugar_no_buffer = Condition(lock)
item_no_buffer = Condition(lock)

# Número de threads
n_threads = 2

# Arrays que armazenarão as threads
produtoras = []
consumidoras = []

# Inicializando as threads
for i in range(n_threads):
  p = Thread(target=produtor)
  produtoras.append(p)
  c = Thread(target=consumidor)
  consumidoras.append(c)

# Encaminhando as threads para suas funções
for i in range(n_threads):
  produtoras[i].start()
  consumidoras[i].start()

# Esperando a finalização das threads
for i in range(n_threads):
  produtoras[i].join()
  consumidoras[i].join()
