#include <stdlib.h>
#include <iostream>
#include <cstdio>
#include <chrono>

using namespace std;
using namespace chrono;

bool minimax = false;
bool limit = false; 
const int numLimit = 3;
bool alfabeta = false;

bool flag = false; // Para testes
char PC; // PC
char player; // Jogador
const char draw = 'D'; // Empate

int ratioTemp = 0;
int nodes = 0;

char mat_in[9] = {'.', '.', '.', '.', '.', '.', '.', '.', '.'};
  
int statesToWin[8][3] =
  {
    {0, 1, 2},
    {3, 4, 5},
    {6, 7, 8},
    {0, 3, 6},
    {1, 4, 7},
    {2, 5, 8},
    {0, 4, 8},
    {2, 4, 6},
  };

double PCgames[5][3] =
  {
    {0, 0, 0},
    {-1, 0, 0},
    {-1, 0, 0},
    {-1, 0, 0},
    {-1, 0, 0},
  };

struct data{
  char mat[9]; // Matriz num array
  int value; // Valor do minmax/alfabeta na respetiva matriz
  int ratio; // Racio (+1 se o PC ganha, -1 se o jogador ganha)
  bool reset; // true para que ao ser o PC a jogar, não comparar o primeiro caso com os valores antigos
};

data save; // Melhor jogada atual

// Declaração de algumas funções
int maxValue(const char matOr[], int alfa, int beta, const int level);
void whoIsWho();
void whoFirst();
void whatAlgorithm();
void wherePlay();

void copy(char mat1[], const char mat2[]) // mat1 fica igual a mat2
{
  for(int i = 0; i < 9; i++)
    mat1[i] = mat2[i];
}

void print(const char mat[]) // Imprime a matriz
{
  cout << "+-------+-------+-------+" << endl;
  cout << "|       |       |       |" << endl;
  cout << "|   ";
  
  for(int i = 0; i < 9; i++)
    {
      if(mat[i] == '.')
	cout << i+1 << "   |   ";
      else
	cout << mat[i] << "   |   ";

      if(i == 8)
	{
	  cout << endl;
	  cout << "|       |       |       |" << endl;
	  cout << "+-------+-------+-------+" << endl;
	}
      else if((i+1)%3 == 0)
	{
	  cout << endl;
	  cout << "|       |       |       |" << endl;
	  cout << "+-------+-------+-------+" << endl;
	  cout << "|       |       |       |" << endl;
	  cout << "|   ";
	}
    }
}

void refresh(const char mat[], int value, int ratio) // Atualiza, caso seja para atualiar, os dados de save
{
  
  bool temp = false;
  if(save.reset)
    {
      copy(save.mat, mat);
      
      save.value = value;
      save.ratio = ratio;
      save.reset = false;
    }
  else
    {
      if(value > save.value)
	temp = true;
      else if(value == save.value)
	if(ratio > save.ratio)
	  temp = true;
      if(temp)
	{
	  copy(save.mat, mat);
	  
	  save.value = value;
	  save.ratio = ratio;
	}
    }
}

char win(const char mat[]) // Verifica se alguem ganhou
{
  bool win;
  for(int i = 0; i < 8; i++)
    {
      win = true;

      for(int j = 0; j < 2; j++)
	if(mat[statesToWin[i][j]] != mat[statesToWin[i][j+1]])
	  win = false;

      if(win)
	return mat[statesToWin[i][0]]; // Caso de vencedor
    }

  for(int i = 0; i < 9; i++)
    if(mat[i] == '.')
      return '.'; // Caso jogo por terminar 
  
  return draw; // Caso de empate
}

int utility(const char mat[], const int level) // Função utility do minmax
{
  nodes++; // Se entra aqui, significa que existe um novo node
  
  char temp = win(mat);
  if(temp == PC)
    {
      ratioTemp ++; // Vitoria do PC
      return 15 - level; // À medida que a arvore desce, o numero fica menor
      // Portanto ao fazer o Maximo, escolhe a melhor jogada do PC
    }
  else if(temp == player)
    {
      ratioTemp --; // Vitoria do jogador
      return -(15 - level); // À medida que a arvore desce, o numero fica maior
      // Portanto ao fazer o Minimo, escolhe a melhor jogada do player
    }
  else if(temp == draw)
    return 0; // Empate
  
  return -979; // Por acabar
}

