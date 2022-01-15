 //-------------------------UNIVERSIDADE FEDERAL DE CAMPINA GRANDE----------------
//.............   DEPARTAMENTO DE ENGENHARIA ELETRICA E INFORMATICA .............
//
//		                   IMPLEMENTACAO FFT - Borboleta

//..................   	     Autor: ROBSON DONATO          ......................
//                        robson.donato@ee.ufcg.edu.br
//-------------------------------------------------------------------------------

#include <iostream>
#include <math.h>
#include<fstream>
#include <sstream>
#include <string>
#include <cstring>

using namespace std;

class Complexo {
  
  float real, imaginario;
  
  public:  
  	Complexo(){} 
    ~Complexo () {}
    
    float getReal(){return real;}
    float getImag(){return imaginario;}
    void setReal(float a){real = a;}
    void setImag(float b){imaginario = b;}
    
    float getRealWn(){return real;}
    float getImagWn(){return imaginario;}
    
    
    void setRealWn(int k, int N){ 
		//Esse if else é utilizado para corrigir um erro de truncamento da função cos da biblioteca math.h
		if (cos(2*3.14159265358979323846264*k/N) < 0.00000001 && cos(2*3.14159265358979323846264*k/N) > -0.00000001) real = 0;
		else real = cos(2*3.14159265358979323846264*k/N);
	}
									
    void setImagWn(int k, int N){ imaginario = -sin(2*3.14159265358979323846264*k/N);}
};



int total_amostras(string);                              							//Funcao que retorna quantas amostras de um sinal possui um arquivo .txt
int indiceBorboleta (int, int);
void calculaFFT(int, int, int, Complexo*);
void salvatxt(char*, char, Complexo*, int);

