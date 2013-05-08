from subprocess import Popen, PIPE, STDOUT

# Loop de execução (50 vezes)
# Ou seja, executamos esse script para cada tipo de requisição
for x in range(0, 50):

	# Executao o client no ip do servidor especificado, usando o usuario do tipo "livraria"
	# Para cada opção executada no cliente, esse arquivo deve ser modificado com uma única instrução no cliente
	p = Popen(['./client', '143.106.16.22', 'library'],stdin=PIPE)

	# Apos a conexao, entramos com uma das opcoes disponiveis para fazer a requisicao no servidor,
	# Onde \n simula a tecla "enter" apertada pelo usuário

	# Exemplo de requisição "Exibir todas informacoes de todos os livros":
	# Entramos com a opção 4 e em seguida com a opção sair (0)
	# p.stdin.write('4\n 0\n')

	# Exemplo de requisição "Alterar a quantidade de um livro"
	# Entramos com a opção 6, em seguida com o ISBN do livro, a nova quantidade, e depois a opção sair (0)
	# p.stdin.write('6\n 0385537859\n 21\n 0\n')

	# Exemplo de requisição "Exibir a quantidade de um livro"
	# Entramos com a opção 5, em seguida com o ISBN do livro, e depois a opção sair (0)
	# p.stdin.write('5\n 0385537859\n 0\n')

	# Esse comando espera a resposta vir do servidor e ser impressa na tela
	p.wait()