int utilityLimit(const char mat[], const int level) // Função utility do minmax com limite de profundidade, contando as linhas, colunas e diagonais possiveis de vitoria
{
  bool add;
  int count = 0;
  char temp;
  //  cout << "######### incio" << endl;
  for(int i = 0; i < 8; i++)
    {
      add = true;
      temp = '.';

      for(int j = 0; j < 3; j++)
	{
	  if(mat[statesToWin[i][j]] != '.' && temp == '.')
	    temp = mat[statesToWin[i][j]];
	  else if(mat[statesToWin[i][j]] != '.' && temp != mat[statesToWin[i][j]])
	    {
	      add = false;
	      break;
	    }
	}

      if(add)
	if(temp != '.') // Se fosse igual tinha que fazer count++ porque o PC pode ganhar e count-- porque o jogador também pode ganhar
	  {
	    if(temp == PC)
	      count ++;
	    else
	      count --;
	  }
    }
 
  return count;
}

int minValue(const char matOr[], int alfa, int beta, const int level) // MinValue do minmax
{
  char mat[9];
  copy(mat, matOr);

  int aux = utility(mat, level);

  if(limit && (level >= numLimit || aux != -979)) // Se for algoritmo com limite e estiver no limite de profundidade ou for um estado terminal...
    return utilityLimit(mat, level);  
  
  if(!limit && aux != -979) // Se acabou... (Estado terminal)
    return aux;
  
  int v = 999; // +Infinito

  for(int i = 0; i < 9; i++) // Percorre a matriz para descobrir nova descendencia
    if(mat[i] == '.')
      {
	mat[i] = player;

	v = min(v, maxValue(mat, alfa, beta, level + 1)); 
	if(alfabeta) // Se for alfabeta...
	  {
	    if (v <= alfa)
	      return v;

	    beta = min(beta, v);
	  }

	mat[i] = '.';
      }
  return v;
}

int maxValue(const char matOr[], int alfa, int beta, const int level) // MaxValue do minmax
{
  char mat[9];
  copy(mat, matOr);

  int aux = utility(mat, level);
  
  if(limit && (level >= numLimit || aux != -979)) // Se for algoritmo com limite e estiver no limite de profundidade ou for um estado terminal...
    return utilityLimit(mat, level);  
  
  if(!limit && aux != -979) // Se acabou... (Estado terminal)
    return aux;
  
  int v = -999; // -Infinito
  
  for(int i = 0; i < 9; i++) // Percorre a matriz para descobrir nova descendencia
    if(mat[i] == '.')
      {
	mat[i] = PC;

	v = max(v, minValue(mat, alfa, beta, level + 1));
	if(alfabeta) // Se for alfabeta
	  {
	    if (v >= beta) 
	      return v;

	    alfa = max(alfa, v);
	  }
	mat[i] = '.';
      }
  return v;
}

void minmax(const char matOr[]) // Começa a gerar a descendencia
{
  char mat[9];
  copy(mat, matOr);
  
  save.reset = true; // Para impedir que haja comparação com os dados antigos
  
  for(int i = 0; i < 9; i++) // Percorre a matriz original para descobrir nova descendencia
    if(mat[i] == '.')
      {
	mat[i] = PC;
	ratioTemp = 0;
	int v = minValue(mat, -999, 999, 0); // Matriz, -Infinito, +Infinito, Nivel

	refresh(mat, v, ratioTemp);

	mat[i] = '.';
      }
}

void PCplay(const char matOr[])
{
  nodes = 0;          

  int temp = PCgames[0][0] ++; // Onde guarda a posição atual da matriz
  
  steady_clock::time_point begin = steady_clock::now(); //Começa a contar o tempo
      
  minmax(mat_in);
  
  steady_clock::time_point end = steady_clock::now(); // Termina de contar o tempo
  
  double diff = (double)duration_cast<nanoseconds> (end - begin).count() / 1000000000; // Faz a diferença do tempo em nanosegundos

  PCgames[temp][0] = PCgames[0][0]; // Altera de -1 para outro numero, e mantem o [0][0] correto
  PCgames[temp][1] = nodes; // Guarda os nodes visitados
  PCgames[temp][2] = diff; // Guarda o tempo
}


