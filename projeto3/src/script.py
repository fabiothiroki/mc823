import os
from subprocess import Popen, PIPE, STDOUT

# Loop de execucao (50 vezes)
# Ou seja, executamos esse script para cada tipo de requisicao
for x in range(0, 50):

        # Executao o client no ip do servidor especificado, usando o usuario do tipo "livraria"
	# Para cada opcao executada no cliente, esse arquivo deve ser modificado com uma unica instrucao no cliente
	# p = Popen(['java','RmiClient','143.106.16.29','49152', 'library'],stdin=PIPE)

        # Apos a conexao, entramos com uma das opcoes disponiveis para fazer a requisicao no servidor,
	# Onde \n simula a tecla "enter" apertada pelo usuario

        # Exemplo de requisicao "Alterar a quantidade de um livro"
	# Entramos com a opcao 6, em seguida com o ISBN do livro, a nova quantidade, e depois a opcao sair (0)
	# p.stdin.write('6\n')
	# p.stdin.write('0385537859\n')
	# p.stdin.write('77\n')
	# p.stdin.write('0\n')

        # Exemplo de requisicao "Exibir a quantidade de um livro"
	# Entramos com a opcao 5, em seguida com o ISBN do livro, e depois a opcao sair (0)
	# p.stdin.write('5\n 0385537859\n 0\n')
        # p.stdin.write('0385537859\n')
        # p.stdin.write('0\n')

        # Exemplo de requisicao "Exibir todas informacoes de todos os livros":
	# Entramos com a opcao 4 e em seguida com a opcao sair (0)
	# p.stdin.write('4\n')
        # p.stdin.write('0\n')

	# Esse comando espera a resposta vir do servidor e ser impressa na tela
	# p.wait()