int main() {
	
			
//-------------------- Variaveis que o usuario precisa configurar ------------------//	
	string sinal_xn = "AmostrasSinais\\sinalxn.txt";                    			//Diretorio do sinal xn
	int *N          = new int(total_amostras(sinal_xn));                            //Armazena quantas amostras tem o arquivo	
//----------------------------------------------------------------------------------//
		
	
//--------Arredonda o valor de N para o proximo multiplo de potencia de 2-----------//	
	int *e_base2   = new int;														//
	int *Nlinha    = new int(0);													//
//    																				//
	if(*N > 0) {																	//
		*e_base2 = log2(*N);														//
		*Nlinha = pow(2, *e_base2);													//
	}																				//
//    																				//
	while(*Nlinha < *N){															//
		*e_base2 = *e_base2 + 1;													//
		*Nlinha = pow(2, *e_base2);													//
	}																				//
	*N = *Nlinha;																	//
//    																				//
	delete Nlinha, e_base2;														    //Variaveis sao deletadas para liberar espaco da memoria			
//----------------------------------------------------------------------------------//
					
		
//-----------Ajuste de algumas variaveis para execucao do programa -----------------//		
	char *csinal_xn = new char[sinal_xn.length() + 1];  							//														
	strcpy(csinal_xn, sinal_xn.c_str());	            							//Convert string "sinal_xn" em um tipo char
//    																				//          							        
	float *xn    = new float[*N];                         							//Vetor para sinal xn
//----------------------------------------------------------------------------------//
	
																			  	
//----------------- Cria Vetor xn a patir do arquivo sinal_xn.txt-------------------//	
	string line;                                        							//Variavel tipo string para auxiliar na manipulacao de arquivo
	int i = 0;																		//Variavel para auxiliar na manipulacao de arquivo
//																					//		
	ifstream arquivo1 (csinal_xn);													//	
	if (arquivo1.is_open()){														//
//																					//		
		while (getline(arquivo1, line))												//
		{																			//				
			stringstream ss;														//
			ss << line;																//
			if(i < *N) ss >> xn[i];													//			
			i++;																	//
		}																			//
		arquivo1.close();															//
//																					//
		if(i < *N){																	//
			for(i; i < *N; i++)														//
				xn[i] = 0;															//
		}																			//
//																					//
	}																				//
	delete [] csinal_xn;															//Variaveis e deletada para liberar espaco da memoria		
//----------------------------------------------------------------------------------//
	

//-------------Cria matriz inicial para aplicação do algoritmo FFT------------------//
	Complexo *Borboleta = new Complexo[*N];											//
	int   *indiceTemporario = new int;												//
// 																					//
	for(int z = 0; z < *N; z++){													//
//																					//
		*indiceTemporario = indiceBorboleta(z, *N);									//
		Borboleta[z].setReal(xn[*indiceTemporario]);								//
		Borboleta[z].setImag(0);													//
//																					//
	}																				//
	delete [] xn; delete indiceTemporario;											// Variaveis sao deletadas para liberar espaco da memoria
//----------------------------------------------------------------------------------//	
		
	
//-----Aplicacao do metodo borboleta a partir do vetor Borboleta[].setReal()--------// 
	int *etapas  = new int(log2(*N));												//
	int k       = 0; 																//
   	for(k; k < *etapas; k++)   														//
   		calculaFFT(*N, *etapas, k, Borboleta);										//
   	delete etapas;																	//
//----------------------------------------------------------------------------------//


//----------------------------------------------------------------------------------//
	string parteReal       = "ParteReal.txt";										//
	string parteImaginaria = "ParteImaginaria.txt";									//
	string modulo          = "Modulo.txt";											//
	string fase            = "Fase.txt";											//
//																					//		
	char *parteReal_c = new char[parteReal.length() + 1];  							//														
	strcpy(parteReal_c, parteReal.c_str());											//
//																					//
	char *parteImaginaria_c = new char[parteImaginaria.length() + 1];  				//														
	strcpy(parteImaginaria_c, parteImaginaria.c_str());								//
//																					//	
	char *modulo_c = new char[modulo.length() + 1];  								//														
	strcpy(modulo_c, modulo.c_str());												//
//																					//
	char *fase_c = new char[fase.length() + 1];  									//														
	strcpy(fase_c, fase.c_str());													//						
//----------------------------------------------------------------------------------//
	
	
//-----------------------Salva todos os dados---------------------------------------//
	salvatxt(parteReal_c, 'r', Borboleta, *N);										//
	salvatxt(parteImaginaria_c, 'i', Borboleta, *N);								//
	salvatxt(modulo_c, 'm', Borboleta, *N);											//
	salvatxt(fase_c, 'f', Borboleta, *N);											//
//----------------------------------------------------------------------------------//
		
	delete [] Borboleta; delete N;
	delete [] parteReal_c, parteImaginaria_c, modulo_c, fase_c;
	return 0;
	
}


//------------------Funcao para le amostras dos aquivos .txt -----------------------//	
int total_amostras(string diretorioArq){									   		//
//																					//
	char *c = new char[diretorioArq.length() + 1];									//
	strcpy(c, diretorioArq.c_str());												//
	int i = 0;																		//
//																					//
	string line;																	//
	ifstream arquivo(c);															//
	if (arquivo.is_open()){															//
//																					//
		while (getline(arquivo, line))												//
		{																			//
			i++;																	//
		}																			//
		arquivo.close();															//
	}																				//
	else return 0;																	//
//																					//	
	delete [] c;																	//
//																					//
	return i;																		//
}																					//
//----------------------------------------------------------------------------------//


//-----Funcao que ordena as amostras de forma adequada para o algoritmo FFT---------//
int indiceBorboleta (int indice, int N){											//
//																					//		
	int novoDecimal = 0;															//
	int etapas = log2((float)N);													//
	bool *binario = new bool[etapas];												//
//																					//
	for(int s = 0; s < etapas; s++){												//	
//																					//
		if(indice != 1)      binario[s] = indice % 2;								//
		else if(indice == 1) binario[s] = indice;									//
//																					//
		indice = indice/2;															//
	}																				//
//																					//
	for(int s = 0; s < etapas; s++)													//						
		novoDecimal = novoDecimal + pow(2,s)*(binario[etapas-1-s]);					//
//																					//
	delete [] binario;																//
	return novoDecimal;																//
}																					//
//----------------------------------------------------------------------------------//