int main() // Main
{
  
  whatAlgorithm(); // Pergunta que algoritmo deseja

  whoIsWho(); // Pergunta quem é quem (O ou X)

  whoFirst(); // Pergunta quem começa primeiro
  
  while(win(mat_in) == '.') // Enquanto o jogo não acaba
    {
      print(mat_in);
      
      wherePlay(); // Pergunta em que posição quer jogar
       
      if(win(mat_in) != '.') // Acontece caso o comece e ganhe o PC
	break;
      
      PCplay(mat_in);
      
      copy(mat_in, save.mat); // Atualiza a matriz de jogo
    }

  print(mat_in);
  
  if(win(mat_in) == player)
    cout << "GANHASTE" << endl;
  else if(win(mat_in) == PC)
    cout << "PERDESTE" << endl;
  else
    cout << "EMPATE" << endl;

  int temp = 0;
  
  cout << endl << "O computador:" << endl; // A partir daqui imprime os nodes visitados e o tempo respetivo a cada jogada

  while(PCgames[temp][0] != -1 && temp < 5)
    {
      cout << "\tNa jogada " << temp + 1 << " visitou " << PCgames[temp][1] << " em " << PCgames[temp][2] << " segundos!" << endl;
      temp ++;
    }
  
  return 0;
}

void whoIsWho() // Para ver quem quer começar primeiro, o/O ou x/X como resposta
{
  char temp;
  
  while(true)
    {
      cout << "Queres ser O ou X?" << endl;
      cin >> temp;
            
      if(temp == 'x' || temp == 'X')
	{
	  player = 'X';
	  PC = 'O';
	  break;
	}
      else if(temp == 'o' || temp == 'O')
	{
	  player= 'O';
	  PC = 'X';
	  break;
	}
      else
	cout << "X para seres o X ou O para seres o O!!" << endl;
    }
  
  cout << endl;  
}

void whoFirst() // Para ver quem começa, n/N (não) ou s/S (sim)
{
  char temp;
  
  while(true)
    {
      cout << "Começas tu?" << endl;
      cin >> temp;

      if(temp == 'n' || temp == 'N')
	{
	  PCplay(mat_in);
	  copy(mat_in, save.mat);
	  
	  break;
	}
      else if(temp == 's' || temp == 'S')
	break;
      else
	cout << "S para começares tu ou N para começar eu!!" << endl;
    }

  cout << endl;  
}

void whatAlgorithm() // Escolher algoritmo
{
  int temp;
  
  while(true)
    {
      cout << "Qual algoritmo?" << endl << "1 -> Minimax\n2 -> Minimax com limite de profundidade\n3 -> AlfaBeta\n4 -> AlfaBeta com limite de profundidade" << endl;
      cin >> temp;

      if(temp == 1)
	{
	  minimax = true;
	  break;
	}
      else if(temp == 2)
	{
	  minimax = true;
	  limit = true;
	  break;
	}
      else if(temp == 3)
	{
	  alfabeta = true;
	  break;
	}
      else if(temp == 4)
	{
	  alfabeta = true;
	  limit = true;
	  break;
	}
      else
	cout << "Operação invalida!! Escreve só um número!!" << endl;
    }

  cout << endl;  
}

void wherePlay()
{
  int x;
  
  while(true)
    {
      cout << "Em que posição queres jogar?" << endl;
      
      cin >> x; // LE A POSIÇÃO QUE O JOGADOR QUER JOGAR
      
      cout << endl;
      
      x--;
      
      if(mat_in[x] == '.' && x >= 0 && x <9)
	break;
      else
	cout << "Essa posição já está ocupada ou não é valida!" << endl << "Escreve uma posição valida, indicando o respetivo numero!" << endl;
    }

  mat_in[x] = player;
}
