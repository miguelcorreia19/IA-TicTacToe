Feito em c++, no SO ubuntu 15.10.

Para compilar o programa:
     g++ galo.cpp -std=c++11

No inicio do prgrama, são feitas algumas perguntas ao jogador:

   -> Qual algoritmo? (1 -> Minimax, 2 -> Minimax com limite, 3 -> AlfaBeta, 4 -> AlfaBeta com limite)
      O jogador deverá responder 1, 2, 3 ou 4, no qual será respetivamente Minimax, Minimax com limite, AlfaBeta ou AlfaBeta com limite.

   -> Queres ser O ou X?
      O jogador deverá responder com o/O ou x/X, no qual será respetivamente O ou X.

   -> Começas tu?
      O jogador deverá responder com s/S ou n/N, no qual será respetivamente Sim ou Não.

Após isto é apresentada a matriz, questionando depois o jogador.

+-------+-------+-------+
|       |       |       |
|   1   |   2   |   3   |   
|       |       |       |
+-------+-------+-------+
|       |       |       |
|   4   |   5   |   6   |   
|       |       |       |
+-------+-------+-------+
|       |       |       |
|   7   |   8   |   9   |   
|       |       |       |
+-------+-------+-------+

   ->Em que posição queres jogar?
   	O jogador deverá responder o respetivo numero (1..9)  que deseja jogar, correspondente à matriz mostrada.

No final são mostrados os nodes visitados e o tempo correspondente a cada jogada do computador.