//----------------------------------------------------------------------------------//
void calculaFFT(int N, int etapas, int k, Complexo *Borboleta){
	
	int ndeW;
	int a;
   	float c, d;
									
		
		ndeW = N/(N/pow(2,k+1));
		a = pow(2,k);
		
		Complexo *Wn = new Complexo[a];
		float *auxReal = new float[a];
		float *auxImag = new float[a];
		
		for(int inst = 0; inst < a; inst++){
			Wn[inst].setRealWn(inst, ndeW);
			Wn[inst].setImagWn(inst, ndeW);
		}
				
		for(int z = 0; z < N/pow(2, k+1); z++){
		
			for (int h = 0; h < ndeW/2; h++){
			
				auxReal[h] = Borboleta[h+z*ndeW].getReal() + Wn[h].getRealWn()*Borboleta[ndeW/2+h+z*ndeW].getReal() - Wn[h].getImagWn()*Borboleta[ndeW/2+h+z*ndeW].getImag();
				auxImag[h] = Borboleta[h+z*ndeW].getImag() + Wn[h].getImagWn()*Borboleta[ndeW/2+h+z*ndeW].getReal() + Wn[h].getRealWn()*Borboleta[ndeW/2+h+z*ndeW].getImag();
				
				c = Borboleta[h+z*ndeW].getReal() - Wn[h].getRealWn()*Borboleta[ndeW/2+h+z*ndeW].getReal() + Wn[h].getImagWn()*Borboleta[ndeW/2+h+z*ndeW].getImag();
				d = Borboleta[h+z*ndeW].getImag() - Wn[h].getImagWn()*Borboleta[ndeW/2+h+z*ndeW].getReal() - Wn[h].getRealWn()*Borboleta[ndeW/2+h+z*ndeW].getImag();					
				
				Borboleta[ndeW/2 + h + z*ndeW].setReal(c);
				Borboleta[ndeW/2 + h + z*ndeW].setImag(d);
				
				Borboleta[h + z*ndeW].setReal(auxReal[h]);
				Borboleta[h + z*ndeW].setImag(auxImag[h]);				
			}		
		}
			
		delete [] Wn; delete [] auxReal; delete[] auxImag;
	
	
}
//--------------------------------------------------------------------------------//

void salvatxt(char* nome, char identificador, Complexo* Borboleta, int N){
		
	if(identificador == 'r'){
		
		ofstream arquivo;
		arquivo.open(nome);
			
		for (int k = 0; k < N; k++){
			arquivo.setf(ios::fixed | ios::showpoint);	arquivo.precision(7); arquivo << Borboleta[k].getReal() << endl;
		}
			
		arquivo.close();
	}
	else if(identificador == 'i'){
		
		ofstream arquivo;
		arquivo.open(nome);
		
		for (int k = 0; k < N; k++){
			arquivo.setf(ios::fixed | ios::showpoint);	arquivo.precision(7); arquivo << Borboleta[k].getImag() << endl;
		}
		
		arquivo.close();
	}
	else if(identificador == 'm'){
		
		ofstream arquivo;
		arquivo.open(nome);
		
		for (int k = 0; k < N; k++){
			arquivo.setf(ios::fixed | ios::showpoint);	arquivo.precision(7); arquivo << 
			sqrt(Borboleta[k].getImag()*Borboleta[k].getImag() + Borboleta[k].getReal()*Borboleta[k].getReal()) << endl;
		}
		
		arquivo.close();
	}
	else if(identificador == 'f'){
		
		ofstream arquivo;
		arquivo.open(nome);
		
		for (int k = 0; k < N; k++){
			arquivo.setf(ios::fixed | ios::showpoint);	arquivo.precision(7); arquivo << 
			atan(Borboleta[k].getImag()/Borboleta[k].getReal()) << endl;
		}
		
		arquivo.close();	
	}	
}

